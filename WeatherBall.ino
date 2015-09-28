#include <ESP8266WiFi.h>

const char *ssid = "";
const char *password = "";
const char *key = "";

const char* host = "api.openweathermap.org";

void setup() {
  //Initialize serial
  Serial.begin(115200);

  // attempt to connect to Wifi network:
  while (true) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);

    // Connect to WEP/WPA/WPA2 network:
    WiFi.begin(ssid, password);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
      break;
    }
    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network ");
  printCurrentNet();
  printWifiData();

}

void loop() {
  // check the network connection once every 10 seconds:
  delay(10000);
  printCurrentNet();
  printWifiData();

  Serial.print("Connecting to ");
  Serial.println(host);

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  String url = "/data/2.5/weather?id=5231851&APPID=" + key;

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  delay(10);

  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}

void printWifiData() {
  // print your ESP8266 IP address:
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // print your MAC address:
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());

}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

}
