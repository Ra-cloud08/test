#include <iostream>
#include <string>
using namespace std;

// Define Product structure
struct Product {
    int id;
    string name;
    double price;
};

int main() {
    Product product;

    cout << "Enter Product ID: ";
    cin >> product.id;

    cout << "Enter Product Name: ",
    cin>>product.name;

    cout << "Enter Product Price: ";
    cin >> product.price;

    Product* ptr = &product;

    cout << "\n===== Product Information =====" << endl;
    cout << "Product ID    : " << ptr->id << endl;
    cout << "Product Name  : " << ptr->name << endl;
    cout << "Product Price : $" << ptr->price << endl;
    // cout << "\nPrice through pointer: $" << ptr->price << endl;

    return 0;
}