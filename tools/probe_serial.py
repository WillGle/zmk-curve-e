#!/usr/bin/env python3
import os
import sys
import time
import select

PORT = "/dev/ttyACM0"

def main():
    if not os.path.exists(PORT):
        print(f"Error: {PORT} does not exist.")
        sys.exit(1)

    print(f"Opening {PORT}...")
    try:
        # Open in non-blocking raw binary mode
        fd = os.open(PORT, os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)
    except PermissionError:
        print(f"Permission denied on {PORT}!")
        print(f"Vui lòng chạy: sudo chmod 666 {PORT}")
        sys.exit(1)

    print(f"Successfully opened {PORT}!")
    print("Sending probe commands (Enter, 'help', 'version', 'AT', etc.)...\n")

    test_payloads = [
        b"\r\n",
        b"help\r\n",
        b"version\r\n",
        b"?\r\n",
        b"AT\r\n",
        b"AT+VERSION\r\n",
        b"status\r\n",
    ]

    for payload in test_payloads:
        print(f"--> Sending: {payload.strip()!r}")
        try:
            os.write(fd, payload)
        except Exception as e:
            print(f"Write error: {e}")
        
        # Wait up to 300ms for response
        start_time = time.time()
        received = b""
        while time.time() - start_time < 0.4:
            r, _, _ = select.select([fd], [], [], 0.1)
            if r:
                chunk = os.read(fd, 1024)
                if chunk:
                    received += chunk
        
        if received:
            print(f"<-- Response: {received!r}")
            try:
                print(f"    (Decoded): {received.decode('utf-8', errors='replace')}")
            except Exception:
                pass
        else:
            print("    (No response)")

    print("\nListening for 3 seconds for any unsolicited logs (e.g. boot/debug messages)...")
    start_time = time.time()
    while time.time() - start_time < 3.0:
        r, _, _ = select.select([fd], [], [], 0.5)
        if r:
            chunk = os.read(fd, 1024)
            if chunk:
                print(f"[LOG]: {chunk!r}")

    os.close(fd)
    print("Probe completed.")

if __name__ == "__main__":
    main()
