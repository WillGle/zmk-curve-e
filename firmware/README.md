# Firmware Curve E

Lưu mỗi bản firmware đã build trong một thư mục riêng tại đây, cùng manifest,
log build, cấu hình thực tế, devicetree sinh ra và manifest dependency đã khóa.
Các tệp này không bị `.gitignore` loại bỏ; có thể commit cùng phần mềm/tài liệu.
Không ghi đè bản đã lưu khi build từ nguồn khác: dùng thư mục mới có ngày,
revision và hậu tố nếu cần.

Bản hiện hành: [2026-10-28-zmk-4dd95d7](2026-10-28-zmk-4dd95d7/).
File để nạp: [zmk.uf2](2026-10-28-zmk-4dd95d7/zmk.uf2).
Hash và nguồn: [build.json](2026-10-28-zmk-4dd95d7/build.json).

**Chưa nạp bản hiện hành:** kiểm tra bootloader thực tế phát hiện family ID
trong dump thiết bị là `0xDE1740E1`, khác `0xADA52840` của UF2 mới. Cần xác
minh family mà bootloader chấp nhận và điều chỉnh bản build trước khi nạp.
Xem [kiểm tra trước khi nạp](../docs/firmware.md#kiểm-tra-trước-khi-nạp).

Dump đọc từ thiết bị nằm tại `firmware/recovery-2026-10-28/` và được Git bỏ qua
riêng. Dump này bao gồm dữ liệu ngoài vùng app; chưa xác nhận quy trình khôi
phục và không dùng làm file nạp trực tiếp.

Workspace dependency và output biên dịch được tạo tại `build/curve-e/` trong repo.
Workspace đã được dọn sau khi xác minh archive; lần build sau tạo lại theo hướng dẫn.
Workspace này được Git bỏ qua vì có thể tạo lại; bản firmware và bằng chứng
trong `firmware/` được giữ riêng để không phụ thuộc vào workspace hoặc `/tmp`.

Xem [hướng dẫn build và trạng thái kiểm chứng](../docs/firmware.md).
