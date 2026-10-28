# Cài firmware Curve E

[Hướng dẫn](README.md) · [Firmware để tải](../firmware/README.md)

Bản hiện hành đã được nạp ngày 28/10/2026; USB/HID/CDC và Studio RPC đã kiểm
tra thành công. Chưa kiểm tra đầy đủ phím/nút hybrid, BLE, sleep/wake.
[Bằng chứng nạp](../docs/evidence/flash-session-2026-10-28.json) ghi cả lỗi I/O
ở bước đồng bộ khi bootloader ngắt kết nối: đủ byte đã ghi và ứng dụng mới
được xác nhận bằng USB/Studio sau đó. Không tự nạp lại chỉ vì lỗi đồng bộ;
kiểm tra thiết bị đã khởi động lại trước.

## Chuẩn bị

1. Chọn bản trong [danh mục firmware](../firmware/README.md), đọc trạng thái
   tương thích và [kết quả preflight](../docs/verification.md#kiểm-tra-trước-khi-nạp).
2. Tại thư mục bản đã chọn dưới `firmware/releases/`, chạy
   `sha256sum -c SHA256SUMS`. Chỉ dùng UF2 khi các checksum đều đạt.
3. Đọc [hướng dẫn khôi phục](recovery.md). Dump `CURRENT.UF2` đang giữ không có
   toàn bộ flash/UICR; chưa đủ để cứu thiết bị nếu bootloader bị mất.

## Vào bootloader và nạp

1. Dùng thao tác RESET kép đã quan sát trên thiết bị để vào ổ `CURVE-E`.
   Đọc `INFO_UF2.TXT` và đối chiếu model/Board-ID với
   [bản khảo sát](../docs/evidence/bootloader-2026-10-28.json).
2. Sao lưu `INFO_UF2.TXT` và `CURRENT.UF2` vào thư mục recovery riêng trước khi
   ghi; giữ dump ngoài Git như quy tắc hiện hành của repo.
3. Ổ từng được mount read-only; lần nạp đã xác nhận mount read-write. Nếu vẫn ở chế độ chỉ đọc,
   dừng ở đây để xác định cách mount ghi trên host; không coi lỗi copy là lỗi UF2.
4. Khi đã kiểm tra đúng thiết bị và ổ có quyền ghi, copy **chỉ `zmk.uf2` của bản
   đã chọn** vào ổ bootloader. Giữ nguồn/cáp ổn định đến khi việc copy và reboot
   hoàn tất. Bootloader nền ghi ứng dụng trực tiếp, không có rollback atomic.

## Kiểm tra sau nạp

Kiểm tra USB nhận lại thiết bị, gõ từng phím, layer/combo, encoder, chuột,
Studio rồi BLE và sleep/wake. Bản mới dùng descriptor mặc định ZMK
`Curve E` / `1D50:615E`, khác firmware vendor trước đó.
Đối chiếu [thao tác keymap](configure-keymap.md); khi đổi HID descriptor BLE,
đọc phần ghép đôi lại trong hướng dẫn đó.

Nếu ứng dụng không chạy, dùng RESET vật lý để quay về bootloader và đọc
[hướng dẫn khôi phục](recovery.md). Chưa có bằng chứng rollback bằng dump cũ
đã được thử thành công.
