import socket
import threading
import uuid
from time import sleep
DEFAULT_PORT = 3333

HOST = '127.0.0.1'
PORT = input(f"Enter the port you want to join: ({DEFAULT_PORT})")
NEXT_MESSAGE_TIMEOUT = 0.2
if PORT == "":
    PORT = f"{DEFAULT_PORT}"
PORT = int(PORT)

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
            stop_event.set()  # Signal the threads to stop
            break
        sock.sendall(input_long.encode())

def start_client():
    # short uuid
    generated_name = str(uuid.uuid4())[:5]
    user_name = input(f"Enter your name: ({generated_name})").strip()
    if user_name == "":
        user_name = generated_name
    

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))

        # Wysłanie nazwy użytkownika na serwer
        s.sendall(user_name.encode())

        # Utworzenie Event object
        stop_event = threading.Event()

        # Tworzenie wątków dla odbierania i wysyłania wiadomości
        receive_thread = threading.Thread(target=receive_messages, args=(s, stop_event))
        send_thread = threading.Thread(target=send_message, args=(s, user_name, stop_event))
        receive_thread.start()
        send_thread.start()

        # Oczekiwanie na zakończenie jednego z wątków
        receive_thread.join()
        send_thread.join()
    print("Disconnected\nThanks for using our chat!")

if __name__ == "__main__":
    start_client()
