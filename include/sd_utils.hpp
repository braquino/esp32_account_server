#pragma once
#include <vector>
#include <WString.h>
#include <Arduino.h>
#include <FS.h>
#include <SD.h>

// DOTO: Move to SDFat
namespace sd {

  enum class f_type {
    DIR, FILE, ALL
  };

  enum class sd_mode {
    READ, WRITE, APPEND
  };

  std::vector<String> ls(fs::FS &fs, const char * dirname, f_type t = f_type::ALL, bool verbose = true);

  void write_file(fs::FS &fs, const char * path, const char * message, sd_mode mode);

  String read_file(fs::FS &fs, const char * path);

  bool file_exists(fs::FS &fs, const char* path);

  void rm(fs::FS &fs, String dir = "/");

  void test_sd(fs::SDFS &sdfs);

}