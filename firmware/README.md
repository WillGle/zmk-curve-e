# Firmware Curve E

[Trang sản phẩm](../README.md) · [Cách cài firmware](../guides/install-firmware.md)

## Chọn bản để sử dụng

| Bản | Trạng thái |
| --- | --- |
| [2026-10-28-zmk-4dd95d7-ble-compat-curved](releases/2026-10-28-zmk-4dd95d7-ble-compat-curved/) | **Bản hiện hành đã cài**: layout cong 47 phím, giữ BLE/passkey-off; Studio xác nhận đủ tọa độ |
| [2026-10-28-zmk-4dd95d7-ble-compat](releases/2026-10-28-zmk-4dd95d7-ble-compat/) | Bản BLE trước thay layout; đã nạp và readback 928 block khớp archive |
| [2026-10-28-zmk-4dd95d7-flash-bound](releases/2026-10-28-zmk-4dd95d7-flash-bound/) | Bản trước: đã nạp và readback 930 block khớp archive; giữ để đối chiếu lịch sử |
| [2026-10-28-zmk-4dd95d7](releases/2026-10-28-zmk-4dd95d7/) | Archive lịch sử trước sửa capacity 8 KiB; dùng bản flash-bound cho bước đánh giá tiếp theo |

Bản hiện hành dành cho target `curve_e//zmk`, nRF52840, app bắt đầu tại
`0x26000`, kết thúc code partition tại `0xEA000`.
Mã bootloader nền được thiết bị báo cáo nhận cả family `0xDE1740E1` và
`0xADA52840`; phần sửa riêng `dirty` chưa có để đối chiếu binary-exact.
Thông tin này chưa bảo đảm tương thích mọi revision Curve E.

- [File zmk.uf2](releases/2026-10-28-zmk-4dd95d7-ble-compat-curved/zmk.uf2).
- [Manifest nguồn và cấu hình build](releases/2026-10-28-zmk-4dd95d7-ble-compat-curved/build.json).
- [SHA256SUMS](releases/2026-10-28-zmk-4dd95d7-ble-compat-curved/SHA256SUMS).
- [Kết quả kiểm chứng và giới hạn](../docs/verification.md).
- [Trạng thái board và checklist nâng cấp local](../docs/update-checklist.md).

Bản `flash-bound` trước đây đã được build offline byte-identical:
[log và manifest](verification/2026-10-28-offline/verification.json).
Kết quả bản trước trên bo: [USB](../docs/evidence/post-flash-usb-2026-10-28.json),
[Studio](../docs/evidence/post-flash-studio-2026-10-28.json).
Manifest build trong release giữ trạng thái tại lúc build; kết quả sau nạp
được lưu riêng ở các bằng chứng trên.

Bản BLE mới: [cấu hình và kiểm chứng](../docs/bluetooth.md),
[Studio sau nạp](../docs/evidence/ble-compat-post-flash-studio-2026-10-28.json).

Layout hiện hành: [xem trước](../docs/assets/curve-e-studio-layout.svg),
[tọa độ đọc lại từ Studio](../docs/evidence/curved-layout-post-flash-studio-2026-10-28.json).

Tại thư mục bản đã chọn, chạy `sha256sum -c SHA256SUMS` trước khi dùng.
Đọc [cách cài](../guides/install-firmware.md) và
[khôi phục](../guides/recovery.md) trước khi ghi thiết bị.

## Lưu trữ và phát triển

Mỗi bản trong `releases/` giữ UF2, log, effective config, devicetree,
manifest dependency và checksum. Không ghi đè bản cũ khi nguồn thay đổi.
Manifest/bằng chứng lịch sử có thể ghi đường dẫn `firmware/<phiên-bản>` trước
khi gom nhóm; các bản đó nay nằm tại `firmware/releases/<phiên-bản>`.
Nội dung manifest và checksum của chúng được giữ nguyên.

Dump riêng tư vẫn ở `firmware/recovery-2026-10-28/`, được Git bỏ qua;
đây không phải release hoặc backup toàn flash, chưa thử restore.
Workspace build tại `build/curve-e/` trong repo, được Git bỏ qua và có thể tạo
lại theo [hướng dẫn build](../guides/build-firmware.md).
Mã nguồn ở `boards/`, `config/`, `zephyr/`; xem [tài liệu firmware](../docs/firmware.md).
