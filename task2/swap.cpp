#include<iostream>
using namespace std;

void swep1(int a,int b){
    int c;
    c=a;
    a=b;
    b=c;
}

void swep2(int *a,int *b){
    int c;
    c=*a;
    *a=*b;
    *b=c;
}

void swep3(int &a,int &b){
    int c;
    c=a;
    a=b;
    b=c;
}


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
