#include <WiFi.h>
#include "DHT.h"

// Pin Definitions
#define DHTPIN 2
#define DHTTYPE DHT11
#define RELAY_PIN 3 // IO3D2

// UART Definitions
#define RXD2 44  // IO9 (A4)
#define TXD2 43 // IO10 (A5) (optional, not used here)

DHT dht(DHTPIN, DHTTYPE);
HardwareSerial mySerial(2); // Use UART2

// WiFi AP credentials
const char *ssid = "AirPurifier";
const char *password = "12345678";
WiFiServer server(80);

// Sensor values
float ppm1 = 0, ppm2 = 0, avg_ppm = 0;
float h = 0, t = 0;
bool fanAuto = true;
bool fanManualState = false;
bool fanState = false;
float baseline_ppm = 0;
#define BASELINE_READS 10

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Fan OFF

  // UART2 for Arduino communication
  mySerial.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // WiFi AP
  WiFi.softAP(ssid, password);
  server.begin();
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Baseline for MQ135
  float total = 0;
  int count = 0;
  while (count < BASELINE_READS) {
    if (mySerial.available()) {
      String line = mySerial.readStringUntil('\n');
      int comma = line.indexOf(',');
      if (comma > 0) {
        float p1 = line.substring(0, comma).toFloat();
        float p2 = line.substring(comma + 1).toFloat();
        total += (p1 + p2) / 2.0;
        count++;
      }
    }
    delay(500);
  }
  baseline_ppm = total / BASELINE_READS;
  Serial.print("Baseline PPM: ");
  Serial.println(baseline_ppm);
}

void loop() {
  // Read DHT11
  h = dht.readHumidity();
  t = dht.readTemperature();

  // Read MQ135 PPM from Arduino
  if (mySerial.available()) {
    String line = mySerial.readStringUntil('\n');
    int comma = line.indexOf(',');
    if (comma > 0) {
      ppm1 = line.substring(0, comma).toFloat();
      ppm2 = line.substring(comma + 1).toFloat();
      avg_ppm = (ppm1 + ppm2) / 2.0;
    }
  }

  // Fan control logic
  bool autoFan = (avg_ppm > baseline_ppm + 200); // Adjust offset as needed
  fanState = fanAuto ? autoFan : fanManualState;
  digitalWrite(RELAY_PIN, fanState ? LOW : HIGH);

  // Web server
  WiFiClient client = server.available();
  if (client) {
    String req = client.readStringUntil('\r');
    client.flush();

    // Manual control
    if (req.indexOf("/fan/on") != -1) {
      fanAuto = false; fanManualState = true;
    }
    if (req.indexOf("/fan/off") != -1) {
      fanAuto = false; fanManualState = false;
    }
    if (req.indexOf("/fan/auto") != -1) {
      fanAuto = true;
    }

    // HTML page
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.println("<html><head><title>Air Purifier</title></head><body>");
    client.println("<h2>Air Purifier Dashboard</h2>");
    client.print("<p><b>Humidity:</b> "); client.print(h); client.println("%</p>");
    client.print("<p><b>Temperature:</b> "); client.print(t); client.println(" &deg;C</p>");
    client.print("<p><b>MQ135 #1 (PPM):</b> "); client.print(ppm1, 1);
    client.print(" | <b>MQ135 #2 (PPM):</b> "); client.print(ppm2, 1);
    client.print(" | <b>Average (PPM):</b> "); client.print(avg_ppm, 1); client.println("</p>");
    client.print("<p><b>Fan Mode:</b> ");
    client.print(fanAuto ? "AUTO" : "MANUAL");
    client.print(" | <b>Status:</b> ");
    client.print((fanState) ? "<span style='color:green'>ON</span>" : "<span style='color:red'>OFF</span>");
    client.println("</p>");
    client.println("<form action='/fan/on'><button type='submit'>Manual ON</button></form>");
    client.println("<form action='/fan/off'><button type='submit'>Manual OFF</button></form>");
    client.println("<form action='/fan/auto'><button type='submit'>Auto Mode</button></form>");
    client.println("</body></html>");
    client.stop();
  }
  delay(500);
}
