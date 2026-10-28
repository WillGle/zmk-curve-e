# Trạng thái và việc cần làm để cập nhật firmware local

Kiểm tra ngày 28/10/2026, 09:00 UTC (16:00 giờ Việt Nam).
[Bằng chứng](evidence/local-update-audit-2026-10-28.json) ·
[Tài liệu](README.md) · [Cách cài](../guides/install-firmware.md)

## Kết luận

**Local đã đủ nguồn, toolchain đã lưu và UF2 để tiến hành nâng cấp ứng dụng
qua bootloader sau bước kiểm tra thiết bị.** Chưa đủ bằng chứng để công bố
bản firmware đã chạy tốt trên bo: chưa nạp, chưa thử từng tính năng.
Không cần sửa PCB hoặc có schematic/BOM để làm luồng cập nhật này.

| Hạng mục | Trạng thái đã kiểm tra |
| --- | --- |
| Board đang cắm | USB `DEKB / CURVE E BLE`, `DE17:40E1`; interface CDC + HID |
| Chế độ hiện tại | Ứng dụng; chưa thấy ổ `CURVE-E` được mount |
| Cổng serial | Kernel gắn `cdc_acm`, tạo `ttyACM0` (166:0); môi trường thực thi này không thấy node `/dev/ttyACM0` |
| Firmware đang cài | Chưa ghi mới trong phiên; chuỗi Zephyr 3.5.0/DEKB 20260426 là kết quả dump trước đó, không phải đọc phiên bản live lần này |
| Nguồn cập nhật | ZMK `4dd95d7840eb3d5a3f59bb29fbf1ab054715ef85`; trùng `main` theo API upstream vừa tra cứu |
| UF2 hiện hành | [flash-bound](../firmware/releases/2026-10-28-zmk-4dd95d7-flash-bound/zmk.uf2), 476160 byte; hash nguồn và toàn bộ checksum archive đạt |
| Cấu trúc UF2 | 930 block đầy đủ, không trùng; family `0xADA52840`; range `[0x26000,0x60200)`; vector hợp lệ và nằm trong giới hạn code `0xEA000` |
| Cấu hình thực sinh | Studio USB/BLE, pointing, EC11, sleep, báo pin, NKRO/USB Boot Protocol và boot retention được bật |
| Môi trường local | Có module board, config/keymap, workspace build và 56 project có HEAD khớp frozen manifest; UF2 workspace khớp archive |
| Toolchain | Docker image theo digest build đã có trên máy; lần audit này không build lại hoặc đo offline build mới |
| Backup cũ | CURRENT.UF2 và INFO còn trong recovery cục bộ, checksum đạt; chưa phải backup toàn flash/UICR hoặc rollback đã thử |
| Giới hạn bootloader | Nguồn nền nhận family chung; thay đổi riêng `dirty` chưa có để đối chiếu binary-exact |

## Đã chuẩn bị

- [x] Có nguồn board/module, keymap, Kconfig và ZMK pin theo commit.
- [x] Có build thành công đã lưu log, config, devicetree, dependency và checksum.
- [x] Kiểm tra lại nguồn local khớp manifest và checksum artifact đạt.
- [x] Kiểm tra lại mọi block UF2, range ghi và vector ứng dụng.
- [x] Xác nhận cache dependency và image toolchain local hiện hữu.
- [x] Có hướng dẫn cài/khôi phục và dump cũ còn nguyên checksum.

## Cần làm để hoàn tất nâng cấp

1. [ ] Người dùng vào bootloader bằng RESET; đọc lại INFO để đối chiếu đúng
   model, Board-ID và thiết bị trước khi ghi. Kiểm tra mount có quyền ghi;
   lần khảo sát trước ổ được mount read-only.
2. [ ] Sao lưu lại CURRENT/INFO theo lần nạp, giữ riêng ngoài Git; đối chiếu với
   backup trước để biết firmware đã thay đổi hay chưa. Không dùng dump làm file
   rollback chưa qua đánh giá.
3. [ ] Nạp **UF2 ứng dụng hiện hành** theo [hướng dẫn](../guides/install-firmware.md),
   giữ bootloader/SoftDevice và không thực hiện SWD recover hoặc mass erase.
4. [ ] Kiểm tra USB nhận lại descriptor mới `Curve E` / `1D50:615E`; kiểm tra
   toàn bộ phím, 4 layer, combo, tap-dance, encoder và mouse emulation.
5. [ ] Kiểm tra Studio đọc/đổi/lưu keymap. Cần truy cập được cổng Studio từ host
   hoặc môi trường được expose thiết bị; chưa thấy node trong phiên này không
   có nghĩa board không tạo serial.
6. [ ] Kiểm tra BLE ghép đôi, chuyển profile/output và báo pin; xử lý cache HID
   khi đổi descriptor theo [hướng dẫn keymap](../guides/configure-keymap.md).
7. [ ] Kiểm tra idle/sleep/wake; nếu cần kết luận về độ trễ, sóng hoặc pin thì
   đo riêng, không suy ra từ +8 dBm hoặc interval cấu hình.
8. [ ] Ghi kết quả, lỗi còn tồn tại và phiên bản đã cài vào tài liệu kiểm chứng;
   chỉ đổi trạng thái firmware thành đã kiểm thử sau khi có kết quả thật.

## Hoàn thiện khả năng tự duy trì về sau

- [ ] Ghi nhận checklist và manifest mới cho mỗi đợt cập nhật upstream; dùng
  thư mục release mới, không ghi đè archive cũ.
- [ ] Khi cần build lặp lại đúng dependency, sử dụng revision trong frozen
  manifest. Hướng dẫn build hiện dùng tag image `stable` và chạy `west update`,
  nên không tự bảo đảm lấy đúng dependency/image của archive cũ về sau.
- [ ] Nếu cần khả năng cứu bo khi mất bootloader, chuẩn bị phiên backup SWD
  riêng để đọc toàn flash/UICR nếu được phép đọc; đây là hạng mục khôi phục
  nâng cao, không phải yêu cầu sửa mạch hoặc thay bootloader để nạp app.

Audit chỉ đọc USB/sysfs và file local, tra cứu upstream công khai. Chưa gửi
phím thử, mở serial, nạp flash, remount ổ hoặc thay cấu hình firmware.
