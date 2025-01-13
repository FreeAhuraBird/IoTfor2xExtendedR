from flask import Flask, request, jsonify
from queue import Queue

app = Flask(__name__)

message_queue = Queue()

@app.route('/api/sensor_data', methods=['POST'])
def sensor_data():
    data = request.json
    if not data:
        return jsonify({"error": "No data received"}), 400

    print(f"Received data: {data}")
    
    topic = data.get("topic")

    if topic == "sensor/sound":
        message_queue.put(data)

    return jsonify({"message": "Data received successfully"}), 200

@app.route('/api/alert', methods=['GET'])
def get_alert():
    if not message_queue.empty():
        latest_message = message_queue.get()
        return jsonify(latest_message)
    else:
        return jsonify({"message": "No alert"}), 200

if __name__=='__main__':
    app.run(host='0.0.0.0', port=5000)
