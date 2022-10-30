#ifndef HTTP_HPP_
#define HTTP_HPP_

#include "WiFi.h"
#include <vector>
#include <map>

using std::vector;

enum class Method { GET, POST };

struct Path {
  Path() {};
  Path(const String& s);
  vector<String> sub_paths;
  std::map<String, String> properties;

  bool check_path(const String& path) const;
};

struct Request {
  Method method;
  Path path;
  String path_str;
  String host;
  String connection;
  String cache_control;
  String user_agent;
  String accept;
  String accept_encoding;
  String accept_language;
};

class Http {
public:
    Http(int timeout = 500);
    String get_request(WiFiClient& client);
    Request parse_request(const String& header, bool lite = false);
private:
    const long timeout;
};

#endif