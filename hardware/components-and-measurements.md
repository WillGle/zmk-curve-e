# Curve E Prototype 1 — nhận diện linh kiện và kế hoạch đo

Ngày kiểm tra: 28/10/2026. Tài liệu phục vụ bảo trì độc lập với người bán;
đây chưa phải BOM đã xác nhận hoặc sơ đồ đủ điều kiện chế tạo.

## Bằng chứng và giới hạn

- [Ảnh mặt linh kiện](evidence/pcb-back-user.png): ảnh gốc 2048 × 1536 px, PCB ghi `Curve E PCB prototype 1`, `Designed by Deemen17`. Các vùng ảnh dưới đây dùng tọa độ pixel gốc, gốc tọa độ ở góc trên trái.
- [Ảnh mặt CURVE E](evidence/pcb-front-user.png): nhiều PCB trống xếp chồng; không coi đây là mặt còn lại của đúng bo đã lắp trong ảnh trên.
- [Devicetree hiện hành](../boards/curve_e/curve_e_nrf52840_zmk.dts), [defconfig](../boards/curve_e/curve_e_nrf52840_zmk_defconfig), [keymap](../boards/curve_e/curve_e.keymap): bằng chứng về cấu hình phần mềm, không xác nhận linh kiện đã lắp.
- [Bản ghi USB](evidence/usb-2026-10-28.json): bằng chứng thiết bị kết nối ở thời điểm khảo sát, không nhận diện được IC sạc/module từ descriptor. Chưa xác minh bo trong ảnh là bo USB hiện tại.

Không có phép đo điện hoặc đọc flash nào được thực hiện cho tài liệu này.
Ảnh được phóng to để quan sát; việc phóng to không tạo thêm chi tiết chữ.
Các mức tin cậy dưới đây áp dụng riêng cho bo trong ảnh.

## Linh kiện nhìn thấy

| Khu vực / bằng chứng | Nhận diện | Mức xác nhận và phần còn thiếu |
| --- | --- | --- |
| Module, vùng x=1035–1160, y=1060–1215 | Module RF EBYTE, họ E73, anten gốm ở đầu sát mép PCB | Cao cho EBYTE/E73 và anten; dòng model phù hợp `E73-2G4M08S1C`, nhưng chữ nhỏ chưa đủ rõ để xác nhận tuyệt đối. Chưa dùng model này để đặt mua thay thế. |
| USB, đỉnh giữa bo | Connector USB-C; R1/R2 ở ngay phía dưới | Cao cho loại connector; chưa biết hãng, mã, footprint chính xác, giá trị R1/R2 hoặc đường CC. |
| Dưới USB | Silkscreen `BREAK TO USE DAUGHTERBOARD`, J6 và hàng pad J5 `V DM DP G` | Cao cho nhãn; cần đo để xác nhận V=VBUS, DM=D−, DP=D+, G=GND. Không bẻ phần USB để khảo sát. |
| Khối giữa, x≈1040–1175, y≈810–1075 | J2 có dấu −/+ và chữ BATTERY; J3 là footprint chưa lắp; U2/U3/U4, Q1/Q2, SD1, R3–R10, X1 hiện diện | Đọc được reference, chưa đọc được mã IC. Không gán TP4056/MCP73831, loại ổn áp, MOSFET hay diode cụ thể. Chưa xác định X1 là crystal nào hoặc tần số bao nhiêu. |
| Bên trái module | Bốn pad `3.3V`, `GND`, `SWD`, `SWC` | Cao cho nhãn; SWD/SWC phù hợp cách EBYTE gọi SWDIO/SWCLK. Phải đo trước khi nối probe. `3.3V` là nhãn, chưa phải điện áp đã đo hoặc quyền cấp nguồn ngoài. |
| Góc dưới phải | Nút RESET, hàng pad `PW SW` | Cao cho nhãn/chức năng dự kiến; chưa biết chân module thực nối reset hoặc cách công tắc ngắt nguồn. |
| Vùng phím | Socket dạng MX hotswap, diode SMD, nhãn MX và RnCm | Cao cho cấu trúc; chưa xác nhận hãng socket, mã diode, hướng diode từng vị trí hay số switch cuối cùng. |
| Các footprint WS và tụ cạnh chúng | Footprint linh kiện bốn pad chưa lắp | Chưa có mã part hoặc chiều pin xác nhận. Không đưa LED RGB cụ thể vào BOM chỉ từ chữ WS. |
| Góc trên trái ảnh mặt linh kiện / các lỗ tương ứng ở mặt chữ | Footprint phím có thêm nhóm lỗ nhỏ; firmware khai báo encoder EC11 | Có khả năng lựa chọn switch/encoder tại khu vực này. Chưa xác nhận vị trí encoder, pin A/B/common hoặc phím nhấn; binding `alps,ec11` không xác nhận hãng/mã encoder lắp thực tế. |

## Tài liệu gốc của nhà sản xuất cho module ứng viên

Nguồn và checksum PDF được gom tại [tài liệu EBYTE](../docs/references/official.md#ebyte-e73).

Đây là nguồn pinout của **module ứng viên**, không phải schematic của Curve E.
Mọi số pad sau chỉ dùng khi đã xác nhận đúng model và hướng pad 1 theo hình
trong manual. Không đếm pad từ ảnh chụp nghiêng.

| Chức năng cần truy vết | GPIO / tên Nordic | Pad module ứng viên theo manual |
| --- | --- | --- |
| C0, C1, C2, C3, C4 | P0.28/AIN4, P0.02/AIN0, P0.29/AIN5, P0.31/AIN7, P0.30/AIN6 | 4, 7, 8, 9, 10 |
| C5, C6, C7, C8 | P0.26, P0.17, P0.20, P0.22 | 12, 30, 32, 34 |
| C9, C10, C11, C12 | P1.00, P1.02, P1.04, P1.06 | 36, 38, 40, 42 |
| R0, R1, R2, R3 | P0.06, P0.08, P0.12, P0.07 | 14, 16, 20, 22 |
| Encoder A, B | P0.24, P0.13 | 35, 33 |
| LED khai báo trong repo | P1.10 | 2 |
| Cảm biến pin dự kiến | AIN2 / P0.04 | 18 |
| SWDIO, SWCLK | SWD, SWC | 37, 39 |
| Reset | P0.18/RESET | 26 |
| USB D−, D+, VBUS | D−, D+, VBS | 29, 31, 27 |
| Nguồn thường, nguồn cao áp, GND | VDD, VDH/VDDH, GND | 19, 23, các pad 5/21/24 |
| Clock 32.768 kHz nếu dùng | XL1, XL2 | 11, 13 |

Không đồng nhất VDD, VDDH và VBUS. Dải nguồn quảng cáo của module không phải
dải cho mọi chân nguồn/GPIO. Khi chưa biết mạch, không cấp 5 V vào pad 3.3V
hoặc chân GPIO, và không chập các rail nguồn để thử.

## Khai báo firmware công khai cần đối chiếu thêm

[Overlay của tác giả tại commit 66f10de4dd1c5728e36670091cb84a9481234cb7](../docs/references/public-sources.md#firmware-vendor-có-ích-nhưng-chưa-phải-schematic)
(commit ngày 05/10/2026, kiểm tra ngày 28/10/2026) khai báo thêm các đường sau.
Đây là nguồn so sánh, chưa chứng minh dây trên bo trong ảnh hoặc bo hiện tại.

| Khai báo nguồn công khai | Pad module ứng viên | Phép đo cần bổ sung |
| --- | --- | --- |
| P0.03: LED xanh; P1.10: LED trắng | 3; 2 | Truy vết cả hai chân, xác nhận màu LED. Repo hiện tại gọi P1.10 là xanh: hai nguồn chưa thống nhất. |
| P1.09: external power, active-high | 17 | Truy vết chân điều khiển tới transistor/khối tải; không coi đây là chân cấp nguồn trực tiếp. |
| P0.15: SPI3 MOSI phục vụ WS2812 | 28 | Truy vết tới các footprint WS, xác định part/chiều dữ liệu/nguồn nếu thực sự có. Giá trị chain-length=10 trong nguồn được chú thích là tùy ý, không phải số LED đã xác nhận. |

Không đổi GPIO hiện hành hoặc gán linh kiện BOM theo các khai báo khác biệt
này trước khi kiểm tra phần cứng. Tra cứu nguồn chi tiết ở
[Khảo sát nguồn vendor](../docs/references/public-sources.md).

## Thu thập còn thiếu để chốt sơ đồ

Thứ tự sau giúp ghi được kết quả có thể kiểm tra lại, không cần nạp firmware.
Mỗi số đo cần ghi bo/revision, hướng ảnh, trạng thái USB/pin/công tắc, hai đầu
que đo, chế độ/thang đo và kết quả. Các giá trị bên dưới là mục tiêu đối chiếu,
không phải số đo sẵn có.

| Bước | Ảnh hoặc phép đo cụ thể | Điều kiện hoàn thành |
| --- | --- | --- |
| 1. Xác nhận bo | Chụp bo USB hiện tại cả hai mặt sau khi mở vỏ; ảnh toàn bo và revision; ảnh vuông góc cận module, connector, cụm nguồn, SWD, RESET/PW SW; đặt thước đo cạnh module | Xác nhận có đúng Prototype 1 và đúng assembly không. Ảnh module đọc được toàn bộ model, logo và pad 1; ảnh nguồn đọc được topmark và dấu định hướng U2/U3/U4/Q1/Q2/SD1/X1. |
| 2. Chốt GND và nguồn | Rút USB, ngắt pin; đo continuity giữa pad GND debug, GND module ứng viên, chân âm J2 và G của J5. Đo riêng USB shield; không mặc định shield nối trực tiếp GND. Truy vết VBUS, BAT+, VDD và VDDH qua từng linh kiện nguồn và các trạng thái PW SW | Có netlist nguồn riêng cho từng rail; nhận diện IC bằng cả topmark, package và chân nối, rồi mới chọn datasheet. Đo thông mạch không đủ để xác định trị số/loại IC. |
| 3. USB | Khi mất nguồn, xác định chân connector theo ảnh đúng hướng; truy vết J5 DM/DP đến D−/D+ module, V đến VBUS, và CC1/CC2 qua R1/R2 nếu thực sự nối như vậy; đo hai nhánh CC tới GND và ghi mọi linh kiện nối tiếp/bảo vệ | Xác nhận cả hai nhánh CC, đường dữ liệu, nguồn và bảo vệ. Không coi việc cáp A–C chạy được là chứng minh C–C đúng. Nếu điện trở đo trong mạch bị đường song song ảnh hưởng, để giá trị chưa xác định. |
| 4. Khối pin/sạc | Truy vết J2/J3, các chân U2/U3/U4, SD1, Q1/Q2, PW SW, điểm lấy điện áp ADC. Đọc mã điện trở/tụ nếu có; đối chiếu footprint và chân IC với datasheet vừa nhận diện | Phân biệt sạc, bảo vệ pin, chọn nguồn, ổn áp và điều khiển tải bằng net thực. Không suy ra dung lượng pin/dòng sạc từ kích thước linh kiện. Không tháo linh kiện chỉ để đo trong lượt đầu. |
| 5. Chia áp | Đo đường AIN2 tới nút chia áp và hai đầu rail/GND. Firmware dự kiến R_trên=820 kΩ, R_dưới=2 MΩ; đo trong mạch có thể sai do đường song song hoặc ảnh hưởng đầu vào | Xác nhận giá trị, rail lấy mẫu và tụ lọc; nếu không đọc/đo chắc chắn, giữ trường BOM chưa biết. |
| 6. Debug/reset | Truy vết SWD→SWDIO, SWC→SWCLK, RESET→chân reset; kiểm tra hai trạng thái nút và công tắc, kiểm tra có điện trở kéo/tụ hay linh kiện nối tiếp | Hoàn tất bảng pad debug với hướng nhìn và rail tham chiếu. Chưa nối SWD probe hoặc ghi/xóa chip trong giai đoạn này. |
| 7. Ma trận | Chụp đọc rõ toàn bộ RnCm; đo GPIO–bus cột/hàng và hai đầu từng switch/diode, dùng diode mode hai chiều; kiểm tra đủ 47 tọa độ firmware và cả pad ngoài transform | Xác nhận `col2row`, đầu cathode, thứ tự switch/diode, các footprint dùng chung và vị trí bỏ trống. Lưu bảng MX/reference diode ↔ RnCm ↔ GPIO; không đảo cột theo hướng nhìn ảnh. |
| 8. Encoder/LED/WS | Truy vết P0.24/P0.13 tới footprint encoder, tìm common và pin nhấn; quay từng nấc khi không cấp nguồn để đo đóng/mở. Truy vết P1.10 qua điện trở/transistor nếu có; truy vết footprint WS riêng | Chốt chân, chiều quay, nấc/xung, nút nhấn, màu/chức năng LED và các tùy chọn thực sự có dây nối. Không dùng metadata firmware để suy ra linh kiện đã lắp. |
| 9. Điện áp | Sau khi đã chốt cực pin và rail, dùng GND đã xác định; đo lần lượt USB-only, battery-only rồi trạng thái kết hợp nếu mạch/loại pin đã rõ. Ghi VBUS, BAT+, VDD, VDDH, debug 3.3V, nút ADC và trạng thái PW SW | Phân biệt điện áp danh định và điện áp thực; tỷ lệ ADC dự kiến 2/2.82. Đồng hồ thường 10 MΩ sẽ tải đáng kể cầu chia cỡ MΩ, vì vậy ghi trở vào đồng hồ và xét tải trước khi kết luận sai điện trở. Không đo dòng bằng cách đặt ampe kế ngang nguồn. |

Khi đo thông mạch/điện trở/diode phải ngắt cả USB và pin, chờ tụ xả và kiểm tra
không còn điện áp. Khi đo điện áp dùng đầu que cách điện, tránh trượt sang pad
kế bên. Không cần thử sạc pin chưa rõ loại hoặc tháo nắp RF để hoàn tất lượt
nhận diện đầu tiên.

Kết quả bàn giao đủ cho sơ đồ gồm: ảnh của đúng bo hiện tại; mã/footprint từng
linh kiện; bảng net–pad có phép đo đối chứng; giá trị và dung sai đã xác nhận;
phần chưa biết đánh dấu riêng. Chỉ sau đó mới chốt symbol, BOM thay thế và file
KiCad. Các GPIO/giá trị từ firmware vẫn giữ nhãn “khai báo” cho đến khi đo.
