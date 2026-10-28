# Phát triển Curve E Firmware

Đọc [trạng thái sản phẩm](README.md), [danh mục hiện hành](product.json) và
[thay đổi](CHANGELOG.md) trước khi sửa nguồn. Giữ thay đổi nhỏ, không dọn trạng
thái local của người khác và không đổi GPIO/partition/identity nếu ngoài phạm vi.

## Nguồn và dependency

- `boards/curve_e/`, `config/`, `zephyr/`: nguồn sản phẩm theo chuẩn module ZMK.
- `config/west.yml`: manifest đã resolve, mọi project khóa full commit SHA;
  không có dependency nhánh trôi hay nested import.
- `product.json`: image digest, target, đường dẫn bản chính và prototype.
- `experimental/esb/`: thử nghiệm riêng; không bật ESB hoặc sửa lifecycle BLE
  của bản chính. BLE là wireless mặc định, phải nghiệm thu ổn định trước phát hành.
- `archive/upstream/`: snapshot tra cứu và license gốc, không sửa tại chỗ.

Không thêm nguyên source ZMK vào repository này. Khi nâng dependency, cập nhật
manifest khóa, metadata nếu cần và tài liệu liên quan cùng nhau; không sửa cache
`build/` rồi coi đó là nguồn duy nhất. Archive release và evidence cũ bất biến.

## Build và kiểm chứng

Từ gốc repo, chạy `./tools/build-firmware.sh`; ESB dùng lệnh riêng
`./tools/build-esb.sh all`. Cần cache/image theo [hướng dẫn build](guides/build-firmware.md).
Lệnh mặc định offline, kiểm cache sạch/đúng revision, không tự `west update`,
reset, flash, pair hoặc mở serial. `archive/legacy-tools/` không thuộc quy trình này.

Thay đổi chỉ tổ chức phải giữ UF2 chính byte-identical. Với thay đổi firmware
có chủ đích, artifact được lưu thành candidate kiểm chứng và kết quả so sánh
baseline được ghi rõ; không được báo không đổi khi binary khác.

Sau thay đổi chạy các kiểm tra rẻ phù hợp: cú pháp shell/Python, JSON/YAML,
liên kết tài liệu, checksum và build cần thiết. Không thêm test duy trì khi chưa
được yêu cầu. Ghi bằng chứng riêng cho compile, probe giả lập và thử phần cứng.
CI được pin revision nhưng không nhận là đạt trước khi có kết quả chạy thực.

## Phát hành

Bản rebuild ở `firmware/verification/`; ESB ở `firmware/experimental/`. Mỗi lượt
có manifest, source/dependency hashes, effective config, DTS và log. Binary ESB
có khóa phải ở private ignored; không commit key, raw dump hoặc private binary.
Không ghi đè thư mục release đã có.

Chỉ đưa một candidate vào `firmware/releases/<id>/` sau kiểm chứng tương ứng,
checksum/provenance, đường recovery và nghiệm thu thực tế USB/BLE/sleep/wake.
Cập nhật `product.json` cùng danh mục firmware, hướng dẫn và trạng thái khi chọn
bản hiện hành mới. Prototype ba mode cần nghiệm thu riêng, không đi vào build
mặc định qua một thay đổi CI vô tình.

Không tự commit/push/tag/publish hoặc nạp firmware chỉ vì build đã đạt.
Giữ copyright, license và attribution theo [NOTICE](NOTICE.md).
