#include <WiFi.h>
#include <WebServer.h>

#define TRIG_PIN 5
#define ECHO_PIN 18

const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

WebServer server(80);

float readDistance()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0)
    return -1;

  return duration * 0.0343 / 2.0;
}

String htmlPage()
{
  return R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Live Dashboard</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      background: #f2f2f2;
      margin: 0;
      padding: 40px 20px;
    }
    .card {
      max-width: 500px;
      margin: auto;
      padding: 30px;
      background: white;
      border-radius: 16px;
      box-shadow: 0 4px 15px rgba(0,0,0,0.15);
    }
    h1 {
      margin-bottom: 10px;
    }
    .label {
      color: #666;
      font-size: 18px;
    }
    #distance {
      font-size: 56px;
      font-weight: bold;
      margin: 20px 0;
    }
    .unit {
      font-size: 24px;
      color: #555;
    }
    .status {
      color: #16803c;
      font-size: 15px;
    }
  </style>
</head>
<body>
  <div class="card">
    <h1>ESP32 Live Dashboard</h1>
    <div class="label">HC-SR04 Distance</div>
    <div id="distance">--</div>
    <div class="unit">cm</div>
    <p class="status">Live reading - refreshes every 1 second</p>
  </div>

  <script>
    async function updateDistance() {
      try {
        const response = await fetch('/data');
        const data = await response.json();

        if (data.distance < 0) {
          document.getElementById('distance').textContent = 'No reading';
        } else {
          document.getElementById('distance').textContent =
            data.distance.toFixed(2);
        }
      } catch (error) {
        document.getElementById('distance').textContent = 'Offline';
      }
    }

    updateDistance();
    setInterval(updateDistance, 1000);
  </script>
</body>
</html>
)rawliteral";
}

void handleRoot()
{
  server.send(200, "text/html", htmlPage());
}

void handleData()
{
  float distance = readDistance();

  String json = "{\"distance\":";
  json += String(distance, 2);
  json += "}";

  server.send(200, "application/json", json);
}

void setup()
{
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("Connecting to WiFi...");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("Open the dashboard at: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/data", handleData);

  server.begin();
  Serial.println("Web server started.");
}

void loop()
{
  server.handleClient();
}