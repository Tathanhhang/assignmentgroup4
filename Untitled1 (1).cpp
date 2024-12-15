#include <stdlib.h>
#include <string.h>
#include<iostream>
using namespace std;
// Cấu trúc lưu món ăn trong menu
typedef struct MenuItem {
    int id;
    char name[50];
    int price;
    struct MenuItem* next;
} MenuItem;

// Cấu trúc lưu đơn hàng
typedef struct Order {
    int id;
    int quantity;
    struct Order* next;
} Order;

// Cấu trúc lưu bàn và các đơn hàng
typedef struct Table {
    int tableNumber;
    Order** orders;
    struct Table* next;
    int check;
} Table;

// Hàm thêm món ăn vào danh sách menu
MenuItem* addMenuItem(MenuItem* head, int id, char name[], int price) {
    MenuItem* newItem = (MenuItem*)malloc(sizeof(MenuItem));
    newItem->id = id;
    strcpy(newItem->name, name);
    newItem->price = price;
    newItem->next = head;
    return newItem;
}

// Hàm tải menu từ file
MenuItem* loadMenuFromFile(const char* filename) {
    FILE* file;
    errno_t err = fopen_s(&file, filename, "r");
    if (err != 0) {
        printf("Không thể mở file menu!\n");
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

// Hiển thị menu
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
Order* makeOrderItem(int id, int quantity) {
    Order* newOrder = new Order();
    newOrder->id = id;
    newOrder->quantity = quantity;
    newOrder->next = NULL;
    return newOrder;
}

// Tìm bàn theo số
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



// Ghi nhận đơn hàng cho bàn
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
            cout << "Nhap id mon an: ";
            int id; cin >> id;
            cout << "so luong: ";
            int quantity;cin >> quantity;
            Order* Neworder = makeOrderItem(id, quantity);
            Oder* temp = *(table->orders);
            if (temp == NULL) {
                temp = Neworder;
            }
            else {
                Neworder->next = temp;
            }
}

// Tính hóa đơn cho bàn
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
                cout << "temp2->name" << 't' << "soluong:" << temp1->quantity << 't' << "gia tien:" << (temp2->price) * (temp1->quantity) << endl;
                sum += (temp2->price) * (temp1->quantity);
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
 
// Giải phóng bộ nhớ danh sách liên kết
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

// Ghi nhận phản hồi khách hàng
void getFeedback() {
    char feedback[255];
    printf("\nNhập phản hồi của bạn: ");
    getchar(); // Loại bỏ ký tự newline từ buffer
    fgets(feedback, sizeof(feedback), stdin);

    printf("Cảm ơn phản hồi của bạn!\n");
}

int main() {
    MenuItem* menu = loadMenuFromFile("menu.txt");
    if (!menu) {
        return 1;
    }

    Table* tables = NULL;

    int choice, tableNumber;
    do {
        printf("\n===== HỆ THỐNG QUẢN LÝ NHÀ HÀNG =====\n");
        printf("1. Xem menu\n");
        printf("2. Đặt bàn\n");
        printf("3. Gọi món\n");
        printf("4. Thanh toán\n");
        printf("5. Gửi phản hồi\n");
        printf("6. Thoát\n");
        printf("Lựa chọn của bạn: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: {
            displayMenu(menu);
            break;
        }
        case 2: {
            printf("Nhập số bàn: ");
            scanf("%d", &tableNumber);
            if (findTable(tables, tableNumber)) {
                printf("Bàn %d đã được đặt!\n", tableNumber);
            }
            else {
                tables = addTable(tables, tableNumber);
                printf("Đặt bàn %d thành công!\n", tableNumber);
            }
            break;
        }

        case 3: {
            printf("Nhập số bàn: ");
            scanf("%d", &tableNumber);
            Table* table = findTable(tables, tableNumber);
            if (table) {
                takeOrderForTable(table, menu);
            }
            else {
                printf("Bàn %d chưa được đặt!\n", tableNumber);
            }

            break;
        }

        case 4: {
            printf("Nhập số bàn: ");
            scanf("%d", &tableNumber);
            Table* billingTable = findTable(tables, tableNumber);
            if (billingTable) {
                calculateBill(menu, billingTable);
            }
            else {
                printf("Bàn %d chưa được đặt!\n", tableNumber);
            }

            break;
        }

        case 5:
        {
            getFeedback();
            break;
        }

        case 6: {
            printf("Cảm ơn bạn đã sử dụng hệ thống!\n");
            break;
        }

        default:
            printf("Lựa chọn không hợp lệ!\n");
        }
    } while (choice != 6);

    // Giải phóng bộ nhớ
    freeMenu(menu);
    freeTables(tables);



    return 0;
}
