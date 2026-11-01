#include <iostream>
#include<vector>
using namespace std;

int main(){
    int n,i,max,min,j,a=0;
    int q,zhong;
    std::vector<int>grade;
    cout <<"Please input grades:"<<endl;
    while(cin >> n){
        grade.push_back(n);
    }
    if(grade.empty()){
        cout <<"没东西"<<endl;
    }
    max = grade[0];
    for (i = 0;i<grade.size();i++){
        if(grade[i]>max){
            max = grade[i];
        }
    }
    cout <<"最大的数是："  <<max<<endl;


    min = grade[0];
    for(i=0;i<grade.size();i++){
        if(min>grade[i]){
            min = grade[i];
        }
    }
    cout << "最小的数是:"<<min<<endl;

    
    j = grade.size();
    for(i=0;i<grade.size();i++){
        a = a+grade[i];
    }
    cout << "平均数是：" <<a/j<<endl;


    for(i=0;i<grade.size()-1;i++){
        for(q=0;q<grade.size()-i-1;q++){
            if(grade[q]<grade[q+1]){
                zhong = grade[q];
                grade[q] = grade[q+1];
                grade[q+1]= zhong;
            }
        }
    }
    cout << "从大到小为："<<endl;
    for(i=0;i<grade.size();i++){
        cout << grade[i]<<" ";
    }
    cout <<endl;
    return 0;
}