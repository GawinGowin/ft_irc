import pytest
import subprocess
import time
import socket
import os
import signal

class IRCServer:
    def __init__(self, binary_path, port, password):
        self.binary_path = binary_path
        self.port = port
        self.password = password
        self.process = None
        
    def start(self):
        self.process = subprocess.Popen(
            [self.binary_path, str(self.port), self.password],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE
        )
        # サーバー起動を確認
        for _ in range(10):
            try:
                sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                sock.connect(("localhost", self.port))
                sock.close()
                return True
            except:
                time.sleep(0.5)
        return False
        
    def stop(self):
        if self.process:
            self.process.terminate()
            self.process.wait(timeout=5)
            self.process = None

@pytest.fixture(scope="session")
def ngircd_server():
    """オリジナルngircdサーバーを起動"""
    server = IRCServer("/usr/bin/ngircd", 6667, "testpassword")
    assert server.start(), "オリジナルngircdの起動に失敗"
    yield server
    server.stop()

@pytest.fixture(scope="session")
def ft_irc_server():
    """ft_ircサーバーを起動"""
    server = IRCServer("./ft_irc", 6668, "testpassword")
    assert server.start(), "ft_ircの起動に失敗"
    yield server
    server.stop()

@pytest.fixture
def irc_client():
    """IRCクライアント共通関数"""
    class IRCClient:
        def __init__(self, host, port):
            self.host = host
            self.port = port
            self.socket = None
            self.buffer = ""
            
        def connect(self):
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((self.host, self.port))
            return self
            
        def disconnect(self):
            if self.socket:
                self.socket.close()
                self.socket = None
                
        def send_raw(self, message):
            if not message.endswith("\r\n"):
                message += "\r\n"
            self.socket.sendall(message.encode())
            
        def receive(self, timeout=2.0):
            """タイムアウト付きでレスポンスを受信"""
            self.socket.settimeout(timeout)
            responses = []
            try:
                while True:
                    data = self.socket.recv(4096)
                    if not data:
                        break
                    self.buffer += data.decode('utf-8', errors='ignore')
                    if self.buffer.endswith("\r\n"):
                        responses.append(self.buffer)
                        self.buffer = ""
                        break
            except socket.timeout:
                if self.buffer:
                    responses.append(self.buffer)
                    self.buffer = ""
            return responses
            
        def register(self, nickname, username="user", realname="Real Name"):
            """IRCに登録"""
            self.send_raw(f"PASS {self.password}")
            self.send_raw(f"NICK {nickname}")
            self.send_raw(f"USER {username} 0 * :{realname}")
            return self.receive()
            
    return IRCClient