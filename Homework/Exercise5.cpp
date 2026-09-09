#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

struct Product {
    int id;
    string name;
    double price;
    int quantity;
};

void addProduct(Product* products, int* count) {
    if (*count >= 10) {
        cout << "Product list is full!" << endl;
        return;
    }
    cout << "Enter Product ID: ";
    cin >> products[*count].id;

    cin.ignore();

    cout << "Enter Product Name: ";
    getline(cin, products[*count].name);

    cout << "Enter Price: ";
    cin >> products[*count].price;

    cout << "Enter Quantity: ";
    cin >> products[*count].quantity;

    (*count)++;

    cout << "Product added successfully!" << endl;
}

void displayProducts(Product* products, int count) {
    if (count == 0) {
        cout << "No products available." << endl;
        return;
    }

    cout << "\nID     Name             Price      Stock" << endl;
    cout << "-----------------------------------------" << endl;

    Product* p = products;

    for (int i = 0; i < count; i++) {
        cout << left << setw(7) << p->id
             << setw(17) << p->name
             << "$" << setw(9) << fixed << setprecision(2)
             << p->price
             << p->quantity << endl;

        p++;
    }
}

Product* findProduct(Product* products, int count, int id) {
    for (int i = 0; i < count; i++) {
        if ((products + i)->id == id) {
            return products + i;
        }
    }

    return nullptr;
}

void sellProduct(Product* product, int quantity) {
    if (quantity <= 0) {
        cout << "Invalid quantity." << endl;
        return;
    }

    if (quantity > product->quantity) {
        cout << "Error: Not enough stock!" << endl;
        cout << "Available stock: " << product->quantity << endl;
        return;
    }

    double total = product->price * quantity;

    product->quantity -= quantity;

    cout << "\nProduct: " << product->name << endl;
    cout << "Price: $" << fixed << setprecision(2)
         << product->price << endl;
    cout << "Quantity: " << quantity << endl;
    cout << "Total: $" << total << endl;
    cout << "Remaining Stock: " << product->quantity << endl;
}

void restockProduct(Product* product, int quantity) {
    if (quantity <= 0) {
        cout << "Invalid quantity." << endl;
        return;
    }

    product->quantity += quantity;

    cout << "Restock successful!" << endl;
    cout << "Stock: " << product->quantity << endl;
}

double calculateInventoryValue(Product* products, int count) {
    double total = 0;

    for (int i = 0; i < count; i++) {
        total += (products + i)->price * (products + i)->quantity;
    }

    return total;
}

int main() {
    Product products[10];

    int count = 0;
    int choice;

    do {
        cout << "\n========== RETAIL MANAGEMENT SYSTEM ==========\n";
        cout << "1. Add Product\n";
        cout << "2. Display Products\n";
        cout << "3. Search Product\n";
        cout << "4. Sell Product\n";
        cout << "5. Restock Product\n";
        cout << "6. Calculate Inventory Value\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            addProduct(products, &count);
        }

        else if (choice == 2) {
            displayProducts(products, count);
        }

        else if (choice == 3) {
            int id;

            cout << "Enter Product ID: ";
            cin >> id;

            Product* p = findProduct(products, count, id);

            if (p != nullptr) {
                cout << "\nProduct Found!" << endl;
                cout << "ID: " << p->id << endl;
                cout << "Name: " << p->name << endl;
                cout << "Price: $" << fixed << setprecision(2)
                     << p->price << endl;
                cout << "Stock: " << p->quantity << endl;
            } else {
                cout << "Product not found." << endl;
            }
        }

        else if (choice == 4) {
            int id;
            int quantity;

            cout << "Enter Product ID: ";
            cin >> id;

            Product* p = findProduct(products, count, id);

            if (p != nullptr) {
                cout << "Enter Quantity: ";
                cin >> quantity;

                sellProduct(p, quantity);
            } else {
                cout << "Product not found." << endl;
            }
        }

        else if (choice == 5) {
            int id;
            int quantity;

            cout << "Enter Product ID: ";
            cin >> id;

            Product* p = findProduct(products, count, id);

            if (p != nullptr) {
                cout << "Enter Quantity to Add: ";
                cin >> quantity;

                restockProduct(p, quantity);
            } else {
                cout << "Product not found." << endl;
            }
        }

        else if (choice == 6) {
            double total = calculateInventoryValue(products, count);

            cout << "\nTotal Inventory Value: $"
                 << fixed << setprecision(2)
                 << total << endl;
        }

        else if (choice == 7) {
            cout << "Exiting program..." << endl;
        }

        else {
            cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != 7);

    return 0;
}