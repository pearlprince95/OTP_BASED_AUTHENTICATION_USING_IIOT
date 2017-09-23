
#include <ESP8266WiFi.h>`
#include <ESP8266mDNS.h>
#include <PubSubClient.h>
#include "Gsender.h"
char ssid[] = "pearlprince95";     
char password[] = "MAHARANI"; 
const char* mqtt_server = "test.mosquitto.org";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
String data,otp,pin,dummy;
//#define otp_request  otp_request
//#define pin_entered  pin_entered

#pragma region Globals
uint8_t connection_state = 0;                    // Connected to WIFI or not
uint16_t reconnect_interval = 10000;             // If not connected wait time to try again
#pragma endregion Globals

void setup() 
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
    reconnect();
      client.publish("otp_request", "otp_request"); 
      Serial.println("otp_requested"); 
    
    
}
void setup_wifi() 
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
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
  dummy = String("otp_request");
  Serial.print("\n OTP ARRIVED " + otp); 
  Serial.print("\n pin ARRIVED " + pin);
  
  if (data != dummy ) 
{
     Gsender *gsender = Gsender::Instance(); 
    String subject = "ALLERT!!!!!....OTP";
    if(gsender->Subject(subject)->Send("receiver@gmail.com", "DOOR OPEN REQUEST RAISED AND YOUR OTP IS: " + otp )) 
    {
        Serial.println("Message send.");
    } else
    {
        Serial.print("Error sending message: ");
        Serial.println(gsender->getError());
    }
}
   
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
    
    if (((pin != "0")&&(pin != "otp_request")))
    {
      digitalWrite(LED_BUILTIN, LOW);
    Gsender *gsender = Gsender::Instance(); 
    String subject = "ALLERT!!!!!....OTP";
    if(gsender->Subject(subject)->Send("receiver@gmail.com", "Intruder entered an PIN which was found to be incorrect with the system OTP ,so the door was OPENED and LOCKED. And the PIN entered is: " + pin ))
    {
    digitalWrite(LED_BUILTIN, HIGH);
    }
    }
  }
}
void reconnect() 
{
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...\n");
    uint32_t chipid=ESP.getChipId();
    char clientid[25];
    snprintf(clientid,25,"OTP BASED DOOR UNLOCKING SYSTEM : ",chipid); //this adds the mac address to the client for a unique id
    Serial.print("\n Client ID: ");
    Serial.println(clientid);
    if (client.connect(clientid)) 
    {
      Serial.println("connected");
      client.subscribe("otp_request");
      client.subscribe("pin_entered"); 
   
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

  if (!client.connected()) 
  {
   reconnect();
 }
  client.loop();
}  



