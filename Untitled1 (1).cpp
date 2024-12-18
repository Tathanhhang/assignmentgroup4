#include <stidio.h>
#include <stdlib.h>
#include <string.h>
#include<iostream>
using namespace std;
// Cau truc luu mon an trong menu
typedef struct MenuItem {
    int id;
    char name[50];
    int price;
    struct MenuItem* next;
} MenuItem;

// Cau truc luu don hang
typedef struct Order {
    int id;
    int soluong;
    struct Order* next;
} Order;

// Cau truc luu ban va cac don hang
typedef struct Table {
    int tableNumber;
    Order** orders;
    struct Table* next;
    int check;
} Table;

// Ham them mon an vao danh sach menu
MenuItem* addMenuItem(MenuItem* head, int id, char name[], int price) {
    MenuItem* newItem = (MenuItem*)malloc(sizeof(MenuItem));
    newItem->id = id;
    strcpy(newItem->name, name);
    newItem->price = price;
    newItem->next = head;
    return newItem;
}

// Ham tai menu tu file
MenuItem* loadMenuFromFile(const char* filename) {
    FILE* file;
    errno_t err = fopen_s(&file, filename, "r");
    if (err != 0) {
        printf("Khong the mo file menu!\n");
        return NULL;
    }

    MenuItem* head = NULL;
    int id, price;
    char name[50];

    while (fscanf(file, "%d,%49[^,],%d", &id, name, &price) == 3) {
        head = addMenuItem(head, id, name, price);
    }

    fclose(file);
    return head;
}

// Hien thi menu
void displayMenu(MenuItem* head) {
    printf("===== MENU =====\n");
    MenuItem* current = head;
    while (current != NULL) {
        printf("%d. %s - %d VND\n", current->id, current->name, current->price);
        current = current->next;
    }
    printf("================\n");
}

// Tao mot mon moi trong don hang cua khach
Order* makeOrderItem(int id, int soluong) {
    Order* newOrder =(Order*)malloc(sizeof(Order));
    newOrder->id = id;
    newOrder->soluong = soluong;
    newOrder->next = NULL;
    return newOrder;
}

// Tim ban theo so
Table* findTable(Table* head, int tableNumber) {
    Table* current = head;
    while (current != NULL) {
        if (current->tableNumber == tableNumber) {
            if (current->check == 1) continue;
            else {
                current->check = 1;
                return current;
        }
        current = current->next;
    }
    return NULL;
}



// Ghi nhan don hang cho ban
void takeOrderForTable(Table* table) {
    if (table->check == 0) {
        cout << "Ban trong" << endl;
        return;
    }
    while(1){
        cout << "bam phim 1 de goi them mon an, bam phim 0 neu nguoc lai: " << endl;
        int luachon;
        cin >> luachon;
        if (luachon == 0) break;
        else{
            int id;  
            printf("Nhap id mon an: ");
            scanf("%d", &id);
            int soluong;
            printf("so luong: ");
            scanf("%d", &soluong);
            Order* Neworder = makeOrderItem(id, soluong);
            Oder* temp = *(table->orders);
            if (temp == NULL) {
                temp = Neworder;
            }
            else {
                Neworder->next = temp;
            }
}

// Tinh hoa don cho ban
int calculateBill(Table* table, MenuItem* menu) {
    if (!table->check) {
        cout << "Ban" << table->tableNumber << "trong" << endl;
        return;
    }
    int sum = 0;
    cout << "Hoa don ban so: " << table -> tableNumber << endl;
    // hien thi danh sach mon an ma ban do da dat
    Order* temp1 = *(table->orders);
    while (temp1 != NULL) {
        MenuItem* temp2 = menu;
        while (temp2 != NULL) {
            if (temp1->id == temp2->id) {
                cout << "temp2->name" << 't' << "soluong:" << temp1->soluong << 't' << "gia tien:" << (temp2->price) * (temp1->soluong) << endl;
                sum += (temp2->price) * (temp1->soluong);
                break;
            }
            temp2 = temp2->next;
        }
        temp1 = temp1->next;
    }
    return sum;
}
// Thanh toan khi khach hang mua online

// Tinh doanh thu
 
// Giai phong bo nho danh sach lien ket
void freeMenu(MenuItem* head) {
    while (head != NULL) {
        MenuItem* temp = head;
        head = head->next;
        free(temp);
    }
}

void freeOrders(Order* head) {
    while (head != NULL) {
        Order* temp = head;
        head = head->next;
        free(temp);
    }
}

void freeTables(Table* head) {
    while (head != NULL) {
        freeOrders(head->orders);
        Table* temp = head;
        head = head->next;
        free(temp);
    }
}

}
// Ghi nhan phan hoi cua khach hang
void getFeedback() {
    char feedback[256];
    printf("\nNhap phan hoi cua ban: ");
    getchar(); // Loai bo ki tu newline tu buffer
    fgets(feedback, sizeof(feedback), stdin);
    writeFeedbackToFile("feedback.txt", feedback);  // Ghi phan hoi vao file
    printf("Cam on phan hoi cua ban!\n");
}

// Luu phan hoi của khach vào file Feedback.txt
void writeFeedbackToFile(const char* filename, const char* feedback) {
    FILE* file = fopen(filename, "a");  // Mo  de them phan hoi vao file 
    if (file != NULL) {
        fprintf(file, "%s", feedback);  // Ghi phan hoi vao file
        fclose(file);  // Đong file
    } else {
        printf("Khong the mo file de ghi!\n");
    }
}
// Hien thi phan hoi cua khach hang
void displayFeedback(const char* filename) {
    FILE* file = fopen(filename, "r");  // Mo file Feedback.txt
    if (file != NULL) {
        printf("\n===== Danh gia khach hang =====\n");
        char line[256];  // tao 1 mang de chua moi dong lay ra tu file
        while (fgets(line, sizeof(line), file)) {  // Doc tung dong
            printf("%s", line);  
        }
        fclose(file);  // Dong file sau khi doc xong
    } else {
        printf("Khong co danh gia nao!\n");  // Thong bao neu khong mo duoc file hoac file rong
    }
}

int main() {
    MenuItem* menu = loadMenuFromFile("menu.txt");
    if (!menu) {
        return 1;
    }

    Table* tables = NULL;

    int choice, tableNumber;
    do {
        printf("\n===== HE THONG QUAN LY NHA HANG =====\n");
        printf("1. Xem menu\n");
        printf("2. Dat ban\n");
        printf("3. Goi mon\n");
        printf("4. Thanh toan\n");
        printf("5. Gui phan hoi\n");
        printf("6. Thoat\n");
        printf("Lua chon cua ban: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: {
            displayMenu(menu);
            break;
        }
        case 2: {
            printf("Ban so: ");
            scanf("%d", &tableNumber);
            if (findTable(tables, tableNumber)) {
                printf("Ban %d đa duoc dat!\n", tableNumber);
            }
            else {
                tables = addTable(tables, tableNumber);
                printf("Dat ban %d thanh cong!\n", tableNumber);
            }
            break;
        }

        case 3: {
            printf("Ban so: ");
            scanf("%d", &tableNumber);
            Table* table = findTable(tables, tableNumber);
            if (table) {
                takeOrderForTable(table, menu);
            }
            else {
                printf("Ban %d chua duoc dat!\n", tableNumber);
            }

            break;
        }

        case 4: {
            printf("Ban so: ");
            scanf("%d", &tableNumber);
            Table* billingTable = findTable(tables, tableNumber);
            if (billingTable) {
                calculateBill(menu, billingTable);
            }
            else {
                printf("Ban %d chua duoc dat!\n", tableNumber);
            }

            break;
        }

        case 5:
        {
            getFeedback();
            break;
        }

        case 6: {
            printf("Cam on ban da su dung he thong!\n");
            break;
        }

        default:
            printf("Lua chon khong hop le!\n");
        }
    } while (choice != 6);

    // Giai phong bo nho
    freeMenu(menu);
    freeTables(tables);



    return 0;
}
