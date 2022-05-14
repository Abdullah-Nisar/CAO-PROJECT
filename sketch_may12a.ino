#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "Error 404";
const char* password = "Abdullah23";

const char* PARAM_INPUT_1 = "00:00";
const char* PARAM_INPUT_2 = "00:00";

const int output26 = 26;

int time_hour_2 = 0;
int time_minute_2 = 0;
long total_time_in_ms_2=0;

// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    label:
    {
      margin-right: 10px;
    }
  </style>
  </head><body>
  <form action="/get">
  <label>Counter time for Switch 1: </label>
  <input type="time" id="input1" name="input1"><br>
  <input type="submit" value="Submit">
  <p id="counter1"></p>
  <br><br><label>Counter time for Switch 2: </label>
  <input type="time" id="input2" name="input2"><br>
  <input type="submit" value="Submit">
  <p id="counter2"></p>
  <br><br>  
  </form>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", index_html);
  });
  server.onNotFound(notFound);
  server.begin();

    pinMode(output26, OUTPUT);
}

void loop() {
//  Serial.print(PARAM_INPUT_1);
  int time_hour_1 = (String(PARAM_INPUT_1[0]) + String(PARAM_INPUT_1[1])).toInt();
  int time_minute_1 = (String(PARAM_INPUT_1[3]) + String(PARAM_INPUT_1[4])).toInt();
  long total_time_in_ms_1 = (time_hour_1 * 3600 + time_minute_1 * 60) * 1000;
  Serial.println(total_time_in_ms_1);
  delay(10000);
  digitalWrite(output26, LOW);
  delay(total_time_in_ms_1);
  digitalWrite(output26, HIGH);
  
}
