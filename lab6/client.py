import socket
import threading
import random
from string import ascii_lowercase
dictionary = ascii_lowercase
RANDOM_NAME_LENGTH = 5

def generate_random_name(length):
    return ''.join(random.choice(dictionary) for i in range(length))

# random.seed(627278)

from time import sleep
DEFAULT_PORT = 3333
DEFAULT_HOST = '146.59.45.28'
HOST = input(f"Enter the host you want to connect to ({DEFAULT_HOST}): ")
PORT = input(f"Enter the port you want to join ({DEFAULT_PORT}): ")

NEXT_MESSAGE_TIMEOUT = 0.2
if PORT == "":
    PORT = f"{DEFAULT_PORT}"
PORT = int(PORT)

if HOST == "":
    HOST = DEFAULT_HOST

def receive_messages(sock, stop_event):
    while not stop_event.is_set():
        data = sock.recv(1024).decode()
        if data and data.startswith("SERVER:"):
            data = data[7:]
            print(f"Server: {data}")
        elif data:
            print(data)
        else:
            stop_event.set()
    sock.close() 

def send_message(sock, user_name, stop_event):
    while not stop_event.is_set():
        sleep(NEXT_MESSAGE_TIMEOUT)
        input_long = input("")

        if input_long.lower() == "q":
            sock.sendall("q".encode())
            stop_event.set()
            break
        sock.sendall(input_long.encode())

def start_client():
    generated_name = generate_random_name(RANDOM_NAME_LENGTH)
    user_name = input(f"Enter your name: ({generated_name})").strip()
    if user_name == "":
        user_name = generated_name

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(user_name.encode())
        stop_event = threading.Event()
        receive_thread = threading.Thread(target=receive_messages, args=(s, stop_event))
        send_thread = threading.Thread(target=send_message, args=(s, user_name, stop_event))
        receive_thread.start()
        send_thread.start()
        receive_thread.join()
        send_thread.join()
    print("Disconnected\nThanks for using our chat!")

if __name__ == "__main__":
    start_client()
