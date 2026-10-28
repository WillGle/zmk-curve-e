# Tự build firmware Curve E

[Hướng dẫn](README.md) · [Tài liệu firmware](../docs/firmware.md)

## Build

GitHub Actions dùng [build.yaml](../build.yaml), target `curve_e//zmk` và
snippet Studio. Workflow ZMK nhận diện `zephyr/module.yml` để nạp module repo.
Chưa push thay đổi đồng nghĩa CI trên GitHub chưa kiểm tra cấu hình local này.

Build từ thư mục gốc repo. Workspace cố định là `build/curve-e/`:

```sh
mkdir -p build/curve-e/config build/curve-e/module
cp -R config/. build/curve-e/config/
cp -R boards zephyr build/curve-e/module/
docker run --rm --user "$(id -u):$(id -g)" \
  -e HOME=/tmp/curve-home \
  -v "$PWD/build/curve-e:/work" -w /work \
  zmkfirmware/zmk-build-arm:stable sh -c '
    mkdir -p "$HOME" &&
    { test -d .west || west init -l config; } &&
    west update --narrow --fetch-opt=--depth=1 &&
    west zephyr-export &&
    west build -p always -s zmk/app -d build -b curve_e//zmk \
      -S studio-rpc-usb-uart -- \
      -DZMK_CONFIG=/work/config -DZMK_EXTRA_MODULES=/work/module &&
    west manifest --freeze --active-only > /work/west-frozen.yml
  ' > build/curve-e/build-final.log 2>&1
```

Lệnh cần Docker và quyền dùng Docker. `HOME=/tmp/curve-home` chỉ là home nội bộ
container; dependency, source snapshot, output và log đều lưu dưới repo.
Nếu xóa/đổi tên tệp nguồn, xóa snapshot `build/curve-e/config` và
`build/curve-e/module` trước khi copy lại để tránh giữ tệp cũ trong workspace.
Chỉ dọn hai snapshot này, không xóa nguồn `config/`, `boards/`, `zephyr/`.

Đầu ra: `build/curve-e/build/zephyr/zmk.uf2`. Sau khi build đạt, lưu vào một
thư mục phiên bản mới dưới `firmware/releases/` cùng `build-final.log`, `.config`,
`zephyr.dts`, `west-frozen.yml` và manifest/hash tương ứng. Không ghi đè
archive cũ bằng firmware từ nguồn khác. Bản hiện hành được liệt kê tại [danh mục firmware](../firmware/README.md).
Không nạp hoặc thay bootloader tự động.
