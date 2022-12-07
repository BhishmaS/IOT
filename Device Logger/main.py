import serial
import pymysql

db = pymysql.connect(host="localhost", user="root", password="root", db="iot")
ser = serial.Serial(port="COM3", baudrate="115200")

cursor = db.cursor()
while True:
    device_log = str(ser.readline())[2:-5]
    print(device_log)

    device_log = device_log.split('--')
    if len(device_log) == 2:
        lockType = device_log[0]
        status = device_log[1]
        query = "INSERT INTO iot.doorlocklogs (lockType, status, dateTime)" + \
                f" VALUES('{lockType}', '{status}', NOW())"

        print(query)
        cursor.execute(query)
        db.commit()
