#include <stdio.h>
#include <string.h>

#define MAX_ZONE 50

struct TicketZone {
    int zone_id;
    char zone_name[30];
    float ticket_price;
    int total_capacity;
    int sold_tickets;
    int is_early_bird;
};

// 1. Nhóm hàm hạt nhân / tính toán (Đưa lên đầu tiên)
int timZoneTheoId(const struct TicketZone dsZone[], int soLuongZone, int zone_id) {
    for (int i = 0; i < soLuongZone; i++) {
        if (dsZone[i].zone_id == zone_id) {
            return i;
        }
    }
    return -1;
}

float tinhTienThanhToan(float giaGoc, int soLuong, int is_early_bird) {
    float donGia = giaGoc;
    if (is_early_bird == 1) {
        donGia = giaGoc * 0.85f;
    }
    return donGia * soLuong;
}

// 2. Nhóm hàm quản lý dữ liệu & hiển thị
void hienThiDanhSachZone(const struct TicketZone dsZone[], int soLuongZone) {
    if (soLuongZone == 0) {
        printf("\n[THONG BAO]: Danh sach khu vuc ve hien dang rong!\n");
        return;
    }

    printf("\n================================ DANH SACH KHU VUC VE ================================\n");
    printf("%-5s %-8s %-20s %-12s %-10s %-10s %-10s %-12s\n",
           "STT", "ID ZONE", "TEN ZONE", "GIA GOC", "TONG VE", "DA BAN", "CON LAI", "EARLY BIRD");
    printf("--------------------------------------------------------------------------------------\n");

    for (int i = 0; i < soLuongZone; i++) {
        int conLai = dsZone[i].total_capacity - dsZone[i].sold_tickets;
        printf("%-5d %-8d %-20s %-12.0f %-10d %-10d %-10d %-12s\n",
               i + 1,
               dsZone[i].zone_id,
               dsZone[i].zone_name,
               dsZone[i].ticket_price,
               dsZone[i].total_capacity,
               dsZone[i].sold_tickets,
               conLai,
               dsZone[i].is_early_bird ? "Co (-15%)" : "Khong");
    }
    printf("--------------------------------------------------------------------------------------\n");
}

int themZoneMoi(struct TicketZone dsZone[], int soLuongZone, struct TicketZone zoneMoi) {
    if (soLuongZone >= MAX_ZONE) {
        printf("[LOI OVERFLOW]: He thong da dat suc chua toi da (%d zone). Khong the them!\n", MAX_ZONE);
        return soLuongZone;
    }

    if (timZoneTheoId(dsZone, soLuongZone, zoneMoi.zone_id) != -1) {
        printf("[LOI DUPLICATE]: Ma Zone ID %d da ton tai trong he thong!\n", zoneMoi.zone_id);
        return soLuongZone;
    }

    dsZone[soLuongZone] = zoneMoi;
    printf("[THANH CONG]: Da them khu vuc '%s' vao he thong.\n", zoneMoi.zone_name);
    return soLuongZone + 1;
}

int capNhatGiaVeZone(struct TicketZone dsZone[], int soLuongZone, int zone_id, float giaMoi) {
    int index = timZoneTheoId(dsZone, soLuongZone, zone_id);

    if (index == -1) {
        printf("[LOI NOT FOUND]: Khong tim thay khu vuc ve co ID %d!\n", zone_id);
        return 0;
    }

    dsZone[index].ticket_price = giaMoi;
    printf("[THANH CONG]: Da cap nhat gia ve moi cho khu vuc '%s': %.0f VND.\n",
           dsZone[index].zone_name, giaMoi);
    return 1;
}

int xoaZone(struct TicketZone dsZone[], int soLuongZone, int zone_id) {
    if (soLuongZone == 0) {
        printf("[LOI UNDERFLOW]: Danh sach khu vuc hien dang rong!\n");
        return soLuongZone;
    }

    int index = timZoneTheoId(dsZone, soLuongZone, zone_id);

    if (index == -1) {
        printf("[LOI NOT FOUND]: Khong tim thay khu vuc ve co ID %d de xoa!\n", zone_id);
        return soLuongZone;
    }

    if (dsZone[index].sold_tickets > 0) {
        printf("[LOI RANG BUOC]: Zone '%s' da ban duoc %d ve. Khong the xoa!\n",
               dsZone[index].zone_name, dsZone[index].sold_tickets);
        return soLuongZone;
    }

    for (int i = index; i < soLuongZone - 1; i++) {
        dsZone[i] = dsZone[i + 1];
    }

    printf("[THANH CONG]: Da xoa khu vuc ve ID %d ra khoi he thong.\n", zone_id);
    return soLuongZone - 1;
}

int xuLyDatVe(struct TicketZone dsZone[], int soLuongZone, int zone_id, int soLuong) {
    if (soLuong < 1 || soLuong > 4) {
        printf("[LOI HAN NGACH]: Moi giao dich chi duoc phep mua tu 1 den 4 ve!\n");
        return 0;
    }

    int index = timZoneTheoId(dsZone, soLuongZone, zone_id);

    if (index == -1) {
        printf("[LOI NOT FOUND]: Khong tim thay khu vuc ve co ID %d!\n", zone_id);
        return 0;
    }

    int veConLai = dsZone[index].total_capacity - dsZone[index].sold_tickets;

    if (soLuong > veConLai) {
        printf("[LOI TON KHO]: Khu vuc '%s' khong du ve! (Con lai: %d ve)\n",
               dsZone[index].zone_name, veConLai);
        return 0;
    }

    float tongTien = tinhTienThanhToan(dsZone[index].ticket_price, soLuong, dsZone[index].is_early_bird);
    dsZone[index].sold_tickets += soLuong;

    printf("\n================ HOA DON GIAO DICH DAT VE ================\n");
    printf("Khu vuc ve         : %s (ID: %d)\n", dsZone[index].zone_name, dsZone[index].zone_id);
    printf("So luong dat       : %d ve\n", soLuong);
    printf("Gia niem yet       : %.0f VND / ve\n", dsZone[index].ticket_price);
    printf("Uu dai Early Bird  : %s\n", dsZone[index].is_early_bird ? "Giam 15%%" : "Khong");
    printf("TONG TIEN THANH TOAN: %.0f VND\n", tongTien);
    printf("==========================================================\n");
    printf("[THANH CONG]: Giao dich hoan tat! Da cap nhat ve da ban.\n");

    return 1;
}

// 3. Hàm main() đặt cuối cùng (Gọi các hàm đã định nghĩa phía trên)
int main() {
    struct TicketZone dsZone[MAX_ZONE] = {
        {101, "Zone SVIP", 2500000.0f, 20, 5, 1},
        {102, "Zone VIP", 1800000.0f, 50, 10, 1},
        {103, "Zone GA", 900000.0f, 100, 0, 0}
    };
    int soLuongZone = 3;
    int luaChon;

    do {
        printf("\n================ TICKETBOX - VPOP STAR ARENA ================\n");
        printf("1. Hien thi danh sach khu vuc ve (Zone)\n");
        printf("2. Them khu vuc ve moi\n");
        printf("3. Cap nhat gia ve khu vuc\n");
        printf("4. Xoa khu vuc ve (Chua ban ve)\n");
        printf("5. Xu ly giao dich dat ve (Booking)\n");
        printf("6. Thoat chuong trinh\n");
        printf("=============================================================\n");
        printf("Lua chon cua ban: ");
        scanf("%d", &luaChon);

        switch (luaChon) {
            case 1:
                hienThiDanhSachZone(dsZone, soLuongZone);
                break;

            case 2: {
                struct TicketZone zm;
                while (getchar() != '\n');

                printf("\n--- THEM KHU VUC VE MOI ---\n");
                printf("Nhap ma Zone ID: ");
                scanf("%d", &zm.zone_id);
                while (getchar() != '\n');

                printf("Nhap ten Zone: ");
                fgets(zm.zone_name, sizeof(zm.zone_name), stdin);
                zm.zone_name[strcspn(zm.zone_name, "\n")] = 0;

                do {
                    printf("Nhap gia ve goc (VND): ");
                    scanf("%f", &zm.ticket_price);
                    if (zm.ticket_price <= 0) {
                        printf("Gia ve phai lon hon 0! Nhap lai.\n");
                    }
                } while (zm.ticket_price <= 0);

                do {
                    printf("Nhap tong so ve phat hanh: ");
                    scanf("%d", &zm.total_capacity);
                    if (zm.total_capacity <= 0) {
                        printf("Tong so ve phat hanh phai lon hon 0! Nhap lai.\n");
                    }
                } while (zm.total_capacity <= 0);

                zm.sold_tickets = 0;

                do {
                    printf("Trang thai Early Bird (1: Co giam 15%%, 0: Khong): ");
                    scanf("%d", &zm.is_early_bird);
                    if (zm.is_early_bird != 0 && zm.is_early_bird != 1) {
                        printf("Gia tri chi duoc nhap 0 hoac 1! Nhap lai.\n");
                    }
                } while (zm.is_early_bird != 0 && zm.is_early_bird != 1);

                soLuongZone = themZoneMoi(dsZone, soLuongZone, zm);
                break;
            }

            case 3: {
                int idFind;
                float giaMoi;
                printf("\n--- CAP NHAT GIA VE ---\n");
                printf("Nhap ma Zone ID can cap nhat gia: ");
                scanf("%d", &idFind);

                do {
                    printf("Nhap gia ve goc moi (VND): ");
                    scanf("%f", &giaMoi);
                    if (giaMoi <= 0) {
                        printf("Gia ve phai lon hon 0! Nhap lai.\n");
                    }
                } while (giaMoi <= 0);

                capNhatGiaVeZone(dsZone, soLuongZone, idFind, giaMoi);
                break;
            }

            case 4: {
                int idFind;
                printf("\n--- XOA KHU VUC VE ---\n");
                printf("Nhap ma Zone ID can xoa: ");
                scanf("%d", &idFind);

                soLuongZone = xoaZone(dsZone, soLuongZone, idFind);
                break;
            }

            case 5: {
                int idFind, soLuongMua;
                printf("\n--- XU LY GIAO DICH DAT VE ---\n");
                printf("Nhap ma Zone ID muon dat: ");
                scanf("%d", &idFind);

                printf("Nhap so luong ve mua: ");
                scanf("%d", &soLuongMua);

                xuLyDatVe(dsZone, soLuongZone, idFind, soLuongMua);
                break;
            }

            case 6:
                printf("\nCam on ban da su dung he thong TicketBox Engine!\n");
                break;

            default:
                printf("\nLua chon khong hop le! Vui long chon tu 1 den 6.\n");
                break;
        }

    } while (luaChon != 6);

    return 0;
}