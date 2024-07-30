#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

std::unordered_map<int, int> keyMap;

std::string md5(const std::string& str) {
  unsigned char result[SHA256_DIGEST_LENGTH];
  SHA256((unsigned char*)str.c_str(), str.length(), result);

  std::ostringstream sout;
  sout << std::hex << std::setfill('0');
  for(long long c: result) {
    sout << std::setw(2) << (long long)c;
  }
  return sout.str();
}

std::string select_s3_bucket(const std::string& data_key, int num_buckets = 10) {
  // Compute MD5 hash of the data key
  std::string hash_hex = md5(data_key);

  // Convert the first 8 characters of the hash to an integer
  unsigned long long hash_int = std::stoull(hash_hex.substr(0, 8), nullptr, 16);

  // Use modulo to get a bucket index
  int bucket_index = hash_int % num_buckets;

  // Map bucket index to your actual bucket names/regions
  std::vector<std::string> bucket_mapping = {
    "bucket-us-east-1",
    "bucket-us-west-2",
    "bucket-eu-west-1",
    "bucket-eu-central-1",
    "bucket-ap-southeast-1",
    "bucket-ap-northeast-1",
    "bucket-sa-east-1",
    "bucket-ca-central-1",
    "bucket-ap-south-1",
    "bucket-ap-southeast-2"
  };

  keyMap[bucket_index]++;
  return bucket_mapping[bucket_index];
}

int main() {
  std::string data_key = "value";

  for (int i = 0; i < 20; i++) {
    std::string keyToCheck = data_key + std::to_string(i);
    std::string selected_bucket = select_s3_bucket(keyToCheck);
    std::cout << "Data with key '" << keyToCheck << "' should be stored in bucket: " << selected_bucket << "\n\n";
  }

  for (const auto &key : keyMap) {
    std::cout << "Hash Code : " << key.first << std::endl;
    std::cout << "Used : " << key.second << std::endl;
    std::cout << "\n";
  }
  return 0;
}
