import socket
import threading


HOST = '127.0.0.1'
PORT = 65432

def receive_messages(sock, stop_event):
    while not stop_event.is_set():
        data = sock.recv(1024)
        if data:
            print('\n| NEW MESSAGE: ', data.decode(), '\n')
        else:
            stop_event.set()

    # Jeśli wątek został zakończony, to zamykamy socket
    sock.close() 

def send_message(sock, user_name, stop_event):
    while not stop_event.is_set():
        # Wprowadzenie wiadomości i odbiorcy
        input_long = input("Enter recipient and message separated by ':' (or 'q' to exit):\n")

        if input_long.lower() == "q":
            sock.sendall("q".encode())
            stop_event.set()  # Signal the threads to stop
            break
        
        try:
            recipient, msg = input_long.split(":", 1)
            recipient = recipient.strip()
        except ValueError:
            print("Invalid input format. Please enter the recipient's name and message separated by a colon (e.g. 'user111:hello')")
            continue
        
        # Wysłanie wiadomości
        sock.sendall(f"{recipient.strip()}: {msg.strip()}".encode())

def start_client():
    # Wprowadzenie nazwy użytkownika
    user_name = input("Enter your name: ")

    # Nawiązanie połączenia z serwerem
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

    print("Disconnected from the server.")

if __name__ == "__main__":
    start_client()
