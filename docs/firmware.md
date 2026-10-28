# Curve E — firmware hiện hành

Phạm vi hiện hành chỉ gồm **cáp USB và BLE**. Hướng USB dongle/ESB 2,4 GHz
đã dừng theo quyết định người dùng; các mục ESB phía dưới chỉ là lịch sử,
không phải việc cần tiếp tục. Không thay đổi firmware đang cài.

[Mục lục tài liệu](README.md) · [Nguồn tham khảo](references/README.md)

## Phiên bản và cấu trúc

Danh mục hiện hành ở [product.json](../product.json). ZMK, Zephyr và toàn bộ
56 dependency được khóa full commit SHA tại [manifest](../config/west.yml),
không còn nested import hoặc nhánh động. Mỗi archive giữ frozen manifest riêng;
manifest lịch sử không bị viết lại khi đổi cấu trúc sản phẩm.

[Module](../zephyr/module.yml) cung cấp board tại `boards/curve_e`.
Board target mới: **`curve_e//zmk`**, SoC nRF52840 QIAA. Không dùng target
legacy `curve_e` hoặc đường dẫn `config/boards/arm/curve_e` nữa.
Giữ nguyên GPIO, chiều diode và storage/boot partition; giới hạn code partition
tại `0xEA000` theo bootloader nền (giảm 8 KiB so với cấu hình cũ). Bổ sung
boot-mode retention Adafruit nRF52. Đã thử vào/thoát bootloader bằng RESET/USB
trên firmware cũ; chưa thử behavior bootloader của firmware mới.

## Tính năng và thao tác

Xem [keymap, layer, encoder, Studio và Bluetooth](../guides/configure-keymap.md).

## Phạm vi khả năng PCB

Các tính năng trên dùng ma trận, encoder và đo pin đã có trong firmware.
Module RF/USB-C và các pad được ghi trong [tài liệu phần cứng](hardware.md).
Không bật RGB, màn hình, cảm biến chuột hay giao thức IEEE 802.15.4 khi chưa
xác nhận linh kiện và đường nối. LED P1.10 mới chỉ có khai báo, chưa gán chức
năng báo trạng thái vì chưa đo kiểu đấu.

+8 dBm, debounce 1/5 ms và interval yêu cầu không bảo đảm không trễ/không mất
phím. Chưa đo độ chính xác báo pin, dòng ngủ, thời lượng pin, tầm sóng hoặc
thời gian thức dậy. Không có kết luận “tắt RGB/SPI hết rò pin” từ cấu hình này.

## Build và kiểm chứng

- [Tự build firmware](../guides/build-firmware.md).
- [Kết quả build và kiểm tra trước khi nạp](verification.md).
- [Chọn bản firmware để sử dụng](../firmware/README.md).

## Nguồn tham khảo

Xem [tài liệu ZMK chính thức](references/official.md#zmk) và
[audit khôi phục](recovery.md).
