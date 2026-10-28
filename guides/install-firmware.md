# Cài firmware Curve E

[Hướng dẫn](README.md) · [Firmware để tải](../firmware/README.md)

**Bản hiện hành chưa được nạp/thử trên bo thật.** Đây là quy trình cài đặt đề
xuất dựa trên bootloader đã khảo sát, chưa phải quy trình cài thành công đã
được chứng minh. Việc sắp xếp repo không thực hiện thao tác ghi thiết bị.

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
3. Ổ khảo sát trước đây được mount read-only. Nếu vẫn ở chế độ chỉ đọc,
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
