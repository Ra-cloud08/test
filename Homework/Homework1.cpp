/*write a small program to manage the flow of trade in beverage shop
    cofee = 2.5$
    juice = 3.35$
    matcha = 3$
    coffe and juice has in stock 10 glasses
*/

#include<iostream>

int main(){

    
    float Coffe , Juice, Matcha;
    //costumer oder
    std::cout<<std::endl;
    std::cout<<"======= Welcome ======"<<std::endl;
    std::cout<<"The costumer buy coffe: ";
    std::cin>>Coffe;
    std::cout<<"The costumehr buy juice: ";
    std::cin>>Juice;
    std::cout<<"The costumehr buy matcha: ";
    std::cin>>Matcha;

    //total drink sold
    float totalitem = Coffe + Juice + Matcha;
    std::cout<<"Total drinks order: "<<totalitem<<" glasses"<<std::endl;
    std::cout<<std::endl;
    
    //cost each type
    std::cout<<"======= Charge ======"<<std::endl;
    float coffeprice = Coffe * 2.5;
    float juiceprice = Juice * 3.35;
    float matchaprice = Matcha * 3;
    std::cout<<"coffe total price: "<<coffeprice<<"$"<<std::endl;
    std::cout<<"juice total price: "<<juiceprice<<"$"<<std::endl;
    std::cout<<"matcha total price: "<<matchaprice<<"$"<<std::endl;
    

    //total price
    float totalprice = coffeprice + juiceprice + matchaprice;
    std::cout<<"Total price: "<<totalprice<<"$"<<std::endl;
    std::cout<<std::endl;
    
    //costumer pay and get change
    std::cout<<"======= Payment ======="<<std::endl;
    float pay;
    std::cout<<"Costumer paying: ";
    std::cin>>pay;
    float change = pay - totalprice;
    std::cout<<"Costumer get change: "<<change<<"$"<<std::endl;
    std::cout<<std::endl;

    //split money between two worker
    std::cout<<"======= Distribute money between 2 worker ======="<<std::endl;
    float moneyperworker = totalprice / 2;
    std::cout<<"Once worker recieve: "<<moneyperworker<<"$"<<std::endl;
    std::cout<<std::endl;

    //stock report
    std::cout<<"======= Stock report ======="<<std::endl;
    int coffestock = 10, juicestock = 14, matchastock = 12;
        if(Coffe <= 10){
        coffestock -= Coffe;
        }
        if(Juice <= 20){
        juicestock -= Juice;
        }
        if(Matcha <= 20){
            matchastock -= Matcha;
        }
    std::cout<<"Remaining coffe stock: "<<coffestock<<" glasses"<<std::endl;
    std::cout<<"Remaining juice stock: "<<juicestock<<" glasses"<<std::endl;
    std::cout<<"Remaining matcha stock: "<<matchastock<<" glasses"<<std::endl;
    std::cout<<std::endl;

    //stock update
    std::cout<<"======= stock update ======="<<std::endl;
    coffestock += Coffe, juicestock += Juice, matchastock += Matcha;
    std::cout<<"Update coffe stock: "<<coffestock<<" glasses"<<std::endl;
    std::cout<<"Update juice stock: "<<juicestock<<" glasses"<<std::endl;
    std::cout<<"Update matcha stock: "<<matchastock<<" glasses"<<std::endl;
    std::cout<<std::endl;
    

    
    return 0;
}