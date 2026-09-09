#include <iostream>
#include <string>
using namespace std;
struct Product {
    int id;
    string name;
    double price;
    int quantity;
};

int main() {
    Product products[5];
    cout << "Enter information for 5 products: "<<endl;

    for (int i = 0; i < 5; i++) {
        cout << "\nProduct " << i + 1 << endl;

        cout << "Enter ID: ";
        cin >> products[i].id;

        cin.ignore(); 

        cout << "Enter name: ";
        getline(cin, products[i].name);

        cout << "Enter price: $";
        cin >> products[i].price;

        cout << "Enter quantity: ";
        cin >> products[i].quantity;
    }

    cout << "\n===== All Products ====="<<endl;

    Product* ptr = products;

    for (int i = 0; i < 5; i++) {
        cout << "\nProduct " << i + 1 << endl;
        cout << "ID: " << ptr->id << endl;
        cout << "Name: " << ptr->name << endl;
        cout << "Price: $" << ptr->price << endl;
        cout << "Quantity: " << ptr->quantity << endl;

        ptr++; // Move pointer to the next product
    }

    double totalValue = 0;
    ptr = products; // Reset pointer to the first product
    for (int i = 0; i < 5; i++) {
        totalValue += ptr->price * ptr->quantity;
        ptr++;
    }
    cout<<endl;
    cout << "Total Value: $" << totalValue << endl;

    return 0;
}