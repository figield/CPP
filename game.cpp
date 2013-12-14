/*
* Copyright (c) Dawid Figiel 2011 All rights reserved.
*/
/*

3 - klasy wrogow : 
a) od 1-4 
b) od 5-9
c) 10 hardcor

losowanie parametrow przeciwnikow
losowanie zdazen trudnosc wraz z trwaniem gry 

max 4 - 6 zdazen 
walki ucieczki, kuferki 
po wygranych starciach + doswiadczenia 
po przegranych - zdrowie 

doswiadczenie - > otwieranie kuferkow 
losowanie bonusow
Podsumowanie GRY
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

/*Global variables*/
string UserName = "Stranger";
int Race = 1;       // Human by default
int Difficulty = 2; // Medium by default
long Score = 0;
int Health = 100;
int Experience = 0;

string JourneysStart[] = {"Your mission is to go to Mordor and destroy the ring.\n", // Humans
                          "Your mission is support humans when they will go to Mordor and destroy the ring.\n",//Elfs 
                          "Go to Mordor and in the meanwhile kill as much live being as you can!\n"}; // Orgs
string ActionStart[]   = {"Meet ",                   
                          "Smell "};
string ActionEnd[]     = {"You won!", 
                          "Escape, enemy is stronger than you!"};
string Rests[]         = {"You are tired, decited to go sleep",
                          "Hungry..., you stopped to eat something"};
string JourneysEnd[]   = {"You have successfully finished your mission!",
                          "You have failed"};
struct man {
  int Strength;
  int Cleverness;
  int Health;
  int Experience;
};

struct man Warrior;

void p(const char* format, ...);

/* function is reading a number as a string */
int get_choice(int Range)
{
  const int MAXBUFFERSIZE = 100; 
  char ch;                     // handles user input 
  char buffer[MAXBUFFERSIZE];  // sufficient to handle one line 
  int char_count = 0;          // number of characters read for this line 
  int number, valid_choice = 0;
  //  puts ("please, enter a name: ");
  //  gets (name);

  while(valid_choice == 0) {
    ch = getchar();
    char_count = 0;
    while((ch != '\n')  &&  (char_count < MAXBUFFERSIZE)) {
      buffer[char_count++] = ch;
      ch = getchar();
    }
    buffer[char_count] = 0x00;      /* null terminate buffer */
    number = atoi( buffer );
    if( (number < 1) || (number > Range) )
      p("Enter a number between 1 and %d\n", Range);
    else
      valid_choice = 1;
  }
  return number;
}

int get_random(int Range)
{
  return rand() % Range;
}

void print_line()
{ 
  p("==========================================================\n");
}

void copyrights()
{
  srand(time(0));
  print_line();
  p("\tAuthor: Dawid Figiel\n");
  p("\tWelcome to my marvellous adventure game!\n");
  print_line();
}

void cleartoendofline()
{
  char ch;
  ch = getchar();
  while( ch != '\n' )
    ch = getchar();
}

void introduction()
{ 
  p("\nWhat is your name Stranger? ");
  cin >> UserName;
  cout << "Hello " << UserName << "!\n";
  cleartoendofline();
}

void pick_race()
{
  p("\nPlease pick your race: \n");
  p("\t1 - Human\n");
  p("\t2 - Orc\n");
  p("\t3 - Elf\n");
  Race = get_choice(3);
  switch (Race)
    {
    case 1:
      Warrior.Strength = 50; 
      Warrior.Cleverness = 50;
      Warrior.Health = 50;
      Warrior.Experience = 50;
      p("\nYou picked the Human race.\n");
      break;
    case 2:
      Warrior.Strength = 90; 
      Warrior.Cleverness = 20;
      Warrior.Health = 60;
      Warrior.Experience = 30;
      p("\nYou picked the Orc race.\n");
      break;
    case 3:
      Warrior.Strength = 20; 
      Warrior.Cleverness = 60;
      Warrior.Health = 90;
      Warrior.Experience = 70;
      p("\nYou picked the Elf race.\n");
      break;
    default:
      Warrior.Strength = 90; 
      Warrior.Cleverness = 20;
      Warrior.Health = 60;
      Warrior.Experience = 30;
      p("\nOnly Orc is so stupid to not being able to choose the right option!\n");
      Race = 2;      
    }
}

void difficulty()
{
  p("\nPick your level difficulty: \n");
  p("\t1 - Easy\n");
  p("\t2 - Medium\n");
  p("\t3 - Hard\n");
  Difficulty = get_choice(3);
  switch (Difficulty)
    {
    case 1:
      p("\nYou picked Easy.\n\n");
      break;
    case 2:
      p("\nYou picked Medium.\n\n");
      break;
    case 3:
      p("\nYou picked Hard.\n\n");
      break;
    default:
      p("\nYou must be Orc...\n\n");
      Difficulty = 3; 
    }
}

int get_bonus()
{

  return 0;
}

void summary(){
  char str[2000];
  fstream file_op("game_log.txt",ios::in);
  while(!file_op.eof())
    {
      file_op.getline(str,2000);
      cout <<str<<endl;
    }
  file_op.close();
  cout <<endl;
}

/* http://stackoverflow.com/questions/1056411/how-to-pass-variable-number-of-arguments-to-printf-sprintf */
void p(const char* format, ...)
{
    char dest[2000];
    va_list argptr;
    va_start(argptr, format);
    vsprintf(dest, format, argptr);
    va_end(argptr);

    cout << dest;    
    FILE *fp;
    if (fp = fopen("game_log.txt", "a"))
      {
        fprintf(fp, dest);
        fclose(fp);
      }
    else
      printf("Error opening game_story.txt\n");
}

void warrior_conditions()
{
  print_line();
  p("Your warrior conditions:\n");
  p("\tStrength - %d %\n", Warrior.Strength);
  p("\tCleverness - %d %\n", Warrior.Cleverness);
  p("\tHealth - %d %\n", Warrior.Health);
  p("\tExperience - %d %\n\n", Warrior.Experience);
}

void action(){
  print_line();
  cout <<  JourneysStart[Race - 1] << "\n";
}

int main()
{
  copyrights();
  introduction();
  pick_race();
  difficulty();
  warrior_conditions();
  action();
  summary();
  
  return 0;
}
