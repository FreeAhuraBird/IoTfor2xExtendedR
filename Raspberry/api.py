from flask import Flask, request, jsonify

app = Flask(__name__)

whole_msg = ""

@app.route('/api/sensor_data', methods=['POST'])
def sensor_data():
    data = request.json
    if not data:
        return jsonify({"error": "No data received"}), 400

    print(f"Received data: {data}")
    
    topic = data.get("topic")

    if topic == "sensor/sound":
        alert = data.get('message')
        print(data.get("message"))
        whole_msg = data

    #if topic == "sensor/motion":
     #   print(data.get("message"))
      #  print(f"topic: {topic}\nmessage: {data.get('message')}")


    return jsonify({"message": "Data received successfully"}), 200

@app.route('/api/alert', methods=['GET'])
def get_alert():
    if whole_msg != "":
        return jsonify(whole_msg)
        # if jsonify doesn't work-> just returm whole_msg
    else:
        return "No alert"

if __name__=='__main__':
    app.run(host='0.0.0.0', port=5000)
