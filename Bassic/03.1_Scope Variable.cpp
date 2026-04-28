#include<iostream>
    //Global scope
    int n = 20;
int main(){

    //Local scope
    int n = 10;
    std::cout<<::n<<std::endl;
    
    return 0;
}