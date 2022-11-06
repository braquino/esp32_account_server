#include "db.hpp"
#include <exception>
#include "utils.hpp"
#include "sd_utils.hpp"


void Db::add_record(fs::FS &fs, String date, String description, float value) 
{
  String row{String(this->last_id + 1) + ";" + date + ";" + description + ";" + String(value) + "\n"};
  if (sd::file_exists(fs, this->db_file.c_str())) 
  {
    sd::write_file(fs, this->db_file.c_str(), row.c_str(), sd::sd_mode::APPEND);
  } 
  else 
  {
    sd::write_file(fs, this->db_file.c_str(), row.c_str(), sd::sd_mode::WRITE);
  }
  this->last_id++;
}

std::vector<std::tuple<long, String, String, float>> Db::get_std_records(fs::FS &fs) 
{
  std::vector<std::tuple<long, String, String, float>> vec;
  long max_id{0};
  if (sd::file_exists(fs, this->db_file.c_str())) 
  {
    String str_db_file = sd::read_file(fs, this->db_file.c_str());
    for (const String& row : split(str_db_file, '\n')) 
    {
      if (row.length() > 0) 
      {
        vector<String> splited_row = split(row, ';');
        long id = splited_row[0].toInt();
        if (id > max_id) max_id = id;
        vec.push_back(std::make_tuple(id, splited_row[1], splited_row[2], splited_row[3].toFloat()));
      }
    }
  }
  this->last_id = max_id;
  return vec;
}

void Db::delete_record(fs::FS& fs, long id) 
{
  String new_table;
  auto records = this->get_std_records(fs);
  for (const auto& row : records) 
  {
    long row_id = std::get<0>(row);
    if (row_id != id) 
    {
      new_table += String(id) + ";" + std::get<1>(row) + ";" + std::get<2>(row) + ";" + String(std::get<3>(row)) + "\n";
    }
  }
  sd::write_file(fs, this->db_file.c_str(), new_table.c_str(), sd::sd_mode::WRITE);
}