#pragma once
#include <vector>
#include <WString.h>
#include <Arduino.h>
#include <FS.h>

using std::vector;

vector<String> split(const String& str, const char sep);

template <typename Iter>
void s_print_coll(const Iter& it) {
  Serial.print("{ ");
  for (const auto& elem : it) {
    Serial.print(elem);
    Serial.print(", ");
  }
  Serial.println("};");
}
