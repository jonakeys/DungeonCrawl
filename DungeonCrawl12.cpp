/* Beginnersoefening van http://www.cplusplus.com/forum/articles/12974
   
   DUNGEON CRAWL
   Versie 1.2 (07.02.2014)
   
   1.0 Het spel werkt. Je kunt bewegen door de dungeon, er is een schat te vinden en er zijn vallen aanwezig.
   1.1 Optimaliseren van de code.
   1.2 Bewegende vijanden toegevoegd. (verplaatsen één locatie per beurt, vijand 1 beweegt zich naar de speler toe)
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
void InitEnemy1();
void InitEnemy2();
void InitEnemy3();

// Declare functions for setting locations
void SetLocations();
void SetPlayerLocation(int row, int col);
void SetPlayerLocationIntro(int row, int col);
void SetTreasureLocation(int row, int col);
void SetEnemy1Location(int row, int col);
void SetEnemy2Location(int row, int col);
void SetEnemy3Location(int row, int col);

// Declare functions for input
void Input();
void InvalidInput();
bool TestValid(int i);
bool TestValidEnemy(int i, int row, int col);

// Declare functions for main program
void DispIntroText();
void DispTitle();

// Declare functions for testing location
bool TestLocFree(int x, int y);
void TreasureEnemy(int x, int y);

// Declare functions for moving enemies
void MoveEnemy1();
void MoveEnemy2();
void MoveEnemy3();

// Declare variables
const int DHEIGHT=10, DWIDTH=20;
char dungeon[DHEIGHT][DWIDTH];
char player = 'P', empty = '.', treasure = 'X', enemy1 = '1', enemy2 = '2', enemy3 = '3';
int playerRow, playerCol;
int treasureRow, treasureCol;
int enemy1Row, enemy1Col;
int enemy2Row, enemy2Col;
int enemy3Row, enemy3Col;
bool PLAY = true, INVALID = false;
int enemyMove;
int stepsCount=0;

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
      cout << "Kies een richting voor de speler:\n\n"
	   << "\t\t(w)boven\n\t(a)links \t(d)rechts\n\t\t(s)onder \t\t\tof (e)inde\n";
      cout << "\nInvoer: ";
      stepsCount++;
      Input();
      MoveEnemy1();
      MoveEnemy2();
      MoveEnemy3();
      if (dungeon[enemy1Row-1][enemy1Col] == 'P' || dungeon[enemy1Row+1][enemy1Col] == 'P' || 
	  dungeon[enemy1Row][enemy1Col-1] == 'P' || dungeon[enemy1Row][enemy1Col+1] == 'P') {
	PLAY = false;
	SetLocations();
	DrawDungeon();
	TreasureEnemy(enemy1Row, enemy1Col);      
      }
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
  system("clear");
  DispTitle();
  cout       << "\nGemaakt door: Jonathan van der Steege (jonakeys@hotmail.com)\n\n\n\n"
	     << "DOEL\t\tJe bent een avonturier die door een kerker loopt. Het doel is om bij de schat te komen.\n"
	     << "\t\tPas op dat je onderweg niet met een vijand in aanraking komt!\n\n"
	     << "SPEELVELD\tP = Speler\t\tX = Schat\t\t1, 2 of 3 = Vijand\n\n\n\n"
	     << "Kies (1) om te spelen, een andere toets om te stoppen: ";
}

// Fill the dungeon with '.' (dots)
void ClearDungeon() {
  for (int i = 0; i < DHEIGHT; i++) {
    for (int j = 0; j < DWIDTH; j++) {
      dungeon[i][j] = '.';
    }
  }
}

// Draw the dungeon. A message is shown when player reaches one of the borders.
void DrawDungeon() {
  system("clear");
  if ( INVALID == true ) { InvalidInput(); INVALID = false; }
  else { DispTitle();
    cout << "\n\n\n";}
  for (int i = 0; i < DHEIGHT; i++) {
    cout << "\t\t";
    for (int j = 0; j < DWIDTH; j++) {
      cout << dungeon[i][j] << " ";
    }
    cout << "\n\n";
  }
  cout << "\n";
}

// Message to warn players they reached the border.
void InvalidInput() {
  DispTitle();
  cout << "\nJe kunt niet verder. Dit is de rand van de kerker.\n\n";
  INVALID = true;
}

// Movement input or exit the game.
void Input() {
  char input;
  cin >> input;
  if ( input == 'e') { 
    cout << "Bedankt voor het spelen, tot een volgende keer!\n";
    PLAY = false;
  }
  else if ( input == 'w') {
    if (TestValid(boven) && TestLocFree(playerRow-1, playerCol)) {
      SetPlayerLocation(playerRow-1, playerCol);
    }
    else if (TestValid(boven)) TreasureEnemy(playerRow-1, playerCol);
    else InvalidInput();
  }
  else if ( input == 's') {
    if (TestValid(onder) && TestLocFree(playerRow+1, playerCol)) {
      SetPlayerLocation(playerRow+1, playerCol);
    }
    else if (TestValid(onder)) TreasureEnemy(playerRow+1, playerCol);
    else InvalidInput();
  }
  else if ( input == 'a') {
    if (TestValid(links) && TestLocFree(playerRow, playerCol-1)) {
      SetPlayerLocation(playerRow, playerCol-1);
    }
    else if (TestValid(links)) TreasureEnemy(playerRow, playerCol-1);
    else InvalidInput();
  }
  else if ( input == 'd') {
    if (TestValid(rechts) && TestLocFree(playerRow, playerCol+1)) {
      SetPlayerLocation(playerRow, playerCol+1);
    }
    else if (TestValid(rechts)) TreasureEnemy(playerRow, playerCol+1);
    else InvalidInput();
  }
}

// Testing if player isn't crossing the borders of the dungeon.
bool TestValid(int i) {
  bool valid = false;
  if (i == boven) {
    if (playerRow > 0) valid = true;
  }
  else if (i == onder) {
    if (playerRow < DHEIGHT-1) valid = true;
  }
  else if (i == links) {
    if (playerCol > 0) valid = true;
  }
  else if (i == rechts) {
    if (playerCol < DWIDTH-1) valid = true;
  }
  return valid;
}

// Testing if enemy isn't crossing the borders of the dungeon.
bool TestValidEnemy(int i, int enemy1Row, int enemy1Col) {
  bool valid = false;
  if (i == boven) {
    if (enemy1Row > 0) valid = true;
  }
  if (i == onder) {
    if (enemy1Row < DHEIGHT-1) valid = true;
  }
  if (i == links) {
    if (enemy1Col > 0) valid = true;
  }
  if (i == rechts) {
    if (enemy1Col < DWIDTH-1) valid = true;
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
void TreasureEnemy(int x, int y) {
  if (dungeon[x][y] == 'X') {
    cout << "\nGeweldig! Je hebt de schat gevonden!\n"
	 << "Je hebt er " << stepsCount << " stappen over gedaan.\n";
    PLAY = false;
  }
  else {
    cout << "\nRAWRRRRR! Je bent opgegeten en je avontuur is helaas voorbij.\n";
    PLAY = false;
  }
}

// Initialize the dungeon, player, treasure and traps (combined function).
void InitGame() {
  ClearDungeon();
  InitPlayer();
  InitTreasure();
  InitEnemy1();
  InitEnemy2();
  InitEnemy3();
}

// Initialize the player at a random location and test if the location is free.
void InitPlayer() {
  bool done = false;
  do {
    playerRow = rand() % DHEIGHT;
    playerCol = rand() % DWIDTH;
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
    treasureRow = rand() % DHEIGHT;
    treasureCol = rand() % DWIDTH;
    if (TestLocFree(treasureRow, treasureCol)) {
      SetTreasureLocation(treasureRow, treasureCol);
      done = true;
    }
  } while (!done);
}

// Initialize the enemies.
void InitEnemy1() {
  bool done = false;
  do {
    enemy1Row = rand() % DHEIGHT;
    enemy1Col = rand() % DWIDTH;
    if (TestLocFree(enemy1Row, enemy1Col)) {
      SetEnemy1Location(enemy1Row, enemy1Col);
      done = true;
    }
  } while (!done);
}

void InitEnemy2() {
  bool done = false;
  do {
    enemy2Row = rand() % DHEIGHT;
    enemy2Col = rand() % DWIDTH;
    if (TestLocFree(enemy2Row, enemy2Col)) {
      SetEnemy2Location(enemy2Row, enemy2Col);
      done = true;
    }
  } while (!done);
}

void InitEnemy3() {
  bool done = false;
  do {
    enemy3Row = rand() % DHEIGHT;
    enemy3Col = rand() % DWIDTH;
    if (TestLocFree(enemy3Row, enemy3Col)) {
      SetEnemy3Location(enemy3Row, enemy3Col);
      done = true;
    }
  } while (!done);
}

// Set locations of player, treasure and enemies (combined function).
void SetLocations() {
      SetPlayerLocation(playerRow, playerCol);
      SetTreasureLocation(treasureRow, treasureCol);
      SetEnemy1Location(enemy1Row, enemy1Col);
      SetEnemy2Location(enemy2Row, enemy2Col);
      SetEnemy3Location(enemy3Row, enemy3Col);
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

// Set location of enemies.
void SetEnemy1Location(int row, int col) {
  dungeon[enemy1Row][enemy1Col] = enemy1;
  enemy1Row = row;
  enemy1Col = col;
}

void SetEnemy2Location(int row, int col) {
  dungeon[row][col] = enemy2;
  enemy2Row = row;
  enemy2Col = col;
}

void SetEnemy3Location(int row, int col) {
  dungeon[row][col] = enemy3;
  enemy3Row = row;
  enemy3Col = col;
}


// Move enemies at each turn.
void MoveEnemy1() {
  bool done=false;
  int whichMove;

  do {
    int enemy1RowTest, enemy1ColTest;
    enemy1RowTest = enemy1Row;
    enemy1ColTest = enemy1Col;

    // Move the enemy towards the player.
    whichMove = rand() % 3;

    if(whichMove == 0) {
      if(enemy1RowTest < playerRow) {
	enemy1RowTest += 1;
      }
      else if(enemy1RowTest == playerRow) {
      }
      else if(enemy1RowTest > playerRow) {
	enemy1RowTest -= 1;
      }
    }
    
    else if (whichMove == 1) {
      if(enemy1ColTest < playerCol) {
	enemy1ColTest += 1;
      }
      else if(enemy1ColTest == playerCol) {
      }
      else if(enemy1ColTest > playerCol) {
	enemy1ColTest -= 1;
      }
    }

    else if (whichMove == 2) {}
    
    if(TestLocFree(enemy1RowTest, enemy1ColTest)){
      dungeon[enemy1Row][enemy1Col] = empty;
      enemy1Row = enemy1RowTest;
      enemy1Col = enemy1ColTest;
      SetEnemy1Location(enemy1Row, enemy1Col);
      done = true;
    }    
    else {
      done = true;
    }
  }
  while(!done);
}

void MoveEnemy2() {
  bool done=false;
  do {
    int enemy2RowTest, enemy2ColTest;
    enemy2RowTest = enemy2Row;
    enemy2ColTest = enemy2Col;
    enemyMove = rand() % 4;
    if (enemyMove == boven) {
      enemy2RowTest += 1;
    }
    else if (enemyMove == onder) {
      enemy2RowTest -= 1;
    }
    else if (enemyMove == links) {
      enemy2ColTest -= 1;
    }
    else if (enemyMove == rechts) {
      enemy2ColTest += 1;
    }
    if(TestLocFree(enemy2RowTest, enemy2ColTest)){
      dungeon[enemy2Row][enemy2Col] = empty;
      enemy2Row = enemy2RowTest;
      enemy2Col = enemy2ColTest;
      SetEnemy2Location(enemy2Row, enemy2Col);
      done = true;
    }
  }
  while(!done);
  done = false;
}

void MoveEnemy3(){
  bool done=false;
  do {
    int enemy3RowTest, enemy3ColTest;
    enemy3RowTest = enemy3Row;
    enemy3ColTest = enemy3Col;
    enemyMove = rand() % 4;
    if (enemyMove == boven) {
      enemy3RowTest += 1;
    }
    else if (enemyMove == onder) {
      enemy3RowTest -= 1;
    }
    else if (enemyMove == links) {
      enemy3ColTest -= 1;
    }
    else if (enemyMove == rechts) {
      enemy3ColTest += 1;
    }
    if(TestLocFree(enemy3RowTest, enemy3ColTest)){
      dungeon[enemy3Row][enemy3Col] = empty;
      enemy3Row = enemy3RowTest;
      enemy3Col = enemy3ColTest;
      SetEnemy3Location(enemy3Row, enemy3Col);
      done = true;
    }
  }
  while(!done);
  done = false;
}

// Display the title of the game.
void DispTitle() {
  cout << "    ____  __  ___   __________________  _   __   __________  ___ _       ____ \n"
       << "   / __ \\/ / / / | / / ____/ ____/ __ \\/ | / /  / ____/ __ \\/   | |     / / / \n"
       << "  / / / / / / /  |/ / / __/ __/ / / / /  |/ /  / /   / /_/ / /| | | /| / / /  \n"
       << " / /_/ / /_/ / /|  / /_/ / /___/ /_/ / /|  /  / /___/ _, _/ ___ | |/ |/ / /___\n"
       << "/_____/\\____/_/ |_/\\____/_____/\\____/_/ |_/   \\____/_/ |_/_/  |_|__/|__/_____/\n";
}
