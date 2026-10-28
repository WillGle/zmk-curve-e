# Curve E — firmware hiện hành

## Phiên bản và cấu trúc

ZMK: `4dd95d7840eb3d5a3f59bb29fbf1ab054715ef85`, đầu `main` được tra cứu
trong đợt nâng cấp ngày 28/10/2026. Khóa commit trong
[west.yml](../config/west.yml) để build có thể lặp lại. Zephyr được ZMK chọn
bằng nhánh `v4.1.0+zmk-fixes`; revision đã resolve khi build sẽ ghi ở phần
kiểm chứng. Dependency nhánh có thể thay đổi khi cập nhật workspace.

[Module](../zephyr/module.yml) cung cấp board tại `boards/curve_e`.
Board target mới: **`curve_e//zmk`**, SoC nRF52840 QIAA. Không dùng target
legacy `curve_e` hoặc đường dẫn `config/boards/arm/curve_e` nữa.
Giữ nguyên GPIO, chiều diode và phân vùng flash từ định nghĩa cũ; bổ sung
boot-mode retention cho cơ chế bootloader Adafruit nRF52 mà board đã khai báo.
Chưa thử vào bootloader trên thiết bị thật.

## Tính năng và thao tác

| Chức năng | Cấu hình hiện hành |
| --- | --- |
| Phím | 47 vị trí, 4 layer; thứ tự trong [keymap](../boards/curve_e/curve_e.keymap) |
| Layer 0 | Gõ chữ; encoder âm lượng |
| Layer 1 | Giữ `mo 1`: số, mũi tên/Home/End/Page; encoder Page Down/Up |
| Layer 2 | Giữ `mo 2`: ký hiệu, di chuột và click trái/giữa/phải; encoder cuộn dọc |
| Layer 3 | Giữ đồng thời `mo 1` và `mo 2`: BT_CLR, BT_SEL 0–4, chọn output, Caps Word, reset/bootloader; encoder độ sáng |
| Tap dance | ESC → tap ESC/double GRAVE; Backspace → tap BSPC/double DEL; cửa sổ 200 ms |
| Combo layer 0 | Q+W → ESC; A+S → TAB; O+P → BSPC; K+L → ENTER; V+B → Caps Word; Z+X → đổi USB/BLE |
| USB | HID keyboard/mouse, Boot Protocol và Studio RPC qua snippet `studio-rpc-usb-uart` |
| Rollover | NKRO ở report mode; USB Boot Protocol dùng báo cáo 6 phím theo yêu cầu host |
| Studio | 47 vị trí trên lưới logic, đúng thứ tự transform; hình hiển thị chưa mô phỏng đường cong PCB |
| Bluetooth | 5 profile; công suất yêu cầu +8 dBm; interval yêu cầu 7.5–15 ms, peripheral latency 0 |
| Nguồn | Idle 30 giây; yêu cầu sleep sau 30 phút; bật báo pin BLE |

Combo chỉ áp dụng khi layer cao nhất đang hoạt động là layer 0. Cửa sổ combo
50 ms và tap-dance 200 ms có thể làm chậm các phím liên quan. Double-tap
Backspace sẽ thành Delete, không phải hai lần xóa lùi. GRAVE tạo ký tự theo
layout của hệ điều hành; dấu `~` còn phụ thuộc Shift/layout.

Encoder giữ tỷ lệ cũ một hành động trên bốn bước quadrature bằng `steps = 4`,
`triggers-per-rotation = 1`. Đây là tỷ lệ chuẩn hóa, chưa phải số xung/vòng
đã đo. Cuộn dùng tốc độ 100 đơn vị/giây và tap 20 ms để vượt chu kỳ input
16 ms của `msc`; chiều quay và cảm giác cuộn cần thử thực tế.

## Studio và Bluetooth

Sau khi build/nạp và USB hoạt động, mở [ZMK Studio](https://zmk.studio/) bằng
trình duyệt hỗ trợ Web Serial rồi chọn cổng Studio. Board vẫn giữ CDC console
cũ và thêm CDC riêng cho RPC; số `ttyACM` có thể thay đổi, không mặc định luôn
là `/dev/ttyACM0`. Studio locking hiện tắt theo cấu hình đã có.

Studio lưu keymap vào settings; settings có thể ghi đè keymap vừa biên dịch.
Sao lưu thay đổi trong Studio trước khi xóa settings để kiểm tra keymap mới.
Studio không thay thế việc build khi cần đổi GPIO, cấu hình radio hoặc thêm
định nghĩa behavior. Không mặc định mọi tính năng ZMK đều chỉnh được trên web.

Nếu cập nhật HID descriptor để có chức năng chuột, cần xóa pairing ở cả host
và profile bàn phím rồi ghép đôi lại để host đọc descriptor mới. BT_CLR tác
động profile đang chọn. Cấu hình BLE experimental hiện được giữ bật; cần thử
tương thích từng host, không coi đây là bảo đảm kết nối tốt hơn.

NKRO không bật extended report để tránh giảm tương thích không cần thiết.
USB Boot Protocol là chế độ dự phòng do host yêu cầu; chưa kiểm thử BIOS/UEFI
hoặc Android. Nếu host không nhận descriptor mới, cần ghép đôi lại hoặc kiểm
tra với cấu hình rollover tương thích của host.

## Phạm vi khả năng PCB

Các tính năng trên dùng ma trận, encoder và đo pin đã có trong firmware.
Module RF/USB-C và các pad được ghi trong [tài liệu phần cứng](hardware/README.md).
Không bật RGB, màn hình, cảm biến chuột hay giao thức IEEE 802.15.4 khi chưa
xác nhận linh kiện và đường nối. LED P1.10 mới chỉ có khai báo, chưa gán chức
năng báo trạng thái vì chưa đo kiểu đấu.

+8 dBm, debounce 1/5 ms và interval yêu cầu không bảo đảm không trễ/không mất
phím. Chưa đo độ chính xác báo pin, dòng ngủ, thời lượng pin, tầm sóng hoặc
thời gian thức dậy. Không có kết luận “tắt RGB/SPI hết rò pin” từ cấu hình này.

## Build

GitHub Actions dùng [build.yaml](../build.yaml), target `curve_e//zmk` và
snippet Studio. Workflow ZMK nhận diện `zephyr/module.yml` để nạp module repo.
Chưa push thay đổi đồng nghĩa CI trên GitHub chưa kiểm tra cấu hình local này.

Build từ thư mục gốc repo. Workspace cố định là `build/curve-e/`:

```sh
mkdir -p build/curve-e/config build/curve-e/module
cp -R config/. build/curve-e/config/
cp -R boards zephyr build/curve-e/module/
docker run --rm --user "$(id -u):$(id -g)" \
  -e HOME=/tmp/curve-home \
  -v "$PWD/build/curve-e:/work" -w /work \
  zmkfirmware/zmk-build-arm:stable sh -c '
    mkdir -p "$HOME" &&
    { test -d .west || west init -l config; } &&
    west update --narrow --fetch-opt=--depth=1 &&
    west zephyr-export &&
    west build -p always -s zmk/app -d build -b curve_e//zmk \
      -S studio-rpc-usb-uart -- \
      -DZMK_CONFIG=/work/config -DZMK_EXTRA_MODULES=/work/module &&
    west manifest --freeze --active-only > /work/west-frozen.yml
  ' > build/curve-e/build-final.log 2>&1
```

Lệnh cần Docker và quyền dùng Docker. `HOME=/tmp/curve-home` chỉ là home nội bộ
container; dependency, source snapshot, output và log đều lưu dưới repo.
Nếu xóa/đổi tên tệp nguồn, xóa snapshot `build/curve-e/config` và
`build/curve-e/module` trước khi copy lại để tránh giữ tệp cũ trong workspace.
Chỉ dọn hai snapshot này, không xóa nguồn `config/`, `boards/`, `zephyr/`.

Đầu ra: `build/curve-e/build/zephyr/zmk.uf2`. Sau khi build đạt, lưu vào một
thư mục phiên bản mới dưới `firmware/` cùng `build-final.log`, `.config`,
`zephyr.dts`, `west-frozen.yml` và manifest/hash tương ứng. Không ghi đè
archive cũ bằng firmware từ nguồn khác. Các đường dẫn bản hiện hành ở dưới.
Không nạp hoặc thay bootloader tự động.

## Kiểm chứng

Build sạch **đạt**, đã tạo `zmk.uf2` (476160 byte), bắt đầu tại `0x26000`.
Dùng 238004 byte flash (29.35% vùng app 792 KiB) và 70852 byte RAM (27.03%).
Đã build tại `build/curve-e/` ngay trong repo; workspace trung gian đã được dọn
sau khi xác minh archive. Lần build sau tạo lại bằng các lệnh ở trên.
Bản firmware và bằng chứng được
lưu tại [firmware/2026-10-28-zmk-4dd95d7](../firmware/2026-10-28-zmk-4dd95d7/):

- [Firmware để nạp](../firmware/2026-10-28-zmk-4dd95d7/zmk.uf2).
- [Manifest kiểm chứng](../firmware/2026-10-28-zmk-4dd95d7/build.json): hash nguồn,
  image digest, hash UF2 và các tùy chọn đã đối chiếu.
- [Log build](../firmware/2026-10-28-zmk-4dd95d7/build.log),
  [effective config](../firmware/2026-10-28-zmk-4dd95d7/effective.config),
  [devicetree](../firmware/2026-10-28-zmk-4dd95d7/zephyr.dts),
  [dependency đã khóa](../firmware/2026-10-28-zmk-4dd95d7/west-frozen.yml).

Các artifact này không bị Git bỏ qua; hiện chưa commit. Không dùng đường dẫn
firmware tạm làm nơi bàn giao nữa.

- ZMK commit: `4dd95d7840eb3d5a3f59bb29fbf1ab054715ef85`.
- Zephyr commit: `10ba6d0cb38bc3d258775d27982f707599320085`.
- UF2 SHA-256: `179e8b2b7df242e11339ac4fb8e63d5b343d9406ed4ea65e866e5cbd87e9583c`.
- Đã kiểm tra Kconfig thực sinh ra cho Studio USB/BLE, pointing, EC11, báo pin,
  sleep, peripheral latency 0, boot-mode retention, NKRO và USB Boot Protocol.
- Đã đối chiếu GPIO, transform và vùng flash với bản trước nâng cấp: giữ nguyên.
- Đã kiểm tra 4 layer × 47 bindings, 47 tọa độ Studio, 6 combo chỉ layer 0,
  liên kết tài liệu và nguồn trong workspace build khớp repo.
- Đã kiểm tra mọi block UF2: đúng family nRF52840, nằm trong vùng code partition;
  không chứa dữ liệu ghi vào bootloader hoặc storage partition.

Còn cảnh báo **upstream**: KSCAN deprecated, địa chỉ clock/power và ACL/flash
controller trùng trong SoC devicetree, và qualifier `const` trong tap-dance.
Không sửa mã ZMK/Zephyr dependency hoặc che các cảnh báo này.

**Chưa nạp firmware mới**, chưa kiểm tra Studio, từng phím, encoder, Bluetooth,
sleep/wake hoặc mức tiêu thụ trên bo thật. Nhận diện USB trong tài liệu phần
cứng là bằng chứng của firmware trước nâng cấp, không phải runtime acceptance
của UF2 này. USB product/VID/PID mới dùng mặc định ZMK (`Curve E`,
`1D50:615E`), không phải descriptor `DEKB CURVE E BLE`/`DE17:40E1` đã đọc trước đó.

Khi thay đổi mã/cấu hình tiếp theo, cần build lại và cập nhật manifest kiểm
chứng cùng đoạn này; hash nguồn cho phép phát hiện bằng chứng build đã cũ.

## Kiểm tra trước khi nạp

Đã kiểm tra chỉ đọc ngày 28/10/2026, sau cleanup:

- Hash nguồn hiện tại khớp manifest build; checksum firmware và các artifact đạt.
- UF2 có 930 block, số block đầy đủ/không trùng, đúng family nRF52840;
  địa chỉ và kích thước căn chỉnh, các khoảng ghi không chồng nhau.
- Khoảng ghi là `[0x26000, 0x60200)`, nằm trong code partition của repo;
  không có block ghi SoftDevice, storage, bootloader hoặc UICR theo sơ đồ này.
- Trước thao tác RESET vật lý, USB nhận `CURVE E BLE`, VID:PID `DE17:40E1`;
  thời điểm đó chưa thấy ổ bootloader. Chưa nạp firmware mới.

Các kiểm tra này chưa xác minh partition/bootloader thực trên thiết bị và
không thay thế thử GPIO, BLE, encoder hay sleep/wake trên bo thật. Bước tiếp
theo là đối chiếu bootloader và kiểm tra có thể thoát về firmware cũ trước khi
ghi file.

Sau thao tác nút vật lý của người dùng, đã nhận ổ `CURVE-E` tại
`/run/media/will/CURVE-E`, mount **read-only**. `INFO_UF2.TXT` cho biết:

- Bootloader: `0.10.0-10-g8023c1e-dirty`.
- Model: `CURVE E`; Board-ID: `dekb-curves-e`.
- Build date bootloader: `Jul 13 2026`; SoftDevice: `S140 6.1.1`.

**Chặn nạp bản UF2 hiện tại:** `CURRENT.UF2` do bootloader xuất ra có family
`0xDE1740E1`, khác `0xADA52840` của bản build. Dump không đủ để chứng minh
bootloader chỉ chấp nhận family nào, nhưng đây là khác biệt chưa giải quyết;
không tự thử nạp để kiểm chứng. Cần đối chiếu mã/cấu hình bootloader và tạo
artifact tương thích, không thay bootloader để né kiểm tra.

Đã đọc và sao lưu `CURRENT.UF2` (1908736 byte, 3728 block, khoảng địa chỉ
`[0x1000, 0xEA000)`) cùng `INFO_UF2.TXT` và checksum vào
`firmware/recovery-2026-10-28/`, được Git bỏ qua riêng. Đây là dump do thiết
bị xuất, có cả vùng ngoài app; chưa xác nhận có thể dùng trực tiếp làm UF2
quay lui. Chưa có thao tác ghi flash hoặc remount read-write.

Sau khi người dùng rút/cắm lại USB, đã quan sát thiết bị ngắt rồi nhận lại
`CURVE E BLE`, VID:PID `DE17:40E1`; ổ bootloader biến mất. Đường thoát về
ứng dụng cũ đã kiểm tra ở mức enumeration, chưa xác nhận gõ phím.
Bằng chứng: [bootloader scan](hardware/evidence/bootloader-2026-10-28.json).

## Tài liệu chính thức đã đối chiếu

- [Board mới](https://zmk.dev/docs/hardware-integration/new-board)
- [Module](https://zmk.dev/docs/development/module-creation)
- [Physical layout](https://zmk.dev/docs/hardware-integration/physical-layouts)
- [Studio](https://zmk.dev/docs/features/studio)
- [Encoder](https://zmk.dev/docs/hardware-integration/encoders)
- [Sensor rotation](https://zmk.dev/docs/keymaps/behaviors/sensor-rotate)
- [Mouse emulation](https://zmk.dev/docs/keymaps/behaviors/mouse-emulation)
- [Bluetooth](https://zmk.dev/docs/config/bluetooth)
- [Nguồn](https://zmk.dev/docs/features/low-power-states)
