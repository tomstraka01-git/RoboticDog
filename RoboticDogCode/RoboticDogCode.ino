#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <WiFi.h>
#include <WebServer.h>

const char* WIFI_SSID     = "YOUR_SSID";
const char* WIFI_PASSWORD = "YOUR_PASS";

#define SERVO_MIN  102
#define SERVO_MAX  512
#define SERVO_FREQ 50
#define I2C_SDA    0
#define I2C_SCL    1

#define LEG_FL_HIP   0
#define LEG_FL_KNEE  1
#define LEG_FR_HIP   2
#define LEG_FR_KNEE  3
#define LEG_BL_HIP   4
#define LEG_BL_KNEE  5
#define LEG_BR_HIP   6
#define LEG_BR_KNEE  7

Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver(0x40);
WebServer server(80);
int servoAngles[8];



void setServoAngle(uint8_t ch, int angle) {
  angle = constrain(angle, 0, 180);
  pca.setPWM(ch, 0, map(angle, 0, 180, SERVO_MIN, SERVO_MAX));
  servoAngles[ch] = angle;
}


void setAllLegs(int hipAngle, int kneeAngle) {
  setServoAngle(LEG_FL_HIP,  hipAngle);
  setServoAngle(LEG_FL_KNEE, kneeAngle);
  setServoAngle(LEG_FR_HIP,  hipAngle);
  setServoAngle(LEG_FR_KNEE, kneeAngle);
  setServoAngle(LEG_BL_HIP,  hipAngle);
  setServoAngle(LEG_BL_KNEE, kneeAngle);
  setServoAngle(LEG_BR_HIP,  hipAngle);
  setServoAngle(LEG_BR_KNEE, kneeAngle);
}


void stand() {
  setAllLegs(90, 90);
}

void sit() {
  setServoAngle(LEG_FL_HIP,  90);  setServoAngle(LEG_FL_KNEE, 130);
  setServoAngle(LEG_FR_HIP,  90);  setServoAngle(LEG_FR_KNEE, 130);
  setServoAngle(LEG_BL_HIP,  90);  setServoAngle(LEG_BL_KNEE, 60);
  setServoAngle(LEG_BR_HIP,  90);  setServoAngle(LEG_BR_KNEE, 60);
}








void moveForward(int steps = 4) {
  for (int s = 0; s < steps; s++) {
  
    setServoAngle(LEG_FL_HIP,  110); setServoAngle(LEG_FL_KNEE, 70);  
    setServoAngle(LEG_BR_HIP,  110); setServoAngle(LEG_BR_KNEE, 70);  
    setServoAngle(LEG_FR_HIP,  80);  setServoAngle(LEG_FR_KNEE, 100); 
    setServoAngle(LEG_BL_HIP,  80);  setServoAngle(LEG_BL_KNEE, 100); 
    delay(200);


    setServoAngle(LEG_FL_HIP,  90);  setServoAngle(LEG_FL_KNEE, 90);
    setServoAngle(LEG_BR_HIP,  90);  setServoAngle(LEG_BR_KNEE, 90);
    setServoAngle(LEG_FR_HIP,  90);  setServoAngle(LEG_FR_KNEE, 90);
    setServoAngle(LEG_BL_HIP,  90);  setServoAngle(LEG_BL_KNEE, 90);
    delay(150);


    setServoAngle(LEG_FR_HIP,  110); setServoAngle(LEG_FR_KNEE, 70);   
    setServoAngle(LEG_BL_HIP,  110); setServoAngle(LEG_BL_KNEE, 70);  
    setServoAngle(LEG_FL_HIP,  80);  setServoAngle(LEG_FL_KNEE, 100); 
    setServoAngle(LEG_BR_HIP,  80);  setServoAngle(LEG_BR_KNEE, 100);  
    delay(200);

   
    stand();
    delay(150);
  }
}


void moveBackward(int steps = 4) {
  for (int s = 0; s < steps; s++) {
    setServoAngle(LEG_FL_HIP,  70);  setServoAngle(LEG_FL_KNEE, 70);
    setServoAngle(LEG_BR_HIP,  70);  setServoAngle(LEG_BR_KNEE, 70);
    setServoAngle(LEG_FR_HIP,  100); setServoAngle(LEG_FR_KNEE, 100);
    setServoAngle(LEG_BL_HIP,  100); setServoAngle(LEG_BL_KNEE, 100);
    delay(200);
    stand(); delay(150);

    setServoAngle(LEG_FR_HIP,  70);  setServoAngle(LEG_FR_KNEE, 70);
    setServoAngle(LEG_BL_HIP,  70);  setServoAngle(LEG_BL_KNEE, 70);
    setServoAngle(LEG_FL_HIP,  100); setServoAngle(LEG_FL_KNEE, 100);
    setServoAngle(LEG_BR_HIP,  100); setServoAngle(LEG_BR_KNEE, 100);
    delay(200);
    stand(); delay(150);
  }
}


void turnLeft(int steps = 3) {
  for (int s = 0; s < steps; s++) {
    setServoAngle(LEG_FL_HIP,  70);  setServoAngle(LEG_FR_HIP,  110);
    setServoAngle(LEG_BL_HIP,  70);  setServoAngle(LEG_BR_HIP,  110);
    delay(250);
    stand(); delay(150);
  }
}


void turnRight(int steps = 3) {
  for (int s = 0; s < steps; s++) {
    setServoAngle(LEG_FR_HIP,  70);  setServoAngle(LEG_FL_HIP,  110);
    setServoAngle(LEG_BR_HIP,  70);  setServoAngle(LEG_BL_HIP,  110);
    delay(250);
    stand(); delay(150);
  }
}






void handleServo() {
  int ch    = server.arg("ch").toInt();
  int angle = server.arg("angle").toInt();
  setServoAngle(ch, angle);
  server.send(200, "application/json",
    "{\"ch\":" + String(ch) + ",\"angle\":" + String(angle) + "}");
}

void handleMove() {
  String cmd = server.arg("cmd");
  int steps = server.arg("steps").toInt();
  if (steps <= 0) steps = 4;

  if (cmd == "forward") moveForward(steps);
  else if (cmd == "backward") moveBackward(steps);
  else if (cmd == "left") turnLeft(steps);
  else if (cmd == "right") turnRight(steps);
  else if (cmd == "stand") stand();
  else if (cmd == "sit") sit();
  else if (cmd == "stop") stop();

  server.send(200, "application/json", "{\"cmd\":\"" + cmd + "\"}");
}

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html><html><body>
<h2>Dog</h2>
<button onclick="cmd('forward')">Forward</button><br>
<button onclick="cmd('left')">Left</button>
<button onclick="cmd('right')">Right</button><br>
<button onclick="cmd('backward')">Backward</button><br><br>
<button onclick="cmd('stop')">Stop</button>
<button onclick="cmd('stand')">Stand</button>
<button onclick="cmd('sit')">Sit</button>
<script>function cmd(n){fetch('/move?cmd='+n+'&steps=4')}</script>
</body></html>
)rawliteral";
  server.send(200, "text/html", html);
}



void setup() {
  Serial.begin(115200);
  Wire.setSDA(I2C_SDA); Wire.setSCL(I2C_SCL); Wire.begin();
  pca.begin();
  pca.setOscillatorFrequency(27000000);
  pca.setPWMFreq(SERVO_FREQ);
  for (int i = 0; i < 8; i++) setServoAngle(i, 90);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nhttp://" + WiFi.localIP().toString());

  server.on("/servo", handleServo);
  server.on("/move", handleMove);  
  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}