// #include<iostream>
// using namespace std;
//     struct Drinkprice{
//         int matcha;
//         float juice;
//         int coffe ;
//     Drinkprice(int m, float j, float c):matcha(m),juice(j),coffe(c){
//         cout<<matcha<<endl;
//         cout<<juice<<endl;
//         cout<<coffe<<endl;
       
//     }
// };

// int main(){
    
    // Drinkprice p1;
    // p1.matcha = 3;
    // p1.juice = 3.35;
    // p1.coffe = 2.5;
    
    
    // Drinkprice p2 ={.matcha = 3, .juice= 3.4, .coffe=5};
    // cout<<p2.matcha<<endl;
    // cout<<p2.juice<<endl;
    // // cout<<p2.coffe<<endl;
//}



//there are 3 type constructer such as 
//1.defualt constructer 2.primary constructer 3.secondary constructer
// int my_function(){



//     return 0;
// }
// int main(){



//     return 0;
// }
#include<iostream>
using namespace std;

struct Drinkprice {
    float matcha;
    float juice;
    float coffe;

    Drinkprice(float m, float j, float c) : matcha(m), juice(j), coffe(c){
        cout<<matcha<<endl;
        cout<<juice<<endl;
        cout<<coffe<<endl;
    }
};
struct car{
    string brand;
    string model;
    int year;
car(string x, string y, int z){
    brand = x;
    model = y;
    year = z;
}
};

    //Secondary Constructor
struct Address {
    std::string street;
    std::string city;
    std::string state;
    std::string postal_code;
    std::string country;
    Address()=default;
    Address(string street, string city, string state, string postal_code):
            street(street),city(city),state(state),postal_code(postal_code){

            };
    Address(string street, string city, string state, string postal_code):
            street(street), city(city), state(state), postal_code(postal_code){

            };
};
void display(Address obj){
    cout<<obj.street<<", "<<obj.city<<", "<<obj.state<<", "<<obj.postal_code<<", "<<obj.country<<endl;
}

int main(){

    Address myaddress{};
    Address myaddress1("310 Preah Monivong Blvd", "Chamkar Mon", "Phnom Penh", "12301", "Cambodia");
    Address myaddress2("thirth group street","Pouk","SiemReap","0904");

    display(myaddress);
    display(myaddress1);
    display(myaddress2);

    // Drinkprice drink(6.5, 7.8, 9.6);
    // Drinkprice p1(3.0, 3.35, 2.5);

    // cout << p1.matcha << endl;
    // cout << p1.juice << endl;
    // cout << p1.coffe << endl;
    
    // car car1("Ford","x5",2022);
    // car car2("Xiaomi","y1",2026);

    // cout<<car1.brand<<" "<<car1.model<<" "<<car1.year<<endl;
    // cout<<car2.brand<<" "<<car2.model<<" "<<car2.year<<endl;
    

    return 0;
}
