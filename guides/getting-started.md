# Bắt đầu với Curve E

[Hướng dẫn](README.md) · [Trang sản phẩm](../README.md)

Mục tiêu sử dụng repo là tự duy trì và cập nhật firmware cho bo hiện có.
Không cần sửa PCB; luồng local là chọn/cập nhật nguồn → build → kiểm tra UF2 →
nạp ứng dụng → kiểm tra trên bo → ghi lại kết quả cùng phiên bản firmware.
Xem [hướng dẫn build](build-firmware.md), [cài firmware](install-firmware.md) và
[danh mục phiên bản](../firmware/README.md).

1. Kiểm tra tên/revision trên PCB. Tư liệu hiện có mô tả Curve E Prototype 1;
   ảnh trong repo chưa được xác nhận là đúng bo đang kết nối USB.
2. Cắm USB để kiểm tra thiết bị hiện tại. Firmware vendor từng nhận tên
   `DEKB CURVE E BLE`, VID:PID `DE17:40E1`; đây là thông tin nhận diện đã ghi
   nhận, chưa phải kiểm chứng toàn bộ bàn phím.
3. Mở [danh mục firmware](../firmware/README.md) để biết bản nào hiện hành,
   đã được kiểm tra đến đâu và có phù hợp bootloader của bạn không.
4. Nếu cần nâng cấp, đọc [cài firmware](install-firmware.md) và
   [khôi phục](recovery.md) trước khi ghi vào thiết bị.
5. Sau khi bản mới hoạt động, dùng [hướng dẫn keymap](configure-keymap.md)
   để đổi phím bằng Studio hoặc chỉnh nguồn rồi build lại.

Nếu mục tiêu là sửa hoặc làm lại PCB, bắt đầu từ
[tư liệu phần cứng](../hardware/README.md). Nếu cần đánh giá mức độ độc lập
với vendor, xem [trạng thái tự chủ](../docs/status.md).
