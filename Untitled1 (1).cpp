#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    Order* orders;
    struct Table* next;
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

// Thêm món vào danh sách đơn hàng
Order* addOrderItem(Order* head, int id, int quantity) {
    Order* newOrder = (Order*)malloc(sizeof(Order));
    newOrder->id = id;
    newOrder->quantity = quantity;
    newOrder->next = head;
    return newOrder;
}

// Tìm bàn theo số
Table* findTable(Table* head, int tableNumber) {
    Table* current = head;
    while (current != NULL) {
        if (current->tableNumber == tableNumber) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Thêm bàn mới vào danh sách
Table* addTable(Table* head, int tableNumber) {
    Table* newTable = (Table*)malloc(sizeof(Table));
    newTable->tableNumber = tableNumber;
    newTable->orders = NULL;
    newTable->next = head;
    return newTable;
}

// Ghi nhận đơn hàng cho bàn
void takeOrderForTable(Table* table, MenuItem* menu) {
    int id, quantity;

    printf("Nhập ID món ăn (0 để kết thúc): ");
    while (scanf("%d", &id) && id != 0) {
        // Kiểm tra món có trong menu hay không
        MenuItem* currentMenu = menu;
        int valid = 0;
        while (currentMenu != NULL) {
            if (currentMenu->id == id) {
                valid = 1;
                break;
            }
            currentMenu = currentMenu->next;
        }
        if (!valid) {
            printf("Món ăn không có trong menu!\n");
            continue;
        }

        printf("Nhập số lượng: ");
        scanf("%d", &quantity);
        table->orders = addOrderItem(table->orders, id, quantity);
        printf("Đã thêm món!\n");

        printf("Nhập ID món ăn (0 để kết thúc): ");
    }
}

// Tính hóa đơn cho bàn
void calculateBill(MenuItem* menuHead, Table* table) {
    if (!table->orders) {
        printf("Bàn %d chưa gọi món!\n", table->tableNumber);
        return;
    }

    int total = 0;
    printf("\n===== HÓA ĐƠN BÀN %d =====\n", table->tableNumber);
    Order* orderCurrent = table->orders;

    while (orderCurrent != NULL) {
        MenuItem* menuCurrent = menuHead;
        while (menuCurrent != NULL) {
            if (menuCurrent->id == orderCurrent->id) {
                int cost = orderCurrent->quantity * menuCurrent->price;
                total += cost;
                printf("%s x %d = %d VND\n", menuCurrent->name, orderCurrent->quantity, cost);
                break;
            }
            menuCurrent = menuCurrent->next;
        }
        orderCurrent = orderCurrent->next;
    }
    printf("Tổng cộng: %d VND\n", total);
    printf("===================\n");
}

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
        case 1:{
        	displayMenu(menu);
			break;
		}
        case 2:{
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
            
        case 3:{
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
            
        case 4:{
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
            
        case 6:{
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
