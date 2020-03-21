from flask import Flask, render_template, jsonify
from bluetooth_mod import printJavascript

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/printString/',methods=['POST'])
def printString():
    return jsonify({'data': render_template('index.html', printJavascript)})


if __name__ == "__main__":
    app.run(debug=True)