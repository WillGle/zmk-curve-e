# Các chế độ kết nối Curve E

> **Quyết định hiện hành — 28/10/2026:** dừng phát triển receiver USB dongle
> và ESB 2,4 GHz. Sản phẩm chỉ tiếp tục với **cáp USB và Bluetooth BLE**, BLE
> là wireless mặc định. Nội dung ESB bên dưới giữ làm lịch sử tham chiếu,
> không phải kế hoạch tiếp tục, hướng dẫn mua receiver hoặc firmware để nạp.

Đối chiếu ngày 28/10/2026. “3 mode” được hiểu là USB, Bluetooth BLE và
2,4 GHz qua receiver riêng; BLE cũng dùng băng tần 2,4 GHz nhưng khác giao thức ESB.

| Chế độ | Bằng chứng | Trạng thái Curve E hiện tại |
| --- | --- | --- |
| USB | ZMK chính thức hỗ trợ | Đã xác nhận gõ qua USB |
| BLE trực tiếp | ZMK chính thức hỗ trợ | Đã bật; chưa kiểm thử pairing/reconnect/độ ổn định thực tế |
| Dongle BLE | ZMK chính thức có hướng dẫn | Chưa cấu hình; cần receiver làm central, bàn phím thành peripheral |
| Receiver 2,4 GHz ESB | Fork ph-design có mã nguồn, vendor có shield DENGLE | Chưa có trong firmware đang nạp; chưa xác minh receiver tương thích |

## ZMK chính thức

FAQ hiện nói chưa có kế hoạch triển khai dongle 2,4 GHz dùng giao thức riêng của
Nordic, đồng thời dẫn tới giải pháp dongle BLE. Hướng dẫn dongle dùng kiến trúc
split: dongle là central, bàn phím là peripheral. Với cách thiết lập được tài
liệu mô tả, bàn phím cần dongle để sử dụng; đây không phải cấu hình sẵn có để
chuyển độc lập giữa ba đầu ra USB/BLE/ESB.

Tài liệu output chính thức có USB, BLE, chuyển USB/BLE và ngắt đầu ra;
không có đầu ra ESB. Nguồn dẫn được gom ở [tài liệu chính thức](references/official.md#chế-độ-kết-nối).

## Bằng chứng từ vendor

Tại snapshot vendor `d65386912abdf0ed52a81ce11fb7794d840c2cdf`, manifest
trỏ tới `ph-design/zmk`, nhánh `v0.3-lts`. Bản fork được khảo sát ở commit
`aba1104a9777d851df1c3ea032468d44c42ed3f0` có:

- `CONFIG_ZMK_2G4` cho bàn phím, `CONFIG_ZMK_2G4_DONGLE` cho receiver.
- Driver ESB cho nRF52 và đầu ra `ZMK_TRANSPORT_2G4` bên cạnh USB/BLE.
- Thiết lập địa chỉ/kênh và khóa AES tương ứng giữa hai đầu.

Snapshot vendor đã lưu trong repo cũng có shield DENGLE bật vai trò receiver,
USB và tắt BLE. Tuy nhiên, cấu hình shield Curve E tại nhánh khảo sát chỉ bật
USB/BLE, không bật `ZMK_2G4`; build matrix chỉ liệt kê DE60. Có mã nguồn chế độ
thứ ba là bằng chứng lời vendor có cơ sở, chưa phải bằng chứng Curve E đã được
build và chạy ổn định ở cả ba chế độ.

nRF52840 của board thuộc họ mà driver ESB hỗ trợ; chưa thấy căn cứ yêu cầu sửa
PCB để thử hướng này. Vẫn cần xác minh receiver, chuyển quyền dùng radio giữa
BLE/ESB, tính tương thích với ZMK mới và kiểm thử thực tế trước khi kết luận.
Các URL/commit vendor được gom tại [nguồn công khai](references/public-sources.md#nguồn-đối-chiếu-3-mode).

## Firmware đang dùng và bước còn thiếu

Bản `2026-10-28-zmk-4dd95d7-ble-compat-curved` dùng upstream ZMK, có
`CONFIG_ZMK_USB=y`, `CONFIG_ZMK_BLE=y`, không bật split và không có triển khai
ESB của fork. Không thể bật chế độ thứ ba chỉ bằng map một phím trong Studio.

Để triển khai tiếp cần nhận diện receiver (DENGLE hoặc phần cứng có firmware
phù hợp), chọn cách tích hợp ESB với upstream mới, build riêng cả hai đầu,
rồi kiểm thử chuyển mode, mất kết nối, reconnect, sleep/wake và USB/BLE hồi quy.
Sau audit đã port/build prototype USB+ESB riêng, chưa nạp hay thử radio; chưa ghép BLE vào prototype. Không coi dongle
Bluetooth hoặc receiver 2,4 GHz bất kỳ là tương thích.

Kế hoạch tự phát triển và audit nguồn ESB: [phát triển độc lập](esb-development.md).
