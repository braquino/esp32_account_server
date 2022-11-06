#include "utils.hpp"

vector<String> split(const String &str, const char sep)
{
  vector<String> result;
  String buffer;
  int len = str.length();
  for (int i = 0; i < len + 1; i++)
  {
    if (str[i] != sep && i < len)
    {
      if (str[i] != '\r')
        buffer += str[i];
    }
    else
    {
      result.push_back(String(buffer));
      buffer = "";
    }
  }
  return result;
}
