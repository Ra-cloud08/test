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

void displayProducts(Product* products, int size) {
    cout << left << setw(8) << "ID"
         << setw(20) << "Name"
         << setw(10) << "Price"
         << setw(8) << "Stock" << endl;

    cout << "--------------------------------------------" << endl;

    for (int i = 0; i < size; i++) {
        cout << left << setw(8) << (products + i)->id
             << setw(20) << (products + i)->name
             << "$" << setw(9) << fixed << setprecision(2)
             << (products + i)->price
             << (products + i)->quantity << endl;
    }
}

Product* findProduct(Product* products, int size, int id) {
    for (int i = 0; i < size; i++) {
        if ((products + i)->id == id) {
            return products + i;
        }
    }

    return nullptr;
}
void sellProduct(Product* product, int quantity) {
    if (quantity > product->quantity) {
        cout << "Error: Not enough stock!" << endl;
        cout << "Available stock: " << product->quantity << endl;
    } else {
        product->quantity -= quantity;
        cout << "Sale successful!" << endl;
        cout << "Stock: " << product->quantity << endl;
    }
}

int main() {
    Product products[5] = {
        {101, "Coca Cola", 1.50, 20},
        {102, "Pepsi", 1.40, 15},
        {103, "Water", 0.50, 50},
        {104, "Orange Juice", 2.00, 10},
        {105, "Bread", 1.80, 20}
    };

    displayProducts(products, 5);

    int id;
    cout << "\nEnter Product ID: ";
    cin >> id;

    Product* p = findProduct(products, 5, id);

    if (p != nullptr) {
        cout << "\nProduct Found!" << endl;
        cout << "ID: " << p->id << endl;
        cout << "Name: " << p->name << endl;
        cout << "Price: $" << fixed << setprecision(2) << p->price << endl;
        cout << "Stock: " << p->quantity << endl;

        int quantity;
        cout << "\nEnter quantity to sell: ";
        cin >> quantity;

        sellProduct(p, quantity);
    } else {
        cout << "\nProduct not found!" << endl;
    }

    return 0;
}