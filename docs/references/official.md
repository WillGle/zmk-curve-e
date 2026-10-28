# Tài liệu chính thức và mã nguồn đối chiếu

Nguồn đã đọc trong đợt kiểm tra ngày 28/10/2026. Các kết quả áp dụng cho
Curve E nằm trong tài liệu phần cứng/firmware; trang này tập trung URL, revision
và giới hạn của tài liệu gốc. Xem [mục lục nguồn](README.md).

## ZMK

- [Board mới](https://zmk.dev/docs/hardware-integration/new-board)
- [Module](https://zmk.dev/docs/development/module-creation)
- [Physical layout](https://zmk.dev/docs/hardware-integration/physical-layouts)
- [Studio](https://zmk.dev/docs/features/studio)
- [Encoder](https://zmk.dev/docs/hardware-integration/encoders)
- [Sensor rotation](https://zmk.dev/docs/keymaps/behaviors/sensor-rotate)
- [Mouse emulation](https://zmk.dev/docs/keymaps/behaviors/mouse-emulation)
- [Bluetooth](https://zmk.dev/docs/config/bluetooth)
- [Khắc phục kết nối BLE](https://zmk.dev/docs/troubleshooting/connection-issues): PHY 2M, công suất phát, profile/output và policy passkey.
  Đối chiếu cả source/docs tại ZMK commit `4dd95d7840eb3d5a3f59bb29fbf1ab054715ef85`
  (`docs/docs/config/bluetooth.md`, `docs/docs/troubleshooting/connection-issues.mdx`,
  `app/Kconfig`); không suy ra ổn định thực tế chỉ từ các khuyến nghị này.
- [Nguồn](https://zmk.dev/docs/features/low-power-states)

### Công cụ Studio

- [ZMK Studio](https://zmk.studio/): ứng dụng chỉnh keymap; cách dùng tại
  [tài liệu firmware](../../guides/configure-keymap.md#studio-và-bluetooth).

## EBYTE E73

Đã đọc trực tiếp ngày 28/10/2026:

- [EBYTE E73-2G4M08S1C — trang sản phẩm](https://www.cdebyte.com/products/E73-2G4M08S1C).
- [Trang tải tài liệu chính hãng](https://www.cdebyte.com/products/E73-2G4M08S1C/4#Downloads).
- [E73-2G4M08S1C User Manual](https://www.cdebyte.com/pdf-down.aspx?id=3866), mục 2–3, trang in 4–7: nRF52840, kích thước 13 × 18 mm, anten gốm, pinout module. Manual ghi 32.768 kHz cần kết nối bên ngoài; cấu hình RC của firmware không xác nhận X1 đã lắp hoặc được dùng.

SHA256 của PDF đã đọc: `ed55bc86e548bbd11ca36e048dcf4388afc51dee502ff1ee60cd366fff066554`.
Hash nhận diện đúng bản đã kiểm tra; URL của hãng có thể cập nhật nội dung.
PDF hiện chỉ dùng trong thư mục tạm để phân tích, chưa đưa vào kho lưu trữ;
không suy ra quyền phân phối lại từ việc tải công khai.

## Bootloader, Nordic và OpenOCD

Nguồn nền được đối chiếu ở commit cố định, không dùng `main` để suy ra binary đang chạy:

- [ghostfat.c](https://github.com/adafruit/Adafruit_nRF52_Bootloader/blob/8023c1e6a0a8014fce0fa1775a67ca54d4ad7454/src/usb/uf2/ghostfat.c): `is_uf2_block`, `in_app_space`, tạo CURRENT, nhánh app/boot của `write_block`.
- [uf2cfg.h](https://github.com/adafruit/Adafruit_nRF52_Bootloader/blob/8023c1e6a0a8014fce0fa1775a67ca54d4ad7454/src/usb/uf2/uf2cfg.h), [Makefile](https://github.com/adafruit/Adafruit_nRF52_Bootloader/blob/8023c1e6a0a8014fce0fa1775a67ca54d4ad7454/Makefile): family, user range, reserve 40 KiB cho nRF52840 và ngoại lệ debug build.
- [flash_nrf5x.c](https://github.com/adafruit/Adafruit_nRF52_Bootloader/blob/8023c1e6a0a8014fce0fa1775a67ca54d4ad7454/src/flash_nrf5x.c): buffer trang, erase/write trực tiếp. File này và `ghostfat.c`/`main.c` có MIT header; không được suy ra toàn bộ bundle đều MIT.
- [dfu_types.h](https://github.com/adafruit/Adafruit_nRF52_Bootloader/blob/8023c1e6a0a8014fce0fa1775a67ca54d4ad7454/lib/sdk11/components/libraries/bootloader_dfu/dfu_types.h), [bootloader.c](https://github.com/adafruit/Adafruit_nRF52_Bootloader/blob/8023c1e6a0a8014fce0fa1775a67ca54d4ad7454/lib/sdk11/components/libraries/bootloader_dfu/bootloader.c), [bootloader_util.c](https://github.com/adafruit/Adafruit_nRF52_Bootloader/blob/8023c1e6a0a8014fce0fa1775a67ca54d4ad7454/lib/sdk11/components/libraries/bootloader_dfu/bootloader_util.c): layout nền, validity/settings, ACL trước app. SDK source có Nordic license header riêng; SoftDevice cũng có điều khoản Nordic riêng. Giữ notices và kiểm tra điều khoản kèm từng thành phần trước phân phối lại bộ firmware/bootloader. Chưa xác minh giấy phép của thay đổi riêng `dirty`.
- [Nordic nrfx nrf52840.svd v3.14.0](https://github.com/NordicSemiconductor/nrfx/blob/v3.14.0/mdk/nrf52840.svd): register UICR.APPROTECT, APPROTECT, ACL. Đây là nguồn chính hãng đã đọc; mô tả phân biệt chip dùng kiểm soát hardware và hardware+software, nên không suy đoán revision chip hiện tại.
- [OpenOCD v0.12.0 nrf52.cfg](https://github.com/openocd-org/openocd/blob/v0.12.0/tcl/target/nrf52.cfg): `nrf52_check_ap_lock` và `nrf52_recover` dùng CTRL-AP ERASEALL, tên/help ghi rõ “Mass erase and unlock”. Đây là nguồn implementation tool, không phải xác nhận đã thử trên bo.

Các trang PDF/Product Specification chính thức Nordic trả HTTP 403 trong lần đọc này; chưa kiểm chứng được phần diễn giải datasheet đầy đủ qua endpoint đó. Không có SWD readout, đo điện hoặc runtime flash trong audit. Các điều kiện chưa xác nhận ở trên vẫn là điều kiện cần trước phục hồi độc lập.
