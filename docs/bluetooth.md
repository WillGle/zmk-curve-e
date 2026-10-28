# Curve E — BLE ưu tiên tương thích

[Tài liệu](README.md) · [Firmware](../firmware/README.md) ·
[Hướng dẫn phím/kết nối](../guides/configure-keymap.md)

Bản đang cài từ 28/10/2026:
`2026-10-28-zmk-4dd95d7-ble-compat-curved`. Đã build offline, preflight và nạp;
USB/Studio RPC hoạt động sau reboot. **Chưa đo độ ổn định BLE trên host.**
Bản curved chỉ bổ sung hình học Studio; effective config BLE giống hệt
bản ble-compat trước. [Bằng chứng layout](evidence/curved-layout-preflight-2026-10-28.json).
Các thay đổi BLE dưới đây là cấu hình ưu tiên tương thích, không phải kết quả đo
độ trễ, tỷ lệ mất phím, mức pin hay độ ổn định dài hạn.

## Kiểm tra và thay đổi

| Cấu hình | Trước | Bản ble-compat / lý do |
| --- | --- | --- |
| Experimental bundle | Bật | Tắt; dùng tùy chọn riêng thay vì kéo theo experimental security |
| Experimental connection/security | Bật do bundle | Tắt |
| PHY 2M | Tắt do experimental connection | Tiếp tục tắt bằng `CONFIG_BT_CTLR_PHY_2M=n`; ZMK nêu đây là biện pháp tương thích với một số chipset |
| Nhập passkey | Bật do experimental security | `CONFIG_ZMK_BLE_PASSKEY_ENTRY=n` theo yêu cầu người dùng |
| Ghi đè bond không xác thực | Bật do experimental security | Tắt; không tự thay bond cũ từ host đã lưu |
| Công suất phát | +8 dBm | Giữ nguyên; tài liệu ZMK đề xuất cho liên kết yếu, chưa có đo RF để chọn mức khác |
| Interval yêu cầu | 6–12 đơn vị 1,25 ms | Giữ 7,5–15 ms, trùng mặc định ZMK; host quyết định giá trị thực |
| Peripheral latency | 0 | Giữ không bỏ qua connection event; thiên về phản hồi, chưa tối ưu pin |
| Supervision timeout | 400 đơn vị 10 ms | Giữ 4 giây, trùng mặc định ZMK |
| Bond/profile | 5 profile, tối đa 5 connection context | Giữ nguyên; chỉ profile được chọn nhận HID output, không đồng nghĩa gõ cho 5 host cùng lúc |
| Secure Connections, bonding, settings | Bật | Giữ nguyên; không tắt mã hóa hoặc xóa pairing |
| Sleep | 30 phút | Giữ nguyên; reconnect sau wake cần thử trên host |

Bản mới dùng pairing không nhập passkey trên bàn phím. Hệ điều hành vẫn có
thể yêu cầu xác nhận hoặc từ chối nếu policy bắt buộc passkey; cấu hình bàn
phím không thể bỏ chính sách của host. Không có xác thực bằng passkey cho
lần ghép đôi mới. Nếu cần làm lại pairing, quên thiết bị ở host và chỉ xóa
bond của profile liên quan sau khi xác định đúng profile; không xóa toàn bộ
bond/settings theo mặc định.

## Bằng chứng và giới hạn

- [Manifest build](../firmware/releases/2026-10-28-zmk-4dd95d7-ble-compat/build.json):
  nguồn khớp snapshot, effective config và hash artifact.
- [Preflight](evidence/ble-compat-preflight-2026-10-28.json): 928 block,
  `[0x26000,0x60000)`, vector và cấu hình hợp lệ.
- [Backup và nạp](evidence/ble-compat-flash-session-2026-10-28.json): backup mới,
  readback toàn bộ 930 block của bản cũ khớp archive; ghi đủ UF2 mới. EIO ở bước
  đồng bộ khi bootloader ngắt được ghi lại, không tự retry.
- [Studio sau nạp](evidence/ble-compat-post-flash-studio-2026-10-28.json):
  thiết bị phản hồi, 4 layer × 47 bindings và layout 47 vị trí; chỉ đọc settings.

Release cũ được giữ nguyên. Chỉ ghi vùng app, không thay bootloader/SoftDevice,
không ghi/xóa vùng NVS chứa keymap Studio và pairing. Việc giữ settings không
tự chứng minh mọi pairing cũ sẽ tương thích sau thay đổi security.

## Kiểm tra BLE còn cần thực hiện

1. Chọn đúng profile trong Studio/keymap; nếu profile đã có bond của host khác,
   dùng profile trống hoặc xử lý đúng bond, không mặc định `BT_CLR` toàn bộ.
2. Khi vẫn cấp nguồn bằng cáp USB, chọn output BLE để thử gõ không dây. Theo
   keymap nguồn mặc định: giữ `mo 1` + `mo 2`, nhấn vị trí I để chọn `OUT_BLE`;
   nhấn vị trí U để trở về `OUT_USB`. Nếu Studio đã đổi bindings, dùng chức
   năng theo keymap đã lưu thay vì dựa vào tên phím nguồn.
3. Ghép đôi trên host, thử gõ, giữ phím, tổ hợp modifier và reconnect sau tắt/bật
   Bluetooth host. Ghi host/OS, khoảng cách và điều kiện thử.
4. Thử chuyển profile rồi quay lại; kiểm tra idle và sleep/wake thực tế. Chưa
   đo các hành vi này trong phiên build/nạp.
5. Chỉ thay tiếp power/timing nếu có lỗi tái hiện và số liệu; không tuyên bố
   ổn định chỉ vì đặt công suất cao hoặc interval thấp.

Nguồn chính thức và commit đã đối chiếu được gom ở
[tài liệu tham khảo BLE](references/official.md#zmk).
