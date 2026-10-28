# ZMK Config for Curve E PCB Prototype 1
WIP
Designed by Deemen17

Hardware documentation: [Tài liệu kết nối phần cứng](docs/hardware/README.md)

## Firmware hiện hành

Board dùng module ZMK và hardware model mới: `curve_e//zmk`.
ZMK được khóa ở commit `4dd95d7840eb3d5a3f59bb29fbf1ab054715ef85`, là đầu
nhánh `main` đã tra cứu trong đợt nâng cấp này; dependency Zephyr là
`v4.1.0+zmk-fixes`. Không dùng số phiên bản tự đặt cho firmware.

- [Định nghĩa board](boards/curve_e/board.yml),
  [devicetree](boards/curve_e/curve_e_nrf52840_zmk.dts),
  [keymap](boards/curve_e/curve_e.keymap),
  [cấu hình](config/curve_e.conf), [manifest](config/west.yml).
- [Firmware đã build và bằng chứng](firmware/README.md).
- [Tính năng, cách build và kiểm chứng](docs/firmware.md).
- [Kết nối phần cứng và bằng chứng USB/ảnh](docs/hardware/README.md).

![image](https://user-images.githubusercontent.com/95753855/190593464-837a6e1b-c1ad-44f9-ad48-c4972ede8f1d.png)
![image](https://user-images.githubusercontent.com/95753855/190593507-abe741ab-4c7a-42a5-9975-7ca61e5cc02b.png)
