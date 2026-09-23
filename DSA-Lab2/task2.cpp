#include<iostream>
using namespace std;
void main(){
int a, *pa;      // Statement 1
pa = &a;           // Statement 2
cout<<"pa = &a --> pa = "<<pa<<endl<<endl;
pa = pa + 1;      // Statement 3
cout<<"pa = pa + 1 --> pa = "<<pa<<endl<<endl; 
	
}

