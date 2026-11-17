#include"swep_function.hpp"

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