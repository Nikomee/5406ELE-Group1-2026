#include <WiFi.h>
#include <WebServer.h>

// =========================
// WIFI HOTSPOT SETTINGS
// =========================
const char* apSSID = "Rover-ESP32";
const char* apPassword = "12345678";

WebServer server(80);

// =========================
// MOTOR PIN DEFINITIONS
// =========================

// Front Left Motor
#define FL_IN1 4
#define FL_IN2 5

// Front Right Motor
#define FR_IN1 6
#define FR_IN2 7

// Rear Left Motor
#define RL_IN1 15
#define RL_IN2 16

// Rear Right Motor
#define RR_IN1 17
#define RR_IN2 18

// =========================
// BASIC MOTOR FUNCTIONS
// =========================
void motorForward(int in1, int in2) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}

void motorBackward(int in1, int in2) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

void motorStop(int in1, int in2) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

// =========================
// MOVEMENT FUNCTIONS
// =========================
void stopMotors() {
  motorStop(FL_IN1, FL_IN2);
  motorStop(FR_IN1, FR_IN2);
  motorStop(RL_IN1, RL_IN2);
  motorStop(RR_IN1, RR_IN2);
}

void moveForward() {
  motorForward(FL_IN1, FL_IN2);
  motorForward(FR_IN1, FR_IN2);
  motorForward(RL_IN1, RL_IN2);
  motorForward(RR_IN1, RR_IN2);
}

void moveBackward() {
  motorBackward(FL_IN1, FL_IN2);
  motorBackward(FR_IN1, FR_IN2);
  motorBackward(RL_IN1, RL_IN2);
  motorBackward(RR_IN1, RR_IN2);
}

void strafeRight() {
  motorForward(FL_IN1, FL_IN2);
  motorBackward(FR_IN1, FR_IN2);
  motorBackward(RL_IN1, RL_IN2);
  motorForward(RR_IN1, RR_IN2);
}

void strafeLeft() {
  motorBackward(FL_IN1, FL_IN2);
  motorForward(FR_IN1, FR_IN2);
  motorForward(RL_IN1, RL_IN2);
  motorBackward(RR_IN1, RR_IN2);
}

void rotateRight() {
  motorForward(FL_IN1, FL_IN2);
  motorBackward(FR_IN1, FR_IN2);
  motorForward(RL_IN1, RL_IN2);
  motorBackward(RR_IN1, RR_IN2);
}

void rotateLeft() {
  motorBackward(FL_IN1, FL_IN2);
  motorForward(FR_IN1, FR_IN2);
  motorBackward(RL_IN1, RL_IN2);
  motorForward(RR_IN1, RR_IN2);
}

void diagFrontRight() {
  motorForward(FL_IN1, FL_IN2);
  motorStop(FR_IN1, FR_IN2);
  motorStop(RL_IN1, RL_IN2);
  motorForward(RR_IN1, RR_IN2);
}

void diagFrontLeft() {
  motorStop(FL_IN1, FL_IN2);
  motorForward(FR_IN1, FR_IN2);
  motorForward(RL_IN1, RL_IN2);
  motorStop(RR_IN1, RR_IN2);
}

void diagBackRight() {
  motorStop(FL_IN1, FL_IN2);
  motorBackward(FR_IN1, FR_IN2);
  motorBackward(RL_IN1, RL_IN2);
  motorStop(RR_IN1, RR_IN2);
}

void diagBackLeft() {
  motorBackward(FL_IN1, FL_IN2);
  motorStop(FR_IN1, FR_IN2);
  motorStop(RL_IN1, RL_IN2);
  motorBackward(RR_IN1, RR_IN2);
}

// =========================
// HTML PAGE (TEXT BUTTONS)
// =========================
String htmlPage() {
  String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>ESP32 Rover Control</title>
<style>
body { font-family: Arial; text-align: center; background: #111; color: white; }
.grid {
  display: grid;
  grid-template-columns: repeat(3, 120px);
  gap: 10px;
  justify-content: center;
  margin-top: 30px;
}
button {
  width: 120px;
  height: 70px;
  font-size: 14px;
  font-weight: bold;
  border-radius: 10px;
  border: none;
  background: #333;
  color: white;
}
button:active { background: #555; }
.stop { background: red; }
</style>
</head>

<body>
<h2>ROVER CONTROL</h2>
<p>Hold button to move</p>

<div class="grid">
  <button onmousedown="sendCmd('diagFL')" onmouseup="sendCmd('stop')" ontouchstart="sendCmd('diagFL')" ontouchend="sendCmd('stop')">FRONT LEFT</button>
  <button onmousedown="sendCmd('forward')" onmouseup="sendCmd('stop')" ontouchstart="sendCmd('forward')" ontouchend="sendCmd('stop')">FORWARD</button>
  <button onmousedown="sendCmd('diagFR')" onmouseup="sendCmd('stop')" ontouchstart="sendCmd('diagFR')" ontouchend="sendCmd('stop')">FRONT RIGHT</button>

  <button onmousedown="sendCmd('left')" onmouseup="sendCmd('stop')" ontouchstart="sendCmd('left')" ontouchend="sendCmd('stop')">LEFT</button>
  <button class="stop" onclick="sendCmd('stop')">STOP</button>
  <button onmousedown="sendCmd('right')" onmouseup="sendCmd('stop')" ontouchstart="sendCmd('right')" ontouchend="sendCmd('stop')">RIGHT</button>

  <button onmousedown="sendCmd('diagBL')" onmouseup="sendCmd('stop')" ontouchstart="sendCmd('diagBL')" ontouchend="sendCmd('stop')">BACK LEFT</button>
  <button onmousedown="sendCmd('backward')" onmouseup="sendCmd('stop')" ontouchstart="sendCmd('backward')" ontouchend="sendCmd('stop')">BACKWARD</button>
  <button onmousedown="sendCmd('diagBR')" onmouseup="sendCmd('stop')" ontouchstart="sendCmd('diagBR')" ontouchend="sendCmd('stop')">BACK RIGHT</button>
</div>

<div class="grid" style="margin-top: 20px;">
  <button onmousedown="sendCmd('rotleft')" onmouseup="sendCmd('stop')" ontouchstart="sendCmd('rotleft')" ontouchend="sendCmd('stop')">ROTATE LEFT</button>
  <div></div>
  <button onmousedown="sendCmd('rotright')" onmouseup="sendCmd('stop')" ontouchstart="sendCmd('rotright')" ontouchend="sendCmd('stop')">ROTATE RIGHT</button>
</div>

<script>
function sendCmd(cmd) {
  fetch("/cmd?move=" + cmd);
}
document.addEventListener("mouseup", () => sendCmd('stop'));
document.addEventListener("touchend", () => sendCmd('stop'));
</script>

</body>
</html>
)rawliteral";
  return page;
}

// =========================
// WEB HANDLERS
// =========================
void handleRoot() {
  server.send(200, "text/html", htmlPage());
}

void handleCommand() {
  String cmd = server.arg("move");

  if (cmd == "forward") moveForward();
  else if (cmd == "backward") moveBackward();
  else if (cmd == "left") strafeLeft();
  else if (cmd == "right") strafeRight();
  else if (cmd == "rotleft") rotateLeft();
  else if (cmd == "rotright") rotateRight();
  else if (cmd == "diagFL") diagFrontLeft();
  else if (cmd == "diagFR") diagFrontRight();
  else if (cmd == "diagBL") diagBackLeft();
  else if (cmd == "diagBR") diagBackRight();
  else stopMotors();

  server.send(200, "text/plain", "OK");
}

// =========================
// SETUP
// =========================
void setup() {
  Serial.begin(115200);

  pinMode(FL_IN1, OUTPUT);
  pinMode(FL_IN2, OUTPUT);
  pinMode(FR_IN1, OUTPUT);
  pinMode(FR_IN2, OUTPUT);
  pinMode(RL_IN1, OUTPUT);
  pinMode(RL_IN2, OUTPUT);
  pinMode(RR_IN1, OUTPUT);
  pinMode(RR_IN2, OUTPUT);

  stopMotors();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSSID, apPassword);

  Serial.println("Hotspot started");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/cmd", handleCommand);
  server.begin();
}

// =========================
// LOOP
// =========================
void loop() {
  server.handleClient();
}