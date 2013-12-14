#include <stdio.h>
#define MAX 10

/*****************************************************************************/
void mnoz(int A[][MAX],int w1,int k1,int B[][MAX],int w2,int k2,int C[][MAX])
 {int i,j,k;
   
   for (i=1;i<=w1;i++)
      for (j=1;j<=k2;j++)
         {
	   C[i][j]=0;      
           for (k=1;k<=w2;k++)
             C[i][j]+=A[i][k]*B[k][j];
         }      
 }
/*****************************************************************************/
void wypisz(int a[][MAX],int w,int k)
{ int i,j;

  for(i=1;i<=w;i++)
  {
    printf("\n\t| ");
    for(j=1;j<=k;j++)
      {
       printf("%d",a[i][j]);
       printf("\t\b");

      }
    printf("\b\b\b\b|");
  }
}
/*****************************************************************************/
void wpisz(int a[][MAX],int w,int k)
{ int i,j;

  for(i=1;i<=w;i++)
   for(j=1;j<=k;j++)
    {printf("[%d][%d]=",i,j);
     scanf("%d",&a[i][j]);}
}
/*****************************************************************************/

main()
{
 int A[MAX][MAX];
 int B[MAX][MAX];
 int C[MAX][MAX];
 int w1,k1,w2,k2;

 printf("\f\t\t\t\t\t\t Mnozenie macierzy A*B=C\n\n\n");

 printf("Podaj ilosc wierszy w macierzy A :");
 scanf("%d",&w1);
 printf("Podaj ilosc  kolumn w macierzy A :");
 scanf("%d",&k1);
 printf("Podaj ilosc wierszy w macierzy B :");
 scanf("%d",&w2);
 printf("Podaj ilosc  kolumn w macierzy B :");
 scanf("%d",&k2);
 


 if (k1==w2)
  {
  
 printf("\nwprowadz kolejno wartosci do macierzy A:\n");
 wpisz(A,w1,k1);
 printf("\nwprowadz kolejno wartosci do macierzy B:\n");
 wpisz(B,w2,k2);

 printf("\n\n Macierz A : \n");
 wypisz(A,w1,k1);

 printf("\n\n Macierz B : \n");
 wypisz(B,w2,k2);
 
  mnoz (A,w1,k1,B,w2,k2,C);
  printf("\n\n Macierz C : \n");
  wypisz(C,w1,k2);
   printf("\n\n\n");}
 else
  printf("Macierzy nie da sie wymnozyc!!!");

}
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
