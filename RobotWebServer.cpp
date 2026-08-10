#include "RobotWebServer.h"

//------------------------------DEFINITELY NOT VIBE-CODED HTML----------------------------------

const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Robot Arm Control Panel</title>
    <style>
        body { font-family: Arial, sans-serif; background-color: #1a1a1a; color: #ffffff; text-align: center; margin: 0; padding: 20px; }
        h1 { color: #00adb5; margin-bottom: 20px; }
        .grid { display: flex; flex-wrap: wrap; justify-content: center; gap: 20px; max-width: 950px; margin: 0 auto; }
        .card { background-color: #222831; border-radius: 10px; padding: 20px; flex: 1; min-width: 300px; box-shadow: 0 4px 10px rgba(0,0,0,0.5); text-align: left; }
        .card h2 { color: #ffffff; margin-top: 0; text-align: center; font-size: 1.3em; border-bottom: 2px solid #393e46; padding-bottom: 10px; }
        
        /* DATA CONTAINERS */
        .data-container { display: flex; justify-content: space-between; margin: 6px 0; padding: 8px 12px; background-color: #393e46; border-radius: 5px; }
        .label { font-weight: bold; color: #eeeeee; }
        .value { font-family: monospace; font-size: 1.1em; color: #00adb5; }
        
        /* INPUTS & SLIDERS */
        .input-group { display: flex; align-items: center; justify-content: space-between; margin: 10px 0; }
        .input-group label { font-weight: bold; width: 140px; text-align: left; }
        .input-group input[type="number"] { width: 100px; padding: 8px; border-radius: 5px; border: 1px solid #393e46; background-color: #1a1a1a; color: #ffffff; text-align: right; font-family: monospace; font-size: 1em; }
        .input-group input[type="range"] { flex: 1; margin: 0 10px; }
        .slider-val { width: 45px; text-align: right; font-family: monospace; color: #00adb5; font-weight: bold; }

        /* BUTTONS */
        .btn { width: 100%; padding: 12px; margin-top: 15px; background-color: #00adb5; color: #ffffff; border: none; border-radius: 5px; font-weight: bold; font-size: 1em; cursor: pointer; transition: background 0.2s; }
        .btn:hover { background-color: #00888e; }
        
        /* SAFETY & ACTION BAR */
        .action-bar { max-width: 950px; margin: 0 auto 20px auto; display: flex; gap: 15px; }
        .btn-estop { flex: 2; background-color: #d9534f; font-size: 1.2em; padding: 15px; margin: 0; }
        .btn-estop:hover { background-color: #c9302c; }
        .btn-estop.active { background-color: #ff0000; box-shadow: 0 0 15px #ff0000; animation: pulse 1s infinite; }
        .btn-home { flex: 1; background-color: #f0ad4e; color: #000000; font-size: 1.1em; padding: 15px; margin: 0; }
        .btn-home:hover { background-color: #ec971f; }

        /* OPMODE NAVIGATION */
        .nav-tabs { display: flex; justify-content: center; gap: 10px; margin-bottom: 20px; max-width: 950px; margin: 0 auto 20px auto; }
        .tab-btn { flex: 1; padding: 12px; background-color: #393e46; color: white; border: none; border-radius: 5px; cursor: pointer; font-weight: bold; font-size: 0.95em; transition: background 0.2s; }
        .tab-btn.active { background-color: #00adb5; color: #ffffff; }

        .mode-panel { display: none; }
        .mode-panel.active { display: block; }

        /* STATUS & TERMINAL */
        .status { margin-top: 10px; font-size: 0.9em; text-align: center; }
        .connected { color: #4caf50; }
        .disconnected { color: #f44336; }
        .console-box { background-color: #0d1117; color: #00ff66; font-family: monospace; font-size: 0.9em; text-align: left; height: 140px; overflow-y: auto; padding: 10px; border-radius: 5px; border: 1px solid #393e46; white-space: pre-wrap; }

        @keyframes pulse { 0% { opacity: 1; } 50% { opacity: 0.5; } 100% { opacity: 1; } }
    </style>
</head>
<body>
    <h1>Robot Arm Control Panel</h1>
    
    <!-- TOP CONTROL BAR: E-STOP & HOME -->
    <div class="action-bar">
        <button id="estopBtn" class="btn btn-estop" onclick="toggleEStop()">EMERGENCY STOP (OFF)</button>
        <button class="btn btn-home" onclick="sendHome()">HOME ARM</button>
    </div>

    <!-- OPMODE SELECTION TABS -->
    <div class="nav-tabs">
        <button id="tab0" class="tab-btn active" onclick="selectOpMode(0)">Manual XYZ (Active)</button>
        <button id="tab1" class="tab-btn" onclick="selectOpMode(1)">Live Sliders</button>
        <button id="tab2" class="tab-btn" onclick="selectOpMode(2)">Remote Control</button>
    </div>

    <div class="grid">
        <!-- GLOBAL TELEMETRY CARD -->
        <div class="card" style="flex: 100%;">
            <h2>Global Telemetry</h2>
            <div style="display: flex; gap: 20px; flex-wrap: wrap;">
                
                <!-- Cartesian Target Data -->
                <div style="flex: 1; min-width: 220px;">
                    <h3 style="color: #00adb5; font-size: 1em; margin-bottom: 8px; text-align: center;">Cartesian Space</h3>
                    <div class="data-container"><span class="label">X:</span><span class="value" id="posX">-- mm</span></div>
                    <div class="data-container"><span class="label">Y:</span><span class="value" id="posY">-- mm</span></div>
                    <div class="data-container"><span class="label">Z:</span><span class="value" id="posZ">-- mm</span></div>
                    <div class="data-container"><span class="label">Phi (&phi;):</span><span class="value" id="posPhi">-- &deg;</span></div>
                </div>

                <!-- Joint Angles Data -->
                <div style="flex: 1; min-width: 220px;">
                    <h3 style="color: #00adb5; font-size: 1em; margin-bottom: 8px; text-align: center;">Joint Angles</h3>
                    <div class="data-container"><span class="label">Turret (&theta;1):</span><span class="value" id="angTurret">-- &deg;</span></div>
                    <div class="data-container"><span class="label">Shoulder (&theta;2):</span><span class="value" id="angShoulder">-- &deg;</span></div>
                    <div class="data-container"><span class="label">Elbow (&theta;3):</span><span class="value" id="angElbow">-- &deg;</span></div>
                    <div class="data-container"><span class="label">Wrist (&theta;4):</span><span class="value" id="angWrist">-- &deg;</span></div>
                    <div class="data-container"><span class="label">Claw:</span><span class="value" id="angClaw">-- &deg;</span></div>
                </div>
                
            </div>
            <div class="status">Connection: <span id="ws-status" class="disconnected">Disconnected</span></div>
        </div>

        <!-- OPMODE 0: MANUAL XYZ NUMERICAL CONTROL -->
        <div id="panel0" class="card mode-panel active" style="flex: 100%;">
            <h2>Manual XYZ Numerical Target (OpMode 0)</h2>
            <div style="max-width: 500px; margin: 0 auto;">
                <div class="input-group">
                    <label>Target X (mm):</label>
                    <input type="number" id="numX" value="0" step="1">
                </div>
                <div class="input-group">
                    <label>Target Y (mm):</label>
                    <input type="number" id="numY" value="150" step="1">
                </div>
                <div class="input-group">
                    <label>Target Z (mm):</label>
                    <input type="number" id="numZ" value="50" step="1">
                </div>
                <div class="input-group">
                    <label>Target Phi (&phi;&deg;):</label>
                    <input type="number" id="numPhi" value="0" step="1">
                </div>
                <div class="input-group">
                    <label>Target Claw (%):</label>
                    <input type="number" id="numClaw" value="0" min="0" max="100" step="1">
                </div>
                <button class="btn" onclick="sendNumericalTarget()">Send Target (OpMode 0)</button>
            </div>
        </div>

        <!-- OPMODE 1: LIVE SLIDERS -->
        <div id="panel1" class="card mode-panel" style="flex: 100%;">
            <h2>Live Kinematic Sliders (OpMode 1)</h2>
            <div style="max-width: 500px; margin: 0 auto;">
                <div class="input-group">
                    <label>X (mm):</label>
                    <input type="range" id="sldX" min="-200" max="200" value="0" oninput="sendSliderTarget()">
                    <span id="valX" class="slider-val">0</span>
                </div>
                <div class="input-group">
                    <label>Y (mm):</label>
                    <input type="range" id="sldY" min="-200" max="200" value="150" oninput="sendSliderTarget()">
                    <span id="valY" class="slider-val">150</span>
                </div>
                <div class="input-group">
                    <label>Z (mm):</label>
                    <input type="range" id="sldZ" min="-200" max="200" value="50" oninput="sendSliderTarget()">
                    <span id="valZ" class="slider-val">50</span>
                </div>
                <div class="input-group">
                    <label>Phi (&phi;&deg;):</label>
                    <input type="range" id="sldPhi" min="0" max="180" value="0" oninput="sendSliderTarget()">
                    <span id="valPhi" class="slider-val">0</span>
                </div>
                <div class="input-group">
                    <label>Claw (%):</label>
                    <input type="range" id="sldClaw" min="0" max="100" value="0" oninput="sendSliderTarget()">
                    <span id="valClaw" class="slider-val">0%</span>
                </div>
            </div>
        </div>

        <!-- OPMODE 2: REMOTE CONTROL -->
        <div id="panel2" class="card mode-panel" style="flex: 100%;">
            <h2>Remote Control Interface (OpMode 2)</h2>
            <p style="text-align: center; color: #aaaaaa;">Reserved for trajectory recording, automated routines, or waypoint sequencing.</p>
        </div>

        <!-- SERIAL CONSOLE STREAM -->
        <div class="card" style="flex: 100%;">
            <h2>Serial Terminal</h2>
            <div class="console-box" id="console"></div>
        </div>
    </div>

    <script>
        const gateway = `ws://${window.location.hostname}/ws`;
        let websocket;
        let isEStop = false;
        let currentOpMode = 0;

        function initWebSocket() {
            websocket = new WebSocket(gateway);
            
            websocket.onopen = function() {
                const statusEl = document.getElementById('ws-status');
                statusEl.innerText = 'Connected';
                statusEl.className = 'status connected';
                selectOpMode(currentOpMode);
            };

            websocket.onclose = function() {
                const statusEl = document.getElementById('ws-status');
                statusEl.innerText = 'Disconnected - Retrying...';
                statusEl.className = 'status disconnected';
                setTimeout(initWebSocket, 2000);
            };

            websocket.onmessage = function(event) {
                if (event.data.startsWith('{')) {
                    const data = JSON.parse(event.data);
                    
                    // Cartesian Telemetry
                    if (data.X !== undefined) document.getElementById('posX').innerText = data.X.toFixed(1) + ' mm';
                    if (data.Y !== undefined) document.getElementById('posY').innerText = data.Y.toFixed(1) + ' mm';
                    if (data.Z !== undefined) document.getElementById('posZ').innerText = data.Z.toFixed(1) + ' mm';
                    if (data.PHI !== undefined) document.getElementById('posPhi').innerText = data.PHI.toFixed(1) + '°';

                    // Joint Telemetry
                    if (data.Turret !== undefined) document.getElementById('angTurret').innerText = data.Turret.toFixed(1) + '°';
                    if (data.Shoulder !== undefined) document.getElementById('angShoulder').innerText = data.Shoulder.toFixed(1) + '°';
                    if (data.Elbow !== undefined) document.getElementById('angElbow').innerText = data.Elbow.toFixed(1) + '°';
                    if (data.Wrist !== undefined) document.getElementById('angWrist').innerText = data.Wrist.toFixed(1) + '°';
                    if (data.Claw !== undefined) document.getElementById('angClaw').innerText = data.Claw.toFixed(1) + '°';

                } else if (event.data.startsWith('[LOG]')) {
                    const consoleElem = document.getElementById('console');
                    consoleElem.innerText += event.data.replace('[LOG]', '') + '\n';
                    consoleElem.scrollTop = consoleElem.scrollHeight;
                }
            };
        }

        function toggleEStop() {
            isEStop = !isEStop;
            const btn = document.getElementById('estopBtn');
            btn.innerText = isEStop ? "E-STOP ENGAGED (CLICK TO RELEASE)" : "EMERGENCY STOP (OFF)";
            btn.className = isEStop ? "btn btn-estop active" : "btn btn-estop";
            if (websocket && websocket.readyState === WebSocket.OPEN) {
                websocket.send(JSON.stringify({ type: "estop", state: isEStop }));
            }
        }

        function sendHome() {
            if (websocket && websocket.readyState === WebSocket.OPEN) {
                websocket.send(JSON.stringify({ type: "home" }));
            }
        }

        function selectOpMode(mode) {
            currentOpMode = mode;
            const tabNames = ["Manual XYZ", "Live Sliders", "Remote Control"];

            for (let i = 0; i < 3; i++) {
                const tab = document.getElementById(`tab${i}`);
                const panel = document.getElementById(`panel${i}`);
                
                if (i === mode) {
                    tab.className = "tab-btn active";
                    tab.innerText = `${tabNames[i]} (Active)`;
                    panel.style.display = "block";
                    panel.className = "card mode-panel active";
                } else {
                    tab.className = "tab-btn";
                    tab.innerText = tabNames[i];
                    panel.style.display = "none";
                    panel.className = "card mode-panel";
                }
            }

            if (websocket && websocket.readyState === WebSocket.OPEN) {
                websocket.send(JSON.stringify({ type: "opmode", mode: mode }));
            }
        }

        function sendNumericalTarget() {
            const x = parseFloat(document.getElementById('numX').value) || 0;
            const y = parseFloat(document.getElementById('numY').value) || 0;
            const z = parseFloat(document.getElementById('numZ').value) || 0;
            const phi = parseFloat(document.getElementById('numPhi').value) || 0;
            const claw = parseFloat(document.getElementById('numClaw').value) || 0;

            if (websocket && websocket.readyState === WebSocket.OPEN) {
                websocket.send(JSON.stringify({
                    type: "target",
                    x: x,
                    y: y,
                    z: z,
                    phi: phi,
                    claw: claw
                }));
            }
        }

        function sendSliderTarget() {
            const x = parseFloat(document.getElementById('sldX').value);
            const y = parseFloat(document.getElementById('sldY').value);
            const z = parseFloat(document.getElementById('sldZ').value);
            const phi = parseFloat(document.getElementById('sldPhi').value);
            const claw = parseFloat(document.getElementById('sldClaw').value);
            
            document.getElementById('valX').innerText = x;
            document.getElementById('valY').innerText = y;
            document.getElementById('valZ').innerText = z;
            document.getElementById('valPhi').innerText = phi;
            document.getElementById('valClaw').innerText = claw + '%';

            if (websocket && websocket.readyState === WebSocket.OPEN && currentOpMode === 1) {
                websocket.send(JSON.stringify({ 
                    type: "target",
                    x: x, 
                    y: y, 
                    z: z, 
                    phi: phi, 
                    claw: claw 
                }));
            }
        }

        window.addEventListener('load', () => {
            selectOpMode(0);
            initWebSocket();
        });
    </script>
</body>
</html>
)rawliteral";

//--------------------------------WEB CODE----------------------------------------

RobotWebServer::RobotWebServer(const char* ssid, const char* password)
  : _ssid(ssid), _password(password), _server(80), _ws("/ws")
  {

  }

// --------------------------------------------------- SETUP --------------------------------------------

void RobotWebServer::onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) 
{
    if (type == WS_EVT_DATA) 
    {
        AwsFrameInfo *info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) 
        {
            data[len] = 0; // Null-terminate string
            String msg = String((char*)data);

            // 1. Handle E-Stop Toggle
            if (msg.indexOf("\"type\":\"estop\"") != -1) 
            {
                _eStopActive = (msg.indexOf("\"state\":true") != -1);
                Serial.printf("[SAFETY] E-Stop State Changed: %s\n", _eStopActive ? "ACTIVATED" : "RELEASED");
                return;
            }

            // If E-Stop is active, ignore all incoming motion/mode requests
            if (_eStopActive) 
            {
                return;
            }

            // 2. Handle Home Request
            if (msg.indexOf("\"type\":\"home\"") != -1) 
            {
                _homeRequested = true;
                return;
            }      

            // 3. Handle OpMode Selection
            if (msg.indexOf("\"type\":\"opmode\"") != -1) 
            {
                int modeVal = 0;
                if (sscanf(msg.c_str(), "{\"type\":\"opmode\",\"mode\":%d}", &modeVal) == 1) 
                {
                    _currentOpMode = static_cast<OpMode>(modeVal);
                    _opModeChanged = true;
                    Serial.printf("[OPMODE] Switched to OpMode %d\n", modeVal);
                }
                return;
            }

            // 4. Handle Target Updates (Checking for "type":"target")
            if (msg.indexOf("\"type\":\"target\"") != -1)
            {
                float x, y, z, phi, claw;

                // Try parsing full target payload (including claw)
                if (sscanf(msg.c_str(), "{\"type\":\"target\",\"x\":%f,\"y\":%f,\"z\":%f,\"phi\":%f,\"claw\":%f}", &x, &y, &z, &phi, &claw) == 5) 
                {
                    _targetX = x;
                    _targetY = y;
                    _targetZ = z;
                    _targetPhi = phi;
                    _targetClaw = claw;
                    _hasNewCommand = true;
                } 
                // Fallback for targets without claw
                else if (sscanf(msg.c_str(), "{\"type\":\"target\",\"x\":%f,\"y\":%f,\"z\":%f,\"phi\":%f}", &x, &y, &z, &phi) == 4) 
                {
                    _targetX = x;
                    _targetY = y;
                    _targetZ = z;
                    _targetPhi = phi;
                    _hasNewCommand = true;
                }
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

// ------------------------------------------- UPDATE ------------------------------------

void RobotWebServer::updateWebServer()
  {
    _ws.cleanupClients();
    
  }

// ----------------------------- SEND TELEMETRY --------------------------------------------

void RobotWebServer::sendTelemetry(float x, float y, float z, float phi, float turretDeg, float shoulderDeg, float elbowDeg, float wristDeg, float clawDeg)
  {
    unsigned long currentMillis = millis();
    if (currentMillis - _lastTelemetrySend >= TELEMETRY_INTERVAL_MS) {
        _lastTelemetrySend = currentMillis;

        if (_ws.count() > 0) {
            // Format telemetry JSON including both XYZ-Phi and Joint Angles
            char buffer[200];
            snprintf(buffer, sizeof(buffer), 
                "{\"X\":%.1f,\"Y\":%.1f,\"Z\":%.1f,\"PHI\":%.1f,\"Turret\":%.1f,\"Shoulder\":%.1f,\"Elbow\":%.1f,\"Wrist\":%.1f,\"Claw\":%.1f}", 
                x, y, z, phi, turretDeg, shoulderDeg, elbowDeg, wristDeg, clawDeg);
            
            _ws.textAll(buffer);
        }
    }
  }

void RobotWebServer::log(const String &message) 
  {
    // Also print to actual USB Serial for debugging during dev
    Serial.println(message);

    // Prefix with [LOG] so the HTML JavaScript knows it's a console string
    String payload = "[LOG] " + message;
    
    // Broadcast to all connected WebSocket clients
    _ws.textAll(payload);
  }

void RobotWebServer::logf(const char *format, ...) 
  {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    log(String(buffer));
  }

// --------------------------- NEW TARGET --------------------------------

bool RobotWebServer::getNewTarget(float &x, float &y, float &z, float &phi, float &claw)
  {
    if (_hasNewCommand) 
      {
        x = _targetX;
        y = _targetY;
        z = _targetZ;
        phi = _targetPhi;
        claw = _targetClaw;
        _hasNewCommand = false; // Reset flag after reading
        return true;
      }

    return false;
  }

// ----------------------------- CHECKING THINGS FROM WEB INTERFACE -----------------------------

bool RobotWebServer::isEStopActive() 
  {
    return _eStopActive;
  }

bool RobotWebServer::checkHomeRequest()
  {
    if (_homeRequested)
      {
        _homeRequested = false;
        return true;
      }
    return false;
  }

bool RobotWebServer::checkOpModeChanged(OpMode &newMode)
  {
    if (_opModeChanged)
      {
        newMode = _currentOpMode;
        _opModeChanged = false;
        return true;
      }
      return false;
  }