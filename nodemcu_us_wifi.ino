#include <ESP8266WiFi.h>
#define TRIGGER_PIN  5  //D1
#define ECHO_PIN     4  //D2
#define SENSOR_HUJAN 16 //D0

const char* ssid     = "Aby";
const char* password = "wicaksono29";

const char* host = "192.168.43.45";

WiFiClient client;
const int httpPort = 80;
String url;
long duration, distance;
unsigned long timeout;
  
void setup() {
  Serial.begin(9600);
  delay(10);
  
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode (SENSOR_HUJAN, INPUT);

  
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void baca_jarak(){
  digitalWrite(TRIGGER_PIN, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration/2) / 29.1;
  Serial.print(distance);
  Serial.println(" cm");
}

void loop() {
  Serial.print("baca jarak ");
  baca_jarak();

  int kondisi_sensor = digitalRead(SENSOR_HUJAN);
  if (kondisi_sensor == LOW)
  Serial.println ("Hujan");
  else 
  Serial.println ("tidak hujan");
  delay (1000);
  
  Serial.print("connecting to ");
  Serial.println(host);

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    //return;
  }

// We now create a URI for the request
  url = "/belajar/index.php/simpan/sensor?data=";
 url += distance;
 url += "&kondisi=";
 url += kondisi_sensor;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);

// This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

// Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
  Serial.println();
  delay(5000);
}
