// Đầu vào (Input):

// Danh sách khu vực vé TicketZone (mã, tên, giá gốc, sức chứa, số vé đã bán).

// Danh sách mã QR QrCheckIn (mã QR, mã khu vực, trạng thái đã dùng).

// Dữ liệu giao dịch bán vé: zone_id, quantity (1-4 vé), is_early_bird (0 hoặc 1).

// Dữ liệu check-in: qr_code đầu vào.

// Đầu ra (Output):

// Tổng tiền cần thanh toán cho giao dịch bán vé.

// Trạng thái cập nhật sold_tickets trong mảng khu vực.

// Trạng thái check-in thành công / thất bại (chuyển is_used từ 0 sang 1).

// Bảng báo cáo danh sách khu vực vé và nhật ký soát vé tại cổng.

#include <stdio.h>
#include <string.h>

#define MAX_ZONES 10
#define MAX_QRS 50

struct TicketZone {
    int zone_id;
    char zone_name[30];
    float ticket_price;
    int total_capacity;
    int sold_tickets;
};

struct QrCheckIn {
    char qr_code[20];
    int zone_id;
    int is_used; // 0: Chưa dùng, 1: Đã dùng
};

int timKhuVucTheoId(const struct TicketZone danhSachZone[], int soLuongZone, int zone_id);
int timQrTheoMa(const struct QrCheckIn danhSachQr[], int soLuongQr, const char qr_code[]);
float tinhTienVe(float giaGoc, int soLuong, int is_early_bird);
int datVeKhuVuc(struct TicketZone danhSachZone[], int soLuongZone, int zone_id, int soLuong, int is_early_bird, float *tongTienOut);
int checkInQuatQR(struct QrCheckIn danhSachQr[], int soLuongQr, const char qr_code[]);
void hienThiDanhSachKhuVuc(const struct TicketZone danhSachZone[], int soLuongZone);

int main() {
    struct TicketZone danhSachZone[MAX_ZONES] = {
        {1, "Khu Vuc VIP", 2000000.0f, 10, 8},
        {2, "Khu Vuc Zone A", 1200000.0f, 20, 5},
        {3, "Khu Vuc GA", 800000.0f, 50, 49}
    };
    int soLuongZone = 3;

    struct QrCheckIn danhSachQr[MAX_QRS] = {
        {"QR1001", 1, 0},
        {"QR1002", 1, 1},
        {"QR2001", 2, 0}
    };
    int soLuongQr = 3;

    float tongTien = 0.0f;
    int status = 0;

    printf("=== HE THONG TICKETBOX - MODULE BAN VE & SOAT VE CONG ===\n\n");

    printf("--- DANH SACH KHU VUC VE BAN DAU ---\n");
    hienThiDanhSachKhuVuc(danhSachZone, soLuongZone);

    printf("\n--- KIEM THU 1: DAT VE SO LUONG KHONG HOP LE (quantity = 5) ---\n");
    status = datVeKhuVuc(danhSachZone, soLuongZone, 1, 5, 1, &tongTien);

    printf("\n--- KIEM THU 2: DAT VE THANH CONG VIP (2 ve, Early Bird) ---\n");
    status = datVeKhuVuc(danhSachZone, soLuongZone, 1, 2, 1, &tongTien);
    if (status == 1) {
        printf("[THANH CONG]: Dat 2 ve VIP thanh cong! Tong tien: %.0f VND\n", tongTien);
    }

    printf("\n--- KIEM THU 3: OVERBOOKING KHU VUC GA (Con 1 ve nhung dat 2 ve) ---\n");
    status = datVeKhuVuc(danhSachZone, soLuongZone, 3, 2, 0, &tongTien);

    printf("\n--- KIEM THU 4: MA KHU VUC KHONG TON TAI (zone_id = 99) ---\n");
    status = datVeKhuVuc(danhSachZone, soLuongZone, 99, 1, 0, &tongTien);

    printf("\n--- KIEM THU 5: CHECK-IN QR HOP LE (QR1001) ---\n");
    checkInQuatQR(danhSachQr, soLuongQr, "QR1001");

    printf("\n--- KIEM THU 6: DOUBLE CHECK-IN (Quet lai QR1001) ---\n");
    checkInQuatQR(danhSachQr, soLuongQr, "QR1001");

    printf("\n--- KIEM THU 7: QUET MA QR KHONG TON TAI (QR9999) ---\n");
    checkInQuatQR(danhSachQr, soLuongQr, "QR9999");

    printf("\n=======================================================================\n");
    printf("               DANH SACH KHU VUC SAU KHI XU LY GIAO DICH               \n");
    printf("=======================================================================\n");
    hienThiDanhSachZone(danhSachZone, soLuongZone);

    return 0;
}

int timKhuVucTheoId(const struct TicketZone danhSachZone[], int soLuongZone, int zone_id) {
    for (int i = 0; i < soLuongZone; i++) {
        if (danhSachZone[i].zone_id == zone_id) {
            return i;
        }
    }
    return -1;
}

int timQrTheoMa(const struct QrCheckIn danhSachQr[], int soLuongQr, const char qr_code[]) {
    for (int i = 0; i < soLuongQr; i++) {
        if (strcmp(danhSachQr[i].qr_code, qr_code) == 0) {
            return i;
        }
    }
    return -1;
}

float tinhTienVe(float giaGoc, int soLuong, int is_early_bird) {
    float donGia = giaGoc;
    if (is_early_bird == 1) {
        donGia = giaGoc * 0.85f;
    }
    return donGia * soLuong;
}

int datVeKhuVuc(struct TicketZone danhSachZone[], int soLuongZone, int zone_id, int soLuong, int is_early_bird, float *tongTienOut) {
    if (soLuong < 1 || soLuong > 4) {
        printf("[LOI CHI TIEU]: So luong ve mua cho moi giao dich phai tu 1 den 4 ve!\n");
        return 0;
    }

    int index = timKhuVucTheoId(danhSachZone, soLuongZone, zone_id);
    if (index == -1) {
        printf("[LOI MA KHU VUC]: Ma khu vuc %d khong ton tai trong he thong!\n", zone_id);
        return 0;
    }

    if (danhSachZone[index].sold_tickets + soLuong > danhSachZone[index].total_capacity) {
        printf("[LOI OVERBOOKING]: Khu vuc '%s' khong du ve! (Con lai: %d ve)\n",
               danhSachZone[index].zone_name,
               danhSachZone[index].total_capacity - danhSachZone[index].sold_tickets);
        return 0;
    }

    danhSachZone[index].sold_tickets += soLuong;
    *tongTienOut = tinhTienVe(danhSachZone[index].ticket_price, soLuong, is_early_bird);
    return 1;
}

int checkInQuatQR(struct QrCheckIn danhSachQr[], int soLuongQr, const char qr_code[]) {
    if (soLuongQr == 0) {
        printf("[LOI UNDERFLOW]: Danh sach ma QR kiem soat hien đang rong!\n");
        return 0;
    }

    int index = timQrTheoMa(danhSachQr, soLuongQr, qr_code);
    if (index == -1) {
        printf("[LOI MA QR]: Ma QR '%s' khong ton tai trong he thong!\n", qr_code);
        return 0;
    }

    if (danhSachQr[index].is_used == 1) {
        printf("[CANH BAO GIAN LAN]: Ma QR '%s' duoc quet lai lan 2! (Double Check-in)\n", qr_code);
        return 0;
    }

    danhSachQr[index].is_used = 1;
    printf("[CHECK-IN THANH CONG]: Ma QR '%s' qua cong kiem soat thanh cong.\n", qr_code);
    return 1;
}

void hienThiDanhSachKhuVuc(const struct TicketZone danhSachZone[], int soLuongZone) {
    if (soLuongZone == 0) {
        printf("[THONG BAO]: Danh sach khu vuc hien dang rong!\n");
        return;
    }

    printf("%-8s %-20s %-15s %-12s %-12s %-10s\n",
           "ID ZONE", "TEN KHU VUC", "GIA GOC", "SUC CHUA", "DA BAN", "CON LAI");
    printf("-----------------------------------------------------------------------\n");

    for (int i = 0; i < soLuongZone; i++) {
        int conLai = danhSachZone[i].total_capacity - danhSachZone[i].sold_tickets;
        printf("%-8d %-20s %-15.0f %-12d %-12d %-10d\n",
               danhSachZone[i].zone_id,
               danhSachZone[i].zone_name,
               danhSachZone[i].ticket_price,
               danhSachZone[i].total_capacity,
               danhSachZone[i].sold_tickets,
               conLai);
    }
}

//ID ZONE  TEN KHU VUC          GIA GOC         SUC CHUA     DA BAN       CON LAI   
// -----------------------------------------------------------------------
//1        Khu Vuc VIP          2000000         10           8            2         
//2        Khu Vuc Zone A       1200000         20           5            15        
//3        Khu Vuc GA           800000          50           49           1         

// --- KIEM THU 1: DAT VE SO LUONG KHONG HOP LE (quantity = 5) ---
// [LOI CHI TIEU]: So luong ve mua cho moi giao dich phai tu 1 den 4 ve!

// --- KIEM THU 2: DAT VE THANH CONG VIP (2 ve, Early Bird) ---
// [THANH CONG]: Dat 2 ve VIP thanh cong! Tong tien: 3400000 VND

// --- KIEM THU 3: OVERBOOKING KHU VUC GA (Con 1 ve nhung dat 2 ve) ---
// [LOI OVERBOOKING]: Khu vuc 'Khu Vuc GA' khong du ve! (Con lai: 1 ve)

// --- KIEM THU 4: MA KHU VUC KHONG TON TAI (zone_id = 99) ---
// [LOI MA KHU VUC]: Ma khu vuc 99 khong ton tai trong he thong!

// --- KIEM THU 5: CHECK-IN QR HOP LE (QR1001) ---
// [CHECK-IN THANH CONG]: Ma QR 'QR1001' qua cong kiem soat thanh cong.

// --- KIEM THU 6: DOUBLE CHECK-IN (Quet lai QR1001) ---
// [CANH BAO GIAN LAN]: Ma QR 'QR1001' duoc quet lai lan 2! (Double Check-in)

// --- KIEM THU 7: QUET MA QR KHONG TON TAI (QR9999) ---
// [LOI MA QR]: Ma QR 'QR9999' khong ton tai trong he thong!