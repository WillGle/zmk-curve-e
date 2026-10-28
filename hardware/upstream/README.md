# Nguồn công khai được giữ để tra cứu Curve E

Được người dùng cho phép lưu trong repo ngày 28/10/2026. URL, commit, checksum,
phạm vi và license nằm trong [sources.json](sources.json). Giữ nguyên nguồn và
notice gốc; các snapshot là tài liệu tham chiếu, không tự thay nguồn board hiện hành.

- `dekb-235fcab.tar.gz` và `dekb-66f10de.tar.gz`: hai snapshot MIT của
  Deemen17/zmk-keyboards-dekb, kèm thư mục giải nén và LICENSE gốc. Đã đối chiếu
  từng file giải nén với archive; archive là bản đầy đủ kể cả file bị quy tắc
  ignore chung của repo loại khỏi danh sách Git.
- `deemen17_Library-6987369a6cf93b043e7f6f9a0715fac06c0e0e0a/`: chỉ footprint
  E73 và file symbol; không có LICENSE cấp repo được tìm thấy. Giữ để tra cứu
  cục bộ, không coi việc công khai mã là quyền tái sử dụng/phân phối.
- [Bootloader nền](adafruit-bootloader-8023c1e/source.json): archive, LICENSE
  gốc và hash các file đã đối chiếu. Cùng commit ID ở fork tác giả không bảo đảm
  mọi file bằng upstream Adafruit: hash Makefile/dfu_types khác nguồn upstream
  được dẫn trong preflight; bộ lọc ghostfat và uf2cfg có hash trùng. Giới hạn
  40 KiB reserve và boot start `0xF4000` trong cấu hình nRF52840 thường đều được
  kiểm tra ở archive. Có nhánh cấu hình debug riêng; chưa biết cờ build thực.
  Giấy phép phụ thuộc áp dụng theo từng file; archive GitHub không tải recursive
  submodule và không chứa thay đổi `dirty` trên thiết bị.

Kiểm tra checksum archive theo manifest bằng `sha256sum <đường-dẫn-archive>`.
Đối chiếu nguồn công khai và các giới hạn tại
[Khảo sát nguồn vendor](../../docs/references/public-sources.md) và
[Audit khôi phục](../../docs/recovery.md).
Không dùng archive nguồn để nạp thiết bị.
