#include "robot_webserver.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// HTML Webpage with styled slider
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Robot Slider</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { text-align: center; font-family: Arial; margin-top: 50px; }
    .slider { width: 80%; max-width: 400px; height: 25px; }
    h3 { margin-top: 30px; }
  </style>
</head>
<body>
  <h2>Robot Control Panel</h2>
  
  <h3>Motor Speed Control</h3>
  <input type="range" min="0" max="255" value="127" class="slider" id="speedSlider" oninput="updateSlider(this.value)">
  <p>Value: <span id="sliderVal">127</span></p>

  <script>
    var ws = new WebSocket(`ws://${window.location.hostname}/ws`);
    
    function updateSlider(val) {
      document.getElementById('sliderVal').innerText = val;
      // Sends "SLIDER:150" or similar to the ESP32
      ws.send("SLIDER:" + val);
    }
  </script>
</body>
</html>
)rawliteral";

// Internal command handler – processes incoming socket payloads locally
void processIncomingCommand(String command) {
  command.trim();
  Serial.print("[Web] Command Received: ");
  Serial.println(command);

  if (command.startsWith("SLIDER:")) {
    int val = command.substring(7).toInt();
    Serial.print("[Web] Parsed Slider Value: ");
    Serial.println(val);
    
    // Add logic or state updates here
  }
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0; // Null-terminate string
    String command = (char*)data;
    processIncomingCommand(command);
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_DATA) {
    handleWebSocketMessage(arg, data, len);
  }
}

void initWebServer(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);
  Serial.print("[WiFi] Connecting to ");
  Serial.print(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n[WiFi] Connected!");
  Serial.print("[WiFi] IP Address: ");
  Serial.println(WiFi.localIP());

  ws.onEvent(onEvent);
  server.addHandler(&ws);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  server.begin();
  Serial.println("[Web] Server started");
}

void updateWebServer() {
  ws.cleanupClients();
}

void sendDataToComputer(String message) {
  ws.textAll(message);
}