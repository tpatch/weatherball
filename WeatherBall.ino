#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

int led = 3;
int color = 0;
int condition = 0;

const char *ssid = "";
const char *password = "";
const char *key = "";
const char* host = "api.openweathermap.org";

void setup() {
  //Initialize serial
  Serial.begin(115200);

  pinMode(led, OUTPUT);
  
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

}

void loop() {
  // check the network connection once every 10 seconds:
  delay(10000);

  // Testing LED w/ random output
  color = random (1,1023);
  analogWrite(led, color);

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
  String jsonString = "";
  
  while(client.available()){
    char c = client.read();
    if (c == '{') {
      openingBracket = true;
    }
    if (openingBracket) {
      if (c == '\n') {
          Serial.println(jsonString);
          decodeJSON(jsonString);
          break;
      }
      
      // Read out JSON string
      jsonString += c;
    }
  }

  Serial.println("Closing connection.");
}

void decodeJSON(String jsonString) {
  char json[jsonString.length() + 1];
  jsonString.toCharArray(json, jsonString.length() + 1);
  Serial.println(json);
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  
  condition = root["weather"][1];
  Serial.println("Condition: " + condition);
  changeWeather(condition);
}

void changeWeather(int condition) {
  if ( condition > 199 && condition < 233 ) {
    //T-storm
    color = 0;

  } else if ( condition > 299 && condition < 322 ) {
    //Drizzle
    color = 0;

  } else if ( condition > 499 && condition < 532 ) {
    //Rain
    color = 0;

  } else if ( condition > 599 && condition < 623 ) {
    //Snow
    color = 0;

  } else if ( condition > 700 && condition < 782 ) {
    //Atmosphere
    color = 0;

  } else if ( condition > 799 && condition < 805 ) {
    //Cloudy states

    if ( condition == 800 ) {
      //Clear
      color = 0;

    } else if ( condition > 800 && condition < 804 ) {
      //Few/scattered/broken
      color = 0;

    } else {
      //Overcast
      color = 0;

    };
  } else if ( condition > 899 && condition < 907 ) {
    //Extreme
    color = 0;
    
  } else if ( condition > 955 && condition < 960 ) {
    //Windy
    color = 0;
    
  };

  analogWrite(led, color);
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
