#include<iostream>
#include<stdio.h>
#include<conio.h>

using namespace std; 

int main(){
  int i,n =100;
  //clrscr();
  cout <<"\nrozmiar zmiennej int i w bajtach " <<sizeof(i)<<"\n\n";
  cout <<"char = " <<sizeof(char) << " bytes\n";
  cout <<"short int = " <<sizeof(short int) << " bytes\n";
  cout <<"int = " <<sizeof(int) << " bytes\n";
  cout <<"long int = " <<sizeof(long int) << " bytes\n";

  cout <<"float = " <<sizeof(float) << " bytes\n";
  cout <<"double = " <<sizeof(double) << " bytes\n";
  cout <<"long duble = " <<sizeof(long double) << " bytes\n";

  //  cout <<"unsigned char = " <<sizeof(unsigned char) << "bytes\n";
  //cout <<"unsigned short int = " <<sizeof(unsigned shor int) << "bytes\n";
  //cout <<"unsigned int = " <<sizeof(unsigned int) << "bytes\n";
  //cout <<"unsigned long int = " <<sizeof(unsigned long int) << "bytes\n";

  cout <<"signed char = " <<sizeof(signed char) << " bytes\n";
  cout <<"signed short int = " <<sizeof(signed short int) << " bytes\n";
  cout <<"signed int = " <<sizeof(signed int) << " bytes\n";
  cout <<"signed long int = " <<sizeof(signed long int) << " bytes\n";

  
  printf("Masz %d częś%s%s\n", n, n == 1 ? "ć" : "ci","...");
  for(int j = 1; j <= n; j++)
    printf("%6d%c", j, ((j%10 == 0) || (j == n)) ? '\n' : ' ');
  cout << "\n";


  return 0;
}
