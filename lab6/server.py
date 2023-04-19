import socket
import threading
import re
import os

PREFIX = "SERVER:"
WELCOME_MESSAGE = f"{PREFIX}Welcome to the chat!\nIn order to disconnect type 'q'\nif you want to send a private message, type '/m <username> <message>'\nTo list current users type '/l'\n"
USERNAME_REGEX = r"^[a-zA-Z0-9_-]+$"
WRONG_USERNAME = f"{PREFIX}Please choose another name. (must match regex: {USERNAME_REGEX} and not already exist)"
HOST = '127.0.0.1'
PORT = 3333

users = {}

def send_to_all(msg):
    for user in users:
        users[user].sendall(msg.encode())

def send_to_all_except(msg, user_name):
    for user in users:
        if user != user_name:
            users[user].sendall(msg.encode())

def log(msg):
    print(f"LOG: {msg}")

def handle_client(conn, addr):
    user_name = conn.recv(1024).decode()
    if user_name in users or user_name == "SERVER" or not user_name or not re.match(USERNAME_REGEX, user_name):
        conn.sendall(WRONG_USERNAME.encode())
        conn.close()
        return

    users[user_name] = conn
    send_to_all_except(f"{PREFIX}{user_name} has joined the chat.", user_name)
    conn.sendall(WELCOME_MESSAGE.encode())
    log(f"{user_name} has joined the chat.")
    while True:
        data = conn.recv(1024)
        if not data or data.decode() == "q":
            del users[user_name]
            conn.close()
            log(f"{user_name} has left the chat.")
            send_to_all(f"{PREFIX}{user_name} has left the chat.")
            break
        msg = data.decode()
        if msg.startswith("/m"):
            try:
                recipient = msg.split(" ")[1]
                if not re.match(USERNAME_REGEX, recipient):
                    conn.sendall(f"{PREFIX}Wrong username format. Usage: \"/m <username> <message>\"".encode())
                    continue
                msg = " ".join(msg.split(" ")[2:])
                if recipient in users:
                    users[recipient].sendall(f"[M] {user_name}: {msg}".encode())
                    log(f"{user_name} sent a private message to {recipient}: {msg}")
                else:
                    conn.sendall(f"{PREFIX}User \"{recipient}\" not found.".encode())
            except:
                conn.sendall(f"{PREFIX}Wrong format. Usage: \"/m <username> <message>\"".encode())
        elif msg.startswith("/l"):
            online = []
            for user in users:
                if user != user_name:
                    online.append(user)
                else:
                    online.append(f"{user} (you)")
            conn.sendall(f"{PREFIX}Users in the room: {', '.join(online)}".encode())
        else:
            send_to_all_except(f"{user_name}: {msg}", user_name)

def start_server(HOST, PORT):
    # Rozpoczęcie nasłuchiwania na porcie
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.bind((HOST, PORT))
            s.listen()

            print(f"Server started. Listening on port {PORT}...")

            while True:
                conn, addr = s.accept()
                threading.Thread(target=handle_client, args=(conn, addr)).start()
    except KeyboardInterrupt:
        send_to_all(f"{PREFIX}Server is shutting down...")
        for user in users:
            users[user].close()
        s.close()
        print("Server closed.")
        exit(0)
    except OSError:
        PORT += 1
        start_server(HOST, PORT)

if __name__ == "__main__":
    start_server(HOST, PORT)
