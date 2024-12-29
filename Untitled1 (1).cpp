#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cau truc luu mon an trong menu
struct MenuItem {
    int id;
    char name[100];  // Thay string bang mang ky tu
    int price;
    struct MenuItem* next;
};

// Cau truc luu don hang
struct Order {
    int id;
    int quantity;
    struct Order* next;
};

// Cau truc luu ban va cac don hang
struct Table {
    int tableNumber;
    struct Order* orders;
    struct Table* next;
    int check;  // 1 nghia la co san, 0 nghia la da chiem
};

// Cau truc luu doanh thu tung ngay
struct Revenue {
    int day;
    int month;
    int year;
    int revenueinday;
    struct Revenue* next;
};

// Ham them mon an vao danh sach menu
void addMenuItem(struct MenuItem** head, int id, const char* name, int price) {
    struct MenuItem* newItem = (struct MenuItem*)malloc(sizeof(struct MenuItem));
    newItem->id = id;
    strcpy(newItem->name, name);  // Su dung strcpy de sao chep chuoi
    newItem->price = price;
    newItem->next = *head;
    *head = newItem;
}

// Ham hien thi menu
void displayMenu(struct MenuItem* head) {
    printf("====MENU====\n");
    struct MenuItem* current = head;
    while (current != NULL) {
        printf("%s\t%d\t%d\n", current->name, current->id, current->price);
        current = current->next;
    }
    printf("================\n");
}

// Ham tai menu tu file
void loadMenuFromFile(struct MenuItem** menu, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Khong the mo file!\n");
        return;
    }

    int id, price;
    char name[100];
    while (fscanf(file, "%d, %99[^,], %d\n", &id, name, &price) != EOF) {
        addMenuItem(menu, id, name, price);
    }

    fclose(file);
}

// Tao mon moi trong don hang cua khach
struct Order* makeOrderItem(int id, int quantity) {
    struct Order* newOrder = (struct Order*)malloc(sizeof(struct Order));
    newOrder->id = id;
    newOrder->quantity = quantity;
    newOrder->next = NULL;
    return newOrder;
}

// Tao doanh thu mot ngay
struct Revenue* makeDailyRevenue(int day, int month, int year, int revenueinday) {
    struct Revenue* newRevenue = (struct Revenue*)malloc(sizeof(struct Revenue));
    newRevenue->day = day;
    newRevenue->month = month;
    newRevenue->year = year;
    newRevenue->revenueinday = revenueinday;
    newRevenue->next = NULL;
    return newRevenue;
}

// Them ban vao danh sach
void addtable(int tablenumber, struct Table** tables) {
    struct Table* newtable = (struct Table*)malloc(sizeof(struct Table));
    newtable->tableNumber = tablenumber;
    newtable->orders = NULL;  // Khoi tao don hang la NULL
    newtable->check = 1;  // Ban co san
    newtable->next = *tables;
    *tables = newtable;
}

// Tim ban theo so
int findTable(struct Table* tablelist, int tableNumber) {
    struct Table* current = tablelist;
    while (current != NULL) {
        if (current->tableNumber == tableNumber) {
            if (current->check == 1) {
                current->check = 0;  // Dang dau ban da chiem
                return 1;  // Ban co san va da chiem
            }
            else {
                return 0;  // Ban da chiem
            }
        }
        current = current->next;
    }
    return 0;  // Ban khong tim thay
}

// Ghi nhan don hang cho ban
void takeOrderForTable(struct Table* table) {
    while (1) {
        printf("Bam phim 1 de goi them mon an, bam phim 0 neu nguoc lai: ");
        int select;
        scanf("%d", &select);
        if (select == 0) break;
        else {
            printf("Nhap id mon an: ");
            int id;
            scanf("%d", &id);
            printf("So luong: ");
            int quantity;
            scanf("%d", &quantity);
            struct Order* Neworder = makeOrderItem(id, quantity);
            if (table->orders == NULL) {
                table->orders = Neworder;  // Don hang dau tien cho ban
            }
            else {
                Neworder->next = table->orders;
                table->orders = Neworder;  // Them vao dau danh sach
            }
        }
    }
}

// Giai phong bo nho danh sach lien ket
void freeMenu(struct MenuItem* head) {
    while (head != NULL) {
        struct MenuItem* temp = head;
        head = head->next;
        free(temp);
    }
}

// Giai phong bo nho danh sach lien ket cua don hang
void freeOrders(struct Order* head) {
    while (head != NULL) {
        struct Order* temp = head;
        head = head->next;
        free(temp);  // Giai phong tung phan tu trong danh sach don Order
    }
}

// Giai phong bo nho danh sach lien ket cua ban
void freeTables(struct Table* head) {
    while (head != NULL) {
        struct Table* temp = head;
        freeOrders(head->orders);  // Giai phong cac don hang truoc khi giai phong ban
        head = head->next;
        free(temp);  // Giai phong ban
    }
}

// Tinh hoa don cho ban
void calculateBill(struct Table* table, struct Revenue** RevenueList, struct MenuItem* menu) {
    int totalprice = 0;
    int day, month, year;
    printf("Nhap ngay thang nam:\n");
    printf("Ngay: "); scanf("%d", &day);
    printf("Thang: "); scanf("%d", &month);
    printf("Nam: "); scanf("%d", &year);
    printf("Hoa don ban so: %d\n", table->tableNumber);
    printf("Ngay: %d\tThang: %d\tNam: %d\n", day, month, year);
    struct Order* currentItemBill = table->orders;
    while (currentItemBill != NULL) {
        struct MenuItem* currentItemMenu = menu;
        while (currentItemMenu != NULL) {
            if (currentItemBill->id == currentItemMenu->id) {
                printf("%s\tSo luong: %d\tGia tien: %d\n", currentItemMenu->name, currentItemBill->quantity, (currentItemMenu->price) * (currentItemBill->quantity));
                totalprice += (currentItemMenu->price) * (currentItemBill->quantity);
                break;
            }
            currentItemMenu = currentItemMenu->next;
        }
        currentItemBill = currentItemBill->next;
    }
    printf("Tong: %d\n", totalprice);
    table->check = 1;  // Dang dau ban co san
    freeOrders(table->orders);
    table->orders = NULL;  // Reset don hang sau thanh toan

    // Them so tien ban do thanh toan vao doanh thu ngay tuong ung
    struct Revenue* currentRevenue = *RevenueList;
    int check = 0;
    while (currentRevenue != NULL) {
        if (currentRevenue->day == day && currentRevenue->month == month && currentRevenue->year == year) {
            currentRevenue->revenueinday += totalprice;
            check = 1;
            break;
        }
        currentRevenue = currentRevenue->next;
    }
    if (!check) {
        struct Revenue* newRevenue = makeDailyRevenue(day, month, year, totalprice);
        newRevenue->next = *RevenueList;
        *RevenueList = newRevenue;
    }
}

// Tinh doanh thu
void calculateRevenue(struct Revenue* RevenueList) {
    int startDay, startMonth, startYear;
    int endDay, endMonth, endYear;

    // Nhap ngay bat dau va ngay ket thuc
    printf("Nhap ngay bat dau (dd mm yyyy): ");
    scanf("%d %d %d", &startDay, &startMonth, &startYear);
    printf("Nhap ngay ket thuc (dd mm yyyy): ");
    scanf("%d %d %d", &endDay, &endMonth, &endYear);

    int totalRevenue = 0;

    // Duyet qua danh sach doanh thu
    struct Revenue* current = RevenueList;
    while (current != NULL) {
        // Kiem tra neu ngay cua moc hien tai nam trong khoang thoi gian yeu cau
        if ((current->year > startYear || (current->year == startYear && current->month > startMonth) || 
            (current->year == startYear && current->month == startMonth && current->day >= startDay)) &&
            (current->year < endYear || (current->year == endYear && current->month < endMonth) || 
            (current->year == endYear && current->month == endMonth && current->day <= endDay))) {
            totalRevenue += current->revenueinday;  // Cong den doanh thu
        }
        current = current->next;  // Chuyen sang moc tiep theo trong danh sach
    }

    // In ra tong doanh thu
    printf("Tong doanh thu tu %d/%d/%d den %d/%d/%d la: %d\n", startDay, startMonth, startYear, endDay, endMonth, endYear, totalRevenue);
}

// Ghi nhan phan hoi cua khach hang
void getFeedback() {
    char feedback[255];
    printf("\n Nhap phan hoi cua ban: ");
    getchar();  // De loi bo ky tu '\n' du tha sau khi nhap so
    fgets(feedback, sizeof(feedback), stdin);

    printf("Cam on phan hoi cua ban");
}

int main() {
    struct MenuItem* menu = NULL;
    struct Table* tables = NULL;
    int quantity;

    // Tai menu tu file
    loadMenuFromFile(&menu, "menu.txt");
    int NumberOfTables;
    printf("Nhap so luong ban: ");
    scanf("%d", &NumberOfTables);
    while (NumberOfTables--) {
        int tableNumber;
        printf("Nhap so ban: "); scanf("%d", &tableNumber);
        addtable(tableNumber, &tables);
    }

    struct Revenue* revenuelist = NULL;
    int choice, tableNumber;
    do {
        printf("\n===== HE THONG QUAN LY NHA HANG =====\n");
        printf("1. Xem menu\n");
        printf("2. Dat ban\n");
        printf("3. Goi mon\n");
        printf("4. Thanh toan\n");
        printf("5. Gui phan hoi\n");
        printf("6. Tinh doanh thu\n");
        printf("7. Thoat\n");
        printf("Lua chon cua ban: ");
        scanf("%d", &choice);
        while(getchar() != '\n');  // Loi bo ky tu du tha trong bo dem sau khi nhap lua chon

        switch (choice) {
        case 1: {
            displayMenu(menu);
            break;
        }
        case 2: {
            printf("Nhap so ban: ");
            scanf("%d", &tableNumber);
            while(getchar() != '\n');
            if (findTable(tables, tableNumber)) {
                printf("Dat ban thanh cong!\n");
            }
            else {
                printf("Ban da duoc dat.\n");
            }
            break;
        }
        case 3: {
            printf("Nhap so ban: ");
            scanf("%d", &tableNumber);
            while(getchar() != '\n');
            struct Table* currentTable = tables;
            while (currentTable != NULL) {
                if (currentTable->tableNumber == tableNumber) {
                    takeOrderForTable(currentTable);
                    break;
                }
                currentTable = currentTable->next;
            }
            break;
        }
        case 4: {
            printf("Nhap so ban: ");
            scanf("%d", &tableNumber);
            while(getchar() != '\n');
            struct Table* currentTable = tables;
            while (currentTable != NULL) {
                if (currentTable->tableNumber == tableNumber) {
                    calculateBill(currentTable, &revenuelist, menu);
                    break;
                }
                currentTable = currentTable->next;
            }
            break;
        }
        case 5: {
            getFeedback();
            break;
        }
        case 6: {
            calculateRevenue(revenuelist);
            break;
        }
        case 7: {
            printf("Cam on ban da su dung he thong!\n");
            break;
        }
        default:
            printf("Lua chon khong hop le!\n");
        }
    } while (choice != 7);

    // Giai phong bo nho
    freeMenu(menu);
    freeTables(tables);  // Goi freeTables thay ve freeOrders o day
    return 0;
}

