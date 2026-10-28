# Curve E Firmware — sản phẩm độc lập

Phạm vi hiện hành chỉ gồm **cáp USB và BLE**. Hướng USB dongle/ESB 2,4 GHz
đã dừng theo quyết định người dùng; các mục ESB phía dưới chỉ là lịch sử,
không phải việc cần tiếp tục. Không thay đổi firmware đang cài.

Trạng thái: đã được duyệt; đã triển khai tại local. Người dùng đã chọn sản
phẩm riêng, ZMK là dependency, yêu cầu dừng kiểm tra repo gốc và báo đã hoàn
thành thao tác detach repository trên GitHub. Không cần thay remote hoặc kiểm
tra lại GitHub để thực hiện phần local.

## Phạm vi

Lấy trạng thái local hiện tại làm điểm tách. Tên hiển thị đề xuất là **Curve E
Firmware**; không đổi tên repository, Git history, target `curve_e//zmk`, USB/BLE
identity, GPIO, flash partition, keymap hoặc firmware đang chạy.

Độc lập nghĩa là có nguồn duy trì, quy trình build/phát hành và tài liệu của
chính dự án. Không đồng nghĩa đã xác nhận mọi chức năng trên phần cứng: BLE
ổn định vẫn là yêu cầu nghiệm thu chưa đo đầy đủ; ESB còn là prototype cách ly.
Không mô tả repo là sản phẩm đã hoàn thiện chỉ vì build đạt.

## Phương án đã chọn

Giữ nguồn board/module của dự án, dùng ZMK/Zephyr cùng dependency được khóa
revision. Không đưa toàn bộ source ZMK vào Git của sản phẩm; không cần fork
lại ZMK để đứng độc lập. Snapshot vendor vẫn là provenance/tra cứu, không là
nguồn build ngầm. Người dùng đã chọn hướng này thay phương án tự duy trì toàn
bộ fork ZMK.

## Cấu trúc đích

```text
README.md                  cửa vào sản phẩm, tính năng và mức kiểm chứng
product.json               tên sản phẩm, target, bản chính, lock và image build
LICENSE                    giấy phép phần mềm do dự án duy trì
NOTICE.md                  attribution và ngoại lệ giấy phép theo nguồn
CHANGELOG.md               thay đổi do dự án thực hiện, có giới hạn kiểm chứng
CONTRIBUTING.md            cách build, kiểm chứng, cập nhật tài liệu và phát hành
boards/curve_e/            board source chuẩn của module ZMK
config/                    cấu hình/keymap người dùng và manifest dependency khóa
zephyr/module.yml          đầu vào module ZMK chuẩn
experimental/esb/          ESB cách ly; không bật vào bản BLE chính
tools/                    các lệnh build được hỗ trợ
archive/legacy-tools/             probe lịch sử, có cảnh báo; không chạy tự động
guides/                   hướng dẫn sử dụng, build và recovery
docs/                     thiết kế, trạng thái, bằng chứng và nguồn dẫn
firmware/releases/        release bất biến, checksum/manifest
firmware/verification/    kết quả rebuild/kiểm chứng bản chính
firmware/experimental/    artifact ESB, binary có khóa nằm trong private ignored
hardware/                 ảnh, nhận diện mạch và snapshot upstream
build/                    cache/workspace sinh ra, ignored
```

Giữ bốn cửa vào guides/docs/firmware/hardware mà người dùng đã yêu cầu trước
đây. Không gom phần cứng vào phần mềm hoặc di chuyển board source chỉ để đổi
hình thức: đường dẫn chuẩn của ZMK được giữ để giảm thay đổi không cần thiết.

## Thay đổi cụ thể

1. Viết lại README thành trang sản phẩm độc lập, attribution rõ ràng; chỉ ra
   bản BLE chính, prototype ESB, thiết bị thực tế và những điều chưa kiểm thử.
   Bổ sung LICENSE/NOTICE cho phần mềm phù hợp MIT hiện có; giữ nguyên copyright
   và notice gốc. Không cấp phép lại CAD/ảnh/PDF/bootloader hoặc toàn bộ snapshot
   chỉ vì đặt LICENSE ở gốc. Những nguồn chưa rõ quyền vẫn giữ ngoại lệ.
2. Thêm `product.json` làm danh mục hiện hành: tên hiển thị, board target,
   manifest dependency, Docker image digest và đường dẫn release baseline.
   Công cụ đọc danh mục này thay các chuỗi release/commit/image rải trong code.
   Trường trạng thái phân biệt firmware chính và ESB thử nghiệm; không gán
   version 1.0 hay nhãn stable khi chưa đủ bằng chứng.
3. Khóa toàn bộ dependency từ frozen manifest đã build thành manifest hiện
   hành trong `config/`, không để Zephyr đi theo nhánh động. Dùng một manifest
   khóa làm nguồn cho build và dependency validation; giữ manifest của release
   cũ nguyên vẹn. Giữ tương thích `west`, board target và module metadata.
4. Thêm lệnh build bản chính tại `tools/build-firmware.sh`, tiếp tục giữ lệnh
   ESB riêng `tools/build-esb.sh`. Hai đường đều dùng manifest/image đã pin,
   lưu artifact/log/config/checksum dưới repo và không ghi đè release. Mặc định
   dùng cache/offline; phần chuẩn bị máy mới được mô tả rõ, không ngầm tải mạng,
   flash, reset hay truy cập serial. Công cụ ESB không còn phụ thuộc tên một
   release cụ thể trong source code.
5. Pin reusable workflow ZMK về đúng revision đang dùng thay `@main`; bản chính
   là build mặc định. Không tự đưa ESB có khóa riêng lên CI/artifact công khai.
   CI mới chỉ được mô tả là cấu hình đã sửa, không nhận là đã chạy trên GitHub.
6. Chuyển hai probe serial/input cũ vào `archive/legacy-tools/` và ghi rõ phạm vi lịch
   sử, cổng hard-code và khuyến nghị quyền truy cập không còn phù hợp. Không chạy
   hoặc mở rộng các probe này trong tái cấu trúc. Bổ sung CONTRIBUTING/CHANGELOG
   và cập nhật liên kết/document song song; giữ evidence và manifest lịch sử.

## Kiểm chứng bắt buộc

- Giữ nguyên mọi thay đổi local đã có; không clean/reset/commit/push.
- Kiểm tra cú pháp công cụ/config, JSON/YAML, checksum và link/anchor local.
- Build offline bản chính sau tái cấu trúc; UF2 phải byte-identical release
  đang dùng. Nếu khác phải tìm nguyên nhân trước khi nhận thay đổi là chỉ tổ chức.
- Build lại ESB keyboard và receiver DK, bảo đảm Kconfig vẫn cấm BLE và không
  đưa private key/binary vào Git. Bản thử không được tự nạp hoặc đổi mặc định BLE.
- Kiểm manifest/nguồn sạch trước build; log và artifact lưu riêng có provenance.
- Không thêm test duy trì theo chỉ dẫn repo. Dùng checks/probe cần thiết và ghi
  rõ build/source verification khác kiểm thử radio/BLE trên phần cứng.

## Ranh giới và hoàn tất

Không đổi bootloader, không sửa mạch, không nạp firmware, không đổi bond/keymap,
không đổi remote/rename repo và không kiểm tra repo gốc thêm. Detach GitHub do
người dùng thực hiện; phần local không cần gọi GitHub API. Không xóa attribution
hoặc cố biến nguồn kế thừa thành tác phẩm hoàn toàn mới.

Hoàn tất đợt này khi cấu trúc và công cụ mới dùng được tại local, tài liệu nhất
quán, rebuild BLE giữ nguyên binary và ESB tiếp tục cách ly. Việc chứng minh
BLE stable, có receiver thật và nghiệm thu ba mode vẫn là công việc riêng.

Bổ sung theo người dùng: mọi snapshot nguồn cũ và công cụ tham chiếu được
đưa vào `archive/`; hardware hiện hành giữ ảnh/nhận diện mạch. Giữ nguyên byte
nguồn và manifest lịch sử, cung cấp ánh xạ đường dẫn cũ sang mới.

Kết quả: [bằng chứng tái cấu trúc](evidence/product-independence-2026-10-28.json).
Build chính byte-identical; receiver/keyboard ESB build đạt. Review độc lập
chưa hoàn tất do giới hạn phiên agent; đã tự kiểm cú pháp, đường dẫn, checksum
và ranh giới build. Không nhận có review độc lập hoàn chỉnh hoặc CI GitHub đạt.
