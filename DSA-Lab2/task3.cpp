#include<iostream>
using namespace std;
int main()
{
    int list[5]={3,6,9,12,15};
	int *pArr= list;
//printing array list using only pointer variable pArr
cout<<"Printing array list using pointer variable pArr: "<<endl;
    for(int i=0;i<5;i++)
    {
        //extracting the value of array list using pointer variable pArr
        //showing element at each index
        cout<<"Element at index "<<i<<" is: "<<*(pArr+i)<<" "<<endl;
        
    }
    cout<<endl;
    return 0;
}