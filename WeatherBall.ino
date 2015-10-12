#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

#define led 2
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, led, NEO_GRB + NEO_KHZ800);

int r = 0;
int g = 0;
int b = 0;
//int condition = 0;

const char *ssid = "";
const char *password = "";
const char *host = "api.openweathermap.org";

void setup() {
  //Initialize serial
  Serial.begin(115200);
  
  // attempt to connect to Wifi network:
  while (true) {
    // Connect to WEP/WPA/WPA2 network:
    WiFi.begin(ssid, password);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
      break;
    }
    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  // Serial.print("You're connected to the network ");
  // printCurrentNet();
  // printWifiData();

  strip.begin();
  strip.setPixelColor(0, strip.Color(255,255,0));
  strip.show();
}

void loop() {
  // check the network connection once every 10 seconds:
  delay(10000);

  Serial.print("Connecting to ");
  Serial.println(host);

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // Set and get our weather data
  String url = "/data/2.5/weather?id=5231851&APPID=";
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  delay(10);

  // Parse our weather data
  boolean openingBracket = false;
  char json[75] = "{";
  int n = 1;

  while (!client.find("\"coord\":{")){}
  
  while(client.available()){
    char c = client.read();
    if (openingBracket) {
      if (c == ']') {
          break;
      }
      
      // Read out JSON string
      json[n] = c;
      n++;
    }
    if (c == '{') {
      openingBracket = true;
    }
  }

  Serial.println(json);
  StaticJsonBuffer<150> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  } else {
    Serial.println("JSON Successfully parsed!");
  }

  const char *cond = root["id"];
  int condition = atoi(cond);
  Serial.println(condition);
  changeWeather(condition);

  Serial.println("Closing connection.");
}

void changeWeather(int condition) {
  Serial.println(condition);
  if ( condition > 199 && condition < 233 ) {
    //T-storm
    r = 255;
    g = 0;
    b = 0;

  } else if ( condition > 299 && condition < 322 ) {
    //Drizzle
    r = 0;
    g = 0;
    b = 200;

  } else if ( condition > 499 && condition < 532 ) {
    //Rain
    r = 0;
    g = 0;
    b = 255;

  } else if ( condition > 599 && condition < 623 ) {
    //Snow
    r = 250;
    g = 250;
    b = 250;

  } else if ( condition > 700 && condition < 782 ) {
    //Atmosphere
    r = 0;
    g = 0;
    b = 0;

  } else if ( condition > 799 && condition < 805 ) {
    //Cloudy states

    if ( condition == 800 ) {
      //Clear
      r = 255;
      g = 255;
      b = 0;

    } else if ( condition > 800 && condition < 804 ) {
      //Few/scattered/broken
      r = 102;
      g = 153;
      b = 204;

    } else {
      //Overcast
      r = 102;
      g = 153;
      b = 204;

    };
  } else if ( condition > 899 && condition < 907 ) {
    //Extreme
    r = 0;
    g = 0;
    b = 0;
    
  } else if ( condition > 955 && condition < 960 ) {
    //Windy
    r = 0;
    g = 0;
    b = 0;
  } else {
    r = 255;
    g = 165;
    b = 0;
  };

  Serial.println(r);
  strip.setPixelColor(0, strip.Color(r, g, b));
  strip.show();
};

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
