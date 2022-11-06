#ifndef DB_HPP_
#define DB_HPP_

#include <vector>
#include <tuple>
#include <WString.h>
#include <FS.h>

struct Expense {
  long id;
  String date;
  String description;
  float value;
};

class Db {
public:
  Db() : last_id{-1} {}
  void add_record(fs::FS &fs, String date, String description, float value);
  std::vector<std::tuple<long, String, String, float>> get_std_records(fs::FS &fs);
  void delete_record(fs::FS &fs, long id);
  long last_id;
private:
  void update_last_id();
  const String db_file{"/expenses_v3.csv"};
};

#endif // DB_HPP_
