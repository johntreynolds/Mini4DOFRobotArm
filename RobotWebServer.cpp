#include "RobotWebServer.h"

//------------------------------DEFINITELY NOT VIBE-CODED HTML----------------------------------
const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Robot Arm Dashboard</title>
    <style>
        body { font-family: Arial, sans-serif; background-color: #1a1a1a; color: #ffffff; text-align: center; margin: 0; padding: 20px; }
        h1 { color: #00adb5; }
        .grid { display: flex; flex-wrap: wrap; justify-content: center; gap: 20px; max-width: 900px; margin: 0 auto; }
        .card { background-color: #222831; border-radius: 10px; padding: 20px; flex: 1; min-width: 300px; box-shadow: 0 4px 10px rgba(0,0,0,0.5); }
        .data-container { display: flex; justify-content: space-between; margin: 10px 0; padding: 8px 12px; background-color: #393e46; border-radius: 5px; }
        .label { font-weight: bold; }
        .value { font-family: monospace; font-size: 1.1em; color: #00adb5; }
        
        .input-group { display: flex; align-items: center; justify-content: space-between; margin: 10px 0; }
        .input-group label { font-weight: bold; width: 60px; text-align: left; }
        .input-group input { width: 120px; padding: 8px; border-radius: 5px; border: 1px solid #393e46; background-color: #1a1a1a; color: #ffffff; text-align: right; font-family: monospace; font-size: 1em; }
        .btn { width: 100%; padding: 12px; margin-top: 15px; background-color: #00adb5; color: #ffffff; border: none; border-radius: 5px; font-weight: bold; font-size: 1em; cursor: pointer; transition: background 0.2s; }
        .btn:hover { background-color: #00888e; }
        
        .status { margin-top: 15px; font-size: 0.9em; }
        .connected { color: #4caf50; }
        .disconnected { color: #f44336; }

        /* Console Output Card */
        .console-box { background-color: #0d1117; color: #00ff66; font-family: monospace; font-size: 0.9em; text-align: left; height: 180px; overflow-y: auto; padding: 10px; border-radius: 5px; border: 1px solid #393e46; white-space: pre-wrap; }
    </style>
</head>
<body>
    <h1>Robot Arm Control Panel</h1>
    
    <div class="grid">
        <!-- Telemetry Card -->
        <div class="card">
            <h2>Current Target / Position</h2>
            <div class="data-container"><span class="label">X:</span><span class="value" id="posX">--</span></div>
            <div class="data-container"><span class="label">Y:</span><span class="value" id="posY">--</span></div>
            <div class="data-container"><span class="label">Z:</span><span class="value" id="posZ">--</span></div>
            <div class="data-container"><span class="label">Phi (&phi;):</span><span class="value" id="posPhi">--</span></div>
            
            <div class="status">Connection: <span id="ws-status" class="disconnected">Disconnected</span></div>
        </div>

        <!-- Target Command Card -->
        <div class="card">
            <h2>Command Target</h2>
            <div class="input-group">
                <label for="cmdX">X (mm):</label>
                <input type="number" id="cmdX" step="0.1" value="0.0">
            </div>
            <div class="input-group">
                <label for="cmdY">Y (mm):</label>
                <input type="number" id="cmdY" step="0.1" value="150.0">
            </div>
            <div class="input-group">
                <label for="cmdZ">Z (mm):</label>
                <input type="number" id="cmdZ" step="0.1" value="50.0">
            </div>
            <div class="input-group">
                <label for="cmdPhi">Phi (&phi;&deg;):</label>
                <input type="number" id="cmdPhi" step="0.1" value="0.0">
            </div>
            
            <button class="btn" onclick="sendTarget()">SEND TARGET</button>
        </div>

        <!-- Serial Console Stream Card -->
        <div class="card" style="flex: 100%;">
            <h2>Serial Terminal</h2>
            <div class="console-box" id="console"></div>
        </div>
    </div>

    <script>
        const gateway = `ws://${window.location.hostname}/ws`;
        let websocket;

        function initWebSocket() {
            websocket = new WebSocket(gateway);
            
            websocket.onopen = function() {
                document.getElementById('ws-status').innerText = 'Connected';
                document.getElementById('ws-status').className = 'status connected';
            };

            websocket.onclose = function() {
                document.getElementById('ws-status').innerText = 'Disconnected - Retrying...';
                document.getElementById('ws-status').className = 'status disconnected';
                setTimeout(initWebSocket, 2000);
            };

            websocket.onmessage = function(event) {
                if (event.data.startsWith('{')) {
                    const data = JSON.parse(event.data);
                    document.getElementById('posX').innerText = data.x.toFixed(1) + ' mm';
                    document.getElementById('posY').innerText = data.y.toFixed(1) + ' mm';
                    document.getElementById('posZ').innerText = data.z.toFixed(1) + ' mm';
                    document.getElementById('posPhi').innerText = data.phi.toFixed(1) + '°';
                } else if (event.data.startsWith('[LOG]')) {
                    const consoleElem = document.getElementById('console');
                    consoleElem.innerText += event.data.replace('[LOG]', '') + '\n';
                    consoleElem.scrollTop = consoleElem.scrollHeight; // Auto scroll down
                }
            };
        }

        function sendTarget() {
            const target = {
                x: parseFloat(document.getElementById('cmdX').value),
                y: parseFloat(document.getElementById('cmdY').value),
                z: parseFloat(document.getElementById('cmdZ').value),
                phi: parseFloat(document.getElementById('cmdPhi').value)
            };
            websocket.send(JSON.stringify(target));
            console.log('Target Sent:', target);
        }

        window.addEventListener('load', initWebSocket);
    </script>
</body>
</html>
)rawliteral";

//--------------------------------WEB CODE----------------------------------------

RobotWebServer::RobotWebServer(const char* ssid, const char* password)
  : _ssid(ssid), _password(password), _server(80), _ws("/ws")
  {

  }

void RobotWebServer::onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) 
  {
    if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
            data[len] = 0; // Null-terminate string
            
            // Basic JSON parsing for target parameters (x, y, z, phi)
            float x, y, z, phi;
            if (sscanf((char*)data, "{\"x\":%f,\"y\":%f,\"z\":%f,\"phi\":%f}", &x, &y, &z, &phi) == 4) {
                _targetX = x;
                _targetY = y;
                _targetZ = z;
                _targetPhi = phi;
                _hasNewCommand = true;
                Serial.printf("[WebServer] New Target Received: X=%.1f, Y=%.1f, Z=%.1f, Phi=%.1f\n", x, y, z, phi);
            }
        }
    }
  }

void RobotWebServer::beginWebServer()
  {
    WiFi.softAP(_ssid, _password);

    Serial.print("[WebServer] Access Point Started! IP: ");
    Serial.println(WiFi.softAPIP());

    // Bind WebSocket event listener
    _ws.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
        this->onWsEvent(server, client, type, arg, data, len);
    });
    
    _server.addHandler(&_ws);

    _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) 
      {
        request->send_P(200, "text/html", HTML_PAGE);
      });

    _server.begin();
    Serial.println("[WebServer] HTTP Server Listening.");
  }

void RobotWebServer::updateWebServer()
  {
    _ws.cleanupClients();
    
  }

void RobotWebServer::sendTelemetry(float x, float y, float z, float phi)
  {
    if (_ws.count() == 0) return;

    if (millis() - _lastTelemetrySend >= TELEMETRY_INTERVAL_MS) 
      {
        _lastTelemetrySend = millis();

        char jsonBuffer[256];
        snprintf(jsonBuffer, sizeof(jsonBuffer),
                 "{\"x\":%.2f,\"y\":%.2f,\"z\":%.2f,\"phi\":%.2f}",
                 x, y, z, phi);

        _ws.textAll(jsonBuffer);
    }
  }

bool RobotWebServer::getNewTarget(float &x, float &y, float &z, float &phi)
  {
    if (_hasNewCommand) 
      {
        x = _targetX;
        y = _targetY;
        z = _targetZ;
        phi = _targetPhi;
        _hasNewCommand = false; // Reset flag after reading
        return true;
      }

    return false;
  }