# Khôi phục và kiểm tra bootloader

[Hướng dẫn](README.md) · [Audit khôi phục](../docs/recovery.md)

Đã quan sát vào bootloader bằng thao tác RESET kép và thoát về ứng dụng cũ
sau khi rút/cắm USB. Mới xác nhận nhận diện USB; chưa thử phục hồi sau lỗi nạp.

1. Khi cần vào bootloader, dùng nút RESET vật lý; kiểm tra ổ `CURVE-E` và
   `INFO_UF2.TXT`. Đối chiếu model/Board-ID trước khi chọn firmware.
2. Nếu chỉ đang kiểm tra khả năng vào/thoát, không copy UF2. Rút/cắm USB hoặc
   RESET một lần rồi kiểm tra ứng dụng nhận lại; trên thiết bị khảo sát là
   `CURVE E BLE`, `DE17:40E1`.
3. Nếu cần nạp lại ứng dụng, chọn artifact đã đối chiếu bootloader và làm theo
   [hướng dẫn cài](install-firmware.md). Khả năng trở lại bootloader không tự
   chứng minh firmware mới phù hợp mọi GPIO hay tính năng.
4. Không dùng nguyên `CURRENT.UF2` làm file rollback chỉ vì đã có checksum:
   dump chứa cả SoftDevice và chưa được thử restore. Đọc
   [phạm vi backup](../docs/recovery.md#đối-chiếu-uf2-softdevice-và-vùng-nhớ).
5. Nếu USB bootloader không còn hoạt động, chuyển sang xác minh pad/module và
   probe SWD theo [điều kiện SWD](../docs/recovery.md#swd-điều-kiện-khôi-phục-an-toàn).
   Chưa có pinout/probe được đo xác nhận. `recover`/ERASEALL có thể xóa dữ liệu
   cần cứu; không dùng để thử mở khóa đọc.

Dump cục bộ tại `firmware/recovery-2026-10-28/` được Git bỏ qua. Giữ bản sao ở
nơi độc lập với máy build; hiện chưa có bộ backup đầy đủ flash và UICR.
