#include "WiFi.h"
#include "http.hpp"
#include "html.hpp"
#include "db.hpp"

enum class led_state {
  ON, OFF
};
led_state pin_5_state{led_state::OFF};
led_state pin_2_state{led_state::OFF};

const char* WIFI_SSID = "FASTWEB-35HNNd";
const char* WIFI_PASSWORD = "xex8jWW3ta";
WiFiServer server{80};

Http http;
Html html;
Db db;

void setup() {
  Serial.begin(115200);

  Serial.print("connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("Wifi connected IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}


void loop() {  
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connect");
    String header = http.get_request(client);
    Serial.println(header);
    Request req = http.parse_request(header);
    Serial.print("Number of sub_paths: ");
      Serial.println(req.path.sub_paths.size());
    for (String s : req.path.sub_paths) {
      Serial.println(s);
    }
    Serial.print("Number of properties: ");
      Serial.println(req.path.properties.size());
    for (auto& kv : req.path.properties) {
      Serial.println(kv.first + "->" + kv.second);
    }
    if (req.path_str == "/" || req.path.check_path("records_page.html")) {
      if (req.path.properties.size() > 0) {
        db.add_record(req.path.properties["date"], req.path.properties["desc"], req.path.properties["value"].toFloat());
      }
      Serial.println("Trying to load records_page...");
      client.print(html.get_records_page(db.get_std_records()));
    } else if (req.path.check_path("insert_page.html")) {
      Serial.println("Trying to load insert_page...");
      client.print(html.get_insert_page());
    }
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
