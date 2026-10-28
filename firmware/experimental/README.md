# Firmware thử nghiệm ESB

**Firmware BLE chính không thay đổi.** Chỉ dùng bản hiện hành ở
[releases](../README.md). Những bản dưới đây chưa nạp hay kiểm chứng radio,
không phải firmware ba mode sẵn sàng sử dụng.

Bộ build hiện có: [20261028T102818Z](builds/20261028T102818Z/).

| Bản | Target | Bằng chứng |
| --- | --- | --- |
| Receiver USB+ESB | nrf52840dk/nrf52840/zmk | [Manifest](builds/20261028T102818Z/receiver/build.json) |
| Keyboard USB+ESB | curve_e//zmk, BLE/Studio/sleep tắt | [Manifest](builds/20261028T102818Z/keyboard/build.json) |
| Hồi quy USB/BLE | curve_e//zmk, source/config chính | [Manifest](builds/20261028T102818Z/baseline/build.json): UF2 byte-identical release hiện hành |

Mỗi thư mục có build log, effective config, DTS, dependency và source checksums.
[Probe log](builds/20261028T102818Z/probes.log) ghi 97 assertion với sanitizer;
transport dùng hardware/queue giả lập. Xem [bằng chứng tổng hợp](../../docs/evidence/esb-prototype-2026-10-28.json).

Binary có khóa riêng nằm ở `private/20261028T102818Z/{keyboard,receiver}/`,
được giữ local và loại khỏi Git. Khóa ở `private/pair-key.h`; không xóa hay
thay khóa một đầu nếu muốn giữ cặp firmware tương ứng. Không công khai binary
hoặc header này. Manifest chỉ ghi đường dẫn/hash, không chứa giá trị khóa.
Receiver DK có ELF/HEX/BIN; không copy UF2 Curve E sang receiver.

Build từ gốc repo: `./tools/build-esb.sh all`. Mỗi lượt tự tạo archive riêng
có timestamp, không chỉ để artifact trong workspace sẽ bị build lại.
Không có thao tác nạp tự động. Cách triển khai và giới hạn ở
[module ESB](../../experimental/esb/README.md) và
[tài liệu phát triển](../../docs/esb-development.md).

Bước tiếp cần phần cứng receiver để thử USB HID trước, sau đó mới lên kế hoạch
nạp thử keyboard có backup/rollback. Chưa thay firmware BLE trên bàn phím.
