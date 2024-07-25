#include "../utils/include/json/json_parser.hpp"
#include <iostream>

int main() {
  JsonParser parser;
  auto data = parser.jsonBuilder_
    ->set_("key_1", "world")
    ->set_("key_2", static_cast<long int>(10))
    ->set_("key_3", static_cast<long double>(15.012))
    ->set_("key_4", {static_cast<long int>(20), static_cast<long double>(34.89), "hello", "world"})
    ->build_();

  for (const auto &value : data) {
    std::cout << value.first << " ";
    if (std::holds_alternative<std::string>(value.second)) {
      std::cout << std::get<std::string>(value.second) << std::endl;
    } else if (std::holds_alternative<long int>(value.second)) {
      std::cout << std::get<long int>(value.second) << std::endl;
    } else if (std::holds_alternative<long double>(value.second)) {
      std::cout << std::get<long double>(value.second) << std::endl;
    } else if (std::holds_alternative<std::vector<std::variant<std::string, long int, long double>>>(value.second)) {

      auto values = std::get<std::vector<std::variant<std::string, long int, long double>>>(value.second);

      for (const auto &vec : values) {
        if (std::holds_alternative<std::string>(vec)) {
          std::cout << std::get<std::string>(vec) << " ";
        } else if (std::holds_alternative<long int>(vec)) {
          std::cout << std::get<long int>(vec) << " ";
        } else if (std::holds_alternative<long double>(vec)) {
          std::cout << std::get<long double>(vec) << " ";
        }
      }

      std::cout << "\n";
    }
  }
}
