#include "db.hpp"
#include <exception>

void Db::add_record(String date, String description, float value) {
  this->records.push_back(Expense{.date = date, .description = description, .value = value});
}

std::vector<std::tuple<String, String, float>> Db::get_std_records() const {
  std::vector<std::tuple<String, String, float>> vec;
  for (const Expense& e : this->records) {
    vec.push_back(std::make_tuple(e.date, e.description, e.value));
  }
  return vec;
}