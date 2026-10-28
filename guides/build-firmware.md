# Build Curve E Firmware

[Hướng dẫn](README.md) · [Quy trình phát triển](../CONTRIBUTING.md)

## Build offline trên workspace hiện có

Từ gốc repository:

```sh
./tools/build-firmware.sh
```

Lệnh này build USB/BLE chính và kiểm 56 dependency sạch/đúng revision,
dùng image digest trong [product.json](../product.json), không tải mạng, flash
hoặc mở serial. Cần Docker và quyền truy cập Docker.

Manifest hiện hành [config/west.yml](../config/west.yml) đã resolve toàn bộ
project, khóa full commit SHA cho ZMK, Zephyr và dependency, không còn import
nhánh động. Không tự sửa cache để vượt qua kiểm tra nguồn sạch.

Bản chính lưu vào `firmware/verification/<timestamp>-product/`, gồm UF2,
config, DTS, frozen manifest, source/dependency hashes và log. `build.json` ghi
kết quả so sánh với baseline tại `product.json`; khác binary không đồng nghĩa
đã được chấp thuận thay firmware đang dùng. Không ghi đè release tự động.

Hướng ESB đã dừng; artifact lịch sử ESB lưu tại `firmware/experimental/builds/<timestamp>/`; binary có khóa và key
header ở `firmware/experimental/private/`, bị loại khỏi Git. Xem
[module ESB](../experimental/esb/README.md). Workspace/cache sinh ra ở
`build/curve-e/`; artifact đã archive không phụ thuộc việc giữ build output.

## Chuẩn bị máy mới — thao tác có mạng riêng

Cần Python 3, Docker, image đúng digest và dependency đúng manifest. Máy mới
chưa được xác nhận offline cho đến khi chuẩn bị đủ cache. Từ gốc repo:

```sh
mkdir -p build/curve-e/config
cp config/west.yml build/curve-e/config/west.yml
image=$(python3 tools/product.py build.image)
docker pull "$image"
docker run --rm --user "$(id -u):$(id -g)" \
  -e HOME=/tmp/curve-bootstrap-home \
  -v "$PWD/build/curve-e:/work" -w /work "$image" sh -ec '
    mkdir -p "$HOME"
    test -d .west || west init -l config
    west update --narrow --fetch-opt=--depth=1
  '
```

Đây là bước bootstrap có mạng được người vận hành chủ động chạy; không nằm
trong build mặc định. Sau đó chạy build offline ở trên. Nếu cache đang có thay
đổi riêng, giữ nguyên và điều tra trước khi cập nhật; không reset/clean tự động.
Các lệnh bootstrap chưa được thử trên một máy trắng trong đợt tái cấu trúc.

## CI và phát hành

Workflow build chính dùng reusable workflow ZMK được pin cùng revision; chỉ
build target chính trong `build.yaml`. Cấu hình CI local không phải bằng chứng
GitHub Actions đã chạy. Không có workflow công khai chứa private pair key ESB.

Quy trình nghiệm thu/phát hành tại [CONTRIBUTING](../CONTRIBUTING.md).
Nguồn tham chiếu cũ chỉ ở [archive](../archive/README.md), không là dependency build.
