import socket
import time
import argparse
import json
import os

def send_command_sequence(server, port, commands):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.settimeout(5)
    sock.connect((server, port))
    responses = []
    for cmd, wait_time in commands:
        if not cmd.endswith('\r\n'):
            cmd += '\r\n'
        sock.send(cmd.encode('utf-8'))
        
        time.sleep(wait_time)

        sock.settimeout(0.1)
        buffer = b""
        try:
            while True:
                data = sock.recv(4096)
                if not data:
                    break
                buffer += data
        except socket.timeout:
            pass

        messages = buffer.decode('utf-8', errors='replace').split('\r\n')
        for msg in messages:
            if msg.strip():
                responses.append(msg)
    
    sock.close()
    return responses

def main():
    parser = argparse.ArgumentParser(description="Simple IRC Test Client")
    parser.add_argument("--org", default="127.0.0.1:6667", help="Original server (host:port)")
    parser.add_argument("--alt", default="127.0.0.1:6668", help="Alternative server (host:port)")
    parser.add_argument("--file", required=True, help="JSON file with test commands")
    args = parser.parse_args()

    orig_host, orig_port = args.org.split(':')
    reimp_host, reimp_port = args.alt.split(':')

    with open(args.file, 'r') as f:
        test_cases = json.load(f)

    for test in test_cases:
        test_name = test['name']
        commands = test['commands']
        
        print(f"=== Testing: {test_name} ===")

        print(f"Original server ({orig_host}:{orig_port}):")
        orig_responses = send_command_sequence(orig_host, int(orig_port), commands)

        print(f"Alternative server ({reimp_host}:{reimp_port}):")
        reimp_responses = send_command_sequence(reimp_host, int(reimp_port), commands)
        
        os.makedirs('log', exist_ok=True)
        with open(f"log/{test_name}_org.log", 'w') as f:
            for resp in orig_responses:
                f.write(f"{resp}\n")
        
        with open(f"log/{test_name}_alt.log", 'w') as f:
            for resp in reimp_responses:
                f.write(f"{resp}\n")
        
        print(f"Results saved to log/{test_name}_*.log")
        print()

if __name__ == "__main__":
    main()