# Thay đổi Curve E Firmware

Các mục ghi thay đổi của dự án độc lập; không thay lịch sử commit hoặc manifest
archive. Chưa đặt nhãn stable/1.0 khi kiểm thử phần cứng còn thiếu.

## Quyết định phạm vi — 28/10/2026

- Dừng phát triển receiver USB dongle và ESB 2,4 GHz; giữ source/artifact cũ
  làm tư liệu tham chiếu. Chỉ phát triển cáp USB và BLE, BLE là wireless mặc định.
- Chỉ cập nhật tài liệu/danh mục sản phẩm; không nạp hoặc đổi firmware.

## Chưa phát hành — sản phẩm độc lập, 28/10/2026

- Định vị Curve E Firmware thành sản phẩm riêng, ZMK là dependency khóa revision.
- Danh mục `product.json`, manifest đầy đủ và công cụ build/artifact riêng cho
  bản chính và prototype; CI dùng revision cố định.
- Giấy phép/phạm vi attribution, quy trình đóng góp/phát hành; probe cũ được
  giữ trong `tools/legacy/`, không chạy tự động.
- Giữ nguyên board target, USB/BLE identity và binary baseline của đợt tái cấu
  trúc. Bằng chứng kết quả cuối ở tài liệu kiểm chứng; không nạp thiết bị.

## Các mốc trước khi tái cấu trúc — 28/10/2026

- Bản chính `2026-10-28-zmk-4dd95d7-ble-compat-curved`: ZMK mới được pin,
  giới hạn app flash phù hợp, BLE compatibility/passkey-off và Studio 47 phím
  cong. Đã nạp/xác nhận USB/Studio; BLE stability và sleep/wake chưa nghiệm thu.
- Prototype USB+ESB riêng: receiver DK, codec/session xác thực, reconnect và
  giữ release khi queue đầy; đã compile/probe, chưa nạp hay thử radio.

Chi tiết và checksum: [firmware chính](firmware/README.md),
[prototype ESB](firmware/experimental/README.md), [kiểm chứng](docs/verification.md).
