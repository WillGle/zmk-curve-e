# Curve ESB — prototype riêng, không thay firmware BLE

Mục tiêu và thiết kế: [phát triển ESB](../../docs/esb-development.md).
Đây là firmware thử nghiệm **USB+ESB**, không phải bản ba mode đã nghiệm thu.
Kconfig bắt buộc `!ZMK_BLE`; không có thay đổi `ble.c` hay lifecycle Bluetooth.
Firmware BLE hiện hành vẫn là bản ở `firmware/releases/`.

Build offline từ cache đã pin:

```sh
./tools/build-esb.sh all
```

Lệnh kiểm 56 dependency theo frozen manifest hiện hành, yêu cầu cache sạch,
tạo lại source bằng `git archive`, áp dụng patch cục bộ rồi build receiver DK,
keyboard và baseline USB/BLE. Không có `west update`, `west flash` hoặc ghi USB.
Cần Docker image đã cache; xem [build chính](../../guides/build-firmware.md).

- Source/cache/log/build: `build/curve-e/esb-work/`.
- Khóa local: `firmware/experimental/private/pair-key.h`, tạo ngẫu nhiên nếu chưa có.
- Các firmware có chứa khóa phải giữ riêng và không đưa vào Git.
- Receiver target: `nrf52840dk/nrf52840/zmk`, shield `curve_esb_receiver`.
- Bàn phím target: `curve_e//zmk`. Keymap thử được sinh riêng, các phím BT
  được thay bằng `none`, vị trí OUT_BLE trên Adjust thành OUT_ESB.
- OUT_TOG đổi USB/ESB, OUT_USB về USB. Prototype khởi động ưu tiên USB;
  ESB chỉ được chọn khi session đã sẵn sàng và người dùng chọn OUT_ESB.
- Studio bị tắt ở prototype để không ghi keymap thử vào settings của bản BLE.
  Endpoint thử dùng namespace `curve_esb_endpoints`, tách khỏi `endpoints`.
- Sleep bị tắt trong prototype; chưa đo pin/latency/range hoặc chứng minh radio.

## Giao thức v1

Chỉ hỗ trợ nRF52840. ESB 2 Mbps, kênh 76, +4 dBm, payload tối đa 32 byte.
Địa chỉ radio lấy từ HMAC của khóa (không lộ trực tiếp byte khóa).
Codec: keyboard gồm modifier + 13 byte NKRO, consumer gồm sáu số LE16,
mouse gồm button + bốn số LE16. Không extended NKRO/smooth-scrolling.

HELLO mang nonce ngẫu nhiên 8 byte của keyboard, được HMAC-SHA256 xác thực
với tag 8 byte. Receiver tạo nonce riêng 8 byte và gửi CHALLENGE được xác thực.
Session key là 16 byte đầu HMAC-SHA256 của domain `curve-esb-v1` + hai nonce.
AES-CCM dùng nonce 13 byte (nonce receiver + counter LE32 + direction byte 0),
header version/type/counter làm AAD, tag 8 byte. Counter bắt đầu ở 1, không
quay vòng. Không dùng lại session sau reconnect; lỗi entropy không được dùng
nonce rỗng để phát. Khóa/nonce hoàn toàn mới hai đầu ngăn gói cũ qua reset,
với xác suất trùng nonce ngẫu nhiên chưa phải bằng không.

Receiver chỉ gia hạn link sau decrypt và kiểm tra đúng chiều dài/replay;
500 ms mất gói hợp lệ thì release-all. Keyboard dùng STATUS có HMAC bằng
session key và counter tăng để xác nhận receiver còn session; ACK radio đơn
thuần không gia hạn kết nối. Sau 1 giây không có STATUS mới, tạo HELLO mới.
HELLO lặp lại hiện tại chỉ gửi lại cùng challenge, không reset counter nhận.
Hàng đợi có giới hạn nhưng giữ mailbox trạng thái mới nhất để thả phím khi
queue đầy; chỉ lặp lại trạng thái nút chuột, không lặp chuyển động.

Đây chưa phải audit mật mã đầy đủ. Không có pairing tự động công khai, không
bảo đảm chống gây nhiễu hoặc DoS bằng phát lại HELLO hợp lệ đã thu được. Không
có Studio qua ESB hay tương thích firmware DENGLE cũ. Phải kiểm thử phần cứng
trước khi thử trên board chính; build pass không phải radio pass.

Driver ESB/header có nguồn Team PHDesign, commit
`aba1104a9777d851df1c3ea032468d44c42ed3f0`, giữ MIT notice và [LICENSE](LICENSE).
Nguồn nguyên bản/checksum ở [snapshot](../../hardware/upstream/ph-design-zmk-aba1104-esb/source.json).
Bản port tách queue, bảo vệ watchdog/ACK buffer, dùng IRQ động Zephyr khi BLE
bị cấm; không giữ thủ thuật thay VTOR của fork. Patch giữ enum upstream
NONE=0, USB=1, BLE=2 và thêm ESB=3 dưới Kconfig thử nghiệm.
