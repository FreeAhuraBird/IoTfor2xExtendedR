import paho.mqtt.client as mqtt
import requests
import json

MQTT_HOST = "172.20.10.8"
MQTT_PORT = 1883
MQTT_KEEPALIVE_INTERVAL = 60
MQTT_TOPIC_SOUND = "sensor/sound"
MQTT_MSG = "Hello MQTT"

API_URL = "http://127.0.0.1:5000/api/sensor_data"

def on_connect(mosq, obj, flags, rc):
    mqttc.subscribe(MQTT_TOPIC_SOUND, 0)
    print(f"Connected with results code {rc}")
    print(f"Subscribed to topics: {MQTT_TOPIC_SOUND}.")


def on_subscribe(mosq, obj, mid, granted_qos):
    print ("Subscribed to MQTT Topic")

def on_message(mosq, obj, msg):
    if msg.topic == MQTT_TOPIC_SOUND: 
        print (f"Sound data: {msg.payload.decode()}")

    data = {
        "topic": msg.topic,
        "message": msg.payload.decode()
    }
    print(f"Forwarding data to API: {data}")

    topic = data.get("topic")
    if topic=="sensor/sound":
        print(f"Sounds value: {data.get('message')}")

        sound_value = int(data.get("message"))
        
        if sound_value > 900:
            try:
                response = requests.post(API_URL, json=data)
                print(f"API response: {response.status_code}, {response.json()}")
            except Exception as e:
                print(f"Failed to send data to API: {e}")


mqttc = mqtt.Client()

mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_subscribe = on_subscribe

mqttc.connect(MQTT_HOST, MQTT_PORT, MQTT_KEEPALIVE_INTERVAL )

mqttc.loop_forever()
