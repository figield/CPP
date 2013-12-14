/*
     oznaczenia:
     0 - wogol nie ma bomb,lub zostaly odkryte ale moga byc wokol liczby
     1-8 - istnieje gdzies 1-8 bomb(a) wogol tej liczby
     * - bomba
     . - pole nieodkryte
     
     oznaczenia dodatkowe:
     # - pole z bomba, takie bomby stawiane sa wokol liczb niezaleznych od innych pol
     x - pole podejrzane o bombe
     @ - bomba dawana jako uzupelnienie na koncu algorytmu
     L - pod tym polem NIE moze byc bomby , ale moze byc liczba
     K - pod tym polem NIE moze byc bomby , ale moze byc liczba (inne oznacznie dla potrzeb rekursji)   
    
    I Zalozenie:
      jesli na poczatku kolo danej liczby sa bomby to liczba zostaje 
      dekrementowana o ilosc bomb wokol niej. Czyli po takim przejsciu  
      przez tablice liczby beda oznaczac ilosc bomb NIEODKRYTYCH 
      jeszcze przy danej liczbie
    
      przyklad planszy:
         
                   5 5 3
		   .....
		   1..2.
		   .*...
		   .2..0
		   .....
    
     etapy algorytmu:
     1)szukanie oczywistych zaleznosci
       * jesli ilosc pol podejrzanych i odkrytych bomb rowna jest szukanej liczbie oraz kazde pole z liczba jest przypisane do ktrejs bomb(y)-> koniec:)  
       * jesli jest znaleziona bomba przy jedynce: zamiana 1->0, i oznaczenie pol wokol na 'L' 
       * analogicznie dla pozostalych pol ktorych liczba jest rowna sasiadujacym wkolo bomba
       * jesli wystepuje '0': wpisanie na nieodkryte pola wogol 'L'
       * dla rekursji zmiany pol w.g powyzszego sposobu moga byc trwale
       
       czyli:            
                   LL...
		   0L.2.
		   L*.LL
		   .1.L0
		   ...LL
       
     2)etapy rekursji z powrotami                 
        c.d. w kodzie
   
   
*/

#include <stdio.h>
#define LIM 1000
#define TEST 0 /* dla TEST ==1 pokazuje rozbudowany wynik programu */
long lim_actual = 0;
long lim_max = LIM;
char **macA=NULL;
int  **lista=NULL;
int koniec_listy=0;
#if TEST 
char **macB=NULL;
int  **macC=NULL;
#endif
int *stack=NULL; 
int wA,kA,wsk_L,ilosc,ilosc_nieodkrytych;
int buff[8];
//int silnia[]={1,1,2,6,24,120,720,5040,40320}; 
/********************* generuje  macierz planszy *******************************************************************/
 int generate(char ***tab2D,int w,int k)
 { 
  int i,odp;
  
    odp=1;
   *tab2D=(char **)malloc(w*sizeof(char *)); 
	        
   for(i=0;i<w;i++)
    {
     (*tab2D)[i]=(char *)malloc(k*sizeof(char));
     if((*tab2D)[i] ==0)
      {
        /*  nie ma miesjca na zaalokowanie takiego obszaru pamieci */
         i=w;
	 odp=0;
      }
    }
  return odp;    
}
/********************* generuje  liste zbiorow  ****************************************************************/
 int generate_list(int ***tab2D,int k)
 { 
  int i,odp;
  
    odp=1;
   *tab2D=(int **)malloc(2*sizeof(int *)); 
	        
   for(i=0;i<2;i++)
    {
     (*tab2D)[i]=(int *)malloc(k*sizeof(int));
     if((*tab2D)[i] ==0)
      {
        /*  nie ma miesjca na zaalokowanie takiego obszaru pamieci */
         i=2;
	 odp=0;
      }
    }
  return odp;    
}
/*********************  dodaje na koniec listy  *******************************************************************/
inline void put_lista(int w,int k){

            lista[0][koniec_listy]=w;
            lista[1][koniec_listy]=k;
            koniec_listy++;

}
/********************* generuje  macierz pamieci kombinacji ****************************************************/
#if TEST
 int generate_mem_komb(int ***tab2D,int w,int k)
 { 
  int i,odp;
  
    odp=1;
   *tab2D=(int **)malloc(w*sizeof(int *)); 
	        
   for(i=0;i<w;i++)
    {
     (*tab2D)[i]=(int *)malloc(k*sizeof(int));
     if((*tab2D)[i] ==0)
      {
        /*  nie ma miesjca na zaalokowanie takiego obszaru pamieci */
         i=w;
	 odp=0;
      }
    }
  return odp;    
}
#endif
/********************* generuje  stos  *******************************************************************/
 int generate_stack(){ 
   
   stack=(int *)malloc(LIM*sizeof(int)); 
   
   if (stack!=NULL) return 1;
   else return 0;      
 }
/*********************  daje na  stos  *******************************************************************/
inline void put(int wart){

   if (lim_actual<=(lim_max-1)){
     stack[lim_actual]=wart;
     lim_actual++;
    }
   else{
     lim_max=lim_max+LIM;
     stack=(int *)realloc(stack,lim_max*sizeof(int));
     stack[lim_actual]=wart;
     lim_actual++;
    }       
}
/********************* zdejmuje ze stosu   *******************************************************************/

#define pop() stack[--lim_actual]

/*****************wczytaj dane do macierzy ***************************************************/
 int wczytajdane(char **tab2D, int w,int k){
  int i=0,j=0,n,kontrol=0;
  char zn; 
  n=w*k;
      
  while ((zn=getchar())!=EOF){ 
    if ((zn>='0'&&zn<='8')||zn=='*'||zn=='.'||zn=='\n'){

       if (zn!='\n'){     /*uwzglednij spacje na koncu !, zbyt duzo znakow w 1 linii */
	   if (++kontrol>n) return 0;  
	   
	   tab2D[i][j]=zn;                            
           j=(++j)%k;
	  
	  }
       else i=(i+1)%w;    	       	  
     }   
  }
   if (kontrol<n) return 0;
   return 1;
}   

/******************* kopiowanie macierzy  ***************************************************/
#if TEST 
void kopiuj_macierz(char** tab2Dd,char** tab2Ds){
 int w,k;

     for(w=0;w<wA;w++)
      for(k=0;k<kA;k++)
        tab2Dd[w][k]=tab2Ds[w][k];

}
#endif

/***** funkcje dekrementujaca wartosci pol i wstawiajaca 'L' wokol zera ******************************/
int dekrementuj_i_wstaw_L_wokol_0(char **tab2D){
  int n,m,i,j,dn,gn,dm,gm,liczba;
    
     for(i=0;i<wA;i++)
      for(j=0;j<kA;j++){
        if(tab2D[i][j]>='0' && tab2D[i][j]<='8'){
            liczba=(int)tab2D[i][j] - 48;
	    
	    
	    if((dn=i-1)<0) dn=0;
	    if((gn=i+1)>(wA-1)) gn=wA-1; 
            if((dm=j-1)<0) dm=0;
	    if((gm=j+1)>(kA-1)) gm=kA-1;
	    
	    
	         for(n=dn;n<=gn;n++)
                   for(m=dm;m<=gm;m++)
		    if(tab2D[n][m]=='*'){
		       liczba--;    /*kolo liczby nie moze byc wiecej bomb niz wskazuje liczba!*/ 
		       if(liczba<0) return 0;
		      }                                
		 
		 if(liczba==0){
		    for(n=dn;n<=gn;n++)
                     for(m=dm;m<=gm;m++)
		      if(tab2D[n][m]=='.') tab2D[n][m]='L';
	           }
	       
	    tab2D[i][j]=(char)(liczba + 48);
          }
      }
 
 return 1;      
}
/*********** funkcja uzupelniajaca pojedyncze liczby lub graniczace tylko z bombami **********************/
/*                               _          _           _           _
	 .....       .....      |3..       |....       |.3..       |...
         .....       .....       ...        .3..        ....        3..
         ..3..  lub  ..1*.  lub  ...  lub   ....  lub   ....  lub   ...   podobnie w innych rogach i na krawedziach
         .....       ..*..                  ....                    ...
         .....       .....
                                    
*/
int uzupelnij_pojedyncze_liczby(char **tab2D){
   int n,m,i,j,dn,gn,dm,gm,kon,liczba;
    
     for(i=0;i<wA;i++){
      for(j=0;j<kA;j++){
        if(tab2D[i][j]>='1' && tab2D[i][j]<='8'){
            liczba=(int)tab2D[i][j] - 48;
            
	    if((dn=i-2)<0) dn=0;
	    if((gn=i+2)>(wA-1)) gn=wA-1; 
            if((dm=j-2)<0) dm=0;
	    if((gm=j+2)>(kA-1)) gm=kA-1; 
            
	    /*sprawdzenie czy liczba jest sama...,czy tez zalezna od innych liczb*/
             kon=1;
	     for(n=dn;n<=gn;n++){
	        for(m=dm;m<=gm;m++){
	          if(!(n==i && m==j)){
		    if(tab2D[n][m]=='.'||tab2D[n][m]=='*'|| tab2D[n][m]=='L'||tab2D[n][m]=='0'||tab2D[n][m]=='#'||tab2D[n][m]=='@'/*???*/) 
		        {/* nic nie zrobi czyli akceptuje*/}
	            else{ 
	                 kon=0;
	                 break;
		        }          
		  }
		}    
	        if(kon==0) break;
	     }
	     
           /*jezeli jest sama to wpisanie wogol liczby tyle bomb ile wynosi jej wartosc*/
	    if(kon){ 
	      if((dn=i-1)<0) dn=0;
	      if((gn=i+1)>(wA-1)) gn=wA-1; 
              if((dm=j-1)<0) dm=0;
	      if((gm=j+1)>(kA-1)) gm=kA-1; 
              
	      for(n=dn;n<=gn;n++){          
	        for(m=dm;m<=gm;m++){
	           if(tab2D[n][m]=='.' && liczba>0){
		     tab2D[n][m]='#';
		     --liczba;
		     --ilosc_nieodkrytych;
		    }
		    
	          if(liczba==0) break;
		}      
	       if(liczba==0) break;
	     }
	     if(liczba!=0){ 
	      /*printf("\nERROR! Bledne dane, nie ma miejsca na bombe(y)\n");*/
              return 0; 
	    }else 
	       for(n=dn;n<=gn;n++)        
	         for(m=dm;m<=gm;m++)
	            if(tab2D[n][m]=='.') tab2D[n][m]='L';	  
          
	  tab2D[i][j]=(char)(liczba + 48);
         }
        }
      }
     } 	 

 if(ilosc!=0){
   if (ilosc_nieodkrytych<0) return 0;
  }
 
return 1; 
}
/*********** funkcja zaspokajajaca liczby wokol ktorych jest tyle kropek ile ma byc bomb ****************/
int zaspakajaj(char **tab2D){

  int w,k,n,m,n2,m2,n3,m3,dn,gn,dm,gm,dn2,gn2,gm2,dm2,dn3,gn3,gm3,dm3,liczba,liczba2;
  int kr=0,zmiana=1;        
  
while(zmiana!=0){
#if TEST
 printf("nowa petla...\n");
#endif 
 zmiana=0;
  
 for(w=0;w<wA;w++){
  for(k=0;k<kA;k++){
    if(tab2D[w][k]>='1' && tab2D[w][k]<='8'){
     
     liczba=(int)tab2D[w][k] - 48;
     
     /*granice pierwszego kwadratu - wokol wrzuconej tu liczby */
     if((dn=w-1)<0) dn=0;
     if((gn=w+1)>(wA-1)) gn=wA-1; 
     if((dm=k-1)<0) dm=0;
     if((gm=k+1)>(kA-1)) gm=kA-1;
	
     kr=0;
     for(n=dn;n<=gn;n++)  
       for(m=dm;m<=gm;m++)
	  if(tab2D[n][m]=='.'){
             kr++;     
            }
    
/*--------------------------------------*/
     if(kr<liczba)
          return 0;
/*--------------------------------------*/      
     else if(kr==liczba){    
         
          zmiana=1;
         
	  for(n=dn;n<=gn;n++){  
            for(m=dm;m<=gm;m++){
              if(tab2D[n][m]=='.'){
	        
	        tab2D[n][m]='#';
		 
		--liczba;
		--ilosc_nieodkrytych;
		
	        /*granice drugiego kwadratu(wokol '#')*/
		if((dn2=n-1)<0) dn2=0;
                if((gn2=n+1)>(wA-1)) gn2=wA-1; 
                if((dm2=m-1)<0) dm2=0;
                if((gm2=m+1)>(kA-1)) gm2=kA-1;
		
		/*trzeba dekrementowac liczby wokol bomby! '#' */
		for(n2=dn2;n2<=gn2;n2++)            
	          for(m2=dm2;m2<=gm2;m2++)
		     if(tab2D[n2][m2]>='1' && tab2D[n2][m2]<='8'){
                           liczba2=(int)tab2D[n2][m2] - 48;
	                   
			   liczba2--;
			  
			   /*jezeli liczba wynosi zero to trzeba w wolne puste miejsca wstawic 'L'*/
			   if(liczba2==0){ 
			     /*granice trzeciego kwadratu (wokol dekrementowanej liczby)*/
			     if((dn3=n2-1)<0) dn3=0;
                             if((gn3=n2+1)>(wA-1)) gn3=wA-1; 
                             if((dm3=m2-1)<0) dm3=0;
                             if((gm3=m2+1)>(kA-1)) gm3=kA-1;
			   
		          
		             for(n3=dn3;n3<=gn3;n3++)
                              for(m3=dm3;m3<=gm3;m3++)
		                if(tab2D[n3][m3]=='.'){
			           tab2D[n3][m3]='L'; 
                                 }        
	                    }
	       
	                  tab2D[n2][m2]=(char)(liczba2 + 48);
			  
                     }
                 }	    
                if(liczba==0) break;   
               }
	      if(liczba==0) break;
             }
          }

      }
    }
  }
}

if(ilosc>0){
  if(ilosc_nieodkrytych < 0) return 0;
 }

return 1; 
}

/************************ funkcja sprawdzajaca czy dany element jest w zbiorze *********************************/
int sprawdz_czy_nalezy(int **list,int w,int k){
  int i=0,T=0;

     while((i<=koniec_listy)&&(!T)){ 
        if(list[0][i]==w && list[1][i]==k) T=1;
        i++;
       }    

return T;
}

/************************ funkcja sprawdzajaca czy dane alementy sa od siebie zalezne *********************************/

int czy_zalezny_sasiad(char **lista,int w1,int k1, int w2, int k2){
 int W1,W2,K1,K2;
       
      /* X={L,*,#} */

      
  if(k1-k2==0){ /*jezeli pkt sa w jednej kolumnie...      
                                                           1|  |1     1  
			                                  XX|  |XX   XXX
                                                           2|  |2     2      */
      if(w1<w2){
           W1=w1;
           W2=w2;
      }else{ 
           W1=w2;
           W2=w1;
      }
      
      K1=k1;
      K2=k2;
         						   
      if(lista[W1+1][K1]=='L' || lista[W1+1][K1]=='*' || lista[W1+1][K1]=='#' || lista[W1+1][K1]=='0')
	  if(K1==kA-1)
	    if(lista[W1+1][K1-1]=='L' || lista[W1+1][K1-1]=='*' || lista[W1+1][K1-1]=='#'|| lista[W1+1][K1-1]=='0')  
	      return 1;
            else return 0;
          else if (K1==0)
            if(lista[W1+1][K1+1]=='L' || lista[W1+1][K1+1]=='*' || lista[W1+1][K1+1]=='#' || lista[W1+1][K1+1]=='0')  
	      return 1;
            else return 0;
	  else  
	    if((lista[W1+1][K1+1]=='L' || lista[W1+1][K1+1]=='*' || lista[W1+1][K1+1]=='#' || lista[W1+1][K1+1]=='0') && (lista[W1+1][K1-1]=='L' || lista[W1+1][K1-1]=='*' || lista[W1+1][K1-1]=='#'|| lista[W1+1][K1-1]=='0'))  
	      return 1;
            else return 0;
      else return 0;    
  
  }
  else if(abs(k1-k2)==1){ /* jezeli pkt NIE sa w jednej kolumnie...
                                                        2   2      1     1 
                                                       XX   XX     XX   XX 
                                                       1     1      2   2     */
      if(w1<w2){
           W1=w1;
	   K1=k1;
           W2=w2;
	   K2=k2; 
      }else{ 
           W1=w2;
	   K1=k2;
           W2=w1;
	   K2=k1; 
      }
    
      if(lista[W1+1][K1]=='L' || lista[W1+1][K1]=='*' || lista[W1+1][K1]=='#' || lista[W1+1][K1]=='0')
	{  
          if(K1>K2)
	    {
	    if(lista[W1+1][K1-1]=='L' || lista[W1+1][K1-1]=='*' || lista[W1+1][K1-1]=='#' || lista[W1+1][K1-1]=='0')  
	      return 1;
            else return 0;
	    }
          else if(K1<K2)
	    {
            if(lista[W1+1][K1+1]=='L' || lista[W1+1][K1+1]=='*' || lista[W1+1][K1+1]=='#' || lista[W1+1][K1+1]=='0')  
	      return 1;
            else return 0;
	    }
	}    
      else return 0;   
  
  }
  else if(abs(k1-k2)==2 && abs(w1-w2)==2){ /* jezeli pkt NIE sa w jednej kolumnie, ani tym samym wierszu ...
  
                                                                    1       1   2      2
                                                                     X     X     X    X
                                                                      2   2       1  1         */

      if(w1<w2){
           W1=w1;
	   K1=k1;
           W2=w2;
	   K2=k2; 
      }else{ 
           W1=w2;
	   K1=k2;
           W2=w1;
	   K2=k1; 
      }
   
      if(K1<K2)
        if(lista[W1+1][K1+1]=='L' || lista[W1+1][K1+1]=='*' || lista[W1+1][K1+1]=='#' || lista[W1+1][K1+1]=='0')
	  return 1;
        else return 0;
      else 
        if(lista[W1+1][K1-1]=='L' || lista[W1+1][K1-1]=='*' || lista[W1+1][K1-1]=='#' || lista[W1+1][K1-1]=='0')  
	  return 1;
        else return 0;
      
  }
  else if(abs(k1-k2)==2 && abs(w1-w2)==0){ /* jezeli pkt sa w tym samym wierszu ...
  
                                                        X     ---       X                                       
                                                       1X2    1X2      2X1           
                                                       ---     X        X              */

      if(k1<k2){
	   K1=k1;
	   K2=k2; 
      }else{ 
	   K1=k2;
	   K2=k1; 
      }
      
      W1=w1;
      W2=w2;
    
      if(lista[W1][K1+1]=='L' || lista[W1][K1+1]=='*' || lista[W1][K1+1]=='#'|| lista[W1][K1+1]=='0')
	  if(W1==wA-1)
	    if(lista[W1+1][K1+1]=='L' || lista[W1+1][K1+1]=='*' || lista[W1+1][K1+1]=='#' || lista[W1+1][K1+1]=='0')  
	      return 1;
            else return 0;
          else if (W1==0)
            if(lista[W1-1][K1+1]=='L' || lista[W1-1][K1+1]=='*' || lista[W1-1][K1+1]=='#' || lista[W1-1][K1+1]=='0')  
	      return 1;
            else return 0;
	  else  
	    if((lista[W1+1][K1+1]=='L' || lista[W1+1][K1+1]=='*' || lista[W1+1][K1+1]=='#' || lista[W1+1][K1+1]=='0') && (lista[W1-1][K1+1]=='L' || lista[W1-1][K1+1]=='*' || lista[W1-1][K1+1]=='#' || lista[W1-1][K1+1]=='0'))  
	      return 1;
            else return 0;
      else return 0;       
  
  }else {/* jezeli pkt NIE  sa w tym samym wierszu ...
  
                                                        X2   X1  2X   1X                                       
                                                       1X   2X    X1   X2           
                                                                           */

      if(k1<k2){
           W1=w1;
	   K1=k1;
           W2=w2;
	   K2=k2; 
      }else{ 
           W1=w2;
	   K1=k2;
           W2=w1;
	   K2=k1; 
      }
    
      if(lista[W1][K1+1]=='L' || lista[W1][K1+1]=='*' || lista[W1][K1+1]=='#'|| lista[W1][K1+1]=='0')
	{ 
         if(W1>W2)
	    if(lista[W1-1][K1+1]=='L' || lista[W1-1][K1+1]=='*' || lista[W1-1][K1+1]=='#' || lista[W1-1][K1+1]=='0')  
	      return 1;
            else return 0;
          else if (W1<W2)
            if(lista[W1+1][K1+1]=='L' || lista[W1+1][K1+1]=='*' || lista[W1+1][K1+1]=='#' || lista[W1+1][K1+1]=='0')  
	      return 1;
            else return 0;
	}    
      else return 0;  
  
  }
  
  
}
/************************ funkcja wydzielajaca niezalezne podzbiory liczb **************************************/

void tworz_zbiory(char **tab2D){

 int ii,i,jj,j,di,gi,dj,gj,n,m,dn,gn,dm,gm,wsk; 

 for(ii=0;ii<wA;ii++)
  for(jj=0;jj<kA;jj++)
     if(tab2D[ii][jj]>='1' && tab2D[ii][jj]<='8')
      if(!sprawdz_czy_nalezy(lista,ii,jj)){
            i=ii;
	    j=jj;
            put_lista(999,999);
#if TEST	    
	    printf("Nowy zbior.....\n");
#endif	    
	    put_lista(i,j);
            wsk=koniec_listy-1;
            
	    while(wsk<koniec_listy){
	     // printf("biore element z listy\n");
	      i=lista[0][wsk];
	      j=lista[1][wsk];
	      wsk++;
	    
	      /*granice pierwszego kwadratu (o pr.== 1) - wokol badanej tu liczby */
              if((di=i-1)<0) di=0;
              if((gi=i+1)>(wA-1)) gi=wA-1; 
              if((dj=j-1)<0) dj=0;
              if((gj=j+1)>(kA-1)) gj=kA-1;
	
     
              for(n=di;n<=gi;n++)  
                for(m=dj;m<=gj;m++)
	            if(!(i==n && j==m) && tab2D[n][m]>='1' && tab2D[n][m]<='8')
		     if(!sprawdz_czy_nalezy(lista,n,m))
	                put_lista(n,m);
		   
		
	      /*granice drugiego kwadratu (o pr.== 2) - wokol badanej tu liczby */
              if((di=i-2)<0) di=0;
              if((gi=i+2)>(wA-1)) gi=wA-1; 
              if((dj=j-2)<0) dj=0;
              if((gj=j+2)>(kA-1)) gj=kA-1;
	 
     
              for(n=di;n<=gi;n++)  
                for(m=dj;m<=gj;m++)
	            if((abs(i-n)==2 || abs(j-m)==2) && tab2D[n][m]>='1' && tab2D[n][m]<='8'){
		    if(!sprawdz_czy_nalezy(lista,n,m)){ 
		      // printf("macA[%d][%d]+macA[%d][%d]\n",i,j,n,m);
		       if(!czy_zalezny_sasiad(macA,i,j,n,m)){
	                  put_lista(n,m);
			    //printf("\t zaleza!\n");
			  }
			//  else printf("\tNIE zaleza!\n");
		      }	  
	  	    }    
	       //printf("wsk=%d , koniec_listy=%d \n",wsk,koniec_listy);	
           }

      }

}
/**************************** funkcja liczaca odkryte bomby  **********************************************/
int ilosc_odkrytych_bomb(char **tab2D){
 int i,j;
 int ilosc=0; 
   for(i=0;i<wA;i++)
      for(j=0;j<kA;j++)
        if(tab2D[i][j]=='*'||tab2D[i][j]=='#'||tab2D[i][j]=='x'||tab2D[i][j]=='@')
               ++ilosc;
	       
 return ilosc;	       
}
/*********************** funkcja liczaca konkretne znaki  **********************************************/
/*
int ilosc_konkretnych_znakow(char **tab2D,char zn){
 int i,j,k;
 int ilosc=0; 

   for(i=0;i<wA;i++)
      for(j=0;j<kA;j++)
        if(tab2D[i][j]==zn)
               ++ilosc;
	       
 return ilosc;	       
}
*/
/************************ wyswietl macierz wynikowa na ekranie*****************************************************/ 
 void wyswietl_wynik(char **tab2D,int w,int k){ 
  int i,j;
  
  for(i=0;i<w;i++)
   for(j=0;j<k;j++)     
     if(tab2D[i][j]=='*'||tab2D[i][j]=='#'||tab2D[i][j]=='x'||tab2D[i][j]=='@'){
        printf("*");
        if(!((j+1)%k))
          printf("\n");
      }
    else{
        printf(".");
        if(!((j+1)%k))
          printf("\n");
     }  
     
   printf("\0");  
 } 
/************************ wyswietl macierz  na ekranie*****************************************************/ 
#if TEST  
void wyswietl_calosc(char **tab2D,int w,int k){ 
  int i,j;
   
  for(i=0;i<w;i++)
   for(j=0;j<k;j++){ 
     printf("%c",tab2D[i][j]);
     if(!((j+1)%k))
     printf("\n");
    }  
 } 

/************************ wyswietl macierz kombinacji na ekranie*****************************************************/   
void wyswietl_kombinacje(int **tab2D,int w,int k){ 
  int i,j;
   
   printf("\nMacierz kombinacji:\n");
  for(i=0;i<w;i++)
   for(j=0;j<k;j++){ 
     printf("%d,",tab2D[i][j]);
     if(!((j+1)%k))
     printf("\n");
    }  
 } 

/************************ wyswietl liste grup na ekranie*****************************************************/   
void wyswietl_liste(int **lista){ 
  int i;
   
  for(i=0;i<koniec_listy;i++){ 
     if(lista[0][i]==999)
     printf("\n nowy zbior:\n");
     printf("%d:[%d,%d],",i,lista[0][i],lista[1][i]);
    }
    
  printf("\n\ndlugosc listy: %d/%d\n\n",i,wA*kA);      
 } 

/************************ wyswietl macierz sprawdzajaca na ekranie*****************************************************/
void wyswietl_spr(char **tab2D, char **tab2Dspr){ 
  int i,j,T=1,liczba,dn,gn,dm,gm,il,n,m;
   
  for(i=0;i<wA;i++)
   for(j=0;j<kA;j++)   
     if(tab2Dspr[i][j]>='0' && tab2Dspr[i][j]<='8'){}
     else if(tab2D[i][j]=='*'||tab2D[i][j]=='#'||tab2D[i][j]=='x'||tab2D[i][j]=='@')
       tab2Dspr[i][j]='*';
     else tab2Dspr[i][j]='.';  
  
  for(i=0;i<wA;i++)
   for(j=0;j<kA;j++){     
        printf("%c",tab2Dspr[i][j]);
        if(!((j+1)%kA))
          printf("\n");
      } 
      
  for(i=0;i<wA;i++){
   for(j=0;j<kA;j++){
      if(tab2Dspr[i][j]>='0' && tab2Dspr[i][j]<='8'){
        il=0; 
        liczba=(int)tab2Dspr[i][j] - 48;
        if((dn=i-1)<0) dn=0;
        if((gn=i+1)>(wA-1)) gn=wA-1; 
        if((dm=j-1)<0) dm=0;
        if((gm=j+1)>(kA-1)) gm=kA-1;
	
        for(n=dn;n<=gn;n++)  
         for(m=dm;m<=gm;m++)
	   if(tab2Dspr[n][m]=='*'){
              il++;     
            }
        
	if(il==liczba) T=1;
	else T=0;  
      }
     if(T==0) break;
    } 
   if(T==0) break;
  }

   if(T)
     printf("\nKazda liczba zostala zaspokojona!\n");
   else
     printf("\nBlad! Istnieja niezaspokojone liczby: [%d][%d]\n",i,j);
}
#endif 

/*********************************  silnia  **********************************************************/
/* niekoniecznie gorzej....*/
inline int silnia(int n){  
 int i,wart=1;
    
    if (n>1)
      for (i=1;i<=n;i++)
          wart=wart*i;

return wart;
}
 
/*********************************  kolejna kombinacja ******************************************************/
void nowa_kombinacja(int nr_kombinacji,int kr, int liczba){
 
int w1,w2,w3,w4,w5,w6,w7,nr;

   for(nr=0;nr<8;nr++) 
      buff[nr]=0;
   nr=0;

/*petli musi byc tyle ile jest liczb (bomb) */
   
   if (liczba==1){
       for(w1=0;w1<=(kr-1);w1++)
	     if ((++nr)==nr_kombinacji){ 
	        buff[w1]=1;
	        break;
             }
      }
    else if (liczba==2){  
       for(w1=0;w1<=(kr-2);w1++){
         for(w2=1;w2<=(kr-1);w2++)
	     if(w1<w2)
              if((++nr)==nr_kombinacji){ 
	        buff[w1]=1;
		buff[w2]=2;
	        break;
              }
           if(nr==nr_kombinacji) break;
          }	     
      }
    else if (liczba==3){  
       for(w1=0;w1<=(kr-3);w1++){
         for(w2=1;w2<=(kr-2);w2++){
	   for(w3=2;w3<=(kr-1);w3++)
	       if(w1<w2&&w2<w3)
                if((++nr)==nr_kombinacji){ 
	          buff[w1]=1;
	          buff[w2]=2;
	          buff[w3]=3;
	          break;
                }
             if(nr==nr_kombinacji) break;
            }
          if(nr==nr_kombinacji) break;	     
         }     
      }
    else if (liczba==4){  
       for(w1=0;w1<=(kr-4);w1++){
         for(w2=1;w2<=(kr-3);w2++){
	   for(w3=2;w3<=(kr-2);w3++){
	     for(w4=3;w4<=(kr-1);w4++)
                 if(w1<w2&&w2<w3&&w3<w4)
	          if((++nr)==nr_kombinacji){ 
	            buff[w1]=1;
		    buff[w2]=2;
		    buff[w3]=3;
		    buff[w4]=4;
	            break;
                   }
	        if(nr==nr_kombinacji) break;
	       } 
             if(nr==nr_kombinacji) break;
            }
          if(nr==nr_kombinacji) break;	     
         }     
      }
    else if (liczba==5){  
       for(w1=0;w1<=(kr-5);w1++){
         for(w2=1;w2<=(kr-4);w2++){
	   for(w3=2;w3<=(kr-3);w3++){
	     for(w4=3;w4<=(kr-2);w4++){
	       for(w5=4;w5<=(kr-1);w5++)
	           if(w1<w2&&w2<w3&&w3<w4&&w4<w5)
                    if((++nr)==nr_kombinacji){ 
	              buff[w1]=1;
		      buff[w2]=2;
		      buff[w3]=3;
		      buff[w4]=4;
	              buff[w5]=5;
		      break;
                     }
		   if(nr==nr_kombinacji) break;
		  }
	        if(nr==nr_kombinacji) break;
	       } 
             if(nr==nr_kombinacji) break;
            }
          if(nr==nr_kombinacji) break;	     
         }     
      }
    else if (liczba==6){  
       for(w1=0;w1<=(kr-6);w1++){
         for(w2=1;w2<=(kr-5);w2++){
	   for(w3=2;w3<=(kr-4);w3++){
	     for(w4=3;w4<=(kr-3);w4++){
	       for(w5=4;w5<=(kr-2);w5++){
	         for(w6=5;w6<=(kr-1);w6++)
		     if(w1<w2&&w2<w3&&w3<w4&&w4<w5&&w5<w6)
                      if((++nr)==nr_kombinacji){ 
	                buff[w1]=1;
		        buff[w2]=2;
		        buff[w3]=3;
		        buff[w4]=4;
	                buff[w5]=5;
			buff[w6]=6;
		        break;
                       }
		     if(nr==nr_kombinacji) break;
		    }    
		  if(nr==nr_kombinacji) break;
		 }
	       if(nr==nr_kombinacji) break;
	      } 
            if(nr==nr_kombinacji) break;
           }
         if(nr==nr_kombinacji) break;	     
        }     
      }
    else if (liczba==7){  
       for(w1=0;w1<=(kr-7);w1++){
         for(w2=1;w2<=(kr-6);w2++){
	   for(w3=2;w3<=(kr-5);w3++){
	     for(w4=3;w4<=(kr-4);w4++){
	       for(w5=4;w5<=(kr-3);w5++){
	         for(w6=5;w6<=(kr-2);w6++){
		   for(w7=6;w7<=(kr-1);w7++)
		       if(w1<w2&&w2<w3&&w3<w4&&w4<w5&&w5<w6&&w6<w7)
                        if((++nr)==nr_kombinacji){ 
	                  buff[w1]=1;
		          buff[w2]=2;
		          buff[w3]=3;
		          buff[w4]=4;
	                  buff[w5]=5;
		   	  buff[w6]=6;
			  buff[w7]=7;
		          break;
                         }
		       if(nr==nr_kombinacji) break;
		      } 
		    if(nr==nr_kombinacji) break;
		   }    
		 if(nr==nr_kombinacji) break;
		}
	      if(nr==nr_kombinacji) break;
	     } 
           if(nr==nr_kombinacji) break;
          }
        if(nr==nr_kombinacji) break;	     
       }     
     }
    else if (liczba==8){  
       	     buff[0]=1;
	     buff[1]=2;
	     buff[2]=3;
	     buff[3]=4;
	     buff[4]=5;
	     buff[5]=6;
	     buff[6]=7;
	     buff[7]=8;	          
     }     
}
/********************************* sprawdz czy sa zera do konca listy  ****************************************/
int czy_zera_do_konca(int wsk){
  int i,T=1;
      

     i=wsk;
     while( T && (i <= koniec_listy-1)){
        if(lista[0][i]!=999){
	    if(macA[lista[0][i]][lista[1][i]]=='0') T=1;
	    else T=0;
	    }
	i++;        
        
       }
     
return T;
}
/*********************************  rekursja ekonomiczna ******************************************************/
int rekursja_ekonomiczna(char **tab2D,int w,int k){   
     
      /* 
        - nie mozna stawiac bomb kolo zera
        - nalezy pamietac o tym by dekrementowac liczby przy ktorych postawilo sie bombe
        - jesli ktoras z liczb bedzie rowna zero to nalezy wokol wypelnic wszystko 'K'
        - nalezy kontrolowac liczbe wszystkich bomb
        - policz ile jest wolnych pol 
        - kazda wpakowana tu liczba MUSI zostac zaspokojona , jesli nie to rekursja bedzie sie cofala!
  	- REGULA TRZECH KWADRATOW...
      */
      
  int n,m,n2,m2,n3,m3,dn,gn,dm,gm,dn2,gn2,gm2,dm2,dn3,gn3,gm3,dm3,li,il_nieod,liczba,liczba2;
  int kr=0;    
  int x,y,wart;    
  int tab[8][2];
  int nr_komb,nr,il_komb,wsk; 
  
     liczba=(int)tab2D[w][k] - 48;
     li=liczba;
     /*granice pierwszego kwadratu - wokol wrzuconej tu liczby */
     if((dn=w-1)<0) dn=0;
     if((gn=w+1)>(wA-1)) gn=wA-1; 
     if((dm=k-1)<0) dm=0;
     if((gm=k+1)>(kA-1)) gm=kA-1;
	
     
     for(n=dn;n<=gn;n++)  
       for(m=dm;m<=gm;m++)
	  if(tab2D[n][m]=='.'){
	     tab[kr][0]=n;
	     tab[kr][1]=m;
             kr++;     
            }

/*--------------------------------------*/
 if(kr<liczba){
      return 0;
/*--------------------------------------*/      
  }
 else{    
        il_nieod=ilosc_nieodkrytych; 
        il_komb=silnia(kr)/(silnia(liczba) * silnia(kr - liczba));
        //il_komb=silnia[kr]/(silnia[liczba] * silnia[kr - liczba]);
      
      for(nr_komb=1;nr_komb<=il_komb;nr_komb++){ 
        liczba=li;
        nowa_kombinacja(nr_komb,kr,li);
    
	put(999);
        put(w);
        put(k);
        put(tab2D[w][k]);
     
        for(nr=1;nr<=li;nr++){
           
	     for(wsk=0;wsk<kr;wsk++) 
	       if (buff[wsk]==nr) break;
	      
	     n=tab[wsk][0]; 
   	     m=tab[wsk][1];
	
             if(tab2D[n][m]=='.' && liczba>0){
		                                                     
	        put(n);
                put(m);
                put(tab2D[n][m]);
	        tab2D[n][m]='x';
		 
		--liczba;
		--ilosc_nieodkrytych;
		
	        /*granice drugiego kwadratu(wokol 'x')*/
		if((dn2=n-1)<0) dn2=0;
                if((gn2=n+1)>(wA-1)) gn2=wA-1; 
                if((dm2=m-1)<0) dm2=0;
                if((gm2=m+1)>(kA-1)) gm2=kA-1;
		
		/*trzeba dekrementowac liczby wokol bomby! 'x' */
		for(n2=dn2;n2<=gn2;n2++)            
	          for(m2=dm2;m2<=gm2;m2++)
		     if(tab2D[n2][m2]>='1' && tab2D[n2][m2]<='8'){
                           liczba2=(int)tab2D[n2][m2] - 48;
	                   
			   put(n2);
                           put(m2);
                           put(tab2D[n2][m2]);          
		           
			   liczba2--;
			   /*jezeli liczba wynosi zero to trzeba w wolne puste miejsca wstawic 'K'*/
			   if(liczba2==0){
			     /*granice trzeciego kwadratu (wokol dekrementowanej liczby)*/
			     if((dn3=n2-1)<0) dn3=0;
                             if((gn3=n2+1)>(wA-1)) gn3=wA-1; 
                             if((dm3=m2-1)<0) dm3=0;
                             if((gm3=m2+1)>(kA-1)) gm3=kA-1;
			   
		           
		              for(n3=dn3;n3<=gn3;n3++)
                               for(m3=dm3;m3<=gm3;m3++)
		                 if(tab2D[n3][m3]=='.'){
			            put(n3);
                                    put(m3);
                                    put(tab2D[n3][m3]); 
			            tab2D[n3][m3]='K'; 
                                  }        
	                    }
	       
	                  tab2D[n2][m2]=(char)(liczba2 + 48);
			  
                     }
              }	    
            if(liczba==0) break;  /* czy ten break jest tu potrzebny ??*/ 
          }
     
	//printf("ilosc nieodkrytych:%d\n",ilosc_nieodkrytych);
	//wyswietl_calosc(macA,wA,kA);
	
	
	
	
	if(liczba==0 && (ilosc_nieodkrytych >= 0 || ilosc ==0)){
	  
	  if(ilosc_nieodkrytych==0){
	    if(czy_zera_do_konca(wsk_L)) return 1;
	   }     
	  else if(ilosc_nieodkrytych>0 || (ilosc_nieodkrytych<0 && ilosc==0)){
	
	    if(czy_zera_do_konca(wsk_L)){ 
	       return 1;
	       }
	    else if(wsk_L<koniec_listy-1){
	       
	     wsk_L++;
	     if(lista[0][wsk_L]==999) {
	       wsk_L++;
	      }
	  
	     x=lista[0][wsk_L];
	     y=lista[1][wsk_L];
	       
#if TEST	      
	     macC[w][k]++;
#endif	 
	     wart=rekursja_ekonomiczna(tab2D,x,y);
	  
	     if (wart==1) return 1;  
          
	     if(wsk_L<=0) return 0;
        
	     //printf("cofanie wsk_L=%d",wsk_L);    
	     wsk_L--;     
	     //printf("    --wsk_L=%d\n",wsk_L);
             if(lista[0][wsk_L]==999) { 
	        wsk_L--;                                       
	        //printf("  dodatkowo (999)  --wsk_L=%d\n",wsk_L);
	      }
	    if(wsk_L<=0) return 0;
	  
	  }
	 }
       }
	
		
	
	
	ilosc_nieodkrytych=il_nieod;                   
	
        wart=pop();     
	while(wart!=999){
	    y=pop(); 
	    x=pop();
	    tab2D[x][y]=wart;
	    wart=pop();
          }   
     }
     return 0;  
    }
/*--------------------------------------*/
 
}
/************************ wstaw w pozostale miejsca *****************************************************/ 
int wstaw_pozostale(char **tab2D){    
        /*
          funkcja, ktora wstawia odpowiednia ilosc bomb w  wolne miejsca
        */
	
  int i,j;
    
    for(i=0;i<wA;i++){
     for(j=0;j<kA;j++){        
        if(tab2D[i][j]=='.' && ilosc_nieodkrytych>0){
	        tab2D[i][j]='@';
		--ilosc_nieodkrytych;
	 }	    
	if(ilosc_nieodkrytych==0) break;
      }      
    if(ilosc_nieodkrytych==0) break;
   }

  if (ilosc_nieodkrytych==0) return 1;
  else return 0;  

}
/*********************************  rekursja glowna **********************************************************/
int procedura_wstepna(char **tab2D){    
  
int i,j,T,L=0;

T=1;

    if(ilosc_nieodkrytych!=0 || ilosc==0){
#if TEST       
        for(i=0;i<wA;i++)
          for(j=0;j<kA;j++)
                 macC[i][j]=0;
#endif    
        for(i=0;i<wA;i++){
          for(j=0;j<kA;j++) 
	     if(tab2D[i][j]>='1' && tab2D[i][j]<='8'){
	       L=1;
	       break;
	      }
	    
	  if(L) break;
	}
      
      wsk_L=1;
      if(L)  T=rekursja_ekonomiczna(tab2D,lista[0][1],lista[1][1]);
     
    }       
   
   /* jezeli T==0 to nie istnieje rozwiazanie dla tej planszy*/
      
    if(ilosc_nieodkrytych!=0 && T==1 && ilosc !=0)
      T=wstaw_pozostale(tab2D);          
       
	       
 return T; 
}

/****************************************************************************************************************\
\****************************************************************************************************************/ 
 
int main(int argc, char *argv[])
 {
   int T,odkryte,i;
   
      scanf("%d %d %d\n",&kA,&wA,&ilosc);

      T=generate(&macA,wA,kA); /*uwzglednij wyjatek*/
        
      if(wczytajdane(macA,wA,kA)==0){ 
            printf("0\n");
	    return 0; }
      else
      {

#if TEST     
            /*--->dla potrzeb sprawdzenia:<---*/
            generate(&macB,wA,kA);

            /*------>kopiuj macierz <------*/ 
            kopiuj_macierz(macB,macA);
#endif	    
        
            /*------>wstepne dane<--------*/ 
            odkryte=ilosc_odkrytych_bomb(macA); /*  {*}  */
	    ilosc_nieodkrytych=ilosc - odkryte;
	  
	    /*------>operacje<--------*/
	    if(ilosc_nieodkrytych!=0 || ilosc ==0){
	         
	         T=dekrementuj_i_wstaw_L_wokol_0(macA);
	         if(T==0){
		       printf("0\n"); 
	               return 0;
		       }
		 
		 T==uzupelnij_pojedyncze_liczby(macA);  /* dodaje bomby '#' */
		 if(T==0){
		       printf("0\n"); 
	               return 0;
		       }
		       
		T=zaspakajaj(macA);
		 if(T==0){
		       printf("0\n"); 
	               return 0;
		       }
		 
		if(ilosc_nieodkrytych!=0 || ilosc==0){
                     
		     /*------>alokacja stosu<-------*/
		     generate_stack();
		     
		     /*------>alokacja listy<-------*/
		     generate_list(&lista,wA*kA);
		
		     /*----->tworzenie zbiorow<-----*/
		     tworz_zbiory(macA);
		     
#if TEST	     
                     /*->alokacja pamieci kombinacji<-*/
		     generate_mem_komb(&macC,wA,kA);
#endif		     
		     T=procedura_wstepna(macA);          /* dodaje bomby 'x'i '@' */
	             if(T==0){
		           printf("0\n"); 
	                   return 0;
		           }
                     
#if ! TEST 
                      /*------>wyniki<--------*/
                      wyswietl_wynik(macA,wA,kA);	               
#else		      
                      /*---->podsumowanie<----*/
		      wyswietl_liste(lista);
                      wyswietl_spr(macA,macB);
		      //wyswietl_calosc(macA,wA,kA);
		      wyswietl_kombinacje(macC,wA,kA);
		      printf("\n\tIlosc wszystkich bomb:           %d\n",ilosc);
	              printf("\tIlosc odkrytych juz bomb:        %d\n",odkryte); 
	              printf("\t-------------------------------------\n");
	              printf("\tIlosc wsz.odkrytych bomb po alg.:%d\n",ilosc_odkrytych_bomb(macA)); 
	              printf("\tIlosc nieodkrytych bomb:         %d\n",ilosc_nieodkrytych);
	              printf("\tstos:\n\t  lim_max: %d\n",lim_max);
		      printf("\t  lim_actual: %d\n\n",lim_actual);
#endif		      
		  }
	        else wyswietl_wynik(macA,wA,kA);
	    }
          else wyswietl_wynik(macA,wA,kA);
      }


   if(stack!=NULL) free(stack);
  
   if(macA!=NULL){
     for(i=0;i<wA;i++) 
        if(macA[i]!=NULL) free(macA[i]);
     free(macA);
    }
    
    if(lista!=NULL){
     for(i=0;i<2;i++) 
        if(lista[i]!=NULL) free(lista[i]);
     free(lista);
    }

#if TEST  
   if(macC!=NULL){
     for(i=0;i<wA;i++) 
        if(macC[i]!=NULL) free(macC[i]);
     free(macC);
    }
 
   if(macB!=NULL){
     for(i=0;i<wA;i++) 
        if(macB[i]!=NULL) free(macB[i]);
     free(macB);
    }
#endif

return 0;
}  

