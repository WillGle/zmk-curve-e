# Tài liệu kỹ thuật Curve E

[Trang sản phẩm](../README.md) · [Hướng dẫn thao tác](../guides/README.md)

| Tài liệu | Nội dung |
| --- | --- |
| [Trạng thái board và việc cần làm](update-checklist.md) | Kiểm tra local mới nhất, mức sẵn sàng nâng cấp và checklist nạp/thử |
| [Kế hoạch triển khai ESB](esb-implementation-plan.md) | Các bước phần mềm đã làm và phần kiểm thử phần cứng còn lại |
| [Phát triển ESB độc lập](esb-development.md) | Prototype cách ly BLE, audit fork, receiver DK và bước thử phần cứng |
| [Các chế độ kết nối](connection-modes.md) | USB/BLE, dongle BLE chính thức và ESB trong fork vendor |
| [BLE ưu tiên tương thích](bluetooth.md) | Cấu hình không nhập passkey, đối chiếu upstream, bằng chứng và bài thử BLE còn thiếu |
| [Firmware](firmware.md) | Phiên bản ZMK, cấu trúc module và giới hạn tính năng PCB |
| [Phần cứng](hardware.md) | GPIO, ma trận, encoder, cầu chia áp và USB từ firmware/khảo sát |
| [Kiểm chứng](verification.md) | Kết quả build, checksum, preflight và phần chưa kiểm thử runtime |
| [Khôi phục](recovery.md) | Phân tích bootloader, sơ đồ bộ nhớ, giới hạn backup và SWD |
| [Trạng thái tự chủ](status.md) | Đã có gì, thiếu gì để độc lập firmware/phần cứng với vendor |
| [Nguồn tham khảo](references/README.md) | URL ngoài repo, commit, giấy phép và phạm vi đã khảo sát |
| [Bằng chứng kiểm chứng](evidence/README.md) | Bản ghi bootloader, chuỗi firmware và kết quả preflight |

File để nạp ở [firmware/](../firmware/README.md); ảnh, nhận diện linh kiện và
nguồn tham khảo phần cứng ở [hardware/](../hardware/README.md).

Khi cập nhật, ghi kết luận theo đúng chủ đề; nguồn dẫn ngoài repo gom vào
`references/`. Giữ nguyên bằng chứng và manifest lịch sử. Bản build mới lưu
riêng dưới `firmware/releases/`, đồng thời cập nhật danh mục bản hiện hành và
kết quả kiểm chứng. Hướng dẫn thao tác được duy trì tại `guides/`.
