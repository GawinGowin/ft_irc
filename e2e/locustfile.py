from locust import User, task, between, events
import socket
import time
import uuid
import random
import os
from dotenv import load_dotenv

# 環境変数の読み込み
load_dotenv()

class IRCClient:
    def __init__(self, host, port, password):
        self.host = host
        self.port = port
        self.password = password
        self.socket = None
        self.nickname = None
        
    def connect(self):
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((self.host, self.port))
            return True
        except Exception:
            return False
            
    def send_command(self, command):
        if not command.endswith("\r\n"):
            command += "\r\n"
        self.socket.sendall(command.encode())
        
    def receive(self, timeout=1.0):
        self.socket.settimeout(timeout)
        response = b""
        try:
            while True:
                data = self.socket.recv(4096)
                if not data:
                    break
                response += data
        except socket.timeout:
            pass
        return response.decode('utf-8', errors='ignore')
        
    def register(self, nickname):
        self.nickname = nickname
        self.send_command(f"PASS {self.password}")
        self.send_command(f"NICK {nickname}")
        self.send_command(f"USER {nickname} 0 * :Test User")
        return self.receive()
        
    def disconnect(self):
        if self.socket:
            try:
                self.send_command("QUIT :Bye")
            except:
                pass
            finally:
                self.socket.close()
                self.socket = None

class IRCUser(User):
    wait_time = between(1, 3)
    
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.ft_client = None
        self.ngircd_client = None
        self.user_id = str(uuid.uuid4())[:8]
        self.channels = []
        
    def on_start(self):
        # ft_irc接続
        host = os.getenv('FT_IRC_HOST', 'ft_irc')
        port = int(os.getenv('FT_IRC_PORT', 6668))
        password = os.getenv('IRC_PASSWORD', 'password')
        
        self.ft_client = IRCClient(host, port, password)
        if not self.ft_client.connect():
            print(f"ユーザー {self.user_id} がft_ircに接続できませんでした")
            return
            
        self.ft_client.register(f"user_{self.user_id}")
        
        # テスト用のチャンネルを作成
        channel = f"#test_{random.randint(1000, 9999)}"
        self.ft_client.send_command(f"JOIN {channel}")
        self.ft_client.receive()
        self.channels.append(channel)
        
    def on_stop(self):
        if self.ft_client:
            self.ft_client.disconnect()
            
    @task(2)
    def join_and_leave_channel(self):
        if not self.ft_client:
            return
            
        channel = f"#random_{random.randint(1000, 9999)}"
        
        # チャンネル参加
        start_time = time.time()
        self.ft_client.send_command(f"JOIN {channel}")
        response = self.ft_client.receive()
        join_time = time.time() - start_time
        
        # JOINの成功を確認
        success = "JOIN" in response
        
        events.request.fire(
            request_type="IRC",
            name="JOIN",
            response_time=join_time * 1000,  # ミリ秒単位
            response_length=len(response),
            exception=None if success else Exception("JOIN failed"),
            context=self.environment
        )
        
        if success:
            self.channels.append(channel)
            
            # チャンネルから退出
            start_time = time.time()
            self.ft_client.send_command(f"PART {channel} :Testing")
            response = self.ft_client.receive()
            part_time = time.time() - start_time
            
            part_success = "PART" in response
            
            events.request.fire(
                request_type="IRC",
                name="PART",
                response_time=part_time * 1000,
                response_length=len(response),
                exception=None if part_success else Exception("PART failed"),
                context=self.environment
            )
            
            if part_success:
                self.channels.remove(channel)
    
    @task(5)
    def send_message(self):
        if not self.ft_client or not self.channels:
            return
            
        channel = random.choice(self.channels)
        message = f"Message from {self.ft_client.nickname}: {uuid.uuid4()}"
        
        start_time = time.time()
        self.ft_client.send_command(f"PRIVMSG {channel} :{message}")
        response = self.ft_client.receive()
        msg_time = time.time() - start_time
        
        # PRIVMSGはエラーがなければ成功
        success = not ("401" in response or "403" in response or "404" in response)
        
        events.request.fire(
            request_type="IRC",
            name="PRIVMSG",
            response_time=msg_time * 1000,
            response_length=len(response),
            exception=None if success else Exception("PRIVMSG failed"),
            context=self.environment
        )
    
    @task(1)
    def mode_command(self):
        if not self.ft_client or not self.channels:
            return
            
        channel = random.choice(self.channels)
        mode = random.choice(["+t", "+i", "-t", "-i"])
        
        start_time = time.time()
        self.ft_client.send_command(f"MODE {channel} {mode}")
        response = self.ft_client.receive()
        mode_time = time.time() - start_time
        
        # エラーコードが含まれていなければ成功
        success = not "482" in response
        
        events.request.fire(
            request_type="IRC",
            name="MODE",
            response_time=mode_time * 1000,
            response_length=len(response),
            exception=None if success else Exception("MODE failed"),
            context=self.environment
        )