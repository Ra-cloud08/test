#include<iostream>
#include<string.h>
// int main1(){

//     int a, b;
//     std::cout<<"Enter number a: ";
//     std::cin>>a;
//     std::cout<<"Enter number b: ";
//     std::cin>>b;
//     int sum = a+b;
//     std::cout<<"The result is: "<<sum<<std::endl;
//     return 0;

// }
int main(){
    char myarray[30], myarray1[30];
    char myarray2[30], myarray3[30];
    int length, i=0;
//change first and second character A and B
    std::cout<<"Please input a string: ";
    std::cin>>myarray;
    length = strlen(myarray);
    strcpy(myarray1, myarray);
    myarray1[0] = 'A';
    myarray1[1] = 'B';
    std::cout<<"1. Output: "<<myarray1<<std::endl;

//change 'z' to character first thirth fith 
    strcpy(myarray2, myarray);
    for(i=0; i<length; i++){
        if(i % 2 == 0){
            myarray2[i] = 'z';
        }
    }
    std::cout<<"2. Output: "<<myarray2<<std::endl;

//change 'z' to character first thirth fith 
    strcpy(myarray3, myarray);
    for(i=0; i<length; i++){
        if(i % 2 != 0){
            myarray3[i] = 'x';
        }
    }
    std::cout<<"3. Output: "<<myarray3<<std::endl;


    
    return 1;
}

