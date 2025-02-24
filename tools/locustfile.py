from dotenv import load_dotenv
from locust import HttpUser, task, between
import os
import socket
import time

load_dotenv(dotenv_path=os.path.join(os.path.dirname(__file__), "..", ".env"))

class MyLocustUser(HttpUser):
  wait_time = between(1, 5)

  @task
  def connect_and_disconnect(self):
    host = os.getenv("HOST", "127.0.0.1")
    port = int(os.getenv("PORT", "8080"))
    print(f"Connecting to {host}:{port}")
    try:
      with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        time.sleep(2)
        message = "PASS pass\r\n"
        print(f"send: {message}")
        s.send(message.encode("utf-8"))

        response = s.recv(1024)
        print(f"resp: {response.decode('utf-8')}")

        time.sleep(5)
        s.close()
            
    except Exception as e:
      print(f"Error: {e}")
