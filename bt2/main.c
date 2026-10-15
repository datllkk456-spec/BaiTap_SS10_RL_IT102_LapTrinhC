#include <stdio.h>
#include <string.h>

#define MAX_VE 50

struct VeSuKien {
    int maVe;
    char tenSuKien[50];
    char loaiVe[20];
    float giaGoc;
    int isEarlyBird; // 1: Có Early Bird (giảm 15%), 0: Vé thường
    int daCheckIn;   // 0: Chưa check-in, 1: Đã check-in
};

float tinhGiaThucTe(const struct VeSuKien *ve);
int timVeTheoMa(const struct VeSuKien dsVe[], int soLuong, int maVe);
int phatHanhVe(struct VeSuKien dsVe[], int soLuong, struct VeSuKien veMoi);
int checkInQR(struct VeSuKien dsVe[], int soLuong, int maVe);
int huyVe(struct VeSuKien dsVe[], int soLuong, int maVe);
void hienThiDanhSachVe(const struct VeSuKien dsVe[], int soLuong);

int main() {
    struct VeSuKien dsVe[MAX_VE];
    int soLuongVe = 0;

    printf("=== TICKETBOX - MODULE QUAN LY & SOAT VE CONG TU DONG ===\n\n");

    // 1. Khởi tạo danh sách gồm 3 vé ban đầu
    struct VeSuKien v1 = {1001, "Live Concert VIBE 2025", "VIP", 2000000.0f, 1, 0};
    struct VeSuKien v2 = {1002, "Live Concert VIBE 2025", "GA", 800000.0f, 0, 0};
    struct VeSuKien v3 = {1003, "Live Concert VIBE 2025", "Zone A", 1200000.0f, 1, 0};

    soLuongVe = phatHanhVe(dsVe, soLuongVe, v1);
    soLuongVe = phatHanhVe(dsVe, soLuongVe, v2);
    soLuongVe = phatHanhVe(dsVe, soLuongVe, v3);

    // 2. In danh sách vé hiện có cùng giá thanh toán thực tế
    printf("\n--- DANH SACH VE KHOI TAO BAN DAU ---\n");
    hienThiDanhSachVe(dsVe, soLuongVe);

    // 3. Thử nghiệm phát hành thêm 1 vé mới
    printf("\n--- PHAT HANH VE MOI ---\n");
    struct VeSuKien v4 = {1004, "Live Concert VIBE 2025", "VIP", 2000000.0f, 0, 0};
    soLuongVe = phatHanhVe(dsVe, soLuongVe, v4);

    // 4. Thực hiện quét mã QR check-in thành công 1 vé
    printf("\n--- THAO TAC SOAT VE CONG (CHECK-IN QR) ---\n");
    checkInQR(dsVe, soLuongVe, 1001);

    // 5. Thử nghiệm quét lại chính mã vé vừa check-in (Duplicate QR Scan)
    printf("\n--- THU NGHIEM SCAN TRUNG LAP (DUPLICATE QR) ---\n");
    checkInQR(dsVe, soLuongVe, 1001);

    // 6. Thực hiện hủy 1 vé theo mã
    printf("\n--- THAO TAC HUY VE ---\n");
    soLuongVe = huyVe(dsVe, soLuongVe, 1002);

    // 7. Thử nghiệm quét mã vé không tồn tại
    printf("\n--- THU NGHIEM MA VE KHONG TON TAI ---\n");
    checkInQR(dsVe, soLuongVe, 9999);

    // 8. Hiển thị lại toàn bộ danh sách vé sau các thao tác
    printf("\n=======================================================================================\n");
    printf("                  DANH SACH VE HE THONG SAU KHI XU LY CAN BAN                         \n");
    printf("=======================================================================================\n");
    hienThiDanhSachVe(dsVe, soLuongVe);

    return 0;
}

float tinhGiaThucTe(const struct VeSuKien *ve) {
    if (ve->isEarlyBird == 1) {
        return ve->giaGoc * 0.85f;
    }
    return ve->giaGoc;
}

int timVeTheoMa(const struct VeSuKien dsVe[], int soLuong, int maVe) {
    for (int i = 0; i < soLuong; i++) {
        if (dsVe[i].maVe == maVe) {
            return i;
        }
    }
    return -1;
}

int phatHanhVe(struct VeSuKien dsVe[], int soLuong, struct VeSuKien veMoi) {
    if (soLuong >= MAX_VE) {
        printf("[LOI OVERFLOW]: He thong da dat suc chua toi da (%d ve). Khong the phat hanh thêm!\n", MAX_VE);
        return soLuong;
    }

    dsVe[soLuong] = veMoi;
    printf("[THANH CONG]: Da phat hanh ve ma %d (%s) thanh cong.\n", veMoi.maVe, veMoi.loaiVe);
    return soLuong + 1;
}

int checkInQR(struct VeSuKien dsVe[], int soLuong, int maVe) {
    if (soLuong == 0) {
        printf("[LOI UNDERFLOW]: Danh sach ve hien dang rong!\n");
        return 0;
    }

    int index = timVeTheoMa(dsVe, soLuong, maVe);

    if (index == -1) {
        printf("[LOI MA VE]: Ma ve %d khong ton tai trong he thong!\n", maVe);
        return 0;
    }

    if (dsVe[index].daCheckIn == 1) {
        printf("[CANH BAO GIAN LAN]: Ve %d da duoc check-in truoc do! (Duplicate QR Scan)\n", maVe);
        return 0;
    }

    dsVe[index].daCheckIn = 1;
    printf("[CHECK-IN THANH CONG]: Ve %d (%s) da qua cong kiem soat.\n", maVe, dsVe[index].loaiVe);
    return 1;
}

int huyVe(struct VeSuKien dsVe[], int soLuong, int maVe) {
    if (soLuong == 0) {
        printf("[LOI UNDERFLOW]: Danh sach ve hien dang rong, khong the huy!\n");
        return soLuong;
    }

    int index = timVeTheoMa(dsVe, soLuong, maVe);

    if (index == -1) {
        printf("[LOI MA VE]: Khong tim thay ma ve %d de huy!\n", maVe);
        return soLuong;
    }

    for (int i = index; i < soLuong - 1; i++) {
        dsVe[i] = dsVe[i + 1];
    }

    printf("[THANH CONG]: Da huy thanh cong ve ma %d ra khoi he thong.\n", maVe);
    return soLuong - 1;
}

void hienThiDanhSachVe(const struct VeSuKien dsVe[], int soLuong) {
    if (soLuong == 0) {
        printf("[THONG BAO]: Danh sach ve hien dang rong!\n");
        return;
    }

    printf("%-8s %-25s %-10s %-15s %-15s %-12s\n",
           "MA VE", "SU KIEN", "LOAI VE", "GIA GOC", "THANH TOAN", "TRANG THAI");
    printf("---------------------------------------------------------------------------------------\n");

    for (int i = 0; i < soLuong; i++) {
        float giaThucTe = tinhGiaThucTe(&dsVe[i]);
        printf("%-8d %-25s %-10s %-15.0f %-15.0f %-12s\n",
               dsVe[i].maVe,
               dsVe[i].tenSuKien,
               dsVe[i].loaiVe,
               dsVe[i].giaGoc,
               giaThucTe,
               dsVe[i].daCheckIn ? "Da Check-in" : "Chua Check-in");
    }
}