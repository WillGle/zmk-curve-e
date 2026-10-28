# Bằng chứng khảo sát

Dữ liệu gốc được giữ nguyên khi gom thư mục. Các kết luận cập nhật nằm trong
[tài liệu phần cứng](../hardware.md) và
[tài liệu firmware](../firmware.md); bản ghi cũ giữ giá trị lịch sử.

| File | Phạm vi |
| --- | --- |
| [product-independence-2026-10-28.json](product-independence-2026-10-28.json) | Sản phẩm độc lập, archive nguồn cũ, build BLE byte-identical và ESB cách ly |
| [esb-prototype-2026-10-28.json](esb-prototype-2026-10-28.json) | Ba build đạt, baseline BLE byte-identical, 97 assertion tạm; chưa flash/radio |
| [esb-source-audit-2026-10-28.json](esb-source-audit-2026-10-28.json) | 18 file nguồn/giấy phép đã kiểm hash, 3 probe tĩnh; chưa build/radio |
| [curved-layout-preflight-2026-10-28.json](curved-layout-preflight-2026-10-28.json) | Chỉ thay hình học 47 phím, config/keymap/transform giữ nguyên |
| [curved-layout-flash-session-2026-10-28.json](curved-layout-flash-session-2026-10-28.json) | Backup và readback 928 block BLE cũ khớp, nạp layout mới |
| [curved-layout-post-flash-studio-2026-10-28.json](curved-layout-post-flash-studio-2026-10-28.json) | 47 bộ tọa độ Studio khớp source, cổng mới ttyACM2 |
| [ble-compat-preflight-2026-10-28.json](ble-compat-preflight-2026-10-28.json) | Bản BLE mới: 928 block, passkey/experimental/2M tắt, SC/bonding/settings còn bật |
| [ble-compat-flash-session-2026-10-28.json](ble-compat-flash-session-2026-10-28.json) | Backup và readback app cũ khớp 930 block, nạp UF2 BLE mới |
| [ble-compat-post-flash-studio-2026-10-28.json](ble-compat-post-flash-studio-2026-10-28.json) | Studio sau nạp BLE mới, 4 layer × 47 bindings |
| [post-flash-hybrid-button-2026-10-28.json](post-flash-hybrid-button-2026-10-28.json) | Lượt đo nút hybrid: chưa thu sự kiện; chưa chốt thời điểm/đầu ra, không kết luận nút lỗi |
| [post-flash-ble-scan-2026-10-28.json](post-flash-ble-scan-2026-10-28.json) | Scan LE ngắn, chưa thấy tên Curve; không pair, không kết luận radio lỗi |
| [flash-session-2026-10-28.json](flash-session-2026-10-28.json) | Backup mới, preflight, copy UF2 và xác nhận ứng dụng khởi động lại |
| [post-flash-usb-2026-10-28.json](post-flash-usb-2026-10-28.json) | Descriptor USB/HID và hai cổng CDC của firmware mới |
| [post-flash-studio-2026-10-28.json](post-flash-studio-2026-10-28.json) | Bốn RPC Studio chỉ đọc, 4 layer × 47 bindings và layout 47 vị trí |
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
