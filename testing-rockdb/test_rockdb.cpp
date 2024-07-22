#include <assert.h>
#include <rocksdb/db.h>
#include <rocksdb/options.h>
#include <rocksdb/status.h>

int main() {
  rocksdb::DB *db;
  rocksdb::Options options;
  options.IncreaseParallelism();
  options.OptimizeLevelStyleCompaction();

  options.create_if_missing = true;
  rocksdb::Status s = rocksdb::DB::Open(options, "/home/karan/Documents/test_rockdb", &db);
  assert(s.ok());

  // Insert value
  s = db->Put(rocksdb::WriteOptions(), "key1", "value");
  assert(s.ok());

  // Get value
  std::string value;
  s = db->Get(rocksdb::ReadOptions(), "key1", &value);
  assert(s.ok());
  assert(value == "value");

  // Close db
  s = db->Close();
  assert(s.ok());
  delete db;

  return 0;
}
