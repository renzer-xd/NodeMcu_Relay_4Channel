// Manuscript: https://github.com/Jadoi/esp8266_Netpie
// Author: Jadoi [https://github.com/Jadoi]
// Modifier: RenZer Developer [https://github.com/renzer-xd]


#include <ESP8266WiFi.h>  //เป็นการเรียกใช้ Library โมดูล ESP8266 * บอร์ท esp8266 เลือกเวอร์ชั่นต่ำๆ 2.7.4
#include <WiFiManager.h>
#include <PubSubClient.h>

const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "9d52271a-8082-4ad7-b48e-7bb96753da46";
const char* mqtt_username = "cnrR6jexpyMqaPbso94og7kdmLzB3GEX";
const char* mqtt_password = "JwUXhpcQ!tQFFeRKWN#!Hx~y94WXcjYx";
WiFiClient espClient;
PubSubClient client(espClient);

char msg[100];

int Pi1 = 0, Pi2 = 0, Pi3 = 0, Pi4 = 0;

#define Relay1 4   // กำหนด D2 คือขา 4
#define Relay2 0   // กำหนด D3 คือขา 0
#define Relay3 14  // กำหนด D5 คือขา 14
#define Relay4 12  // กำหนด D6 คือขา 12

#define Channel1 Relay1
#define Channel2 Relay2
#define Channel3 Relay3
#define Channel4 Relay4

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("@msg/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  String tpc;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];
  }
  Serial.println(message);
  tpc = getMsg(topic, message);
  tpc.toCharArray(msg, (tpc.length() + 1));
  client.publish("@shadow/data/update", msg);
}

void setup() {
  pinMode(Channel1, OUTPUT);
  pinMode(Channel2, OUTPUT);
  pinMode(Channel3, OUTPUT);
  pinMode(Channel4, OUTPUT);

  digitalWrite(Channel1, LOW);
  digitalWrite(Channel2, LOW);
  digitalWrite(Channel3, LOW);
  digitalWrite(Channel4, LOW);

  Serial.begin(115200);
  onAutoConnWifi("");  //เชื่อมต่อAuto และ ตั้งค่าการเชื่อมต่อเมื่อเจอเครือข่ายใหม่ โดย IP Access point 192.168.4.1
  WiFi.mode(WIFI_STA);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  Serial.println("");
  Serial.println("Wi-Fi connected");  //แสดงว่าเชื่อมต่อ Wi-Fi ได้แล้ว
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());  //แสดง IP ของบอร์ดที่ได้รับแจกจาก AP
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  //------Check connection------
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(1000);
}

void onAutoConnWifi(String chk) {
  WiFiManager wifiManager;
  if (chk == "reset") {
    wifiManager.resetSettings();
    Serial.println("reset wifi");
  } else {
    wifiManager.autoConnect("Relay 4 Channel");  // ชื่อ Access point เริ่มต้น
    Serial.println(WiFi.localIP());
  }
}

/* 
  datasources NetPi
  ======================================
  datasources["???"]["shadow"]["Pi1"] && Indicator Light : datasources["???"]["shadow"]["Pi1"]=='1' | = Relay 1
  datasources["???"]["shadow"]["Pi2"] && Indicator Light : datasources["???"]["shadow"]["Pi2"]=='1' | = Relay 2
  datasources["???"]["shadow"]["Pi3"] && Indicator Light : datasources["???"]["shadow"]["Pi3"]=='1' | = Relay 3
  datasources["???"]["shadow"]["Pi4"] && Indicator Light : datasources["???"]["shadow"]["Pi4"]=='1' | = Relay 4
 */

String getMsg(String topic_, String message_) {
  String tpc;
  if (topic_ == "@msg/Pi1") {
    if (message_ == "CH01_ON") {              //netpie["???"].publish("@msg/Pi1","CH01_ON")
     
      tpc = "{\"data\": {\"Pi1\":1}}";        //datasources["???"]["shadow"]["Pi1"]=='1'
      digitalWrite(Channel1, HIGH);
      // Serial.println("CH01_ON");
    } else if (message_ == "CH01_OFF") {      //netpie["???"].publish("@msg/Pi1","CH01_OFF")
      
      tpc = "{\"data\": {\"Pi1\":0}}";        //datasources["???"]["shadow"]["Pi1"]=='0'
      digitalWrite(Channel1, LOW);
      // Serial.println("CH01_OFF");
    }
  } else if (topic_ == "@msg/Pi2") {
    if (message_ == "CH02_ON") {               //netpie["???"].publish("@msg/Pi2","CH02_ON")
      digitalWrite(Channel2, HIGH);
      tpc = "{\"data\": {\"Pi2\":1}}";         //datasources["???"]["shadow"]["Pi2"]=='1'
      // Serial.println("CH02_ON");
    } else if (message_ == "CH02_OFF") {       //netpie["???"].publish("@msg/Pi2","CH02_OFF")
      digitalWrite(Channel2, LOW);
      tpc = "{\"data\": {\"Pi2\":0}}";         //datasources["???"]["shadow"]["Pi2"]=='0'
      // Serial.println("CH02_OFF");
    }
  } else if (topic_ == "@msg/Pi3") {
    if (message_ == "CH03_ON") {               //netpie["???"].publish("@msg/Pi3","CH03_ON")
      digitalWrite(Channel3, HIGH);
      tpc = "{\"data\": {\"Pi3\":1}}";         //datasources["???"]["shadow"]["Pi3"]=='1'
      // Serial.println("CH03_ON");
    } else if (message_ == "CH03_OFF") {       //netpie["???"].publish("@msg/Pi3","CH03_OFF")
      digitalWrite(Channel3, LOW);
      tpc = "{\"data\": {\"Pi3\":0}}";        //datasources["???"]["shadow"]["Pi3"]=='0'
      // Serial.println("CH03_OFF");
    }
  } else if (topic_ == "@msg/Pi4") {
    if (message_ == "CH04_ON") {             //netpie["???"].publish("@msg/Pi4","CH04_ON")
      digitalWrite(Channel4, HIGH);
      tpc = "{\"data\": {\"Pi4\":1}}";      //datasources["???"]["shadow"]["Pi4"]=='1'
      // Serial.println("CH04_ON");
    } else if (message_ == "CH04_OFF") {   //netpie["???"].publish("@msg/Pi4","CH04_OFF")
      digitalWrite(Channel4, LOW); 
      tpc = "{\"data\": {\"Pi4\":0}}";    //datasources["???"]["shadow"]["Pi4"]=='0'
      // Serial.println("CH04_OFF");
    }
  }

  return tpc;
}
