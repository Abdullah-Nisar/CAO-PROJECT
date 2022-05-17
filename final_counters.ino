/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-web-server-timer-pulse/
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "Error 404";
const char* password = "Abdullah23";

const char* PARAM_INPUT_1 = "state1";
const char* PARAM_INPUT_2 = "value1";

const char* PARAM_INPUT_3 = "state2";
const char* PARAM_INPUT_4 = "value2";


const int output1 = 2;
const int output2 = 26;

String timerSliderValue1 = "30";
String timerSliderValue2 = "30";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP Web Server</title>
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 2.4rem;}
    p {font-size: 2.2rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch1 {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch1 input {display: none}
    .slider1 {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider1:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider1 {background-color: #2196F3}
    input:checked+.slider1:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
    .slider2.1 { -webkit-appearance: none; margin: 14px; width: 300px; height: 20px; background: #ccc;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider2.1::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 30px; height: 30px; background: #2f4468; cursor: pointer;}
    .slider2.1::-moz-range-thumb { width: 30px; height: 30px; background: #2f4468; cursor: pointer; } 

    .switch2 {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch2 input {display: none}
    .slider2 {position: relative; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider2:before {position: relative; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider2 {background-color: #2196F3}
    input:checked+.slider2:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
    .slider2.2 { -webkit-appearance: none; margin: 14px; width: 300px; height: 20px; background: #ccc;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider2.2::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 30px; height: 30px; background: #2f4468; cursor: pointer;}
    .slider2.2::-moz-range-thumb { width: 30px; height: 30px; background: #2f4468; cursor: pointer; } 
    
  </style>
</head>
<body>
  <h2>ESP Web Server</h2>
  <p><span id="timerValue1">%TIMERVALUE1%</span> Minute</p>
  <p><input type="range" onchange="updateSliderTimer1(this)" id="timerSlider1" min="1" max="30" value="%TIMERVALUE1%" step="1" class="slider2.1"></p>
  %BUTTONPLACEHOLDER1%
  <p><span id="timerValue2">%TIMERVALUE2%</span> Minute</p>
  <p><input type="range" onchange="updateSliderTimer2(this)" id="timerSlider2" min="1" max="30" value="%TIMERVALUE2%" step="1" class="slider2.2"></p>
  %BUTTONPLACEHOLDER2%
<script>
function toggleCheckbox(element) {
  var sliderValue1 = document.getElementById("timerSlider1").value;
  var sliderValue2 = document.getElementById("timerSlider2").value;
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?state1=1", true); xhr.send();
    var count = sliderValue1, timer = setInterval(function() {
      count--; document.getElementById("timerValue1").innerHTML = count;
      if(count == 0){ clearInterval(timer); document.getElementById("timerValue1").innerHTML = document.getElementById("timerSlider1").value; }
    }, 1000);
    sliderValue1 = sliderValue1*1000;
    setTimeout(function(){ xhr.open("GET", "/update?state1=0", true); 
    document.getElementById(element.id).checked = false; xhr.send(); }, sliderValue1);
  }
  if(element.checked){ xhr.open("GET", "/update?state2=1", true); xhr.send();
    var count = sliderValue2, timer = setInterval(function() {
      count--; document.getElementById("timerValue2").innerHTML = count;
      if(count == 0){ clearInterval(timer); document.getElementById("timerValue2").innerHTML = document.getElementById("timerSlider2").value; }
    }, 1000);
    sliderValue2 = sliderValue2*1000;
    setTimeout(function(){ xhr.open("GET", "/update?state2=0", true); 
    document.getElementById(element.id).checked = false; xhr.send(); }, sliderValue2);
  }
}
function updateSliderTimer1(element) {
  var sliderValue = document.getElementById("timerSlider1").value;
  document.getElementById("timerValue1").innerHTML = sliderValue;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+sliderValue, true);
  xhr.send();
}

function updateSliderTimer2(element) {
  var sliderValue = document.getElementById("timerSlider2").value;
  document.getElementById("timerValue2").innerHTML = sliderValue;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+sliderValue, true);
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER1"){
    String buttons = "";
    String outputStateValue = outputState();
    buttons+= "<p><label class=\"switch1\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output\" " + outputStateValue + "><span class=\"slider1\"></span></label></p>";
    return buttons;
  }
  else if(var == "BUTTONPLACEHOLDER2"){
    String buttons = "";
    String outputStateValue = outputState();
    buttons+= "<p><label class=\"switch2\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output\" " + outputStateValue + "><span class=\"slider2\"></span></label></p>";
    return buttons;
  }
  else if(var == "TIMERVALUE1"){
    return timerSliderValue1;
  }
  else if(var == "TIMERVALUE2"){
    return timerSliderValue2;
  }
  return String();
}

String outputState(){
  if(digitalRead(output1)){
    return "checked";
  }
  else if(digitalRead(output2)){
    return "checked";
  }
  else {
    return "";
  }
  return "";
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(output1, OUTPUT);
  pinMode(output2, OUTPUT);
  
  digitalWrite(output1, LOW);
  digitalWrite(output2, LOW);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/update?state=<inputMessage>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      digitalWrite(output1, inputMessage.toInt());
    }
    else if (request->hasParam(PARAM_INPUT_3)) {
      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      digitalWrite(output2, inputMessage.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  
  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      timerSliderValue1 = inputMessage;
    }
    else if (request->hasParam(PARAM_INPUT_4)) {
      inputMessage = request->getParam(PARAM_INPUT_4)->value();
      timerSliderValue2 = inputMessage;
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  
  // Start server
  server.begin();
}
  
void loop() {
  
}
