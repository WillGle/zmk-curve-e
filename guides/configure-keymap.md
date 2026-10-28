# Tùy chỉnh phím và kết nối

[Hướng dẫn](README.md) · [Firmware để tải](../firmware/README.md)

Các thao tác dưới đây dành cho firmware mới trong repo sau khi đã nạp và
xác minh USB. Firmware mới đã nạp và đọc được keymap qua Studio; thao tác phím/encoder/BLE
còn cần kiểm tra thực tế. Firmware vendor cũ có thể có keymap và tính năng khác.

## Tính năng và thao tác

Bo người dùng hiện lắp **nút bấm tại vị trí hybrid, không có encoder**.
Các hành động xoay trong bảng là cấu hình firmware cho tùy chọn encoder;
không áp dụng cho cấu hình lắp này. Nút hybrid dùng binding tại vị trí ma
trận của nó; chưa xác minh tọa độ thực tế, không tự đổi binding.

| Chức năng | Cấu hình hiện hành |
| --- | --- |
| Phím | 47 vị trí, 4 layer; thứ tự trong [keymap](../boards/curve_e/curve_e.keymap) |
| Layer 0 | Gõ chữ; encoder âm lượng |
| Layer 1 | Giữ `mo 1`: số, mũi tên/Home/End/Page; encoder Page Down/Up |
| Layer 2 | Giữ `mo 2`: ký hiệu, di chuột và click trái/giữa/phải; encoder cuộn dọc |
| Layer 3 | Giữ đồng thời `mo 1` và `mo 2`: BT_CLR, BT_SEL 0–4, chọn output, Caps Word, reset/bootloader; encoder độ sáng |
| Tap dance | ESC → tap ESC/double GRAVE; Backspace → tap BSPC/double DEL; cửa sổ 200 ms |
| Combo layer 0 | Q+W → ESC; A+S → TAB; O+P → BSPC; K+L → ENTER; V+B → Caps Word; Z+X → đổi USB/BLE |
| USB | HID keyboard/mouse, Boot Protocol và Studio RPC qua snippet `studio-rpc-usb-uart` |
| Rollover | NKRO ở report mode; USB Boot Protocol dùng báo cáo 6 phím theo yêu cầu host |
| Studio | 47 vị trí trên lưới logic, đúng thứ tự transform; hình hiển thị chưa mô phỏng đường cong PCB |
| Bluetooth | 5 profile; công suất yêu cầu +8 dBm; interval yêu cầu 7.5–15 ms, peripheral latency 0 |
| Nguồn | Idle 30 giây; yêu cầu sleep sau 30 phút; bật báo pin BLE |

Combo chỉ áp dụng khi layer cao nhất đang hoạt động là layer 0. Cửa sổ combo
50 ms và tap-dance 200 ms có thể làm chậm các phím liên quan. Double-tap
Backspace sẽ thành Delete, không phải hai lần xóa lùi. GRAVE tạo ký tự theo
layout của hệ điều hành; dấu `~` còn phụ thuộc Shift/layout.

Encoder giữ tỷ lệ cũ một hành động trên bốn bước quadrature bằng `steps = 4`,
`triggers-per-rotation = 1`. Đây là tỷ lệ chuẩn hóa, chưa phải số xung/vòng
đã đo. Cuộn dùng tốc độ 100 đơn vị/giây và tap 20 ms để vượt chu kỳ input
16 ms của `msc`; chiều quay và cảm giác cuộn cần thử thực tế.

## Studio và Bluetooth

Sau khi build/nạp và USB hoạt động, mở [ZMK Studio](../docs/references/official.md#công-cụ-studio) bằng
trình duyệt hỗ trợ Web Serial rồi chọn cổng Studio. Board vẫn giữ CDC console
cũ và thêm CDC riêng cho RPC; số `ttyACM` có thể thay đổi, không mặc định luôn
là `/dev/ttyACM0`. Studio locking hiện tắt theo cấu hình đã có.

Studio lưu keymap vào settings; settings có thể ghi đè keymap vừa biên dịch.
Sao lưu thay đổi trong Studio trước khi xóa settings để kiểm tra keymap mới.
Studio không thay thế việc build khi cần đổi GPIO, cấu hình radio hoặc thêm
định nghĩa behavior. Không mặc định mọi tính năng ZMK đều chỉnh được trên web.

Nếu cập nhật HID descriptor để có chức năng chuột, cần xóa pairing ở cả host
và profile bàn phím rồi ghép đôi lại để host đọc descriptor mới. BT_CLR tác
động profile đang chọn. Bản `ble-compat` đang cài đã tắt experimental BLE và nhập passkey; giữ PHY 2M
tắt rõ ràng. Xem [cấu hình BLE hiện hành](../docs/bluetooth.md); chưa có kết quả
thử ổn định dài hạn trên host.

NKRO không bật extended report để tránh giảm tương thích không cần thiết.
USB Boot Protocol là chế độ dự phòng do host yêu cầu; chưa kiểm thử BIOS/UEFI
hoặc Android. Nếu host không nhận descriptor mới, cần ghép đôi lại hoặc kiểm
tra với cấu hình rollover tương thích của host.

## Chỉnh keymap từ nguồn

Sửa bindings trong [boards/curve_e/curve_e.keymap](../boards/curve_e/curve_e.keymap),
giữ thứ tự 47 vị trí theo transform; cấu hình tính năng nằm ở
[config/curve_e.conf](../config/curve_e.conf). Làm theo [hướng dẫn build](build-firmware.md),
lưu artifact ở thư mục phiên bản mới rồi [kiểm tra trước khi nạp](../docs/verification.md#kiểm-tra-trước-khi-nạp).
Settings Studio có thể ghi đè keymap biên dịch; sao lưu cấu hình Studio trước
khi cân nhắc xóa settings.
