# Curve E — kiểm toán bootloader và khôi phục, 28/10/2026

## Kết luận

UF2 ZMK đã lưu tại `firmware/releases/2026-10-28-zmk-4dd95d7` phù hợp với bộ lọc UF2 của **mã nguồn nền** bootloader được thiết bị báo cáo. Khác family ID của `CURRENT.UF2` không tự nó là lỗi tương thích: nguồn nền nhận cả family riêng của bo và family nRF52840 chung. Không cần nạp thử để xác minh kết luận tĩnh này. Chưa thể khẳng định chạy đúng trên bo hoặc bootloader nhị phân có cùng hành vi: phiên bản thiết bị có hậu tố `dirty`, thay đổi riêng chưa được cung cấp.

Bản sao `CURRENT.UF2` là phương án khôi phục **một phần flash qua bootloader còn hoạt động**; chưa phải bản sao phục hồi toàn bộ thiết bị. Chưa có phương án SWD đã kiểm chứng hoặc bootloader riêng của bo được sao lưu. Audit này chỉ đọc dữ liệu đã lưu và nguồn công khai; không kết nối debugger, đổi mount, nạp, xóa hoặc ghi thiết bị.

## Bằng chứng và phạm vi

- [Snapshot bootloader](evidence/bootloader-2026-10-28.json): `UF2 Bootloader 0.10.0-10-g8023c1e-dirty`, `CURVE E`, `dekb-curves-e`, S140 6.1.1; người dùng đã vào bootloader bằng reset và USB đã trở lại `CURVE E BLE` sau rút/cắm.
- [Kiểm tra UF2](evidence/uf2-preflight-2026-10-28.json): commit nền `8023c1e6a0a8014fce0fa1775a67ca54d4ad7454`, 930 block ứng dụng hợp lệ theo parser nền. Trạng thái “family mismatch unresolved” trong snapshot đầu là nhận định lịch sử đã được kiểm tra nguồn sau đó giải quyết; không dùng nó làm kết luận hiện hành.
- Dump cục bộ, không đưa vào Git: `firmware/recovery-2026-10-28/CURRENT.UF2`, 1,908,736 byte / 3,728 block, SHA-256 `7591f3f7cbaea6604c220be42f3c3b219750adbf6c487793b87c60b5e63ae89b`.
- Artifact được đánh giá: `firmware/releases/2026-10-28-zmk-4dd95d7/zmk.uf2`, SHA-256 `179e8b2b7df242e11339ac4fb8e63d5b343d9406ed4ea65e866e5cbd87e9583c`. Không suy rộng kết quả sang artifact mới nếu chưa kiểm tra lại.

## Phát hiện, xếp theo tác động

1. **Cao — chưa có bản khôi phục độc lập với bootloader đang chạy.** Dump chỉ chứa `[0x1000,0xEA000)`; thiếu MBR `[0,0x1000)`, vùng từ `0xEA000` đến cuối flash, UICR và trạng thái debug. Vì vậy thiếu cả bootloader, MBR parameter page, bootloader settings và vùng settings/storage cuối flash. Không thể dùng riêng dump này để cứu bo sau khi bootloader/MBR/UICR bị hỏng hoặc bị mass erase. Cần sao lưu flash đầy đủ và UICR bằng SWD nếu được phép đọc, hoặc có bộ bootloader+SoftDevice+MBR+UICR đúng bo được xác minh trước. Chưa có bằng chứng các điều kiện này đã đạt.
2. **Trung bình — cấu hình capacity của artifact cũ vượt giới hạn UF2 nền 8 KiB.** Code partition cũ `[0x26000,0xEC000)` trong khi `USER_FLASH_END=0xEA000`; build lớn về sau có thể liên kết thành công nhưng các block từ `0xEA000` bị bootloader nền từ chối. Artifact đang xét kết thúc ở `0x60200`, còn xa giới hạn, nên không gặp lỗi này. Bounded remediation: kết thúc code partition tại `0xEA000`, kiểm tra artifact mới sau build; không cần đổi bootloader. Đây là phát hiện đối với archive cũ, không kết luận source hiện tại chưa được sửa trong phiên làm việc khác.
3. **Trung bình — UF2 ứng dụng không phải cập nhật atomic hoặc rollback tự động.** `write_block()` ghi từng block trực tiếp vào vùng ứng dụng/SoftDevice; `flash_nrf5x_flush()` erase/write cả trang khi nội dung thay đổi, trước khi nhận toàn bộ file. Mất điện hoặc rút cáp có thể để lại ứng dụng dở dang. Bộ lọc family/address không phải kiểm tra chữ ký, phần cứng hay chức năng. Bảo toàn bootloader và khả năng reset vật lý giúp nạp lại, nhưng chưa phải bảo đảm cứu được mọi trường hợp.
4. **Trung bình — SWD đọc được hay không vẫn chưa biết.** `CONFIG_NRF_APPROTECT_USE_UICR=y`, `CONFIG_NRF_APPROTECT_LOCK` không bật, `CONFIG_NRF_ACL_FLASH_REGION_SIZE=0` trong build mới không chứng minh UICR hoặc debug hiện tại đã mở. Chưa đọc FICR/revision, UICR.APPROTECT, CTRL-AP status hoặc toàn bộ flash qua SWD. Nếu access port bị khóa, thao tác `recover` thông dụng là mass erase, không phải sao lưu hay mở khóa giữ nguyên dữ liệu. Không chạy recover để “thử kết nối”.
5. **Giới hạn xác nhận — custom `dirty` chưa truy nguyên.** Commit nền và dump cho bằng chứng tương thích mạnh, nhưng không cung cấp source/binary bootloader riêng, pin LED/reset/USB chính xác hay debug policy của bản đã nạp. Không thay bootloader bằng binary của bo khác chỉ vì cùng nRF52840 hoặc cùng version.

## Đối chiếu UF2, SoftDevice và vùng nhớ

Mọi khoảng dưới đây có điểm cuối loại trừ.

| Hạng mục | Kết quả / ý nghĩa |
| --- | --- |
| Family xuất từ dump | `0xDE1740E1`; nguồn nền tạo từ VID/PID của bo |
| Family ứng dụng mới | `0xADA52840`; được `case CFG_UF2_FAMILY_APP_ID` của nền nhận song song family riêng |
| Payload / căn chỉnh | 256 byte / địa chỉ căn 256; 930 block mới qua kiểm tra magic, cờ family, không NOFLASH, family và range nền |
| SoftDevice hiện tại | magic `0x51B1E5DB` tại `0x3004`; trường size tại `0x3008` là `0x26000`, trùng địa chỉ ứng dụng mới |
| Vector cũ | SP `0x20010938`, reset `0x31B55` |
| Vector mới | SP `0x2000E2C0`, reset `0x2F8C9`; SP nằm trong RAM nRF52840, handler Thumb nằm trong image ứng dụng |
| Ghi ứng dụng mới | `[0x26000,0x60200)`; không chứa block SoftDevice, MBR, UICR hoặc bootloader |
| Dump CURRENT | `[0x1000,0xEA000)`; chứa SoftDevice và app cùng phần flash chưa dùng, không chỉ image đang chạy |
| Giới hạn nền thường | bootloader start `0xF4000`, trừ reserve `10×4096` → user end `0xEA000`; MBR params `0xFE000`, bootloader settings `0xFF000` |
| Layout archive cũ | code `[0x26000,0xEC000)`, storage `[0xEC000,0xF4000)`, boot `[0xF4000,0x100000)`; tên partition không đo được nội dung flash hiện tại |

`CURRENT.UF2` không phải file khôi phục hoàn toàn đáng tin chỉ nhờ checksum: checksum xác minh bản sao file, không chứng minh firmware ban đầu hoạt động đủ phím/BLE hoặc rằng mọi settings đã có trong dump. Nạp lại dump qua bootloader nền sẽ ghi cả SoftDevice từ `0x1000`; nó có phạm vi rộng hơn UF2 ứng dụng mới. Chỉ cân nhắc khi cần quay lại, sau khi kiểm tra nguyên vẹn file và chấp nhận phạm vi ghi. Không có bằng chứng đã thử rollback. Không gán lại family ID để né kiểm tra; hiện không cần thao tác đó.

## SWD: điều kiện khôi phục an toàn

Baseline `bootloader_util_app_start()` thiết lập ACL chặn **write/erase** MBR và bootloader qua cuối settings trước khi nhảy vào ứng dụng; không đặt bit cấm đọc. Nordic SVD mô tả các register ACL `ADDR/SIZE/PERM` là write-once và bit READ tách biệt WRITE. Vì vậy bảo vệ này không tương đương APPROTECT khóa access port, và không thể suy ra SWD bị khóa chỉ vì bootloader có ACL. Ngược lại, đây không phải bằng chứng binary `dirty` không cấu hình ACL khác. Không sửa UICR/ACL/APPROTECT để kiểm tra.

Trước một buổi sao lưu SWD riêng cần:

1. Xác minh chính bo đang dùng, mã module/revision chip, và continuity từ pad `SWD`, `SWC`, `GND` tới SWDIO/SWCLK/GND; ảnh silkscreen chỉ là gợi ý. Xác minh mức VDD/VTref và reset. Không mặc định pad `3.3V` là đầu vào nguồn hay để probe cấp nguồn đồng thời USB/pin.
2. Chuẩn bị probe tương thích nRF52840, điện áp tham chiếu đúng và có truy cập CTRL-AP nếu sau này cần recovery; nhận diện cáp và cách ngắt nguồn pin rõ ràng. Chưa có probe hoặc pinout đã xác minh trong bằng chứng hiện tại.
3. Đọc rõ policy của tool; tắt mọi auto-unlock/auto-recover/auto-program/erase. Chỉ attach và đọc khi đã được phép cho buổi đó. Reset/halt thay đổi runtime, vì vậy không gọi một phiên debug là hoàn toàn không xâm nhập chỉ vì không ghi flash.
4. Nếu đọc được: sao lưu toàn flash `[0,0x100000)` và UICR theo memory map nRF52840; ghi FICR/revision, trạng thái debug, checksum và đọc lặp để đối chiếu. Giữ riêng dữ liệu định danh/bonding; không công khai dump nguyên vẹn. Lưu bản sao ở nơi độc lập với repo/máy đang build. Backup không tự chứng minh đã restore thành công.
5. Nếu bị khóa: dừng, giữ nguyên bo. CTRL-AP ERASEALL / `nrf52_recover` là đường khôi phục mất dữ liệu; cần có bộ nạp lại đúng bo và chấp nhận xóa flash/UICR trước một quyết định riêng. Không thể hứa lấy lại bootloader riêng hoặc Bluetooth bonds từ chip đã khóa bằng thao tác mass erase.

## Nguồn chính xác và giấy phép

Nguồn, revision, phạm vi và giấy phép được gom tại
[tài liệu bootloader/Nordic/OpenOCD](references/official.md#bootloader-nordic-và-openocd).

## Kết quả xử lý tiếp sau audit

Code partition đã được giới hạn tại `0xEA000` và build lại riêng ở
[archive flash-bound](../firmware/releases/2026-10-28-zmk-4dd95d7-flash-bound/build.json).
[Preflight mới](evidence/uf2-preflight-flash-bound-2026-10-28.json) xác nhận 930 block,
range `[0x26000,0x60200)`, vector và hash nguồn. Finding capacity của archive
cũ đã xử lý trong source/artifact mới; các giới hạn backup/SWD/dirty vẫn còn.
Nguồn nền được giữ tại [upstream](../hardware/upstream/README.md); hash một số
file fork khác upstream cùng commit ID, nên manifest nguồn giữ riêng từng bản.

## Phiên nạp ứng dụng sau audit

Đã sao lưu CURRENT/INFO mới và nạp UF2 ứng dụng hiện hành qua bootloader
ngày 28/10/2026; USB mới và Studio RPC hoạt động.
[Xem bằng chứng nạp](evidence/flash-session-2026-10-28.json). Không thay
bootloader/SoftDevice hoặc thử rollback/SWD. Các giới hạn backup và phục hồi
trong audit trên vẫn còn.
