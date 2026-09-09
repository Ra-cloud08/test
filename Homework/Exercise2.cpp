#include <iostream>
#include <string>
using namespace std;

struct Product {
    int id;
    string name;
    double price;
};
void updatePrice(Product* p, double newPrice) {
    p->price = newPrice;
}

int main() {
    Product product = {123, "Laptop", 550.70};

    cout << "Original Product Information:" << endl;
    cout << "ID: " << product.id << endl;
    cout << "Name: " << product.name << endl;
    cout << "Price: $" << product.price << endl;

    double newPrice;
    cout << "\nEnter a new price: $";
    cin >> newPrice;

    updatePrice(&product, newPrice);
    cout << "\nUpdated Product Information:" << endl;
    cout << "ID: " << product.id << endl;
    cout << "Name: " << product.name << endl;
    cout << "Price: $" << product.price << endl;

    return 0;
}