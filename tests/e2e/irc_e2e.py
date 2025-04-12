import socket
import time
import argparse
import json
import os
import threading
import queue
import datetime
import concurrent.futures

class IRCClient:
    """各IRCクライアントを表すクラス"""
    def __init__(self, client_id, server, port):
        self.client_id = client_id
        self.server = server
        self.port = port
        self.responses = []
        self.sock = None
        self.connected = False

    def connect(self):
        """サーバーへの接続"""
        if self.sock:
            return

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.settimeout(5)
        try:
            self.sock.connect((self.server, self.port))
            self.connected = True
        except Exception as e:
            print(f"Error connecting client {self.client_id}: {e}")
            raise
    
    def send_command(self, command):
        """コマンドを送信"""
        if not self.connected:
            self.connect()

        # if not command.endswith('\r\n'):
        #     command += '\r\n'
        self.sock.send(command.encode('utf-8'))
        
        timestamp = datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]
        return timestamp
    
    def receive_response(self):
        """レスポンスを受信"""
        if not self.connected:
            return []
            
        self.sock.settimeout(0.5)
        buffer = b""
        try:
            while True:
                data = self.sock.recv(4096)
                if not data:
                    break
                buffer += data
        except socket.timeout:
            pass
        
        timestamp = datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]
        messages = buffer.decode('utf-8', errors='replace').split('\r\n')
        responses = []
        
        for msg in messages:
            if msg.strip():
                response = {
                    'message': msg,
                    'timestamp': timestamp
                }
                self.responses.append(response)
                responses.append(response)
                
        return responses
    
    def close(self):
        """接続を閉じる"""
        if self.sock and self.connected:
            self.sock.close()
            self.connected = False


def run_ordered_multi_client_test(test_case, server, port):
    client_ids = test_case['clients']
    steps = test_case['steps']

    clients = {}
    for client_id in client_ids:
        clients[client_id] = IRCClient(client_id, server, port)
    
    for client in clients.values():
        try:
            client.connect()
        except Exception as e:
            print(f"Failed to connect client {client.client_id}: {e}")
            for c in clients.values():
                c.close()
            return {}, []
    
    all_events = []
    for step in steps:
        step_id = step['step_id']
        client_id = step['client_id']
        command = step['command']
        wait_after = step.get('wait_after', 0.5)
        
        if client_id not in clients:
            print(f"Error: Client {client_id} not found for step {step_id}")
            continue
        
        client = clients[client_id]
        try:
            timestamp = client.send_command(command)
            all_events.append({
                'type': 'sent',
                'step_id': step_id,
                'client_id': client_id,
                'command': command.strip(),
                'timestamp': timestamp
            })
            print(f"[{timestamp}] Step {step_id}: Client {client_id} sent: {command.strip()}")

            time.sleep(wait_after)

            responses = client.receive_response()
            for resp in responses:
                all_events.append({
                    'type': 'received',
                    'step_id': step_id,
                    'client_id': client_id,
                    'message': resp['message'],
                    'timestamp': resp['timestamp']
                })
                print(f"[{resp['timestamp']}] Step {step_id}: Client {client_id} received: {resp['message']}")
                
        except Exception as e:
            print(f"Error in step {step_id} for client {client_id}: {e}")
    
    for client in clients.values():
        client.close()

    client_responses = {client_id: client.responses for client_id, client in clients.items()}
    
    return client_responses, all_events

def run_test_on_server(test, host, port, server_type):
    """指定されたサーバーでテストを実行"""
    print(f"\n{server_type} server ({host}:{port}):")
    responses, events = run_ordered_multi_client_test(test, host, int(port))
    return server_type, responses, events

def main():
    parser = argparse.ArgumentParser(description="Ordered Multi-client IRC Testing Tool")
    parser.add_argument("--org", default="127.0.0.1:6667", help="Original server (host:port)")
    parser.add_argument("--alt", default="127.0.0.1:6668", help="Another server (host:port)")
    parser.add_argument("files", nargs=argparse.REMAINDER, help='List of test files to run')
    args = parser.parse_args()

    orig_host, orig_port = args.org.split(':')
    reimp_host, reimp_port = args.alt.split(':')

    if not args.files:
        print("Error: No test files specified")
        parser.print_help()
        exit(1)

    for file in args.files:
        with open(file, 'r') as f:
            test_cases = json.load(f)

        for test in test_cases:
            test_name = test['name']
            
            print(f"\n=== Testing: {test_name} ===")

            with concurrent.futures.ThreadPoolExecutor(max_workers=2) as executor:
                futures = {
                    executor.submit(run_test_on_server, test, orig_host, orig_port, "Original"): "org",
                    executor.submit(run_test_on_server, test, reimp_host, reimp_port, "Alternative"): "alt"
                }
                results = {}
                for future in concurrent.futures.as_completed(futures):
                    server_type_long, responses, events = future.result()
                    server_type = futures[future]
                    results[server_type] = (responses, events)

            os.makedirs('log', exist_ok=True)

            for server_type, (responses, events) in results.items():
                with open(f"log/{test_name}_{server_type}.log", 'w') as f:
                    server_name = "Original" if server_type == "org" else "Alternative"
                    f.write(f"=== Test: {test_name} on {server_name} Server ===\n\n")
                    
                    f.write("=== Event Log (Step Order) ===\n")
                    for event in sorted(events, key=lambda e: e['step_id']):
                        if event['type'] == 'sent':
                            f.write(f"[{event['timestamp']}] Step {event['step_id']}: Client {event['client_id']} sent: {event['command']}\n")
                        elif event['type'] == 'received':
                            f.write(f"[{event['timestamp']}] Step {event['step_id']}: Client {event['client_id']} received: {event['message']}\n")
                    
                    for client_id, client_responses in responses.items():
                        f.write(f"\n=== Client {client_id} Responses ===\n")
                        for resp in client_responses:
                            f.write(f"[{resp['timestamp']}] {resp['message']}\n")
            
            print(f"Results saved to log/{test_name}_*.log")

if __name__ == "__main__":
    main()
