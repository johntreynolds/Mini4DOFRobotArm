**4 Degree of Free Robot Arm**

<hr>

<img width="1270" height="1056" alt="image" src="https://github.com/user-attachments/assets/61fdbec2-639f-4fc4-987a-0966982bfe4a" />

<hr>

This robot arm is built using 3 Bristol Bot Builders Servos for the turret, shoulder, and elbow. These are made for direct 2S lipo connections. It uses 2 metal-geared servos for the wrist and claw.
The arm is controlled using an Inverse Kinematics algorithm in addition to a PD Controller. A Direct Kinematics algorithm is used in order to help calculate telemetry at every point. I ran 2S right into the PCA Servo Driver. I ran a 5V BEC to the electronics power and a 3.3V BEC to the 3.3V port of the ESP32.

An Arduino Nano Clone was used in the beginning of the project before I decided to switch to a board that could support Bluetooth an WIFI.

The CAD model is provided in the main repository files.

<hr>

**Parts Links:**

[Arduino (used in the beginning of the project)](https://www.amazon.com/dp/B08PZ36KBX?ref=fed_asin_title&th=1) <br>
[ESP32 DEVKITV1 Board](https://www.amazon.com/dp/B09XDMVS9N?ref=fed_asin_title&th=1) <br>
[PCA Servo Driver](https://www.amazon.com/dp/B07BRS249H?ref=fed_asin_title) <br>
[Mini 9V Servos](https://www.amazon.com/dp/B0BWJ26PX2?ref=fed_asin_title&th=1) <br>
[Bristol Bot Builders Servos](https://shop.bristolbotbuilders.com/product/servo/) <br>
[5V BEC](https://www.amazon.com/dp/B0FCXTHX6W?ref=fed_asin_title) <br>
[3.3V BEC](https://www.amazon.com/dp/B0BVFYXPQ8?ref=fed_asin_title&th=1) <br>
[2S Battery](https://palmbeachbots.com/products/palm-power-2s-250mah-45c-lipo-battery?_pos=6&_fid=49acd3ecf&_ss=c)

<hr>

The code opens a separate web interface that includes an E Stop, Homing Button, and Telemetry. 

//Placeholder for top row of buttons and telemetry

<hr>

**There are four OpModes that can be accessed in the web interface:**

Manual XYZ:

//placeholder

<hr>

Live Sliders:

//placeholder

<hr>

RC Control

//placeholder

<hr>

Manual Angle.

//placeholder

<hr>

The RC mode utilizes a standard Bluetooth controller (Xbox, PS, Switch).

<img width="672" height="433" alt="image" src="https://github.com/user-attachments/assets/cdb4813c-202f-4f8c-84ec-f77965ee2c0a" />

<hr>

Here is a link to the PDF doc I utilized for the Inverse Kinematics: [ROBOT Arm Math And Planning.pdf](https://github.com/user-attachments/files/31055481/ROBOT.Arm.Math.And.Planning.pdf)

<hr>

*Note that a good portion of the code was vibe-coded using a combination of Gemini, Copilot, and Claude. Especially the Web Interface, "RobotWebServer".*
