from flask import Flask, render_template, jsonify
from gpiozero import DigitalInputDevice
import serial, json, requests

app = Flask(__name__)

# MQ-2 gas sensor
mq2 = DigitalInputDevice(17)

# Arduino Nano serial connection
arduino = serial.Serial("/dev/ttyUSB0", 9600, timeout=1)

LOCATION_CACHE = None


def get_arduino_data():
    try:
        line = arduino.readline().decode().strip()
        if line.startswith("{"):
            return json.loads(line)
    except:
        pass
    return {"temp": None, "hum": None, "soil": None}


def get_ip_location():
    global LOCATION_CACHE

    if LOCATION_CACHE:
        return LOCATION_CACHE

    try:
        r = requests.get("http://ip-api.com/json/", timeout=5)
        data = r.json()

        LOCATION_CACHE = {
            "ip": data.get("query"),
            "city": data.get("city"),
            "region": data.get("regionName"),
            "country": data.get("country"),
            "lat": data.get("lat"),
            "lon": data.get("lon")
        }

        return LOCATION_CACHE
    except:
        return None


@app.route("/")
def index():
    return render_template("dashboard.html", location=get_ip_location())


@app.route("/data")
def data():
    arduino_data = get_arduino_data()
    gas = mq2.value

    return jsonify({
        "temperature": arduino_data["temp"],
        "humidity": arduino_data["hum"],
        "soil": arduino_data["soil"],
        "gas": gas
    })


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)