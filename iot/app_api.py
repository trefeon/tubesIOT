from flask import Flask, request, jsonify, render_template
import datetime
import json

app = Flask(__name__)

# Variabel untuk menyimpan data terakhir per deviceId
latest_data_store = {} 

@app.route('/data', methods=['POST'])
def handle_data():
    global latest_data_store
    try:
        content_type = request.headers.get('Content-Type')
        if (content_type == 'application/json'):
            data_diterima = request.json
            timestamp_server = datetime.datetime.now().isoformat()
            
            print(f"[{timestamp_server}] Data diterima dari Jembatan (NodeMCU) ke /data:")
            print(json.dumps(data_diterima, indent=2))
            
            device_id = data_diterima.get("deviceId", "unknown_device")
            current_device_data = latest_data_store.get(device_id, {"history": []})
            
            # Simpan data saat ini
            current_data_point = data_diterima.copy() # Buat salinan
            current_data_point['server_timestamp'] = timestamp_server
            
            # Tambahkan ke histori (misalnya, simpan 5 data terakhir)
            current_device_data["history"].insert(0, current_data_point)
            current_device_data["history"] = current_device_data["history"][:5] # Batasi histori
            
            # Update data terbaru untuk device tersebut
            current_device_data["latest"] = current_data_point
            latest_data_store[device_id] = current_device_data

            return jsonify({"status": "sukses", "pesan": "Data diterima oleh server API", "data_diterima": data_diterima}), 200
        else:
            return jsonify({"status": "error", "pesan": "Content-Type harus application/json"}), 400
    except Exception as e:
        print(f"Error memproses request API di /data: {e}")
        return jsonify({"status": "error", "pesan": str(e)}), 500

@app.route('/api/latest_data', methods=['GET'])
def get_latest_data():
    global latest_data_store
    # Mengembalikan data terbaru dari semua device yang tersimpan
    return jsonify(latest_data_store)

@app.route('/', methods=['GET'])
def dashboard_page():
    return render_template('dashboard.html') # Ini akan mencari dashboard.html di folder templates

if __name__ == '__main__':
    print("Menjalankan Flask API server di http://0.0.0.0:5000")
    # Pastikan dashboard.html ada di folder static atau templates jika menggunakan render_template
    # Untuk contoh ini, dashboard akan diakses sebagai file statis atau endpoint terpisah.
    app.run(host='0.0.0.0', port=5000, debug=True)
