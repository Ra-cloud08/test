// #include<iostream>
// #include<string>
// using namespace std;

//     // defualt constucter (build in)
//     struct StudentInformation{
    
//         string name;
//         int age;
//         string sex;
//         float height;
//         string ID;
//     };
//     struct Car{
//         string brand;
//         string model;
//         int year;
        
//         Car():brand (""), model (""), year(0){}
//     };
//     struct Phone{
//         string model;
//         int ram;
        
//         Phone() = default;
//     };
//     struct MyIinformation {
    
//         string name;
//         int age;
//         string sex;
//         float height;
//         string ID;
//     };
//     //constructer with parameter
//     struct Library{
//         int science_book;
//         int physic_book;
//         int history_book;
//         int chemical_book;

//         Library(int science, int physic,int history,int chemical)
//         :science_book(science),physic_book(physic),history_book(history),chemical_book(chemical){

//         }
//     };
   
// int main(){

//     StudentInformation p1;
//     p1.name = "Theara";
//     p1.age = 18;
//     p1.height = 1.61;
//     p1.sex = "Male";
//     p1.ID = "12940294";
//     cout<<p1.name<<" "<<p1.age<<" "<<p1.height<<" "<<p1.sex<<" "<<p1.ID<<endl;

//     Car p2;
//     p2.brand = "Lexus";
//     p2.model = "570";
//     p2.year = 2024;
//     cout<<p2.brand<<endl;
//     cout<<p2.model<<endl;
//     cout<<p2.year<<endl;
    
//     Phone p3;
//     p3.model = "17 pro max";
//     p3.ram = 8;
//     cout<<p3.model<<" "<<p3.ram<<endl;

//     Library p4 (2430, 4508, 785, 452);
//     cout<<p4.science_book<<" "<<p4.physic_book<<" "<<p4.history_book<<" "<<p4.chemical_book<<endl;
    

//     return 0;
// }
