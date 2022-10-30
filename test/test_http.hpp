#include <unity.h>
#include "http.hpp"

void test_parse_request() {
  Http http;
  String header = 
R"-header-(GET / HTTP/1.1
Host: 192.168.1.53
Connection: keep-alive
Cache-Control: max-age=0
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/106.0.0.0 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Accept-Encoding: gzip, deflate
Accept-Language: pt-BR,pt;q=0.9,en-US;q=0.8,en;q=0.7,it;q=0.6

)-header-";
  Request r = http.parse_request(header);
  Request expected{
    .method = Method::GET,
    .path = "/",
    .host = "192.168.1.53",
    .connection = "keep-alive",
    .cache_control = "max-age=0",
    .user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/106.0.0.0 Safari/537.36",
    .accept = "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9",
    .accept_encoding = "gzip, deflate",
    .accept_language = "pt-BR,pt;q=0.9,en-US;q=0.8,en;q=0.7,it;q=0.6"
  };
  TEST_ASSERT_TRUE(r.method == expected.method);
}

void test_http_main() {
  UNITY_BEGIN();

  RUN_TEST(test_parse_request);

  UNITY_END();
}