// Beginnersoefening van http://www.cplusplus.com/forum/articles/12974
// Programma is een dungeon-achtig spel

#include <iostream>
#include <stdlib.h>
#include <ctime>
using namespace std;

// Declaring functions
void ClearDungeon();
void DrawDungeon();
void SetPlayerLocation(int row, int col);
void Input();
bool TestValid(int i);
void InvalidInput();

// Declaring variables
char dungeon[7][10];
char player = 'P', empty = '.';
int playerRow = 1, playerCol = 1;
bool PLAY = true, INVALID = false;

enum MOVEMENT { boven, onder, links, rechts };

int main()
{
  // Starting menu.
  int command;
  srand ( time(NULL) );
  cout << "Welkom bij Dungeon Crawler!\n\n"
       << "DOEL\nJe bent een avonturier die door een kerker loopt.\nHet doel is om bij de schat te komen.\n\n"
       << "SPEELVELD\nP = Speler\nV = Val\nX = Schat\n\n"
       << "Druk op (1) om te spelen of (2) om te stoppen.\n";
  cin >> command;

  // Initialize the player at a random location.
  playerRow = rand() % 7;
  playerCol = rand() % 10;
  
  ClearDungeon();
  if (command == 1) {
    while (PLAY) {
      SetPlayerLocation(playerRow, playerCol);
      DrawDungeon();
      cout << "Beweeg de speler:\n\n\t(W)boven\n(A)links \t(D)rechts\n\t(S)onder \t\t\tof (E)inde: ";
      Input(); 
    }
  }
  
  // Exiting.
  cout << "Tot ziens!\n";

  return 0;
}

// Dungeon gets filled with '.'
void ClearDungeon() {
  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 10; j++) {
      dungeon[i][j] = '.';
    }
  }
}

// Drawing of the dungeon. A message is given when player reaches one of the borders.
void DrawDungeon() {
  system("clear");
  if ( INVALID == true ) { InvalidInput(); INVALID = false; }
   for (int i = 0; i < 7; i++) {
     cout << "\t";
     for (int j = 0; j < 10; j++) {
       cout << dungeon[i][j] << " ";
     }
     cout << "\n\n";
   }
}

// Setting of player location.
void SetPlayerLocation(int row, int col) {
  dungeon[playerRow][playerCol] = empty;
  dungeon[row][col] = player;
  playerRow = row;
  playerCol = col;
}

// Movement input or exiting the game.
void Input() {
  char input;
  cin >> input;
  if ( input == 'e') { PLAY = false;}
  else if ( input == 'w') {
    if (TestValid(boven)) {
      SetPlayerLocation(playerRow-1, playerCol);
    }
    else InvalidInput();
  }
  else if ( input == 's') {
    if (TestValid(onder)) {
      SetPlayerLocation(playerRow+1, playerCol);
    }
    else InvalidInput();
  }
  else if ( input == 'a') {
    if (TestValid(links)) {
      SetPlayerLocation(playerRow, playerCol-1);
    }
    else InvalidInput();
  }
  else if ( input == 'd') {
    if (TestValid(rechts)) {
      SetPlayerLocation(playerRow, playerCol+1);
    }
    else InvalidInput();
  }
}

// Testing if player isn't crossing the borders of the dungeon.
bool TestValid(int i) {
  bool valid = false;
  if (i == boven) {
    if (playerRow > 0) valid = true;
  }
  if (i == onder) {
    if (playerRow < 6) valid = true;
  }
  if (i == links) {
    if (playerCol > 0) valid = true;
  }
  if (i == rechts) {
    if (playerCol < 9) valid = true;
  }
  return valid;
}

// Message to warn players they reached the border.
void InvalidInput() {
  cout << "\nJe kunt niet verder. De rand van de dungeon is bereikt.\n\n";
  INVALID = true;
}
