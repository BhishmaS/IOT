import pymysql
from flask import Flask, render_template

app = Flask(__name__)

# @app.route('/')
@app.route('/', methods=['GET'])
def hello_world():

    db = pymysql.connect(host="localhost", user="root", password="root", db="iot")
    cursor = db.cursor()
    query = "SELECT TIMESTAMPDIFF(SECOND, '1970-01-01 00:00:00.00', dateTime) * 1000 as utcSeconds," + \
            "CAST(value AS FLOAT) AS value FROM iot.devicelogs ORDER BY dateTime"

    cursor.execute(query)
    data = cursor.fetchall()

    print(data)
    sensor_data = data

    # This is where we pass the sensor data to index.html
    return render_template('index.html', trendData=sensor_data)

if __name__ == '__main__':
    app.run(debug=True)
