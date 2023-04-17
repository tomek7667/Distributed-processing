import socket
import threading

HOST = '127.0.0.1'
PORT = 65432

users = {}

def handle_client(conn, addr):
    # Wprowadzenie nazwy użytkownika
    user_name = conn.recv(1024).decode()

    # Dodanie użytkownika do listy użytkowników
    users[user_name] = conn

    print(f"{user_name} has joined the chat.")

    while True:
        # Odbiór wiadomości
        data = conn.recv(1024)
        if not data or data.decode() == "q":
            # Jeśli użytkownik wyszedł, usunięcie go z listy użytkowników i zamknięcie połączenia
            del users[user_name]
            conn.close()
            print(f"{user_name} has left the chat.")
            break

        # Wysłanie wiadomości do wybranego użytkownika
        msg = data.decode()
        recipient, msg = msg.split(":")
        recipient = recipient.strip()
        if recipient in users:
            users[recipient].sendall(f"{user_name}: {msg}".encode())

def start_server():
    # Rozpoczęcie nasłuchiwania na porcie
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen()

        print(f"Server started. Listening on port {PORT}...")

        while True:
            # Akceptowanie nowych połączeń i tworzenie nowego wątku dla każdego użytkownika
            conn, addr = s.accept()
            threading.Thread(target=handle_client, args=(conn, addr)).start()

if __name__ == "__main__":
    start_server()
