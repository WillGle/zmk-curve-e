# Probe lịch sử — không thuộc công cụ được hỗ trợ

Hai script được giữ nguyên để bảo toàn lịch sử điều tra thiết bị:

- `probe_serial.py`: hard-code `/dev/ttyACM0`, gửi các chuỗi thử vào serial,
  có hướng dẫn quyền `chmod 666` không còn phù hợp với quy trình hiện hành.
- `curve_sniffer.py`: hard-code input device, hướng dẫn cũ liên quan passkey.

Không chạy tự động, không coi chúng là công cụ nhận diện cổng hoặc kiểm chứng
firmware hiện hành. Cổng thay đổi sau reset; thao tác quyền và pairing phải theo
[hướng dẫn hiện hành](../../guides/configure-keymap.md). Không dùng `chmod 666`.
Các script này không được chạy hay sửa nội dung trong đợt tái cấu trúc.
