import random
from flask import Flask, request

app = Flask(__name__)

@app.route('/random')
def get_random():
    max_val = request.args.get('max', type=int)
    return str(random.randint(0, max_val))

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80)
