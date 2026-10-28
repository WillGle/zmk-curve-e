# Tư liệu phần cứng Curve E Prototype 1

[Trang sản phẩm](../README.md) · [Tài liệu kết nối](../docs/hardware.md)

Tư liệu hiện có phục vụ nhận diện, sửa chữa và tiếp tục phục dựng mạch.
**Chưa đủ để đặt sản xuất PCB hoặc lắp ráp lại thiết bị.**

| Tư liệu cần triển khai | Trạng thái / vị trí |
| --- | --- |
| Sơ đồ nguyên lý và netlist đã đo | Chưa có bản đầy đủ; [kết nối suy ra từ firmware](../docs/hardware.md) là điểm bắt đầu |
| PCB CAD, Gerber và thông số chế tạo | Chưa tìm thấy trong phạm vi nguồn công khai đã khảo sát |
| BOM đủ mã linh kiện/giá trị | Chưa có; IC nguồn/sạc và nhiều linh kiện chưa nhận diện |
| Ảnh và bản ghi USB | [evidence/](evidence/README.md) |
| Nhận diện linh kiện, pinout ứng viên, bảng phép đo | [components-and-measurements.md](components-and-measurements.md) |
| Nguồn tham khảo gốc, thư viện và bootloader nền | [nguồn lưu trữ](../archive/upstream/README.md), kèm manifest và license khi có |

## Cấu hình lắp thực tế

Người dùng xác nhận vị trí hybrid đang lắp **nút bấm, không có encoder**.
[Bản ghi xác nhận](evidence/assembly-user-confirmation-2026-10-28.json).
Khai báo EC11 trong firmware mô tả khả năng của footprint; không chứng minh
encoder hiện diện. Chưa xác minh tọa độ ma trận của nút hybrid.

## Tiếp tục phục dựng

1. Đối chiếu đúng bo/revision với ảnh; đọc mã module và linh kiện nguồn.
2. Thực hiện phép đo theo [bảng nhận diện và đo](components-and-measurements.md),
   ghi rõ điều kiện đo và hai đầu net/pad.
3. Sau khi đủ dữ liệu mới dựng schematic/PCB/BOM; đánh dấu từng kết nối đã đo
   và phần chưa xác nhận. Không lấy pinout ứng viên làm pinout đấu dây đã chốt.

Xem [trạng thái tự chủ](../docs/status.md) và
[nguồn đã khảo sát](../docs/references/public-sources.md). Chưa tạo thư mục
schematic/PCB/BOM vì chưa có các file thiết kế tương ứng.
