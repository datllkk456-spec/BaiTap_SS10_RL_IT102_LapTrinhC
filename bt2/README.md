tinhGiaThucTe(const struct VeSuKien *ve)

Input: Con trỏ hằng trỏ tới một bản ghi vé (const struct VeSuKien*).

Output: Giá vé thực tế sau chiết khấu (float).

timVeTheoMa(const struct VeSuKien dsVe[], int soLuong, int maVe)

Input: Mảng struct vé, số lượng vé hiện tại (soLuong), mã vé cần tìm (maVe).

Output: Chỉ số index của vé trong mảng (0 đến soLuong - 1), hoặc -1 nếu không tìm thấy.

phatHanhVe(struct VeSuKien dsVe[], int soLuong, struct VeSuKien veMoi)

Input: Mảng struct vé, số lượng vé hiện tại, bản ghi vé mới.

Output: Số lượng vé mới sau khi thêm (int).

checkInQR(struct VeSuKien dsVe[], int soLuong, int maVe)

Input: Mảng struct vé, số lượng vé hiện tại, mã vé cần check-in.

Output: Trạng thái thao tác (1: Thành công, 0: Thất bại).

huyVe(struct VeSuKien dsVe[], int soLuong, int maVe)

Input: Mảng struct vé, số lượng vé hiện tại, mã vé cần hủy.

Output: Số lượng vé mới sau khi dồn mảng xóa (int).

hienThiDanhSachVe(const struct VeSuKien dsVe[], int soLuong)

Input: Mảng struct vé, số lượng vé hiện tại.

Output: In bảng danh sách toàn bộ vé ra terminal.

Sơ Đồ Xử Lý Logic

Luồng Check-in QR:

Kiểm tra soLuong == 0: Nếu đúng, báo lỗi "Danh sách rỗng" và kết thúc (trả về 0).

Gọi timVeTheoMa(dsVe, soLuong, maVe) lấy index.

Nếu index == -1: Báo lỗi "Mã vé không tồn tại trong hệ thống" và kết thúc (trả về 0).

Nếu dsVe[index].daCheckIn == 1: Báo lỗi cảnh báo gian lận "Vé đã check-in trước đó (Duplicate QR Scan)" và kết thúc (trả về 0).

Nếu dsVe[index].daCheckIn == 0: Cập nhật dsVe[index].daCheckIn = 1, in thông báo check-in thành công và trả về 1.

Luồng Hủy Vé:

Kiểm tra soLuong == 0: Nếu đúng, báo lỗi "Danh sách rỗng" và trả về soLuong.

Gọi timVeTheoMa(dsVe, soLuong, maVe) lấy index.

Nếu index == -1: Báo lỗi "Không tìm thấy mã vé cần hủy" và trả về soLuong.

Nếu tìm thấy: Thực hiện dồn mảng từ index đến soLuong - 2 bằng câu lệnh dsVe[i] = dsVe[i + 1].

Giảm số lượng soLuong - 1, in thông báo hủy thành công và trả về soLuong mới để đồng bộ ngoài main().