#include "WiFi.h"
#include "http.hpp"
#include "html.hpp"
#include "db.hpp"
#include "SD.h"
#include "SPI.h"
#include "sd_utils.hpp"

enum class led_state { ON, OFF };

const char *WIFI_SSID = "FASTWEB-35HNNd";
const char *WIFI_PASSWORD = "xex8jWW3ta";
WiFiServer server{80};

Http http;
Html html;
Db db;

void setup()
{
  Serial.begin(115200);
  
  Serial.print("connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("Wifi connected IP: ");
  Serial.println(WiFi.localIP());

  server.begin();

  if (!SD.begin(5))
  {
    Serial.println("Card Mount Failed");
    return;
  }
  sd::test_sd(SD);
}

void loop()
{
  WiFiClient client = server.available();
  if (client)
  {
    Serial.println("Client connect");
    String header = http.get_request(client);
    Serial.println(header);
    Request req = http.parse_request(header);
    Serial.print("Number of sub_paths: ");
    Serial.println(req.path.sub_paths.size());
    for (String s : req.path.sub_paths)
    {
      Serial.println(s);
    }
    Serial.print("Number of properties: ");
    Serial.println(req.path.properties.size());
    for (auto &kv : req.path.properties)
    {
      Serial.println(kv.first + "->" + kv.second);
    }
    // TODO: isolate router and process method POST
    if (req.path_str == "/" || req.path.check_path("records_page.html"))
    {
      if (req.path.properties.find("date") != req.path.properties.end())
      {
        db.add_record(SD, req.path.properties["date"], req.path.properties["desc"], req.path.properties["value"].toFloat());
      }
      else if (req.path.properties.find("id") != req.path.properties.end())
      {
        Serial.println("Deliting record id: " + req.path.properties["id"]);
        db.delete_record(SD, req.path.properties["id"].toInt());
      }
      Serial.println("Trying to load records_page...");
      client.print(html.get_records_page(db.get_std_records(SD)));
    }
    else if (req.path.check_path("insert_page.html"))
    {
      Serial.println("Trying to load insert_page...");
      client.print(html.get_insert_page());
    }
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
