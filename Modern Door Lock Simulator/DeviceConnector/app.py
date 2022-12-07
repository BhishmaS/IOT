from datetime import datetime

import pymysql
from flask import Flask, render_template


class DoorLockLog:
    def __init__(self, _lock_type, _status, _logged_on):
        self.lockType = _lock_type
        self.status = _status
        self.loggedOn = str(datetime.fromtimestamp(_logged_on))


app = Flask(__name__)


@app.route('/', methods=['GET'])
def startup():
    db = pymysql.connect(host="localhost", user="root", password="root", db="iot")
    cursor = db.cursor()

    query = "SELECT TIMESTAMPDIFF(SECOND, '1970-01-01 00:00:00.00', dateTime) as loggedOn," + \
            "lockType, status FROM iot.doorlocklogs ORDER BY dateTime"

    cursor.execute(query)
    data = cursor.fetchall()
    print(data)

    logs = []
    for item in data:
        logs.append(DoorLockLog(item[1], item[2], item[0]))

    for item in logs:
        print(item.lockType + " " + item.status + " " + item.loggedOn)

    # This is where we pass the sensor data to index.html
    return render_template('index.html', logs=logs)


if __name__ == '__main__':
    app.run(debug=True)
