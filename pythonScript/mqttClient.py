import json
import paho.mqtt.client as mqtt
from pymongo import MongoClient
import datetime
import requests


# api-endpoint
URL = "https://staging-rs.datagrama.io/stats"

# is the id of the node
# Change and add as a command line argument
#id = 'a0b450f3-451c-4a3a-adbc-a3a5bbf62920'
id_a8 = '35fb717e-941c-4898-8b3b-7e4ca86d05a4'  #   a8
id_6b = '380857b5-4fed-4950-bd0f-9e5069d83a49'   #  6b


broker="127.0.0.1"
port=1883
topic="application/28/#"

db_url = 'mongodb://localhost:27017/'
db_airQuality = 'airQualityDB'
col_sensors = 'airQualityData'
col_lora = 'loraData'
doc = {}
s_data = {}
l_data = {}
sensors = {}
lora = {}
i = 0
def sensors_data(data):
    s_data={}
    payload=json.loads(data['objectJSON'])
    s_data={'data':{'pm_01':payload['pm_0.1'],
    'pm_25':payload['pm_2.5'],
    'pm_10':payload['pm_10'],
    'ozone':payload['ozone'],
    'so2':payload['so2'],
    'temp':payload['temperature'],
    'hum':payload['humidity'],
    'co':payload['co'],
    'no2':payload['no2']}}
    return s_data

def lora_data(data):
    l_data={}
    payload = json.loads(data['objectJSON'])
    l_data = {'txInfo':{'power':payload['power'],
        'frequency':data['txInfo']['frequency'],
        'bandwidth':data['txInfo']['loRaModulationInfo']['bandwidth'],
        'spreadingFactor':data['txInfo']['loRaModulationInfo']['spreadingFactor'],
        'codeRate':data['txInfo']['loRaModulationInfo']['codeRate']},
        'rxInfo':{'SNR':data['rxInfo'][0]['loRaSNR'],'RSSI':data['rxInfo'][0]['rssi']}
    }
    return l_data

# The callback for when the client receives a CONNACK response from the server.
def order_data(data):
    device = data['deviceName']
    if device == "Node_1":
        doc = {'node_id':"0080e115000aa0a8"}
    elif device == "Node_2":
        doc = {'node_id':"0080e115000a956b"}
    dt = datetime.datetime.now()
    date = {"date":dt.strftime("%x")}
    time = {"time":dt.strftime("%X")}
    doc.update(date)
    doc.update(time)
    sensors = sensors_data(data)
    lora = lora_data(data)
    s_data.update(doc)
    s_data.update(sensors)
    l_data.update(doc)
    l_data.update(lora)
    return s_data,l_data

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe(topic)

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    global i
    i = i + 1
    data = str(msg.payload.decode("utf-8"))
    data = json.loads(data)
    sensors_data_mongo,lora_data_mongo=order_data(data)
    sensors_data_mongo.update({'_id':i})
    lora_data_mongo.update({'_id':i})
    print(sensors_data_mongo)
    print(lora_data_mongo)
    send_ubi(sensors_data_mongo)
    my_col_sensors.insert_one(sensors_data_mongo)
    my_col_lora.insert_one(lora_data_mongo)

def send_ubi(data):
    if data['node_id']=='0080e115000a956b':
        PARAMS = {'id': id_6b}
        PARAMS['imei'] = id_6b
    elif data['node_id']=='0080e115000aa0a8':
        PARAMS = {'id': id_a8}
        PARAMS['imei'] = id_a8
    # required params
    #PARAMS = {'id': id}
    #PARAMS['imei'] = id
    PARAMS['sv'] = 'test'
    PARAMS['hw'] = 'none'
    PARAMS['srv'] = 'heartbeat'
    # include all datapoints that you need
    PARAMS['pm01'] = data['data']['pm_01']
    PARAMS['pm25'] = data['data']['pm_25']
    PARAMS['pm10'] = data['data']['pm_10']
    PARAMS['o3'] = data['data']['ozone']
    PARAMS['so2'] = data['data']['so2']
    PARAMS['tmp'] = data['data']['temp']
    PARAMS['hum'] = data['data']['hum']
    PARAMS['co'] = data['data']['co']
    PARAMS['no2'] = data['data']['no2']
    PARAMS['seq'] = 1
    r = requests.get(url=URL, params=PARAMS)
    data = r.json()
    code = data['code']
    print("REQ SENT", code)



def mongo_connection():
    global i
    my_db= mongoClient[db_airQuality]
    my_col_sensors = my_db[col_sensors]
    my_col_lora = my_db[col_lora]
    #check id
    cursor = list(my_col_sensors.find().sort('_id',-1).limit(1))
    if len(cursor)==0:
        i = 0
    else:
        i = cursor[0]['_id']
    return i,my_col_sensors,my_col_lora



mongoClient = MongoClient(db_url)
print("Connection successful to database")

i,my_col_sensors,my_col_lora=mongo_connection()
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(broker, port, 60)

client.loop_forever()

