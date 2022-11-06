#include "sd_utils.hpp"
#include <map>
#include "utils.hpp"

namespace sd {
  std::vector<String> ls(fs::FS &fs, const char *dirname, f_type t, bool verbose)
  {
    if (verbose)
      Serial.printf("Listing directory: %s\n", dirname);
    std::vector<String> files;
    File root = fs.open(dirname);
    if (!root)
    {
      Serial.println("Failed to open directory");
      return files;
    }
    if (!root.isDirectory())
    {
      Serial.println("Not a directory");
      return files;
    }

    File file = root.openNextFile();
    while (file)
    {
      if (file.isDirectory())
      {
        String f = dirname;
        f += file.name();
        if (t == f_type::DIR || t == f_type::ALL) files.push_back(f);
        if (verbose)
        {
          Serial.println(f);
        }
      }
      else
      {
        String f = dirname;
        f += file.name();
        if (t == f_type::FILE || t == f_type::ALL) files.push_back(f);
        if (verbose)
        {
          Serial.print(f);
          Serial.print("  SIZE: ");
          Serial.println(file.size());
        }
      }
      file = root.openNextFile();
    }
    return files;
  }

  std::map<sd_mode, const char *> mode_map{{sd_mode::READ, "r"}, {sd_mode::WRITE, "w"}, {sd_mode::APPEND, "a"}};

  void write_file(fs::FS &fs, const char *path, const char *message, sd_mode mode)
  {
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, mode_map[mode]);
    if (!file)
    {
      Serial.println("Failed to open file for writing");
      return;
    }
    if (file.print(message))
    {
      Serial.println("File written");
    }
    else
    {
      Serial.println("Write failed");
    }
    file.close();
  }

  String read_file(fs::FS &fs, const char *path)
  {
    String s;

    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if (!file)
    {
      Serial.println("Failed to open file for reading");
      return s;
    }

    Serial.print("Read from file: ");
    while (file.available())
    {
      auto f = file.read();
      Serial.write(f);
      s += (char)f;
    }
    file.close();
    Serial.println(s);
    return s;
  }

  bool file_exists(fs::FS &fs, const char *path)
  {
    return fs.exists(path);
  }

  void rm(fs::FS &fs, String dir)
  {
    auto files = ls(fs, dir.c_str(), f_type::FILE);
    Serial.print("Files to remove: ");
    s_print_coll(files);
    for (const String& f : files)
    {
      String path = dir + f;
      path.replace("//", "/");
      Serial.println("Removing file: " + path);
      fs.remove(path);
    }
    auto dirs = ls(fs, dir.c_str(), f_type::DIR);
    Serial.print("Dirs to enter: ");
    s_print_coll(dirs);
    for (const String& d : dirs)
    {
      String path = dir + d;
      path.replace("//", "/");
      rm(fs, path);
      Serial.println("Removing directory: " + path);
      fs.rmdir(d);
    }
  }

  void test_sd(fs::SDFS &sdfs)
  {
    uint8_t cardType = sdfs.cardType();

  if (cardType == CARD_NONE)
  {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC)
  {
    Serial.println("MMC");
  }
  else if (cardType == CARD_SD)
  {
    Serial.println("SDSC");
  }
  else if (cardType == CARD_SDHC)
  {
    Serial.println("SDHC");
  }
  else
  {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  }
}
