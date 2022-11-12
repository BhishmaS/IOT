import serial
import pymysql

db = pymysql.connect(host="localhost", user="root", password="root", db="iot")
ser = serial.Serial(port="COM3", baudrate="115200")

cursor = db.cursor()
while True:
    device_log = str(ser.readline())[2:-5]
    print(device_log)

    device_log = device_log.split()
    sensor_id = device_log[0]
    sensor_type = device_log[1]
    value = device_log[2]
    query = "INSERT INTO iot.devicelogs (sensorId, sensorType, value, dateTime)" + \
            f" VALUES({sensor_id}, '{sensor_type}', '{value}', NOW())"

    print(query)
    cursor.execute(query)
    db.commit()