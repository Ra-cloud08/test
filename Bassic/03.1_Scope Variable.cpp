#include<iostream>

//Global scope
int n = 10;
int main(){

    //Local Scope
    int n = 20;
    //Ouput 20
    std::cout<<::n<<std::endl;
    return 0;
}