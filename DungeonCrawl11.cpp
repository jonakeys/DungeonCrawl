/* Beginnersoefening van http://www.cplusplus.com/forum/articles/12974
   
   DUNGEON CRAWL
   Versie 1.1 (28.04.2013)
   
   1.0 Het spel werkt. Je kunt bewegen door de dungeon, er is een schat te vinden en er zijn vallen aanwezig.
   1.1 Optimaliseren van de code.
*/

#include <iostream>
#include <stdlib.h>
#include <ctime>
using namespace std;

// Declare functions for the dungeon
void ClearDungeon();
void DrawDungeon();

// Declare functions for initialization
void InitGame();
void InitPlayer();
void InitTreasure();
void InitTrap1();
void InitTrap2();
void InitTrap3();

// Declare functions for setting locations
void SetLocations();
void SetPlayerLocation(int row, int col);
void SetPlayerLocationIntro(int row, int col);
void SetTreasureLocation(int row, int col);
void SetTrap1Location(int row, int col);
void SetTrap2Location(int row, int col);
void SetTrap3Location(int row, int col);

// Declare functions for input
void Input();
void InvalidInput();
bool TestValid(int i);

// Declare functions for main program
void DispIntroText();

// Declare functions for testing location
bool TestLocFree(int x, int y);
void TreasureTrap(int x, int y);

// Declare variables
char dungeon[7][10];
char player = 'P', empty = '.', treasure = 'X', trap1 = 'T', trap2 = 'T', trap3 = 'T';
int playerRow, playerCol;
int treasureRow, treasureCol;
int trap1Row, trap1Col;
int trap2Row, trap2Col;
int trap3Row, trap3Col;
bool PLAY = true, INVALID = false;

enum MOVEMENT { boven, onder, links, rechts };

// Main program
int main()
{
  // Make the random numbers dependent of time.
  srand ( time(NULL) );

  // Starting the menu.
  int command;
  DispIntroText();
  cin >> command;

  // Initialize the game. Set up the dungeon; fill it with player, treasure and traps.
  InitGame();
  if (command == 1) {
    while (PLAY) {
      SetLocations();
      DrawDungeon();
      cout << "Beweeg de speler:\n\n\t(W)boven\n(A)links \t(D)rechts\n\t(S)onder \t\t\tof (E)inde: ";
      Input();
    }
  }
  // Exit the game.
  else {
    cout << "Tot ziens!\n";
  }
  return 0;
}

// Intro text and main menu.
void DispIntroText () {
  cout << "Welkom bij Dungeon Crawl!\n\n"
       << "DOEL\nJe bent een avonturier die door een kerker loopt.\nHet doel is om bij de schat te komen. "
       << "Pas op dat je onderweg niet in een val terechtkomt!\n\n"
       << "SPEELVELD\nP = Speler\nX = Schat\nT = Val\n\n"
       << "Druk op (1) om te spelen of een andere toets om te stoppen.\n";
}

// Fill the dungeon with '.' (dots)
void ClearDungeon() {
  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 10; j++) {
      dungeon[i][j] = '.';
    }
  }
}

// Draw the dungeon. A message is shown when player reaches one of the borders.
void DrawDungeon() {
  system("clear");
  if ( INVALID == true ) { InvalidInput(); INVALID = false; }
  else cout << "Dungeon Crawl\n\n\n\n";
  for (int i = 0; i < 7; i++) {
    cout << "\t";
    for (int j = 0; j < 10; j++) {
      cout << dungeon[i][j] << " ";
    }
    cout << "\n\n";
  }
}

// Message to warn players they reached the border.
void InvalidInput() {
  cout << "Dungeon Crawl\n\nJe kunt niet verder. Dit is de rand van de kerker.\n\n";
  INVALID = true;
}

// Movement input or exit the game.
void Input() {
  char input;
  cin >> input;
  if ( input == 'e') { PLAY = false;}
  else if ( input == 'w') {
    if (TestValid(boven) && TestLocFree(playerRow-1, playerCol)) {
      SetPlayerLocation(playerRow-1, playerCol);
    }
    else if (TestValid(boven)) TreasureTrap(playerRow-1, playerCol);
    else InvalidInput();
  }
  else if ( input == 's') {
    if (TestValid(onder) && TestLocFree(playerRow+1, playerCol)) {
      SetPlayerLocation(playerRow+1, playerCol);
    }
    else if (TestValid(onder)) TreasureTrap(playerRow-1, playerCol);
    else InvalidInput();
  }
  else if ( input == 'a') {
    if (TestValid(links) && TestLocFree(playerRow, playerCol-1)) {
      SetPlayerLocation(playerRow, playerCol-1);
    }
    else if (TestValid(links)) TreasureTrap(playerRow, playerCol-1);
    else InvalidInput();
  }
  else if ( input == 'd') {
    if (TestValid(rechts) && TestLocFree(playerRow, playerCol+1)) {
      SetPlayerLocation(playerRow, playerCol+1);
    }
    else if (TestValid(rechts)) TreasureTrap(playerRow, playerCol-1);
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

// Test if the wanted location is free.
bool TestLocFree(int x, int y) {
  bool free = false;
  if (dungeon[x][y] == '.') {
    free = true;
  }
  return free;
}

// When the location is not free, you've reached the treasure or entered a trap.
// The game ends after this.
void TreasureTrap(int x, int y) {
  if (dungeon[x][y] == 'X') {
    cout << "\nGeweldig! Je hebt de schat gevonden!\n";
    PLAY = false;
  }
  else {
    cout << "\nJe bent in een val beland. Het avontuur is voorbij.\n";
    PLAY = false;
  }
}

// Initialize the dungeon, player, treasure and traps (combined function).
void InitGame() {
  ClearDungeon();
  InitPlayer();
  InitTreasure();
  InitTrap1();
  InitTrap2();
  InitTrap3();
}

// Initialize the player at a random location and test if the location is free.
void InitPlayer() {
  bool done = false;
  do {
    playerRow = rand() % 7;
    playerCol = rand() % 10;
    if (TestLocFree(playerRow, playerCol)) {
      SetPlayerLocationIntro(playerRow, playerCol);
      done = true;
    }
  } while (!done);
}

// Initialize the treasure.
void InitTreasure() {
  bool done = false;
  do {
    treasureRow = rand() % 7;
    treasureCol = rand() % 10;
    if (TestLocFree(treasureRow, treasureCol)) {
      SetTreasureLocation(treasureRow, treasureCol);
      done = true;
    }
  } while (!done);
}

// Initialize the traps.
void InitTrap1() {
  bool done = false;
  do {
    trap1Row = rand() % 7;
    trap1Col = rand() % 10;
    if (TestLocFree(trap1Row, trap1Col)) {
      SetTrap1Location(trap1Row, trap1Col);
      done = true;
    }
  } while (!done);
}

void InitTrap2() {
  bool done = false;
  do {
    trap2Row = rand() % 7;
    trap2Col = rand() % 10;
    if (TestLocFree(trap2Row, trap2Col)) {
      SetTrap2Location(trap2Row, trap2Col);
      done = true;
    }
  } while (!done);
}

void InitTrap3() {
  bool done = false;
  do {
    trap3Row = rand() % 7;
    trap3Col = rand() % 10;
    if (TestLocFree(trap3Row, trap3Col)) {
      SetTrap3Location(trap3Row, trap3Col);
      done = true;
    }
  } while (!done);
}

// Set locations of player, treasure and traps (combined function).
void SetLocations() {
      SetPlayerLocation(playerRow, playerCol);
      SetTreasureLocation(treasureRow, treasureCol);
      SetTrap1Location(trap1Row, trap1Col);
      SetTrap2Location(trap2Row, trap2Col);
      SetTrap3Location(trap3Row, trap3Col);
}

// Set player location.
void SetPlayerLocation(int row, int col) {
  dungeon[playerRow][playerCol] = empty;
  dungeon[row][col] = player;
  playerRow = row;
  playerCol = col;
}

// Set player location at initialization.
void SetPlayerLocationIntro(int row, int col) {
  dungeon[row][col] = player;
}

// Set location of treasure.
void SetTreasureLocation(int row, int col) {
  dungeon[row][col] = treasure;
}

// Set location of traps.
void SetTrap1Location(int row, int col) {
  dungeon[row][col] = trap1;
}

void SetTrap2Location(int row, int col) {
  dungeon[row][col] = trap2;
}

void SetTrap3Location(int row, int col) {
  dungeon[row][col] = trap3;
}


