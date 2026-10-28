# Bằng chứng khảo sát

Dữ liệu gốc được giữ nguyên khi gom thư mục. Các kết luận cập nhật nằm trong
[tài liệu phần cứng](../hardware.md) và
[tài liệu firmware](../firmware.md); bản ghi cũ giữ giá trị lịch sử.

| File | Phạm vi |
| --- | --- |
| [local-update-audit-2026-10-28.json](local-update-audit-2026-10-28.json) | Audit local mới: USB, checksum, UF2, cache dependency/toolchain và upstream |
| [bootloader-2026-10-28.json](bootloader-2026-10-28.json) | INFO_UF2, metadata dump, kiểm tra thoát bootloader và nhận định tiếp sau |
| [installed-firmware-strings-2026-10-28.json](installed-firmware-strings-2026-10-28.json) | Chuỗi phiên bản trong dump cũ; chưa định danh nguồn byte-exact |
| [uf2-preflight-2026-10-28.json](uf2-preflight-2026-10-28.json) | Đánh giá archive đầu; phát hiện capacity vượt giới hạn 8 KiB |
| [uf2-preflight-flash-bound-2026-10-28.json](uf2-preflight-flash-bound-2026-10-28.json) | Kiểm tra artifact mới sau khi giới hạn code partition tại 0xEA000 |

Log build, effective config và checksum ở từng thư mục dưới
[firmware/](../../firmware/README.md). Dump `CURRENT.UF2` cục bộ tại
`firmware/recovery-2026-10-28/` được Git bỏ qua; không phải backup toàn flash.

Ảnh PCB và bản ghi USB nằm ở [bằng chứng phần cứng](../../hardware/evidence/README.md).
Đường dẫn artifact trong các JSON là đường dẫn lịch sử; tra bản tương ứng tại
`firmware/releases/<phiên-bản>/`. JSON gốc được giữ nguyên để bảo toàn bằng chứng.
