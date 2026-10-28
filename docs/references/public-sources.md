# Curve E — nguồn công khai và giới hạn tự chủ

Khảo sát ngày 28/10/2026. **Chưa tìm thấy schematic, PCB KiCad, Gerber, BOM hoặc
file đặt lắp ráp của Curve E Prototype 1 trong phạm vi dưới đây.** Có mã nguồn
firmware công khai cho đúng tên Curve E, nhưng chưa đủ để chế tạo lại bo hoặc
khẳng định tái tạo đúng firmware đã cài. Không liên hệ tác giả, không nạp thiết bị,
không chạy build trong khảo sát này.

## Nơi đã kiểm tra

SHA trong bảng là commit đầu nhánh tại lúc đọc. API recursive tree đều trả
`truncated: false`; lịch sử ghi rõ bên dưới đã được đọc bằng clone Git đầy đủ,
không chỉ tìm tên file ở HEAD.

| Nguồn | Commit/phạm vi | Kết quả và license |
| --- | --- | --- |
| [Deemen17/Curve](https://api.github.com/repos/Deemen17/Curve), URL trên PCB | API trả 404 | Không truy cập được repo công khai tại URL này; không kết luận bị xóa hay private |
| [Deemen17/CruveE](https://github.com/Deemen17/CruveE), chú ý lỗi chính tả | [metadata](https://api.github.com/repos/Deemen17/CruveE), [branches](https://api.github.com/repos/Deemen17/CruveE/branches), [commits](https://api.github.com/repos/Deemen17/CruveE/commits) | Repo tồn tại nhưng size=0, không có branch/release; cây và commits trả 409 `Git Repository is empty`. Không có source/license để lấy |
| [Deemen17_PCB_Collection](https://github.com/Deemen17/Deemen17_PCB_Collection/tree/ef6a5e9b689455a38b76ec552e7a13c28d05073b) | `ef6a5e9b689455a38b76ec552e7a13c28d05073b`; toàn bộ 8 commits, 8 đường dẫn lịch sử | Chỉ README, Decan và dee1800FL firmware/VIA; không Curve hoặc CAD/Gerber/BOM. Không LICENSE ở cây đã đọc |
| [keyboards](https://github.com/Deemen17/keyboards/tree/ce31db7ec6b27c5923c153b5a386527d12d09d3a) | `ce31db7ec6b27c5923c153b5a386527d12d09d3a`; 6.681 entries tại HEAD | Fork định nghĩa VIA, không kho PCB. Không đường dẫn curve/cruve hay schematic/PCB trong cây. GPL-3.0; không khảo sát toàn bộ lịch sử fork này |
| [zmk-config-curve_e](https://github.com/Deemen17/zmk-config-curve_e/tree/d893cdfc7c6dec67cb0cbdb0e3a94bd61472b980) | `d893cdfc7c6dec67cb0cbdb0e3a94bd61472b980`; toàn bộ 24 commits, 17 đường dẫn lịch sử | README ghi `Curve E PCB Prototype 1`, WIP, Designed by Deemen17; chỉ firmware. Không LICENSE cấp repo; file có thể có notice riêng |
| [zmk-deemen17-keyboards](https://github.com/Deemen17/zmk-deemen17-keyboards/tree/c80b093dc650f5674a743818048b6092dab4f213) | `c80b093dc650f5674a743818048b6092dab4f213`; toàn bộ 142 commits, 125 đường dẫn lịch sử | Board firmware khác; không đường dẫn Curve hoặc CAD/Gerber/BOM trong lịch sử đã đọc. Không LICENSE cấp repo |
| [zmk-keyboards-dekb](https://github.com/Deemen17/zmk-keyboards-dekb) | 4 nhánh công khai, toàn bộ 16 commits, 311 đường dẫn lịch sử | Có Curve ở nhánh dev/lịch sử, không ở main hiện tại; không CAD/Gerber/BOM. LICENSE MIT và SPDX MIT trong các file Curve đã đọc |
| [deemen17_Library](https://github.com/Deemen17/deemen17_Library/tree/6987369a6cf93b043e7f6f9a0715fac06c0e0e0a) | `6987369a6cf93b043e7f6f9a0715fac06c0e0e0a`; cây HEAD | Symbol/footprint KiCad chung, gồm E73-2G4M08S1C-52840, SWD, USB và switch. Không thiết kế Curve, không LICENSE cấp repo |
| [Kimiro-nRF52840](https://github.com/Deemen17/Kimiro-nRF52840/tree/b341151dfd24fccba4f258e1bd628e3aa54d3c7a) | `b341151dfd24fccba4f258e1bd628e3aa54d3c7a`; cây HEAD | Có `Kimiro.kicad_sch`, `.kicad_pcb`, `.kicad_pro`, thư viện. LICENSE MIT, copyright 2022 kien242. Đây là thiết kế khác; không bằng chứng dùng thay cho Curve |

Danh sách repo lấy từ [GitHub users/Deemen17/repos?per_page=100](https://api.github.com/users/Deemen17/repos?per_page=100)
trả 38 repo công khai. Các tìm kiếm tên repo `CurveE` (30 kết quả đầu trên tổng
82, phần lớn curve editor) và `curve_e keyboard` (0 kết quả) không tìm thêm nguồn
hardware phù hợp. Không khảo sát Internet toàn diện, fork/private/deleted repo,
GitHub Actions artifact hết hạn hoặc toàn bộ nội dung mọi repo của tác giả.
Các repo CruveE, PCB Collection, zmk-config-curve_e, zmk-deemen17-keyboards và
zmk-keyboards-dekb không có GitHub Release công khai tại lúc đọc; điều này không
loại trừ firmware được phát qua kênh khác.

## Firmware vendor có ích nhưng chưa phải schematic

`zmk-keyboards-dekb` có bốn nhánh:

| Nhánh | Commit |
| --- | --- |
| main | `8742d3164fba18e7299dc6e4c9e4070e3c1548b1` |
| codex/set-up-zmk-repo-for-dekb | `ce7eaaf3d518180cf75841034ac2b7c478c5e8f0` |
| dev | `66f10de4dd1c5728e36670091cb84a9481234cb7` |
| ph-design/2g4-impl | `d65386912abdf0ed52a81ce11fb7794d840c2cdf` |

[Overlay Curve ở dev](https://github.com/Deemen17/zmk-keyboards-dekb/blob/66f10de4dd1c5728e36670091cb84a9481234cb7/boards/shields/curve_e/curve_e.overlay)
trùng GPIO ma trận, encoder và cầu chia áp với firmware cũ. Nó còn khai báo:

| Khai báo firmware vendor mới | Giới hạn |
| --- | --- |
| LED blue P0.03, white P1.10 | Khác nhãn blue P1.10 trong config cũ; cần đo bo để chọn |
| Ext-power P1.09 | Chưa xác định transistor, cực tính và khối tải thực |
| WS2812 SPI MOSI P0.15, chain-length 10 | Tác giả ghi `arbitrary; change at will`; không xác nhận 10 LED trên PCB |
| Hai physical layouts LEFT SPACE 1U/2U | Có thêm dữ liệu hình học phím; không thay file mép PCB/plate/case |

[Config USB](https://github.com/Deemen17/zmk-keyboards-dekb/blob/66f10de4dd1c5728e36670091cb84a9481234cb7/boards/shields/curve_e/boards/de_core_nrf52.conf)
khai báo VID/PID DE17:40E1, DEKB, CURVE E BLE, phù hợp descriptor đã quan sát.
Tên/ID này không nhận diện duy nhất revision PCB hay hash firmware.
[build.yaml](https://github.com/Deemen17/zmk-keyboards-dekb/blob/66f10de4dd1c5728e36670091cb84a9481234cb7/build.yaml)
hiện chỉ liệt kê hai DE60, nên chưa có bằng chứng Curve dev được build thành
công trong CI. Không chạy thử build dev; không khuyến nghị nạp nguyên config này
trước khi đối chiếu phần nguồn/RGB.

Marker `#DEKB BLE ZMK Firmware 20260426` có ở
[boards/arm/common.keymap:58](https://github.com/Deemen17/zmk-keyboards-dekb/blob/235fcab4b58eb3d2238fc81f9f841fd848e5740c/boards/arm/common.keymap#L58)
commit `235fcab4b58eb3d2238fc81f9f841fd848e5740c`. Commit này không có file Curve;
marker là behavior dùng chung, không chứng minh byte-identical với firmware
trong dump. Tra [Deemen17/zmk commit dacab4875df7](https://api.github.com/repos/Deemen17/zmk/commits/dacab4875df7)
trả `No commit found for SHA`; chưa giải được hash build theo nguồn công khai này.

## Archive và phụ thuộc để tiếp tục độc lập

Hai archive công khai đã lưu tại `hardware/upstream/` sau khi người dùng cho phép.
Source đã giải nén nằm trong hai thư mục `zmk-keyboards-dekb-<commit>` cùng
LICENSE MIT nguyên bản. [Manifest nguồn và checksum](../../hardware/upstream/sources.json)
ghi URL, commit, SHA256, kích thước và số file đã đối chiếu từng byte với archive.
Thư mục `deemen17_Library-6987369a6cf93b043e7f6f9a0715fac06c0e0e0a` chỉ giữ
footprint E73 và file symbol nguyên bản, không phải toàn bộ thư viện. Không có
LICENSE cấp repo cho thư viện này; lưu để tra cứu không chứng minh quyền tái sử dụng.

| Snapshot | Archive, kích thước và SHA256 |
| --- | --- |
| Marker tương ứng tên phiên bản | [235fcab4b58eb3d2238fc81f9f841fd848e5740c.tar.gz](https://codeload.github.com/Deemen17/zmk-keyboards-dekb/tar.gz/235fcab4b58eb3d2238fc81f9f841fd848e5740c), 118.663 byte; `f65b0287371e63cd8e283c86fdc889618e8dd06cd52a009fb8d93c8b8ff02755` |
| dev có source Curve | [66f10de4dd1c5728e36670091cb84a9481234cb7.tar.gz](https://codeload.github.com/Deemen17/zmk-keyboards-dekb/tar.gz/66f10de4dd1c5728e36670091cb84a9481234cb7), 97.689 byte; `b8ab54fafc2ad283efa9e3e1dc28e88aea7bf02dd32550370a0f1d70facbdac5` |

[west.yml của snapshot marker](https://github.com/Deemen17/zmk-keyboards-dekb/blob/235fcab4b58eb3d2238fc81f9f841fd848e5740c/west.yml)
trỏ `zmkfirmware/zmk v0.3.0`, tag hiện resolve tới
`edf5c0814fd3ea202e43aad2d68fd32e882a518c`.
[config/west.yml của dev](https://github.com/Deemen17/zmk-keyboards-dekb/blob/66f10de4dd1c5728e36670091cb84a9481234cb7/config/west.yml)
trỏ `ph-design/zmk v0.3-lts`, nhánh hiện resolve tới
`aba1104a9777d851df1c3ea032468d44c42ed3f0`.
CI workflow dùng `zmkfirmware/zmk/.github/workflows/build-user-config.yml@v0.3`.
Nhánh/tag không phải lockfile bất biến; cần pin mọi dependency khi dựng môi trường.

Hai manifest nhập tiếp có thể đọc tại
[ZMK v0.3.0 app/west.yml](https://github.com/zmkfirmware/zmk/blob/edf5c0814fd3ea202e43aad2d68fd32e882a518c/app/west.yml)
và [ph-design app/west.yml](https://github.com/ph-design/zmk/blob/aba1104a9777d851df1c3ea032468d44c42ed3f0/app/west.yml).
Cả hai lấy `zmkfirmware/zephyr v3.5.0+zmk-fixes` và nanopb
`8c60555d6277a0360c876bd85d491fc4fb0cd74a`. Studio messages lần lượt là
`zmkfirmware/zmk-studio-messages 6cb4c283e76209d59c45fbcb218800cd19e9339d`
và `ph-design/zmk-studio-messages v0.3_lts`. Zephyr còn nhập các module qua
manifest riêng; chưa tải toàn bộ dependency hoặc xác nhận build tái lập.

## Điều có thể làm tiếp

Có thể dùng các source firmware và giấy phép tương ứng làm tài liệu GPIO,
layout và nguồn để tự duy trì firmware. Để tự sản xuất/sửa phần cứng vẫn cần
đối chiếu module, mạch nguồn/sạc, USB, net và BOM từ bo thật; sau đó mới dựng
schematic/PCB của bản phục dựng. Thư viện chung và Kimiro chỉ là tài liệu tham
khảo; không gán linh kiện của chúng sang Curve và không coi chúng là giấy phép
cho thiết kế Curve chưa tìm thấy. Public source không kèm license cũng chưa
cấp quyền tái phân phối tùy ý. Chưa có căn cứ tuyên bố đã loại bỏ hoàn toàn phụ
thuộc vendor.

## Nguồn đối chiếu 3 mode

Đọc ngày 28/10/2026, cố định revision để tránh suy ra từ nhánh thay đổi:

- [Manifest vendor](https://github.com/Deemen17/zmk-keyboards-dekb/blob/d65386912abdf0ed52a81ce11fb7794d840c2cdf/config/west.yml).
- [Cấu hình Curve E](https://github.com/Deemen17/zmk-keyboards-dekb/blob/d65386912abdf0ed52a81ce11fb7794d840c2cdf/boards/shields/curve_e/boards/de_core_nrf52.conf).
- [Build matrix vendor](https://github.com/Deemen17/zmk-keyboards-dekb/blob/d65386912abdf0ed52a81ce11fb7794d840c2cdf/build.yaml).
- [Kconfig ESB/2G4 của fork](https://github.com/ph-design/zmk/blob/aba1104a9777d851df1c3ea032468d44c42ed3f0/app/Kconfig).
- [Đầu ra USB/BLE/2G4 của fork](https://github.com/ph-design/zmk/blob/aba1104a9777d851df1c3ea032468d44c42ed3f0/app/include/zmk/endpoints_types.h).
- [Driver ESB Kconfig](https://github.com/ph-design/zmk/blob/aba1104a9777d851df1c3ea032468d44c42ed3f0/app/module/drivers/esb/Kconfig).

Shield DENGLE còn có trong snapshot `66f10de4dd1c5728e36670091cb84a9481234cb7`
đã lưu tại `hardware/upstream/`; xem manifest nguồn hiện có. Có nguồn ESB không
xác nhận Curve E đã build/chạy ba mode. Xem [kết luận](../connection-modes.md).

Các file fork đã audit được lưu nguyên bản cùng LICENSE và hash từng file tại
[manifest ESB](../../hardware/upstream/ph-design-zmk-aba1104-esb/source.json).
