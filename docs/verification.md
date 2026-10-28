# Curve E — kết quả kiểm chứng

[Tài liệu kỹ thuật](README.md) · [Firmware để tải](../firmware/README.md)

## Kiểm chứng

Build sạch **đạt**, đã tạo `zmk.uf2` (476160 byte), bắt đầu tại `0x26000`.
Dùng 238004 byte flash (29.65% vùng app 784 KiB) và 70852 byte RAM (27.03%).
Đã build tại `build/curve-e/` ngay trong repo; workspace lần build này được giữ lại.
Bản firmware và bằng chứng được
lưu tại [firmware/releases/2026-10-28-zmk-4dd95d7-flash-bound](../firmware/releases/2026-10-28-zmk-4dd95d7-flash-bound):

- [Firmware để nạp](../firmware/releases/2026-10-28-zmk-4dd95d7-flash-bound/zmk.uf2).
- [Manifest kiểm chứng](../firmware/releases/2026-10-28-zmk-4dd95d7-flash-bound/build.json): hash nguồn,
  image digest, hash UF2 và các tùy chọn đã đối chiếu.
- [Log build](../firmware/releases/2026-10-28-zmk-4dd95d7-flash-bound/build.log),
  [effective config](../firmware/releases/2026-10-28-zmk-4dd95d7-flash-bound/effective.config),
  [devicetree](../firmware/releases/2026-10-28-zmk-4dd95d7-flash-bound/zephyr.dts),
  [dependency đã khóa](../firmware/releases/2026-10-28-zmk-4dd95d7-flash-bound/west-frozen.yml).

Các artifact này không bị Git bỏ qua. Archive cũ được giữ làm bằng chứng lịch sử. Không dùng đường dẫn
firmware tạm làm nơi bàn giao nữa.

- ZMK commit: `4dd95d7840eb3d5a3f59bb29fbf1ab054715ef85`.
- Zephyr commit: `10ba6d0cb38bc3d258775d27982f707599320085`.
- UF2 SHA-256: `8a188c54e9b7b8ccbb2349fdf21feac3aa2c8fc1c9679d27e6e4f0d4a511a3ed`.
- Đã kiểm tra Kconfig thực sinh ra cho Studio USB/BLE, pointing, EC11, báo pin,
  sleep, peripheral latency 0, boot-mode retention, NKRO và USB Boot Protocol.
- Đã đối chiếu GPIO/transform giữ nguyên; code partition mới kết thúc tại `0xEA000`.
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

Đã đối chiếu dump và mã nguồn nền bootloader như dưới đây. Các kiểm tra tĩnh
không thay thế thử GPIO, BLE, encoder hay sleep/wake trên bo thật.

Sau thao tác nút vật lý của người dùng, đã nhận ổ `CURVE-E` tại
`/run/media/will/CURVE-E`, mount **read-only**. `INFO_UF2.TXT` cho biết:

- Bootloader: `0.10.0-10-g8023c1e-dirty`.
- Model: `CURVE E`; Board-ID: `dekb-curves-e`.
- Build date bootloader: `Jul 13 2026`; SoftDevice: `S140 6.1.1`.

**Khác family ID đã được giải thích bằng mã nguồn nền:** `CURRENT.UF2` xuất
`0xDE1740E1`, còn bản build dùng `0xADA52840`. `ghostfat.c` tại commit
`8023c1e6a0a8014fce0fa1775a67ca54d4ad7454` chấp nhận cả ID riêng và ID nRF52840
chung; không cần sửa ID UF2. Bootloader thực có hậu tố `dirty`, nên chưa chứng
minh binary-exact. [Audit khôi phục](recovery.md) giữ đầy đủ giới hạn.

Đã sửa code partition kết thúc tại `0xEA000`, build lại và kiểm tra riêng
[930 block của artifact mới](evidence/uf2-preflight-flash-bound-2026-10-28.json).
Bản [preflight đầu](evidence/uf2-preflight-2026-10-28.json) là bằng chứng
lịch sử của archive cũ trước khi sửa capacity; không dùng nó thay kết quả mới.

Đã đọc và sao lưu `CURRENT.UF2` (1908736 byte, 3728 block, khoảng địa chỉ
`[0x1000, 0xEA000)`) cùng `INFO_UF2.TXT` và checksum vào
`firmware/recovery-2026-10-28/`, được Git bỏ qua riêng. Đây là dump do thiết
bị xuất, có cả vùng ngoài app; chưa xác nhận có thể dùng trực tiếp làm UF2
quay lui. Chưa có thao tác ghi flash hoặc remount read-write.

Sau khi người dùng rút/cắm lại USB, đã quan sát thiết bị ngắt rồi nhận lại
`CURVE E BLE`, VID:PID `DE17:40E1`; ổ bootloader biến mất. Đường thoát về
ứng dụng cũ đã kiểm tra ở mức enumeration, chưa xác nhận gõ phím.
Bằng chứng: [bootloader scan](evidence/bootloader-2026-10-28.json).
