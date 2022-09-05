import json
import paho.mqtt.client as mqtt
from pymongo import MongoClient

broker="127.0.0.1"
port=1883
topic="application/26/#"

db_url = 'mongodb://localhost:27017/'
db_name = 'airQualityDB'
col_name = 'airQualityData'
data = {}

# The callback for when the client receives a CONNACK response from the server.
def order_data(data):
    application = data['applicationID']
    device = data['deviceName']
    payload = json.loads(data['objectJSON'])
    #print(payload)
    if device == "Node_1":
        data = {'node_id':"0080e115000aa0a8"}
    elif device == "Node_2":
        data = {'node_id':"0080e115000a956b"}
    data.update(payload)
    print(data)
    return data

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe(topic)

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    data = str(msg.payload.decode("utf-8"))
    data = json.loads(data)

    data_mongo=order_data(data)
    my_col.insert_one(data_mongo)

mongoClient = MongoClient(db_url)
print("Connection successful to database")

my_db = mongoClient[db_name]
my_col = my_db[col_name]

print(mongoClient.list_database_names())

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(broker, port, 60)

client.loop_forever()

