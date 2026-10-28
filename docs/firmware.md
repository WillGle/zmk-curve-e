# Curve E — firmware hiện hành

[Mục lục tài liệu](README.md) · [Nguồn tham khảo](references/README.md)

## Phiên bản và cấu trúc

ZMK: `4dd95d7840eb3d5a3f59bb29fbf1ab054715ef85`, đầu `main` được tra cứu
trong đợt nâng cấp ngày 28/10/2026. Khóa commit trong
[west.yml](../config/west.yml) để build có thể lặp lại. Zephyr được ZMK chọn
bằng nhánh `v4.1.0+zmk-fixes`; revision đã resolve khi build được ghi trong
[kết quả kiểm chứng](verification.md). Dependency nhánh có thể thay đổi khi cập nhật workspace.

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
