import paho.mqtt.client as mqtt
from pymongo import MongoClient
import json

# MongoDB setup
mongo_uri = "mongodb+srv://rahatdhrubo123:rahat12345@cowdy.mrcf0.mongodb.net/?retryWrites=true&w=majority&appName=Cowdy"  # MongoDB URI
mongo_client = MongoClient(mongo_uri)
db = mongo_client["cowdy_database"]  # Database name
collection = db["cowdy_collection"]  # Collection name

print(f"Connected to MongoDB collection: {collection.name}")

# Callback for when the MQTT client connects to the broker
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT broker successfully!")
        # Subscribe to the desired topic
        client.subscribe("esp32/30AEA4F261DC/mpu6050")  # Replace with your MQTT topic
        print("Subscribed to topic: esp32/30AEA4F261DC/mpu6050")
    else:
        print(f"Failed to connect to MQTT broker. Return code: {rc}")

# Callback for when a message is received on a subscribed topic
def on_message(client, userdata, msg):
    try:
        # Decode message payload
        payload = msg.payload.decode("utf-8")
        print(f"Received MQTT message: {payload}")

        # Convert payload to JSON
        data = json.loads(payload)

        # Validate and format the data before inserting into MongoDB
        if isinstance(data, dict):  # Ensure the payload is a valid JSON object
            data["topic"] = msg.topic  # Optionally add the topic as metadata
            collection.insert_one(data)
            print("Data inserted into MongoDB:", data)
        else:
            print("Received payload is not a valid JSON object.")

    except json.JSONDecodeError as e:
        print("Failed to decode JSON:", e)
    except Exception as e:
        print("Error while processing message:", e)

# MQTT setup
mqtt_client = mqtt.Client()
mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message

# Connect to the MQTT broker
try:
    mqtt_client.connect("broker.emqx.io", 1883, 60)  # Replace with your MQTT broker address and port
    print("Connected to MQTT broker: broker.emqx.io")
except Exception as e:
    print(f"Failed to connect to MQTT broker: {e}")
    exit(1)

# Start the MQTT loop
print("Starting MQTT client loop...")
mqtt_client.loop_forever()
