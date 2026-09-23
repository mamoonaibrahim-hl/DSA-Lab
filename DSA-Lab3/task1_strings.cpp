#include<iostream>
#include<conio.h>
#include<string>
#include<cctype>
using namespace std;
//function to check if a string is palindrome or not 
bool isPalindrome(string str)
 {
    //two pointer approach to check if the string is palindrome or not
   int i=0,j=str.size()-1;
   //loop it before the middle of the string
    while(i<j)
    { //check if the character is alphanumeric or not 
      // skip the non-alphanumeric characters
      if(!isalnum(str[i])) 
      {
        i++;
        continue;
      }
      if(!isalnum(str[j]))
      {
        j--;
        continue;
      }
      //check if the character is same or not
      if(tolower(str[i]) != tolower(str[j]))
      {
        return false;
      }
      i++;
      j--;
    }
    return true;
  }
int main()
{ 
  string str;
  //take input from user
  cout<<"Enter a string: ";
  getline(cin,str);
  //call the function to check if the string is palindrome or not
  if(isPalindrome(str))
    cout<<"The string is a palindrome."<<endl;
  else
    cout<<"The string is not a palindrome."<<endl;
    return 0;
}
