# Curve E — PCB Prototype 1

Bàn phím Curve E do Deemen17 thiết kế. Repo này phục vụ tự cập nhật và duy trì
firmware ZMK cho bo hiện có bằng công cụ local, tiếp tục cập nhật khi vendor
không còn cung cấp bản mới. Không cần sửa PCB để cập nhật firmware ứng dụng.
Tư liệu phần cứng được giữ riêng để hỗ trợ bảo trì và phục dựng khi cần.

| Bạn cần gì? | Bắt đầu ở đâu? |
| --- | --- |
| Sử dụng, nạp firmware, đổi phím hoặc khôi phục | [Hướng dẫn thao tác](guides/README.md) |
| Hiểu thiết kế, cấu hình và kết quả kiểm chứng | [Tài liệu kỹ thuật](docs/README.md) |
| Lấy firmware để nạp | [Danh mục firmware](firmware/README.md) |
| Sửa chữa hoặc triển khai lại phần cứng | [Tư liệu phần cứng](hardware/README.md) |

## Trạng thái hiện tại

- Firmware mới đã build offline, nạp và xác nhận USB/Studio; **còn chờ thử
  phím/nút hybrid, BLE và sleep/wake**.
  [Bản hiện hành và checksum](firmware/README.md) ghi rõ phạm vi tương thích.
- Đã ghi nhận GPIO từ firmware, ảnh PCB, USB và bootloader. **Chưa có schematic,
  PCB CAD hoặc BOM đủ để chế tạo.** Xem [phần còn thiếu](docs/status.md).
- Nguồn dẫn ngoài repo được gom tại [nguồn tham khảo](docs/references/README.md).

Bản đang cài: `2026-10-28-zmk-4dd95d7-ble-compat-curved` — bỏ nhập passkey,
BLE ưu tiên tương thích; [chi tiết và phần còn cần thử](docs/bluetooth.md).

## Phát triển firmware

Mã nguồn board tại [boards/curve_e/](boards/curve_e/board.yml), cấu hình tại
[config/](config/curve_e.conf), module tại [zephyr/module.yml](zephyr/module.yml).
Target `curve_e//zmk`; ZMK khóa commit trong [config/west.yml](config/west.yml).
Làm theo [hướng dẫn build](guides/build-firmware.md); workspace ở
`build/curve-e/`, bản bàn giao được lưu riêng trong `firmware/releases/`.

![PCB Curve E Prototype 1](hardware/evidence/pcb-front-user.png)
