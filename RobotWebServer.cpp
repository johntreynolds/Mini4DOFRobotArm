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
        
        .data-container { display: flex; justify-content: space-between; margin: 6px 0; padding: 8px 12px; background-color: #393e46; border-radius: 5px; }
        .label { font-weight: bold; color: #eeeeee; }
        .value { font-family: monospace; font-size: 1.1em; color: #00adb5; }
        
        .input-group { display: flex; align-items: center; justify-content: space-between; margin: 10px 0; }
        .input-group label { font-weight: bold; width: 140px; text-align: left; }
        .input-group input[type="number"] { width: 100px; padding: 8px; border-radius: 5px; border: 1px solid #393e46; background-color: #1a1a1a; color: #ffffff; text-align: right; font-family: monospace; font-size: 1em; }
        .input-group input[type="range"] { flex: 1; margin: 0 10px; }
        .slider-val { width: 45px; text-align: right; font-family: monospace; color: #00adb5; font-weight: bold; }

        .btn { width: 100%; padding: 12px; margin-top: 15px; background-color: #00adb5; color: #ffffff; border: none; border-radius: 5px; font-weight: bold; font-size: 1em; cursor: pointer; transition: background 0.2s; }
        .btn:hover { background-color: #00888e; }
        
        .action-bar { max-width: 950px; margin: 0 auto 20px auto; display: flex; gap: 15px; }
        .btn-estop { flex: 2; background-color: #d9534f; font-size: 1.2em; padding: 15px; margin: 0; }
        .btn-estop:hover { background-color: #c9302c; }
        .btn-estop.active { background-color: #ff0000; box-shadow: 0 0 15px #ff0000; animation: pulse 1s infinite; }
        .btn-home { flex: 1; background-color: #f0ad4e; color: #000000; font-size: 1.1em; padding: 15px; margin: 0; }
        .btn-home:hover { background-color: #ec971f; }

        .nav-tabs { display: flex; justify-content: center; gap: 10px; margin-bottom: 20px; max-width: 950px; margin: 0 auto 20px auto; }
        .tab-btn { flex: 1; padding: 12px; background-color: #393e46; color: white; border: none; border-radius: 5px; cursor: pointer; font-weight: bold; font-size: 0.95em; transition: background 0.2s; }
        .tab-btn.active { background-color: #00adb5; color: #ffffff; }

        .mode-panel { display: none; }
        .mode-panel.active { display: block; }

        .status { margin-top: 10px; font-size: 0.9em; text-align: center; }
        .connected { color: #4caf50; }
        .disconnected { color: #f44336; }
        .console-container { max-width: 950px; margin: 20px auto 0 auto; text-align: left; }
        .console-box { background-color: #0d1117; color: #00ff66; font-family: monospace; font-size: 0.85em; height: 160px; overflow-y: auto; padding: 12px; border-radius: 8px; border: 1px solid #393e46; white-space: pre-wrap; box-shadow: inset 0 2px 5px rgba(0,0,0,0.8); }

        @keyframes pulse { 0% { opacity: 1; } 50% { opacity: 0.5; } 100% { opacity: 1; } }
    </style>
</head>
<body>
    <h1>Robot Arm Control Panel</h1>
    
    <div class="action-bar">
        <button id="estopBtn" class="btn btn-estop" onclick="toggleEStop()">EMERGENCY STOP (OFF)</button>
        <button class="btn btn-home" onclick="sendHome()">HOME ARM</button>
    </div>

    <div class="nav-tabs">
        <button id="tab0" class="tab-btn active" onclick="selectOpMode(0)">Manual XYZ (Active)</button>
        <button id="tab1" class="tab-btn" onclick="selectOpMode(1)">Live Sliders</button>
        <button id="tab2" class="tab-btn" onclick="selectOpMode(2)">Remote Control</button>
        <button id="tab3" class="tab-btn" onclick="selectOpMode(3)">Angle Testing</button>
    </div>

    <div class="grid">
        <!-- GLOBAL TELEMETRY CARD -->
        <div class="card" style="flex: 100%;">
            <h2>Global Telemetry Overview</h2>
            <div style="display: flex; gap: 20px; flex-wrap: wrap;">
                <div style="flex: 1; min-width: 220px;">
                    <h3 style="color: #00adb5; font-size: 1em; margin-bottom: 8px; text-align: center;">Cartesian Space</h3>
                    <div class="data-container"><span class="label">X:</span><span class="value pos-x">-- mm</span></div>
                    <div class="data-container"><span class="label">Y:</span><span class="value pos-y">-- mm</span></div>
                    <div class="data-container"><span class="label">Z:</span><span class="value pos-z">-- mm</span></div>
                    <div class="data-container"><span class="label">Reach (R):</span><span class="value pos-r">-- mm</span></div>
                    <div class="data-container"><span class="label">Phi (&phi;):</span><span class="value pos-phi">-- &deg;</span></div>
                </div>

                <div style="flex: 1; min-width: 220px;">
                    <h3 style="color: #00adb5; font-size: 1em; margin-bottom: 8px; text-align: center;">Joint Angles</h3>
                    <div class="data-container"><span class="label">Turret (&theta;1):</span><span class="value ang-turret">-- &deg;</span></div>
                    <div class="data-container"><span class="label">Shoulder (&theta;2):</span><span class="value ang-shoulder">-- &deg;</span></div>
                    <div class="data-container"><span class="label">Elbow (&theta;3):</span><span class="value ang-elbow">-- &deg;</span></div>
                    <div class="data-container"><span class="label">Wrist (&theta;4):</span><span class="value ang-wrist">-- &deg;</span></div>
                    <div class="data-container"><span class="label">Claw:</span><span class="value ang-claw">-- &deg;</span></div>
                </div>
            </div>
            <div class="status">WebSocket: <span id="ws-status" class="disconnected">Disconnected</span></div>
        </div>

        <!-- OPMODE 0 -->
        <div id="panel0" class="card mode-panel active" style="flex: 100%;">
            <h2>Manual XYZ Input</h2>
            <div style="max-width: 500px; margin: 0 auto;">
                <div class="input-group"><label>Target X (mm):</label><input type="number" id="numX" value="0"></div>
                <div class="input-group"><label>Target Y (mm):</label><input type="number" id="numY" value="0"></div>
                <div class="input-group"><label>Target Z (mm):</label><input type="number" id="numZ" value="0"></div>
                <div class="input-group"><label>Target Phi (&phi;&deg;):</label><input type="number" id="numPhi" value="0"></div>
                <div class="input-group"><label>Claw Closed (%):</label><input type="number" id="numClawPct0" value="0" min="0" max="100"></div>
                <button class="btn" onclick="sendNumericalTarget()">Send Target</button>
            </div>
        </div>

        <!-- OPMODE 1 -->
        <div id="panel1" class="card mode-panel" style="flex: 100%;">
            <h2>Live Sliders</h2>
            <div style="max-width: 500px; margin: 0 auto;">
                <div class="input-group"><label>X (mm):</label><input type="range" id="sldX" min="-200" max="200" value="0" oninput="queueSliderSend()"><span id="valX" class="slider-val">0</span></div>
                <div class="input-group"><label>Y (mm):</label><input type="range" id="sldY" min="30" max="200" value="150" oninput="queueSliderSend()"><span id="valY" class="slider-val">150</span></div>
                <div class="input-group"><label>Z (mm):</label><input type="range" id="sldZ" min="-100" max="250" value="50" oninput="queueSliderSend()"><span id="valZ" class="slider-val">50</span></div>
                <div class="input-group"><label>Phi (&phi;&deg;):</label><input type="range" id="sldPhi" min="0" max="180" value="0" oninput="queueSliderSend()"><span id="valPhi" class="slider-val">0</span></div>
                <div class="input-group"><label>Claw (% Closed):</label><input type="range" id="sldClaw" min="0" max="100" value="0" oninput="queueSliderSend()"><span id="valClaw" class="slider-val">0%</span></div>  
            </div>
        </div>

        <!-- OPMODE 2 -->
        <div id="panel2" class="card mode-panel" style="flex: 100%;">
            <h2>Remote Controller Mode</h2>
            <div style="max-width: 550px; margin: 0 auto;">
                <div class="data-container" style="justify-content: center;"><span class="label">Gamepad State:&nbsp;</span><span id="controller-status" class="disconnected">Disconnected (Press Any Button)</span></div>
                
                <h3 style="color: #00adb5; font-size: 1em; margin: 15px 0 8px 0; text-align: center;">Stick Mappings & Input Readouts</h3>
                <div class="data-container"><span class="label">Left Stick Left/Right (R Reach In/Out):</span><span class="value" id="dbgLX">0.00</span></div>
                <div class="data-container"><span class="label">Left Stick Up/Down (Z Height Up/Down):</span><span class="value" id="dbgLY">0.00</span></div>
                <div class="data-container"><span class="label">Right Stick Left/Right (Turret Rotation):</span><span class="value" id="dbgRX">0.00</span></div>
                <div class="data-container"><span class="label">Left Trigger (Close Claw):</span><span class="value" id="dbgLT">0.00</span></div>
                <div class="data-container"><span class="label">Right Trigger Left/Right (Open Claw):</span><span class="value" id="dbgRT">0.00</span></div>

                <h3 style="color: #00adb5; font-size: 1em; margin: 15px 0 8px 0; text-align: center;">Active Controller Target Readout</h3>
                <div class="data-container"><span class="label">Calculated Reach (R):</span><span class="value" id="ctrlR">150 mm</span></div>
                <div class="data-container"><span class="label">Calculated Height (Z):</span><span class="value" id="ctrlZ">50 mm</span></div>
                <div class="data-container"><span class="label">Calculated Turret Angle:</span><span class="value" id="ctrlTurret">90 &deg;</span></div>
            </div>
        </div>

        <!-- OPMODE 3 -->
        <div id="panel3" class="card mode-panel" style="flex: 100%;">
            <h2>Direct Angle Control</h2>
            <div style="max-width: 500px; margin: 0 auto;">
                <div class="input-group"><label>Turret (&deg;):</label><input type="number" id="num-turret" min="0" max="180" value="90"></div>
                <div class="input-group"><label>Shoulder (&deg;):</label><input type="number" id="num-shoulder" min="0" max="180" value="90"></div>
                <div class="input-group"><label>Elbow (&deg;):</label><input type="number" id="num-elbow" min="0" max="180" value="90"></div>
                <div class="input-group"><label>Wrist (&deg;):</label><input type="number" id="num-wrist" min="0" max="180" value="90"></div>
                <div class="input-group"><label>Claw Angle (&deg;):</label><input type="number" id="num-claw" min="0" max="180" value="180"></div>
                <button class="btn" onclick="sendManualAngles()">Send Angles (OpMode 3)</button>
            </div>
        </div>
    </div>

    <div class="console-container">
        <h2 style="font-size: 1.1em; color: #00adb5; margin-bottom: 8px;">Serial Terminal Log</h2>
        <div id="console" class="console-box"></div>
    </div>

    <script>
    const gateway = `ws://${window.location.hostname}/ws`;
    let websocket;
    let currentOpMode = 0;
    let isEStopped = false;

    // Local target variables for OpMode 2 calculation bench
    let targetR = 150;      
    let targetZ = 50;       
    let targetTurret = 90;  

    let sliderSendTimer = null;
    let lastSliderSendTime = 0;

    const BOUNDS = {
        MIN_R: 30, MAX_R: 200,
        MIN_Z: -100, MAX_Z: 250,
        MIN_TURRET: 0, MAX_TURRET: 180
    };

    function toggleEStop() {
    isEStopped = !isEStopped;
    const btn = document.getElementById('estopBtn');
    
    if (isEStopped) {
        btn.innerText = "EMERGENCY STOP (ACTIVE)";
        btn.classList.add('active');
    } else {
        btn.innerText = "EMERGENCY STOP (OFF)";
        btn.classList.remove('active');
    }

    if (websocket && websocket.readyState === WebSocket.OPEN) {
        websocket.send(JSON.stringify({
            type: "estop",
            state: isEStopped
        }));
    }
}

function sendHome() {
    if (websocket && websocket.readyState === WebSocket.OPEN) {
        websocket.send(JSON.stringify({
            type: "home"
        }));
    }
}

    function updateTelemetryDisplay(selector, valueStr) {
        document.querySelectorAll(selector).forEach(el => el.innerText = valueStr);
    }

    function initWebSocket() {
        websocket = new WebSocket(gateway);
        websocket.onopen = function() {
            const statusEl = document.getElementById('ws-status');
            if (statusEl) { statusEl.innerText = 'Connected'; statusEl.className = 'status connected'; }
            selectOpMode(currentOpMode);
        };
        websocket.onclose = function() {
            const statusEl = document.getElementById('ws-status');
            if (statusEl) { statusEl.innerText = 'Disconnected - Retrying...'; statusEl.className = 'status disconnected'; }
            setTimeout(initWebSocket, 2000);
        };
        websocket.onmessage = function(event) {
    if (event.data.startsWith('{')) {
        const data = JSON.parse(event.data);
        
        // Cartesian Space
        if (data.X !== undefined && !isNaN(data.X)) updateTelemetryDisplay('.pos-x', data.X.toFixed(1) + ' mm');
        if (data.Y !== undefined && !isNaN(data.Y)) updateTelemetryDisplay('.pos-y', data.Y.toFixed(1) + ' mm');
        if (data.Z !== undefined && !isNaN(data.Z)) updateTelemetryDisplay('.pos-z', data.Z.toFixed(1) + ' mm');
        if (data.PHI !== undefined && !isNaN(data.PHI)) updateTelemetryDisplay('.pos-phi', data.PHI.toFixed(1) + '°');
        
        // Reach: use explicit 'R' from JSON, or fall back to hypot(X, Y)
        if (data.R !== undefined && !isNaN(data.R)) {
            updateTelemetryDisplay('.pos-r', data.R.toFixed(1) + ' mm');
        } else if (data.X !== undefined && data.Y !== undefined) {
            updateTelemetryDisplay('.pos-r', Math.hypot(data.X, data.Y).toFixed(1) + ' mm');
        }

        // Joint Space
        if (data.Turret !== undefined) updateTelemetryDisplay('.ang-turret', data.Turret.toFixed(1) + '°');
        if (data.Shoulder !== undefined) updateTelemetryDisplay('.ang-shoulder', data.Shoulder.toFixed(1) + '°');
        if (data.Elbow !== undefined) updateTelemetryDisplay('.ang-elbow', data.Elbow.toFixed(1) + '°');
        if (data.Wrist !== undefined) updateTelemetryDisplay('.ang-wrist', data.Wrist.toFixed(1) + '°');
        if (data.Claw !== undefined) updateTelemetryDisplay('.ang-claw', data.Claw.toFixed(1) + '°');
    } else if (event.data.startsWith('[LOG]')) {
        const consoleBox = document.getElementById('console');
        if (consoleBox) {
            consoleBox.innerText += event.data.substring(6) + '\n';
            consoleBox.scrollTop = consoleBox.scrollHeight;
        }
    }
};
    }

    function deadzone(val, thresh = 0.20) {
        return Math.abs(val) < thresh ? 0.0 : val;
    }

    function updateDpadDisplay(elemId, isPressed) {
        const elem = document.getElementById(elemId);
        if (elem) {
            elem.innerText = isPressed ? "ON" : "OFF";
            elem.className = isPressed ? "val-active" : "val-inactive";
        }
    }

    // --- OPMODE 1: LIVE SLIDERS (ACTIVE WEBSOCKET TRANSMISSION) ---
    function sendLiveSliderData() {
    if (!websocket || websocket.readyState !== WebSocket.OPEN || currentOpMode !== 1) return;

    // Fixed element IDs (matching sldX, sldY, sldZ, sldPhi, sldClaw)
    const x = parseFloat(document.getElementById('sldX')?.value || 0);
    const y = parseFloat(document.getElementById('sldY')?.value || 150);
    const z = parseFloat(document.getElementById('sldZ')?.value || 50);
    const phi = parseFloat(document.getElementById('sldPhi')?.value || 0);
    const pct = parseFloat(document.getElementById('sldClaw')?.value || 0);

    // Fixed text readout targets
    document.getElementById('valX').innerText = x + " mm";
    document.getElementById('valY').innerText = y + " mm";
    document.getElementById('valZ').innerText = z + " mm";
    document.getElementById('valPhi').innerText = phi + "°";
    document.getElementById('valClaw').innerText = pct + "%"; // Matches id="valClaw"

    const now = Date.now();
    if (now - lastSliderSendTime >= 50) {
        lastSliderSendTime = now;
        websocket.send(JSON.stringify({
            type: "target",
            x: x, y: y, z: z, phi: phi, claw: pct
        }));
    } else {
        clearTimeout(sliderSendTimer);
        sliderSendTimer = setTimeout(sendLiveSliderData, 50);
    }
}

    // --- OPMODE 2: REMOTE CONTROL MONITOR (NO WEBSOCKET TARGETS SENT) ---
    function pollGamepad() {
        const gamepads = navigator.getGamepads ? navigator.getGamepads() : [];
        let gp = null;

        for (let i = 0; i < gamepads.length; i++) {
            if (gamepads[i] && gamepads[i].connected) {
                gp = gamepads[i];
                break;
            }
        }

        const statusElem = document.getElementById('controller-status');

        if (gp) {
            if (statusElem) {
                statusElem.innerText = `Connected (${gp.id.substring(0, 18)}...)`;
                statusElem.className = "val-active";
            }

            // Read triggers (buttons 6 and 7 in standard Gamepad API)
const leftTrigger  = gp.buttons[6] ? gp.buttons[6].value : 0; // Close claw
const rightTrigger = gp.buttons[7] ? gp.buttons[7].value : 0; // Open claw
let clawPct = Math.max(0, Math.min(100, (rightTrigger - leftTrigger) * 100));

const lx = deadzone(gp.axes[0] || 0);
const ly = deadzone(-(gp.axes[1] || 0));
const rx = deadzone(gp.axes[2] !== undefined ? gp.axes[2] : (gp.axes[3] || 0));
const ltVal = gp.buttons[6] ? gp.buttons[6].value : 0;
const rtVal = gp.buttons[7] ? gp.buttons[7].value : 0;
const dUp    = gp.buttons[12] ? gp.buttons[12].pressed : false;
const dDown  = gp.buttons[13] ? gp.buttons[13].pressed : false;
const dLeft  = gp.buttons[14] ? gp.buttons[14].pressed : false;
const dRight = gp.buttons[15] ? gp.buttons[15].pressed : false;

const rcPayload = {
    type: "rc_input",
    lx: gp.axes[0],
    ly: gp.axes[1],
    rx: gp.axes[2],
    clawPct: clawPct,
    dpadUp: dUp,
    dpadDown: dDown,
    openClaw: rtVal,
    closeClaw: ltVal
};

websocket.send(JSON.stringify(rcPayload));

            // Update Box 1 UI
            const dbgLX = document.getElementById('dbgLX');
            const dbgLY = document.getElementById('dbgLY');
            const dbgRX = document.getElementById('dbgRX');
            const dbgRY = document.getElementById('dbgRY');
            const dbgLT = document.getElementById('dbgLT');
            const dbgRT = document.getElementById('dbgRT');

            if (dbgLX) dbgLX.innerText = lx.toFixed(2);
            if (dbgLY) dbgLY.innerText = ly.toFixed(2);
            if (dbgRX) dbgRX.innerText = rx.toFixed(2);
            if (dbgRY) dbgRY.innerText = ry.toFixed(2);
            if (dbgLT) dbgLT.innerText = ltVal.toFixed(2);
            if (dbgRT) dbgRT.innerText = rtVal.toFixed(2);

            updateDpadDisplay('dbgDUp', dUp);
            updateDpadDisplay('dbgDDown', dDown);
            updateDpadDisplay('dbgDLeft', dLeft);
            updateDpadDisplay('dbgDRight', dRight);

            // Update Box 2 UI
            const step = 1.0;
            if (lx !== 0) targetR = Math.max(BOUNDS.MIN_R, Math.min(BOUNDS.MAX_R, targetR + (lx * step)));
            if (ly !== 0) targetZ = Math.max(BOUNDS.MIN_Z, Math.min(BOUNDS.MAX_Z, targetZ + (ly * step)));
            if (rx !== 0) targetTurret = Math.max(BOUNDS.MIN_TURRET, Math.min(BOUNDS.MAX_TURRET, targetTurret + (rx * step)));

            const ctrlR = document.getElementById('ctrlR');
            const ctrlZ = document.getElementById('ctrlZ');
            const ctrlTurret = document.getElementById('ctrlTurret');
            if (ctrlR) ctrlR.innerText = Math.round(targetR);
            if (ctrlZ) ctrlZ.innerText = Math.round(targetZ);
            if (ctrlTurret) ctrlTurret.innerText = Math.round(targetTurret);

            // *** NOTICE: ZERO websocket.send() CALLS ARE MADE HERE ***
            // The arm remains 100% still while in OpMode 2.

        } else {
            if (statusElem) {
                statusElem.innerText = "Disconnected";
                statusElem.className = "val-inactive";
            }
        }

        requestAnimationFrame(pollGamepad);
    }

    // --- OPMODE 0 & 3 TRANSMISSION FUNCTIONS ---
    function sendNumericalTarget() {
        const x = parseFloat(document.getElementById('numX')?.value || 0);
        const y = parseFloat(document.getElementById('numY')?.value || 0);
        const z = parseFloat(document.getElementById('numZ')?.value || 0);
        const phi = parseFloat(document.getElementById('numPhi')?.value || 0);
        const pct = parseFloat(document.getElementById('numClawPct0')?.value || 0);

        if (websocket && websocket.readyState === WebSocket.OPEN) 
          {
            websocket.send(JSON.stringify({
                type: "target",
                x: x, y: y, z: z, phi: phi, claw: pct   // send raw percent; moveClaw() on the ESP32 does the real mapping
            }));
          }
    }

    function sendManualAngles() {
        const turret   = parseFloat(document.getElementById('num-turret')?.value || 0);
        const shoulder = parseFloat(document.getElementById('num-shoulder')?.value || 0);
        const elbow    = parseFloat(document.getElementById('num-elbow')?.value || 0);
        const wrist    = parseFloat(document.getElementById('num-wrist')?.value || 0);
        const claw     = parseFloat(document.getElementById('num-claw')?.value || 0);

        if (websocket && websocket.readyState === WebSocket.OPEN) {
            websocket.send(JSON.stringify({
                type: "manual_angles",
                turret: turret, shoulder: shoulder, elbow: elbow, wrist: wrist, claw: claw
            }));
        }
    }

    function selectOpMode(mode) {
        currentOpMode = mode;
        const tabNames = ["Manual XYZ", "Live Sliders", "Remote Control", "Angle Testing"];

        for (let i = 0; i < 4; i++) {
            const tab = document.getElementById(`tab${i}`);
            const panel = document.getElementById(`panel${i}`);
            if (tab && panel) {
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
        }

        // Notify ESP32 of OpMode switch
        if (websocket && websocket.readyState === WebSocket.OPEN) {
            websocket.send(JSON.stringify({ type: "opmode", mode: mode }));
        }
    }

    window.addEventListener('load', () => {
    selectOpMode(0);
    initWebSocket();

    // Updated element IDs to match the HTML range inputs
    ['sldX', 'sldY', 'sldZ', 'sldPhi', 'sldClaw', 'clawMinAngle1'].forEach(id => {
        const el = document.getElementById(id);
        if (el) el.addEventListener('input', sendLiveSliderData);
    });

    requestAnimationFrame(pollGamepad);
});
    </script>
</body>
</html>
)rawliteral";

//--------------------------------WEB CODE----------------------------------------

RobotWebServer::RobotWebServer(const char *ssid, const char *password)
  : _ssid(ssid), _password(password), _server(80), _ws("/ws") {
}

// --------------------------------------------------- SETUP --------------------------------------------

void RobotWebServer::onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_DATA) {
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
      data[len] = 0;  // Null-terminate incoming packet
      String msg = String((char *)data);

      // Robust JSON value extraction lambda
      auto getJsonVal = [&msg](const char *key) -> float {
        int idx = msg.indexOf(key);
        if (idx == -1) return 0.0f;
        int start = msg.indexOf(':', idx);
        if (start == -1) return 0.0f;
        start++;  // Skip past ':'
        int end = msg.indexOf(',', start);
        if (end == -1) end = msg.indexOf('}', start);
        if (end == -1) return 0.0f;

        String valStr = msg.substring(start, end);
        valStr.trim();
        if (valStr == "true") return 1.0f;
        if (valStr == "false") return 0.0f;
        return valStr.toFloat();
      };

      // -------------------------------------------------------------
      // 1. Direct Emergency Stop Trigger
      // -------------------------------------------------------------
      if (msg.indexOf("\"type\":\"estop\"") != -1) {
        bool state = (getJsonVal("\"state\"") > 0.5f);
        if (_controller) {
          if (state) {
            _controller->emergencyStop();
          } else {
            _controller->releaseEStop();
          }
        }
        return;
      }

      // Lock out all other incoming web processing if E-stop is active
      if (_controller && _controller->isEStopped()) {
        return;
      }

      // -------------------------------------------------------------
      // 2. Bluetooth Gamepad / RC Inputs
      // -------------------------------------------------------------
      if (msg.indexOf("\"type\":\"rc_input\"") != -1) {
        RCInputs rc{};

        // Extract analog axes
        rc.lx = getJsonVal("\"lx\"");
        rc.ly = getJsonVal("\"ly\"");
        rc.rx = getJsonVal("\"rx\"");

        // Extract buttons
        rc.dpadUp = (getJsonVal("\"dpadUp\"") > 0.5f);
        rc.dpadDown = (getJsonVal("\"dpadDown\"") > 0.5f);
        rc.openClaw = getJsonVal("\"openClaw\"");
        rc.closeClaw = getJsonVal("\"closeClaw\"");

        _rcInputs = rc;
        _hasNewRC = true;
        return;
      }

      // -------------------------------------------------------------
      // 3. Home Arm Command
      // -------------------------------------------------------------
      if (msg.indexOf("\"type\":\"home\"") != -1) {
        _homeRequested = true;
        return;
      }

      // -------------------------------------------------------------
      // 4. Operation Mode Switcher
      // -------------------------------------------------------------
      if (msg.indexOf("\"type\":\"opmode\"") != -1) {
        int modeVal = static_cast<int>(getJsonVal("\"mode\""));
        _currentOpMode = static_cast<OpMode>(modeVal);
        _opModeChanged = true;
        return;
      }

      // -------------------------------------------------------------
      // 5. XYZ Cartesian Target Coordinates
      // -------------------------------------------------------------
      if (msg.indexOf("\"type\":\"target\"") != -1) {
        _targetX = getJsonVal("\"x\"");
        _targetY = getJsonVal("\"y\"");
        _targetZ = getJsonVal("\"z\"");
        _targetPhi = getJsonVal("\"phi\"");
        _targetClaw = getJsonVal("\"claw\"");
        _hasNewCommand = true;
        return;
      }

      // -------------------------------------------------------------
      // 6. Direct Joint Angle Command (Testing Mode)
      // -------------------------------------------------------------
      if (msg.indexOf("\"type\":\"manual_angles\"") != -1) {
        float t = getJsonVal("\"turret\"");
        float s = getJsonVal("\"shoulder\"");
        float e = getJsonVal("\"elbow\"");
        float w = getJsonVal("\"wrist\"");
        float c = getJsonVal("\"claw\"");
        _manualAngles = { t, s, e, w, c };
        _hasNewManualCommand = true;
        return;
      }
    }
  }
}

void RobotWebServer::beginWebServer() {
  WiFi.softAP(_ssid, _password);

  Serial.print("[WebServer] Access Point Started! IP: ");
  Serial.println(WiFi.softAPIP());

  // Bind WebSocket event listener
  _ws.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    this->onWsEvent(server, client, type, arg, data, len);
  });

  _server.addHandler(&_ws);

  _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", HTML_PAGE);
  });

  _server.begin();
  Serial.println("[WebServer] HTTP Server Listening.");
}

// ------------------------------------------- UPDATE ------------------------------------

void RobotWebServer::updateWebServer() {
  _ws.cleanupClients();
}

// ----------------------------- SEND TELEMETRY --------------------------------------------

void RobotWebServer::sendTelemetry(float x, float y, float z, float phi, float turretDeg, float shoulderDeg, float elbowDeg, float wristDeg, float clawDeg) {
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

void RobotWebServer::log(const String &message) {
  // Also print to actual USB Serial for debugging during dev
  Serial.println(message);

  // Prefix with [LOG] so the HTML JavaScript knows it's a console string
  String payload = "[LOG] " + message;

  // Broadcast to all connected WebSocket clients
  _ws.textAll(payload);
}

void RobotWebServer::logf(const char *format, ...) {
  char buffer[256];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);

  log(String(buffer));
}

void RobotWebServer::updateTelemetry() 
  {
    // Safety check: ensure controller pointer is valid
    if (!_controller) return;

    unsigned long currentMillis = millis();

    // 20 Hz Telemetry Update (every 50ms)
    if (currentMillis - _lastTelemetry >= 50) 
      {
        _lastTelemetry = currentMillis;

        float curX, curY, curZ, curPhi;
        float t, s, e, w, c;

        // Pull current telemetry directly from the controller reference
        _controller->getCurrentPose(curX, curY, curZ, curPhi);
        _controller->getCurrentAngles(t, s, e, w, c);

        // Send over WebSocket connection
        sendTelemetry(curX, curY, curZ, curPhi, t, s, e, w, c);
      }
  }

// --------------------------- NEW TARGET --------------------------------

bool RobotWebServer::getNewTarget(float &x, float &y, float &z, float &phi, float &claw) {
  if (_hasNewCommand) {
    x = _targetX;
    y = _targetY;
    z = _targetZ;
    phi = _targetPhi;
    claw = _targetClaw;
    _hasNewCommand = false;  // Reset flag after reading
    return true;
  }

  return false;
}

bool RobotWebServer::getManualAngles(float &turret, float &shoulder, float &elbow, float &wrist, float &claw) {
  turret = _manualAngles.turret;
  shoulder = _manualAngles.shoulder;
  elbow = _manualAngles.elbow;
  wrist = _manualAngles.wrist;
  claw = _manualAngles.claw;
  bool hasNew = _hasNewManualCommand;
  _hasNewManualCommand = false;
  return hasNew;
}

bool RobotWebServer::getRCInputs(RCInputs &outRC) {
  if (_hasNewRC) {
    _hasNewRC = false;
    outRC = _rcInputs;
    return true;
  }
  return false;
}

// ----------------------------- CHECKING THINGS FROM WEB INTERFACE -----------------------------

bool RobotWebServer::checkHomeRequest() {
  if (_homeRequested) {
    _homeRequested = false;
    return true;
  }
  return false;
}

bool RobotWebServer::checkOpModeChanged(OpMode &newMode) {
  if (_opModeChanged) {
    newMode = _currentOpMode;
    _opModeChanged = false;
    return true;
  }
  return false;
}