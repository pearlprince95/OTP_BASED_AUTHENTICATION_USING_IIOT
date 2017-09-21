# from twilio.rest import TwilioRestClient IF WINDOWS 
from twilio.rest import Client # IF RPI
import paho.mqtt.client as mqtt
import random
ACCOUNT_SID = 'AXXX'
AUTH_TOKEN = 'ZZZZ'
cell = '+91xxxxxxx'
twilio = '+xxxxxxxx'
sms = TwilioRestClient(ACCOUNT_SID, AUTH_TOKEN)#IF WINDOWS
sms = Client(ACCOUNT_SID, AUTH_TOKEN)#RPI
def on_connect(client1, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client1.subscribe("pin_entered")
    client1.subscribe("otp_request")
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    if msg.payload.decode() == "otp_request":
        print("X")
        otp = random.randint(1111,9999)
        print("Y")
        sms.messages.create(to=cell,from_=twilio,body=otp)
        client1.publish("otp_request", otp);
client1 = mqtt.Client()
client1.on_connect = on_connect
client1.on_message = on_message
client1.connect("test.mosquitto.org", 1883, 60)
client1.loop_forever()
sms.loop_forever()
