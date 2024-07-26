#include "../../include/json/json_parser.hpp"
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using namespace rapidjson;

std::string JsonParser::encodeToJsonString_(JsonDataBuilder::JsonGenericParams &jsonDataMap) {
  Document document;
  document.SetObject();


  for (const auto &data : jsonDataMap) {
    Value paramNameValue, valueParam;
    paramNameValue.SetString(data.first.c_str(), document.GetAllocator());

    auto value = data.second;

    if (std::holds_alternative<std::string>(value)) {
      std::string result = std::get<std::string>(value);
      valueParam.SetString(result.c_str(), document.GetAllocator());

    } else if (std::holds_alternative<long int>(value)) {
      long int result = std::get<long int>(value);
      valueParam.SetInt(result);

    } else if (std::holds_alternative<long double>(value)) {
      long double result = std::get<long double>(value);
      valueParam.SetDouble(result);

    } else if (std::holds_alternative<JsonDataBuilder::JsonGenericParamsVector>(value)) {
      auto listValues = std::get<JsonDataBuilder::JsonGenericParamsVector>(value);
      valueParam.SetArray();

      for (const auto &val : listValues) {
        Value tempValue;

        if (std::holds_alternative<std::string>(val)) {
          auto result = std::get<std::string>(val);
          tempValue.SetString(result.c_str(), document.GetAllocator());

        } else if (std::holds_alternative<long int>(val)) {
          auto result = std::get<long int>(val);
          tempValue.SetInt(result);

        } else if (std::holds_alternative<long double>(val)) {
          auto result = std::get<long double>(val);
          tempValue.SetDouble(result);
        }

        valueParam.PushBack(tempValue, document.GetAllocator());
      }
    }
    document.AddMember(paramNameValue, valueParam, document.GetAllocator());
  }

  StringBuffer buffer;
  Writer<StringBuffer> writer {buffer};
  document.Accept(writer);

  return buffer.GetString();
}

std::string JsonParser::encodeToJsonString_(const JsonDataBuilder *jsonBuilderPtr) {
  auto data = jsonBuilderPtr->build_();
  return encodeToJsonString_(data);
}

JsonDataBuilder::JsonGenericParamsVector JsonParser::getListValues__(const rapidjson::Value::ConstArray &dataList) {
  JsonDataBuilder::JsonGenericParamsVector resultList;
  for (SizeType i = 0; i < dataList.Size(); i++) {
    if (dataList[i].IsString()) {
      resultList.push_back(dataList[i].GetString());
    } else if (dataList[i].IsInt()) {
      resultList.push_back(dataList[i].GetInt());
    } else if (dataList[i].IsDouble()) {
      resultList.push_back(dataList[i].GetDouble());
    }
  }
  return resultList;
}

JsonDataBuilder::JsonGenericParams JsonParser::decodeToDataMap_(std::string &jsonString) {
  JsonDataBuilder::JsonGenericParams decodedJsonDataMap;
  
  Document document;
  document.Parse(jsonString.c_str());

  if (document.HasParseError()) {
    // Log for an error in parsing the json string
    return {};
  }

  for (Value::ConstMemberIterator itr = document.MemberBegin();
        itr != document.MemberEnd(); itr++) {

    std::string paramName = itr->name.GetString();
    if (itr->value.IsString()) {
      decodedJsonDataMap[paramName] = itr->value.GetString();
    } else if (itr->value.IsInt()) {
      decodedJsonDataMap[paramName] = itr->value.GetInt();
    } else if (itr->value.IsDouble()) {
      decodedJsonDataMap[paramName] = itr->value.GetDouble();
    } else if (itr->value.IsArray()) {
      decodedJsonDataMap[paramName] = getListValues__(itr->value.GetArray());
    }
  }
  return decodedJsonDataMap;
}
