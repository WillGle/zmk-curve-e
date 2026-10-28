# Nguồn gốc và phạm vi giấy phép

Curve E Firmware là dự án được bảo trì độc lập. Phần cứng Curve E do
Deemen17 thiết kế; nguồn board/config ban đầu được kế thừa từ
[Deemen17/zmk-config-curve_e](https://github.com/Deemen17/zmk-config-curve_e).
Việc tách repository không thay đổi quyền tác giả của nguồn kế thừa.

## Phần mềm

[LICENSE](LICENSE) cấp phép MIT cho phần mềm do dự án này duy trì. Các file
kế thừa có SPDX MIT giữ nguyên copyright và notice gốc; không thay chúng bằng
copyright của dự án mới. ZMK là dependency bên ngoài; không phải mã do dự án
này sở hữu. Zephyr và mọi dependency đi theo giấy phép riêng của chúng.
`config/west.yml` xác định đúng revision cần kiểm tra trước khi phân phối binary.

Driver ESB và header kế thừa từ Team PHDesign có notice MIT trong file và
[LICENSE riêng](experimental/esb/LICENSE). Nguồn gốc, commit và checksum tại
[manifest snapshot](archive/upstream/ph-design-zmk-aba1104-esb/source.json).
Codec/session và tích hợp của dự án được duy trì riêng; không nhận toàn bộ fork
vendor là phần mềm do dự án này viết.

## Tài liệu, phần cứng và snapshot

LICENSE ở gốc **không cấp lại quyền** đối với ảnh người dùng/vendor, PDF,
CAD, footprint, symbol, archive upstream, bootloader, Nordic SDK hoặc SoftDevice.
Các thành phần đó giữ điều khoản/nguồn gốc riêng như ghi tại
[archive/upstream](archive/upstream/README.md) và
[nguồn tham khảo](docs/references/README.md).

Đặc biệt, thư viện KiCad chưa xác định được license cấp repo; khả năng tải
công khai không được coi là quyền phân phối lại. Snapshot bootloader có
thành phần theo điều khoản khác nhau và không chứa bản sửa `dirty` thực tế.
Không suy ra toàn bộ bundle hay binary firmware đều MIT từ LICENSE ở gốc.

Khóa ghép ESB và binary chứa khóa nằm trong thư mục local private ignored;
không đưa chúng vào bản công khai. Tên phần cứng, VID/PID và attribution không
phải tuyên bố sở hữu nhãn hiệu hoặc chứng nhận từ nhà thiết kế gốc.
