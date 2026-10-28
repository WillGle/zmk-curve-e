# Curve E — trạng thái tự chủ, 28/10/2026

Mục tiêu hiện hành là **tự cập nhật firmware cho bo đang hoạt động bằng nguồn
và công cụ tại local**, tiếp tục duy trì ZMK khi vendor không còn cung cấp
cập nhật. Không cần sửa mạch, dựng lại PCB hoặc hoàn thiện BOM để thực hiện
luồng cập nhật ứng dụng này.

Bản đang cài là `2026-10-28-zmk-4dd95d7-ble-compat`: tắt passkey và experimental
BLE, PHY 2M tắt rõ ràng; xem [cấu hình BLE](bluetooth.md).
Đã có nguồn board/keymap, bản build và checksum trong repo. Đã build lại
offline, nạp ứng dụng và xác nhận USB/Studio trên thiết bị. Studio trả đúng
4 layer × 47 phím và layout 47 vị trí. Còn cần thử thực tế phím/nút hybrid, BLE
và sleep/wake; xem [checklist hiện hành](update-checklist.md).

Khảo sát schematic/PCB/BOM là tư liệu bảo trì riêng. Việc thiếu các file đó
không chặn cập nhật firmware hiện hành. Backup đầy đủ flash/UICR và nguồn
bootloader riêng vẫn chưa có; đây là giới hạn phục hồi nếu bootloader bị hỏng,
không phải yêu cầu thay mạch cho mỗi lần cập nhật ứng dụng.

| Khả năng | Bằng chứng hiện có | Giới hạn |
| --- | --- | --- |
| Tự duy trì firmware | [Board và build](firmware.md); ZMK/Zephyr commit đã ghi; artifact mới trong repo | Build offline byte-identical; đã nạp, USB/Studio đạt; còn thử chức năng thực tế |
| Giữ nguồn vendor công khai | [Manifest snapshot](../hardware/upstream/sources.json), archive MIT kèm LICENSE | Nhánh dev không phải firmware đã chứng minh chạy trên bo |
| Xác định firmware đang cài | [Chuỗi trong dump](evidence/installed-firmware-strings-2026-10-28.json): Zephyr 3.5.0, DEKB 20260426, build dacab4875df7 | Chưa truy ra nguồn chính xác; marker dùng chung nhiều board |
| Nạp app qua bootloader | [Phiên nạp thực tế](evidence/flash-session-2026-10-28.json), USB và Studio mới hoạt động | Chưa thử rollback; bản bootloader `dirty` chưa đối chiếu binary |
| Sao lưu firmware cũ | CURRENT.UF2 riêng tư, checksum; vùng `[0x1000,0xEA000)` | Thiếu MBR, bootloader, settings cuối flash, UICR; chưa thử restore |
| Khôi phục qua SWD | Pad SWD/SWC/GND/3.3V nhìn thấy trong ảnh | Chưa đo pinout/VTref hoặc kiểm tra quyền đọc; không chạy recover/erase |
| Tái tạo phần cứng | Matrix/encoder/battery từ firmware; ảnh; pinout module ứng viên | Thiếu CAD, BOM, IC nguồn/sạc, đường USB và nhiều net cần đo |

## Nguồn đã giữ

[Hướng dẫn snapshot](../hardware/upstream/README.md) ghi phạm vi, license và
cách kiểm tra. Hai snapshot firmware MIT, nguồn nền bootloader kèm license gốc,
và hai file thư viện KiCad được giữ tại `hardware/upstream/`. Thư viện chưa tìm
thấy license cấp repo; không suy ra quyền tái sử dụng/phân phối. Nguồn bootloader
nền không chứa phần sửa `dirty` hoặc toàn bộ submodule. Chưa có bản mirror đầy đủ
mọi dependency/toolchain để cam kết build offline trên máy mới.

[Khảo sát nguồn công khai](references/public-sources.md) ghi repo, commit và phạm vi tìm kiếm.
Repo `CruveE` tồn tại nhưng rỗng; chưa tìm thấy schematic/PCB/Gerber/BOM Curve
trong phạm vi đã khảo sát. Không dùng thiết kế Kimiro làm sơ đồ Curve.

## Kết nối mới tìm thấy cần đo

Source vendor dev trùng ma trận, encoder và cầu chia áp với repo này. Nó còn
khai báo LED xanh P0.03, LED trắng P1.10, ext-power P1.09, WS2812 MOSI P0.15.
Config cũ gọi P1.10 là xanh, còn số LED 10 trong dev được tác giả ghi tùy ý.
Các net này là ứng viên từ phần mềm; chưa bật trong firmware hiện hành.
Pinout EBYTE E73-2G4M08S1C và phép đo cụ thể nằm trong
[nhận diện linh kiện](../hardware/components-and-measurements.md). Model module chưa chốt
chỉ bằng ảnh; không dùng bảng ứng viên làm pinout đấu dây đã xác nhận.

## Khảo sát phần cứng nếu cần phục dựng sau này

1. Đối chiếu đúng PCB đang kết nối với ảnh; chụp rõ hai mặt, chữ module,
   U2/U3/U4, Q1/Q2, SD1, X1 và cụm USB/nguồn. Giữ nguyên revision và hướng ảnh.
2. Rút USB và ngắt pin trước phép đo continuity/điện trở. Đối chiếu pad debug,
   matrix, cực diode, USB D+/D−/CC và đường pin/cầu chia áp theo bảng phép đo.
3. Đo điện áp tại GND/VTref đã xác nhận trước khi nối probe. Nếu có probe SWD,
   nhận diện probe rồi chỉ đọc chip/flash/UICR khi tool không tự unlock/recover.
   Nếu debug bị khóa thì dừng; mass erase sẽ phá dữ liệu cần lấy.
4. Sau khi đủ net và mã linh kiện mới dựng KiCad/BOM phục dựng, đánh dấu rõ
   phần đã đo và phần chưa xác nhận. Tách kiểm tra firmware runtime khỏi việc
   xác nhận sơ đồ phần cứng.

Chi tiết giới hạn backup, cập nhật không atomic và quy trình SWD:
[audit khôi phục](recovery.md). Đã ghi firmware ứng dụng qua UF2; không đổi
bootloader, SoftDevice hoặc chạy SWD recover; chưa thử rollback.
