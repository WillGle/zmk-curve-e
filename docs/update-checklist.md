# Trạng thái và việc cần làm để cập nhật firmware local

Cập nhật sau nạp ngày 28/10/2026, khoảng 09:46 UTC (16:46 giờ Việt Nam).
[Tài liệu](README.md) · [Cách cài](../guides/install-firmware.md)

## Kết luận

**Đã tự build offline, nạp firmware local và kiểm chứng USB/Studio trên bo.**
Người dùng xác nhận không lắp encoder: vị trí hybrid là nút bấm. Còn chờ thử
phím/nút hybrid, BLE, lưu keymap và sleep/wake. Không cần sửa PCB
hoặc có schematic/BOM cho luồng nâng cấp ứng dụng này.

| Hạng mục | Trạng thái hiện hành |
| --- | --- |
| Board đang cắm | Ứng dụng mới `ZMK Project / Curve E`, `1D50:615E`; HID keyboard/mouse, hai cổng CDC |
| Firmware hiện hành | [ble-compat](../firmware/releases/2026-10-28-zmk-4dd95d7-ble-compat/zmk.uf2), SHA-256 `d880cee41ef409b0690c8ac80a736abda7fd4c09b9d0df68684756389ac97e6b` |
| Nguồn cập nhật | ZMK `4dd95d7840eb3d5a3f59bb29fbf1ab054715ef85`; trùng `main` tại lần tra API trong audit |
| Build offline | Build `--network none` mới đạt; bản flash-bound trước có kiểm chứng byte-identical riêng |
| Môi trường local | Image pinned digest và 56 project có HEAD khớp frozen manifest đã được kiểm tra; source snapshot khớp repo |
| Preflight | 928 block UF2 mới, range `[0x26000,0x60000)`, vector/cấu hình/hash đạt; không ghi vùng bootloader/SoftDevice |
| Backup trước nạp | CURRENT/INFO mới giữ riêng ngoài Git; readback 930 block app cũ khớp archive. Thiếu MBR/bootloader/UICR; chưa thử rollback |
| Phiên nạp | Bản BLE mới ghi đủ 475136 byte; EIO ở bước đồng bộ khi bootloader ngắt. Sau đó USB mới và Studio RPC đáp ứng, không tự retry |
| Studio | `ttyACM1`, unlocked; đọc được 4 layer × 47 bindings và layout 47 vị trí |
| Quyền cổng serial | Thấy ngoài sandbox; user host thiếu dialout. Đã đọc Studio qua container tạm cấp đúng device/group, không đổi quyền host lâu dài |
| Chưa kiểm chứng | Phím/nút hybrid, save/reload keymap, BLE, pin và sleep/wake; rollback/SWD |

Bản hiện hành tắt passkey, experimental BLE và unauthenticated bond overwrite;
PHY 2M tắt rõ ràng. Xem [cấu hình BLE và lý do](bluetooth.md).
Người dùng đã xác nhận gõ phím chữ qua USB trên bản trước và mở được Studio;
bản BLE mới xác nhận USB/Studio, chưa có xác nhận gõ BLE.

## Đã hoàn tất

- [x] Kiểm tra nguồn, checksum release và cấu trúc UF2.
- [x] Build lại offline từ toolchain/dependency local; UF2 byte-identical.
- [x] Xác nhận đúng INFO/model/Board-ID và mount có quyền ghi sau RESET vật lý.
- [x] Sao lưu CURRENT/INFO mới theo phiên, đối chiếu checksum với backup cũ.
- [x] Nạp UF2 ứng dụng; giữ nguyên bootloader/SoftDevice, không mass erase.
- [x] Xác nhận USB mới, HID keyboard/mouse và hai cổng CDC.
- [x] Studio RPC chỉ đọc: device info, lock state, keymap và physical layouts.
- [x] Lưu bằng chứng và đồng bộ trạng thái tài liệu; pin image trong hướng dẫn build.

## Cần thao tác thực tế tiếp theo

1. [ ] Thử toàn bộ phím/nút hybrid, 4 layer, combo, tap-dance và mouse emulation;
   không kiểm thử xoay vì không lắp encoder.
2. [ ] Mở Studio từ host có quyền serial; thử đổi/lưu/reload một binding rồi
   khôi phục theo ý người dùng. Phiên tự động chỉ đọc, chưa sửa settings.
3. [ ] Kiểm tra BLE ghép đôi, chuyển profile/output, báo pin; xử lý HID cache
   khi đổi descriptor theo [hướng dẫn keymap](../guides/configure-keymap.md).
4. [ ] Kiểm tra idle/sleep/wake; đo riêng nếu cần kết luận về độ trễ, sóng,
   độ chính xác báo pin hoặc thời lượng pin.
5. [ ] Ghi kết quả các mục trên trước khi công bố mọi tính năng đã đạt.

Quét LE tự động 12 giây chưa thấy quảng bá tên Curve; adapter đã dừng quét.
[Bằng chứng scan](evidence/post-flash-ble-scan-2026-10-28.json). Không suy ra
radio hỏng từ một lần scan ngắn, chưa thay pairing hoặc chọn profile.

Lượt đo nút hybrid: người dùng xác nhận đã nhấn 3 lần nhưng cửa sổ evdev
45 giây chưa ghi được sự kiện. Chưa xác minh thời điểm thao tác trùng cửa sổ
đo hoặc đầu ra USB/BLE; không kết luận nút hỏng hay tự đổi keymap.
[Bằng chứng](evidence/post-flash-hybrid-button-2026-10-28.json).
Người dùng đã xác nhận phím chữ thông thường gõ được qua USB trên bản trước.
Chưa chốt tín hiệu riêng nút hybrid; không còn xem đây là lỗi gõ USB chung. Nguồn ZMK cho phép lưu preferred output; Studio RPC hoạt
động không tự chứng minh HID output đang được chọn USB.

## Duy trì về sau

- [x] Có hướng dẫn build offline đã chạy đạt và image pin theo digest.
- [ ] Mỗi lần cập nhật nguồn upstream: lấy revision mới, build vào thư mục
  phiên bản mới, preflight, backup, nạp và ghi kết quả. Không ghi đè release cũ.
- [ ] Khi cần tái tạo archive cũ trên máy mới, chuẩn bị đúng toolchain và mọi
  revision từ frozen manifest. Cache máy hiện tại không tự là mirror lâu dài.
- [ ] Nếu cần cứu bo khi mất bootloader, làm phiên backup SWD riêng cho toàn
  flash/UICR nếu có quyền đọc; chưa có bằng chứng này, không chạy recover để thử.

## Bằng chứng

- [Preflight BLE mới](evidence/ble-compat-preflight-2026-10-28.json).
- [Backup/nạp BLE mới](evidence/ble-compat-flash-session-2026-10-28.json).
- [Studio trên bản BLE mới](evidence/ble-compat-post-flash-studio-2026-10-28.json).

Các bằng chứng sau là lịch sử bản flash-bound:

- [Audit trước nạp](evidence/local-update-audit-2026-10-28.json): snapshot lịch sử.
- [Build offline](../firmware/verification/2026-10-28-offline/verification.json)
  và [log](../firmware/verification/2026-10-28-offline/build.log).
- [Backup/preflight/nạp](evidence/flash-session-2026-10-28.json).
- [USB sau nạp](evidence/post-flash-usb-2026-10-28.json).
- [Studio RPC sau nạp](evidence/post-flash-studio-2026-10-28.json).
