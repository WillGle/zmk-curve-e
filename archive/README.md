# Kho lưu trữ tham chiếu

Các nguồn ở đây phục vụ provenance, đối chiếu và điều tra lịch sử. Chúng không
phải mã nguồn hiện hành để build Curve E Firmware.

| Nhóm | Nội dung |
| --- | --- |
| [Nguồn upstream cũ](upstream/README.md) | Snapshot vendor, fork ESB, thư viện KiCad và bootloader nền; giữ license/hash gốc |
| [Công cụ điều tra cũ](legacy-tools/README.md) | Hai probe serial/input giữ nguyên, không chạy tự động |
| [Ánh xạ di chuyển](relocations.json) | Đường dẫn cũ → mới và checksum trước khi chuyển |

Evidence/manifest lịch sử có thể ghi `hardware/upstream/` hoặc `tools/legacy/`.
Dùng ánh xạ trên để tra cứu; không viết lại manifest lịch sử. Nội dung nguồn
snapshot được giữ nguyên, trừ README chỉ dẫn được cập nhật khi cần.

Nguồn được duy trì nằm tại `boards/`, `config/`, `experimental/esb/` và `tools/`.
Firmware release và bằng chứng build giữ tại `firmware/`; ảnh/nhận diện phần cứng
hiện hành giữ tại `hardware/`. [Về sản phẩm](../README.md).
