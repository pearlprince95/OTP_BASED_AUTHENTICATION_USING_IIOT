

#include <ESP8266WiFi.h>`
#include <ESP8266mDNS.h>
#include <PubSubClient.h>
char ssid[] = "XXX";     
char password[] = "XXX"; 
const char* mqtt_server = "test.mosquitto.org";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
String data,otp,pin;
//#define otp_request  otp_request
//#define pin_entered  pin_entered


void setup() 
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
    reconnect();    
}
void setup_wifi() 
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
 Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
for (int i = 0; i < length; i++) 
  {
    Serial.print((char)payload[i]);
    data += (char)payload[i];
  }   
  String TOPIC = String(topic);
  if (TOPIC == "otp_request")
  {
  otp = data;
  pin = String(0000);
  Serial.print("\n OTP ARRIVED " + otp); 
  Serial.print("\n pin ARRIVED " + pin); 
  }
  if (TOPIC == "pin_entered")
 {
  pin = data;
  Serial.print("\n OTP ARRIVED " + otp); 
  Serial.print("\n pin ARRIVED " + pin); 
  } 
  Serial.println();
  data="";
  if(otp == pin)
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  if(otp != pin)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
void reconnect() 
{
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...\n");
    uint32_t chipid=ESP.getChipId();
    char clientid[25];
    snprintf(clientid,25,"OTP BASED DOOR UNLOCKING SYSTEM : ",chipid); 
    Serial.print("\n Client ID: ");
    Serial.println(clientid);
    if (client.connect(clientid)) 
    {
      Serial.println("connected");
      client.subscribe("XXX");
      client.subscribe("XXX");      
    }
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void loop() 
{

  if (!client.connected()) {
   reconnect();
 }
  client.loop();
}  
