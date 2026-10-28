# Phát triển USB/BLE/ESB độc lập cho Curve E

> **Quyết định hiện hành — 28/10/2026:** dừng phát triển receiver USB dongle
> và ESB 2,4 GHz. Sản phẩm chỉ tiếp tục với **cáp USB và Bluetooth BLE**, BLE
> là wireless mặc định. Nội dung ESB bên dưới giữ làm lịch sử tham chiếu,
> không phải kế hoạch tiếp tục, hướng dẫn mua receiver hoặc firmware để nạp.

Ngày: 28/10/2026. Trạng thái: **thiết kế đã duyệt; đã port/build prototype USB+ESB riêng;
chưa nạp hoặc thử radio**. BLE vẫn là firmware chính, không sửa lifecycle BLE.

Người dùng xác nhận vendor không cung cấp receiver và không định phát triển
thử nghiệm này tiếp; mục tiêu là tự duy trì firmware hai đầu. Người dùng đã
chọn **giữ ZMK mới và port ESB có chọn lọc**. Không đặt mục tiêu tương thích
binary với DENGLE hay phụ thuộc vendor cung cấp thêm tài liệu.

## Nền tảng và phương án

- Đã chọn: upstream ZMK `4dd95d7840eb3d5a3f59bb29fbf1ab054715ef85`,
  port phần cần thiết từ fork `aba1104a9777d851df1c3ea032468d44c42ed3f0`.
  Ưu điểm: giữ đường cập nhật ZMK/Studio; đổi lại phải tích hợp lifecycle BLE,
  endpoint, tài nguyên radio và mã hóa với nền Zephyr hiện tại.
- Không chọn: dùng nguyên fork `v0.3-lts` làm firmware chính. Có thể giúp thử
  code ESB cũ nhanh hơn, nhưng tăng chi phí cập nhật về sau.
- Dongle BLE upstream không giải quyết mục tiêu ba đầu ra USB/BLE/ESB độc lập.

## Audit nguồn gốc — finding làm căn cứ port

Các vị trí bên dưới tương đối với [snapshot nguồn nguyên bản](../archive/upstream/ph-design-zmk-aba1104-esb/source.json).
Đây là kết quả đọc mã; chưa phải lỗi được tái hiện bằng radio thật. Không sửa
trực tiếp snapshot để giữ bằng chứng gốc.

- **Cao — lỗi dừng BLE bị bỏ qua:** `app/src/ble.c:705–725` đặt
  `ble_started=false`, gọi `bt_disable()` nhưng vẫn trả 0 khi thất bại.
  `app/src/endpoints.c:190–202` có thể tiếp tục khởi động ESB dù controller
  chưa dừng. Phải truyền lỗi, giữ trạng thái đúng và phục hồi đầu ra khi đổi
  mode thất bại; không dựa vào reboot watchdog làm đường xử lý bình thường.
- **Cao — receiver gia hạn kết nối trước xác thực:**
  `app/src/2g4_dongle.c`, `process_rx_payload()` đặt `kb_connected` và gia hạn
  timeout trước decrypt/MIC/replay check. Gói không hợp lệ liên tiếp cùng
  kênh/địa chỉ có thể ngăn release-all sau mất kết nối. Chỉ cập nhật trạng thái
  sống sau xác thực và kiểm tra đúng độ dài loại bản tin.
- **Cao — định dạng gói phụ thuộc kích thước struct HID:** receiver dùng
  `MIN(dec_len - 1, sizeof(report->body))` rồi copy lên trạng thái cũ, không
  yêu cầu đủ chiều dài. Phải kiểm tra phiên bản giao thức và chiều dài chính
  xác; khi mất link phải xóa keyboard/consumer/mouse. Hai đầu phải thống nhất
  NKRO/consumer/mouse và serialization, không mặc định mọi cấu hình tương thích.
- **Cao — quản lý khóa/session chưa đủ để dùng thực tế:** fork có AES-CCM,
  MIC 4 byte, counter và cửa sổ replay 64 gói. Tuy nhiên lỗi lưu `tx_epoch`
  chỉ được log rồi vẫn cho phát; replay state phía nhận ở RAM mất khi reboot.
  Phải thiết kế vòng đời session chống tái sử dụng nonce và replay qua reset;
  tạm dừng phát khi không thể lưu trạng thái cần thiết. Không dùng khóa mẫu
  công khai của vendor cho bản sử dụng thật. Chưa audit mật mã đầy đủ.
- **Trung bình — driver gắn chặt kiến trúc cũ:** dùng trực tiếp `NRF_RADIO`,
  `NRF_CCM`, TIMER2, PPI, IRQ động và sửa VTOR với bảng vector 64 entry.
  Phải kiểm tra quyền sở hữu tài nguyên và phục hồi IRQ/vector trên Zephyr
  mới. Đây là chuyển quyền radio tuần tự; chưa thấy cơ chế chạy BLE/ESB đồng
  thời bằng timeslot trong các file khảo sát.
- **Trung bình — giới hạn payload:** mặc định ESB 32 byte, overhead mã hóa
  12 byte nên chỉ còn 20 byte plaintext, gồm cả loại bản tin. CCM giới hạn
  plaintext 23 byte. Phải kiểm tra mọi report lúc build và reject quá cỡ;
  không mặc định bật extended NKRO hoặc đổi report vẫn chạy được.
- **Trung bình — khai báo giao thức rộng hơn implementation:** có mã loại
  Studio RPC/system report nhưng switch receiver đã đọc chỉ xử lý
  keyboard/consumer/mouse/boot/keepalive. Chưa có căn cứ hứa Studio qua ESB.
  Đích dài hạn giữ Studio qua USB; prototype cách ly hiện tắt Studio để bảo vệ keymap BLE. Pairing của fork qua radio chưa được xác minh.

## Receiver đề xuất

| Phần cứng | Vai trò đề xuất | Giới hạn |
| --- | --- | --- |
| Nordic nRF52840 DK, PCA10056 | Mẫu phát triển đầu tiên | Lớn và đắt hơn dongle; có debugger tích hợp để xử lý radio/IRQ |
| Nordic nRF52840 Dongle, PCA10059 | Receiver nhỏ sau khi prototype đạt | Cần quy trình DFU đúng bootloader; không coi là UF2 Curve E |
| Board nRF52840 USB khác | Có thể port sau | Phải biết model, schematic, bootloader, clock và flash layout |

Đây là lựa chọn kỹ thuật đề xuất, chưa phải phần cứng người dùng đang có.
Nền local đã có board target `nrf52840dk/nrf52840/zmk` và
`nrf52840dongle/nrf52840/zmk`; receiver DK đã build ESB đạt, bản dongle nhỏ chưa port/build.
Không cần pin hay ma trận phím cho receiver. DK cần cổng USB của nRF52840
để xuất HID; cổng debugger là chức năng riêng. Không yêu cầu sửa PCB Curve E.
Chưa mua phần cứng hoặc cam kết giá/tồn kho. Chưa cần receiver thật để port/build;
phải có phần cứng để kiểm chứng radio và USB HID.

## Thiết kế đích đã duyệt — live switching hoãn để cách ly BLE

Giữ cấu hình và bản release USB/BLE đang dùng. Tạo cấu hình ESB thử nghiệm
riêng cho keyboard và receiver, không bật mặc định trong firmware chính.
Nguồn thử nghiệm đặt trong module riêng của repo; giữ driver/protocol tách
khỏi board GPIO, chỉ dùng patch upstream tối thiểu cho endpoint và BLE lifecycle.
Khóa chính xác upstream/Zephyr/fork và lưu patch để có thể rebuild, không sửa
checkout cache bằng tay làm nguồn duy nhất.

Chuyển mode theo thứ tự: gửi release-all ở đầu ra cũ, ngừng công việc đang
chờ, dừng controller cũ và kiểm tra kết quả, nhả IRQ/PPI/timer/CCM, khởi động
transport mới rồi mới công bố trạng thái thành công. Nếu thất bại, trở lại
đầu ra cũ khi có thể hoặc USB; không báo thành công giả. USB HID/Studio phải
vẫn dùng được khi ESB không có receiver. BLE và ESB không phát đồng thời.

Hai đầu dùng giao thức có version, loại bản tin và chiều dài cố định được
kiểm tra; dùng codec rõ ràng thay vì dựa vào padding/ABI của struct. Receiver
chỉ chuyển report hợp lệ sang USB. Mất link phải release-all trong thời hạn
đã định; khởi điểm tham khảo 500 ms từ fork, cần đo trên máy thật.

Đề xuất bản đầu provision khóa riêng cho từng cặp qua quy trình local có chủ
đích, không quảng bá tự pair; không commit khóa vào Git hay log. Key management,
session freshness qua reset và lưu settings phải được chốt trong implementation
trước phát hành. Không tự thiết kế primitive mật mã mới. Ở đích ba mode Studio chỉ qua USB; prototype cách ly tắt Studio; không hứa receiver vendor sẽ dùng được với giao thức đã sửa.

Build trong `build/` của repo; đầu ra kiểm chứng thử nghiệm lưu riêng dưới
`firmware/experimental/`, không ghi đè `firmware/releases/`. Mỗi bộ gồm artifact
keyboard/receiver, manifest revision, config, log và checksum; không lưu key
trong artifact metadata công khai. Bản firmware có chứa key cũng cần giữ riêng,
không tự đưa vào Git. Không nạp bootloader, xóa bond hay settings khi chưa có
lý do và phạm vi cụ thể.

## Điều kiện nghiệm thu phần cứng và đích ba mode

1. Thiết kế đã duyệt; [kế hoạch](esb-implementation-plan.md) và [module thử](../experimental/esb/README.md) đã có. Prototype USB+ESB được cách ly theo yêu cầu BLE sau duyệt.
2. Port module/patch trên upstream cố định; xử lý các finding ở trên, build
   keyboard và receiver. Kiểm tra kích thước, flash range, symbols/config và
   hồi quy build USB/BLE. Không thêm bộ test duy trì nếu người dùng chưa yêu cầu;
   dùng checks/probe hiện có và kiểm chứng tạm có ghi kết quả.
3. Chọn/xác nhận board receiver thực tế và đường recover. Nạp receiver trước,
   kiểm tra USB HID, rồi mới thử keyboard với backup/preflight như các lần trước.
4. Thử nhấn/thả, giữ nhiều phím, consumer/mouse, mất receiver khi giữ phím,
   mất sóng, reset cả hai thứ tự, sai key/gói/replay, đổi mode nhiều lần,
   sleep/wake và reconnect. Đo thay vì hứa zero-lag/không hao pin.
5. Chỉ đưa vào release chính khi các kết quả USB/BLE/ESB đạt và có ghi bằng
   chứng. Có thể build trước khi mua receiver; build pass không phải radio pass.

Người dùng chưa có receiver để kiểm chứng. Chưa nạp firmware, không thay keymap/config đang dùng, không commit/push.

Bằng chứng: [audit nguồn](evidence/esb-source-audit-2026-10-28.json). Đã kiểm
hash 18 file nguồn/giấy phép và 3 probe tĩnh về đường dừng BLE, timeout trước
xác thực và nhận report ngắn. Kiểm tra thiết kế độc lập xác nhận các finding
có căn cứ trong snapshot; chưa xác nhận hành vi phần cứng.

## Ràng buộc bổ sung sau duyệt

Người dùng yêu cầu thử ESB không tác động BLE, BLE là wireless mặc định và
bắt buộc ổn định. Ruling: bước đầu build USB+ESB riêng, Kconfig cấm BLE trong
bản thử nghiệm; không patch `ble.c`/lifecycle hay đổi release BLE hiện tại.
Giữ USB/BLE làm firmware chính và build hồi quy riêng. Live switching ba mode
được hoãn tới khi có receiver và đủ bằng chứng BLE, không coi bản USB+ESB là
firmware ba mode hoàn chỉnh. Quyết định này thay yêu cầu ghép BLE vào bản thử
nghiệm ngay ở các bước trên; mục tiêu dài hạn vẫn là ba mode.

## Kết quả triển khai cách ly

Nguồn ở [module ESB](../experimental/esb/README.md), build bằng
[script offline](../tools/build-esb.sh). Codec/session mới không tương thích
DENGLE cũ. Đã xử lý xác thực trước liveness, exact-length report, khóa riêng,
session mới sau reset, ACK xác thực, mailbox chống mất release khi queue đầy;
prototype cấm BLE nên không dùng đường stop/start và VTOR cũ.

Ba build đạt: receiver DK, keyboard USB+ESB và baseline USB/BLE. Baseline
UF2 trùng từng byte bản hiện hành; đó là kiểm chứng không đổi binary BLE,
không phải xác nhận độ ổn định BLE thực tế. Không có radio scan/pair/flash trong
đợt này. Có 97 assertion tạm chạy với ASan/UBSan (63 codec/session, 26 keyboard,
8 receiver), phần transport dùng queue/hardware giả lập, không thay thử trên bo.

Bằng chứng hiện hành: [prototype ESB](evidence/esb-prototype-2026-10-28.json).
Artifact/log/config/checksum được gom tại [firmware thử nghiệm](../firmware/experimental/README.md).
