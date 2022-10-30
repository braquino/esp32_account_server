#ifndef DB_HPP_
#define DB_HPP_

#include <vector>
#include <tuple>
#include <WString.h>

struct Expense {
    String date;
    String description;
    float value;
};

class Db {
public:
  void add_record(String date, String description, float value);
  const std::vector<Expense>& get_records() const { return records; } 
  std::vector<std::tuple<String, String, float>> get_std_records() const;
private:
  std::vector<Expense> records;
};

#endif // DB_HPP_
