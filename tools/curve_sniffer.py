#!/usr/bin/env python3
import struct
import sys
import os

# Linux input_event structure:
# struct input_event {
#     struct timeval time; (16 bytes on 64-bit: 8 bytes sec + 8 bytes usec)
#     __u16 type;          (2 bytes)
#     __u16 code;          (2 bytes)
#     __s32 value;         (4 bytes)
# }; -> Total: 24 bytes

EVENT_DEVICE = "/dev/input/event17"

# Standard Linux keycode to string mapping
KEY_MAP = {
    1: "ESC", 2: "1", 3: "2", 4: "3", 5: "4", 6: "5", 7: "6", 8: "7", 9: "8", 10: "9", 11: "0",
    12: "-", 13: "=", 14: "BACKSPACE", 15: "TAB", 16: "Q", 17: "W", 18: "E", 19: "R", 20: "T",
    21: "Y", 22: "U", 23: "I", 24: "O", 25: "P", 26: "[", 27: "]", 28: "ENTER",
    29: "L_CTRL", 30: "A", 31: "S", 32: "D", 33: "F", 34: "G", 35: "H", 36: "J", 37: "K",
    38: "L", 39: ";", 40: "'", 41: "`", 42: "L_SHIFT", 43: "\\", 44: "Z", 45: "X", 46: "C",
    47: "V", 48: "B", 49: "N", 50: "M", 51: ",", 52: ".", 53: "/", 54: "R_SHIFT",
    56: "L_ALT", 57: "SPACE", 58: "CAPSLOCK", 59: "F1", 60: "F2", 61: "F3", 62: "F4",
    63: "F5", 64: "F6", 65: "F7", 66: "F8", 67: "F9", 68: "F10", 69: "NUM_LOCK",
    70: "SCROLL_LOCK", 71: "KP_7", 72: "KP_8", 73: "KP_9", 74: "KP_MINUS", 75: "KP_4",
    76: "KP_5", 77: "KP_6", 78: "KP_PLUS", 79: "KP_1", 80: "KP_2", 81: "KP_3",
    82: "KP_0", 83: "KP_DOT", 87: "F11", 88: "F12", 96: "KP_ENTER", 97: "R_CTRL",
    98: "KP_SLASH", 100: "R_ALT", 102: "HOME", 103: "UP", 104: "PAGE_UP", 105: "LEFT",
    106: "RIGHT", 107: "END", 108: "DOWN", 109: "PAGE_DOWN", 110: "INSERT", 111: "DELETE",
    125: "L_GUI (WIN)", 126: "R_GUI (WIN)"
}

def main():
    if not os.path.exists(EVENT_DEVICE):
        print(f"Error: Device {EVENT_DEVICE} not found! Is the keyboard plugged in?")
        sys.exit(1)

    print("=" * 60)
    print("      DEKB CURVE E - REAL-TIME KEY PRESS SNIFFER")
    print("=" * 60)
    print(f"Listening on: {EVENT_DEVICE}")
    print("Nhấn thử từng nút trên bàn phím Curve E để xem tín hiệu:")
    print("Mục tiêu: Tìm các phím 0-9 và phím ENTER để ghép đôi Bluetooth.")
    print("Bấm Ctrl+C để thoát.\n")

    EVENT_FORMAT = "llHHI" if struct.calcsize("l") == 4 else "qqHHi"
    # Actually standard 64-bit linux: timeval (8 + 8) + u16 (2) + u16 (2) + s32 (4) = 24 bytes
    EVENT_SIZE = 24

    with open(EVENT_DEVICE, "rb") as f:
        while True:
            data = f.read(EVENT_SIZE)
            if not data or len(data) < EVENT_SIZE:
                break
            sec, usec, ev_type, code, value = struct.unpack("qqHHi", data)
            
            # ev_type == 1 is EV_KEY
            if ev_type == 1:
                state = "NHẤN (DOWN)" if value == 1 else ("THẢ (UP)" if value == 0 else "GIỮ (REPEAT)")
                key_name = KEY_MAP.get(code, f"CODE_{code}")
                
                # Highlight Numbers and Enter
                highlight = ""
                if key_name in ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "ENTER", "KP_ENTER"]:
                    highlight = " <==== [PHÍM CẦN DÙNG CHO BLUETOOTH PAIRING!]"
                
                if value == 1: # Only print on key down for clean display
                    print(f"[{state}] Key: {key_name:15} (Scancode: {code:3}){highlight}")

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nĐã dừng sniffer.")
