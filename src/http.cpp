#include "http.hpp"
#include <algorithm>


Http::Http(int timeout) : timeout{timeout} 
{

}

vector<String> split(const String& str, const char sep) {
  vector<String> result;
  String buffer;
  int len = str.length();
  for (int i=0; i<len + 1; i++) {
    if (str[i] != sep && i < len) {
      if (str[i] != '\r') buffer += str[i];
    } else {
      result.push_back(String(buffer));
      buffer = "";
    }
  }
  return result;
}

Path::Path(const String& s) {
  vector<String> pair = split(s, '?');
  Serial.println("pair parseds: " + String(pair.size()));
  this->sub_paths = split(pair[0], '/');
  Serial.println("subpath parseds: " + String(sub_paths.size()));
  if (pair.size() > 1){
    for (const String& att : split(pair[1], '&')) {
      vector<String> vk = split(att, '=');
      vk[1].replace('+', ' ');
      this->properties[vk[0]] = vk[1];
    }
  }
}

bool Path::check_path(const String& path) const {
  return std::find(sub_paths.begin(), sub_paths.end(), path) != sub_paths.end();
}

String Http::get_request(WiFiClient& client) {
  unsigned long start_time = millis();
  String header;
  String curr_line;
  while (client.connected()) { // && millis() - start_time < timeout) { // TODO: test timeout
    if (client.available()) { 
      char c = client.read(); 
      header += c;
      if (c == '\n') {
        if (curr_line.length() == 0) {
          break;
        } else { 
          curr_line = "";
        }
      } else if (c != '\r') { 
        curr_line += c;
      }
    }
  }
  return header;
}

Request Http::parse_request(const String& header, bool lite) {
  String lower_h = header;
  lower_h.toLowerCase();
  vector<String> splited = split(lower_h, '\n');
  Request req{};
  Serial.println("Start parsing lines: ");
  for (const String& line : splited) {
    Serial.println(line);
    if (line.startsWith("get")) {
      req.method = Method::GET;
      req.path_str = split(line, ' ')[1];
      req.path = Path{req.path_str};
      continue;
    }
    if (line.startsWith("post")) {
      req.method = Method::POST;
      req.path = split(line, ' ')[1];
      continue;
    }
    if (!lite) {
      if (line.startsWith("host:")) {
        req.host = line.substring(6);
        continue;
      }
      if (line.startsWith("connection:")) {
        req.connection = line.substring(12);
        continue;
      }
      if (line.startsWith("cache-control:")) {
        req.cache_control = line.substring(15);
        continue;
      }
      if (line.startsWith("user-agent:")) {
        req.user_agent = line.substring(12);
        continue;
      }
      if (line.startsWith("accept:")) {
        req.accept = line.substring(8);
        continue;
      }
      if (line.startsWith("accept-encoding:")) {
        req.accept_encoding = line.substring(17);
        continue;
      }
      if (line.startsWith("accept-language:")) {
        req.accept_language = line.substring(17);
        continue;
      }
    }
  }
  return req;
}
