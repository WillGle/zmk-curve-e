# Firmware Curve E

[Trang sản phẩm](../README.md) · [Cách cài firmware](../guides/install-firmware.md)

## Chọn bản để sử dụng

| Bản | Trạng thái |
| --- | --- |
| [2026-10-28-zmk-4dd95d7-flash-bound](releases/2026-10-28-zmk-4dd95d7-flash-bound/) | Bản hiện hành: đã build và kiểm tra tĩnh; **chưa nạp/thử trên bo** |
| [2026-10-28-zmk-4dd95d7](releases/2026-10-28-zmk-4dd95d7/) | Archive lịch sử trước sửa capacity 8 KiB; dùng bản flash-bound cho bước đánh giá tiếp theo |

Bản hiện hành dành cho target `curve_e//zmk`, nRF52840, app bắt đầu tại
`0x26000`, kết thúc code partition tại `0xEA000`.
Mã bootloader nền được thiết bị báo cáo nhận cả family `0xDE1740E1` và
`0xADA52840`; phần sửa riêng `dirty` chưa có để đối chiếu binary-exact.
Thông tin này chưa bảo đảm tương thích mọi revision Curve E.

- [File zmk.uf2](releases/2026-10-28-zmk-4dd95d7-flash-bound/zmk.uf2).
- [Manifest nguồn và cấu hình build](releases/2026-10-28-zmk-4dd95d7-flash-bound/build.json).
- [SHA256SUMS](releases/2026-10-28-zmk-4dd95d7-flash-bound/SHA256SUMS).
- [Kết quả kiểm chứng và giới hạn](../docs/verification.md).
- [Trạng thái board và checklist nâng cấp local](../docs/update-checklist.md).

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
