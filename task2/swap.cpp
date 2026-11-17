#include<iostream>
#include"swep_function.hpp"
using namespace std;




int main(){
    int a,b;
    a = 10;
    b = 20;
    swep1(a,b);
    cout << "传值的输出：" << endl;
    cout << "a=" << a <<endl;
    cout << "b=" << b <<endl;


    swep2(&a,&b);
    cout << "传指针的输出：" << endl;
    cout << "a=" << a <<endl;
    cout << "b=" << b <<endl;

    
    a=10;
    b=20;
    swep3(a,b);
    cout << "传引用的输出：" << endl;
    cout << "a=" << a <<endl;
    cout << "b=" << b <<endl;
}
