# Kế hoạch thực hiện sản phẩm độc lập

Phạm vi hiện hành chỉ gồm **cáp USB và BLE**. Hướng USB dongle/ESB 2,4 GHz
đã dừng theo quyết định người dùng; các mục ESB phía dưới chỉ là lịch sử,
không phải việc cần tiếp tục. Không thay đổi firmware đang cài.

> Thực hiện trực tiếp, tuần tự bằng `executing-plans`; thiết kế đã được người dùng duyệt.

**Thiết kế:** [Curve E Firmware](product-independence-design.md).
**Mục tiêu:** repo tự duy trì có metadata, manifest khóa, build/release rõ ràng;
BLE binary không đổi, ESB cách ly. Không commit/push/flash hoặc kiểm tra GitHub.

## Các bước

- [x] 1. Thêm `product.json` và `tools/product.py`; chuyển frozen manifest đã
  kiểm chứng thành `config/west.yml` hiện hành, khóa 56 dependency. Các công cụ
  đọc tên target/image/baseline/revision từ metadata/manifest thay hằng rải rác.
  Kiểm JSON, SHA 40 ký tự và đối chiếu toàn bộ dependency với cache sạch.
- [x] 2. Thêm `tools/build-firmware.sh` cùng helper kiểm dependency và archive
  bản chính; chuẩn hóa build ESB/prepare/archive đọc metadata. Giữ từng lượt
  artifact riêng trong repo, binary ESB có khóa private, không nạp/mạng mặc định.
  Build BLE và hai target ESB; đối chiếu UF2 chính với release baseline.
- [x] 3. Pin `.github/workflows/build.yml` về revision ZMK trong manifest, giữ
  `build.yaml` chỉ board chính. Chuyển hai probe cũ sang `archive/legacy-tools/` nguyên
  nội dung, thêm README cảnh báo; không chạy chúng.
- [x] 4. Định vị README sản phẩm; thêm LICENSE/NOTICE/CHANGELOG/CONTRIBUTING,
  sửa hướng dẫn build và trạng thái liên quan. Attribution/ngoại lệ tài liệu,
  ảnh, CAD, snapshot được giữ; không gán giấy phép của mình cho nguồn bên ngoài.
- [x] 5. Rà soát độc lập, xử lý finding trong phạm vi, kiểm JSON/YAML/Python/shell,
  link/anchor, checksum, secret ignore và source/manifest archive. Ghi evidence
  rebuild BLE byte-identical, ESB compile-only và CI chưa chạy trên GitHub.

## Ràng buộc kiểm chứng

Không sửa GPIO/keymap/cấu hình BLE hay bootloader. Khóa dependency phải có cùng
revision với manifest release đã kiểm chứng, không chỉ pin ZMK trong khi Zephyr
còn trôi nhánh. Bản chính không chứa module ESB. Artifact cũ bất biến; riêng
metadata hiện hành được cập nhật. Không thêm test duy trì theo chỉ dẫn repo.

## Tiến độ và quyết định

Điểm bắt đầu: thay đổi local duy nhất chưa commit là tài liệu thiết kế. Các thay
đổi firmware/ESB trước đó đã nằm trong HEAD; giữ nguyên. Dùng workspace build
riêng để không sửa cache source. Bất kỳ khác biệt UF2 BLE nào đều chặn kết luận
không thay firmware. Người dùng đã duyệt toàn bộ thiết kế, không xin lại quyền
cho các bước local có thể đảo ngược trong phạm vi này.

Kết quả: [bằng chứng tái cấu trúc](evidence/product-independence-2026-10-28.json).
Build chính byte-identical; receiver/keyboard ESB build đạt. Review độc lập
chưa hoàn tất do giới hạn phiên agent; đã tự kiểm cú pháp, đường dẫn, checksum
và ranh giới build. Không nhận có review độc lập hoàn chỉnh hoặc CI GitHub đạt.
