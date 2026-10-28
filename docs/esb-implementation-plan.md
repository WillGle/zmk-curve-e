# Kế hoạch triển khai ESB Curve E

> Thực hiện tuần tự trong phiên này bằng skill `executing-plans`.

**Mục tiêu:** build được cặp firmware USB/BLE/ESB thử nghiệm trên ZMK mới;
không nạp thiết bị và không thay release đang dùng.
**Thiết kế:** [ESB độc lập](esb-development.md), được người dùng duyệt.
**Kiến trúc:** module ESB cục bộ, patch upstream được lưu rõ ràng; snapshot
upstream sạch chỉ được patch trong workspace build riêng. Giao thức hai đầu
có xác thực session mới sau reset, không cần tương thích DENGLE cũ.
**Nền:** ZMK 4dd95d7840eb3d5a3f59bb29fbf1ab054715ef85 / Zephyr đã pin.

## Ràng buộc và điểm cần kiểm tra

Không commit/push/flash, không thêm test duy trì, không sửa snapshot nguồn.
Giữ enum NONE=0/USB=1/BLE=2 để không làm sai settings hiện tại.
Sai khóa/gói ngắn/replay không gia hạn link; session mới sau reset phải ngăn
phát lại gói cũ. Lỗi chuyển radio không được báo thành công. Report HID hai
đầu phải cùng codec/giới hạn. Kết quả compile không xác nhận radio hoạt động.
Mã/bằng chứng/build đều ở repo; khóa và binary chứa khóa ở thư mục riêng ignored.

## Công việc

- [x] 1. Tạo module `experimental/esb/` gồm Kconfig/CMake, driver ESB có nguồn
  gốc/notice, cấu hình receiver DK và keyboard; source build riêng dưới
  `build/curve-e/esb-work/`. Ghi revision nguồn và patch áp dụng vào workspace.
- [x] 2. Codec và session: `include/curve_esb/protocol.h`, `src/protocol.c`;
  các hàm encode/decode chính xác chiều dài/version, HMAC-SHA256 cho handshake,
  AES-CCM cho report với session key mới từ nonce hai đầu. Probe tạm bằng
  công cụ local kiểm sai version/length/tag/replay/reset/counter exhaustion.
- [x] 3. Transport keyboard/receiver trong `src/transport.c`; report keyboard,
  consumer và mouse được serialize rõ ràng, timeout release-all sau 500 ms
  chỉ gia hạn bằng report xác thực; retry handshake khi không có receiver.
  Patch endpoint/metadata trong `patches/zmk-esb.patch`; cấm BLE ở Kconfig
  prototype theo ràng buộc bổ sung. Không sửa BLE stop/start.
- [x] 4. `tools/build-esb.sh`: chuẩn bị copy sạch upstream đã pin, áp dụng patch,
  build offline receiver `nrf52840dk/nrf52840/zmk`, keyboard `curve_e//zmk`
  với Studio tắt để bảo vệ settings/keymap BLE và cấu hình hồi quy USB/BLE. Ghi log/config/DTS/hash/manifest
  riêng, loại khóa khỏi log và metadata công khai.
- [x] 5. Kiểm tra source/format/hash, probe codec/session, cả ba build; rà soát
  độc lập thay đổi và xử lý finding quan trọng. Cập nhật thiết kế/trạng thái,
  hướng dẫn receiver/provision/build/giới hạn chưa kiểm thử trên phần cứng.

## Quyết định thực thi

Mỗi bước được ghi kết quả tại cuối tài liệu này. Không tạo commit tự động.
Dùng workspace build riêng để không thay checkout cache hoặc release hiện hành.
Chỉ tích hợp source sau thiết kế đã duyệt; chưa cần mua receiver để compile.

## Ràng buộc bổ sung sau duyệt

Người dùng yêu cầu thử ESB không tác động BLE, BLE là wireless mặc định và
bắt buộc ổn định. Ruling: bước đầu build USB+ESB riêng, Kconfig cấm BLE trong
bản thử nghiệm; không patch `ble.c`/lifecycle hay đổi release BLE hiện tại.
Giữ USB/BLE làm firmware chính và build hồi quy riêng. Live switching ba mode
được hoãn tới khi có receiver và đủ bằng chứng BLE, không coi bản USB+ESB là
firmware ba mode hoàn chỉnh. Quyết định này thay yêu cầu ghép BLE vào bản thử
nghiệm ngay ở các bước trên; mục tiêu dài hạn vẫn là ba mode.

## Kết quả thực thi

Cả 5 bước phần mềm của prototype cách ly đã thực hiện. 56 dependency sạch/pin
được kiểm trước build; 3 target build đạt; baseline UF2 byte-identical. 97 assertion
tạm với sanitizer đạt; không thêm test duy trì. Rà soát độc lập tìm finding ở
reconnect, namespace settings, queue overflow, nguồn stale và watchdog; đã sửa
và kiểm lại bằng probe/build. Không có lượt review độc lập thứ hai sau sửa.

Live switching ba mode, thử radio, pairing/reconnect BLE, sleep/wake và đo pin
vẫn chưa hoàn thành; không nằm trong khẳng định prototype đã build. Xem
[bằng chứng](evidence/esb-prototype-2026-10-28.json).
