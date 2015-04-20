
//SKELETON PROGRAM

//---------------------------------

// Include libraries

// Include standard libraries

#include <iostream > // For output and input: cin >> and cout <<
#include <iomanip> // For formatted output in 'cout'
#include <conio.h> // For getch()
#include <string> // For string
#include <vector>	// For vectors
#include <sstream> // For string streams (advanced string handling)
#include <ctime> // For date/time
#include <fstream>
using namespace std;

// Include our own libraries

#include "RandomUtils.h" // For Seed, Random
#include "ConsoleUtils.h" // For Clrscr, Gotoxy, etc.

//---------------------------------
// Define constants
//---------------------------------

// Define global constants

// Defining the size of the grid
const int SIZEY(12); // Vertical dimension
const int SIZEX(20); // Horizontal dimension

// Defining symbols used for display of the grid and content
const char SPOT('@'); // Spot
const char TUNNEL(' '); // Open space
const char WALL('#'); // Border
const char HOLE('0'); // Holes
const char PILL('*'); // Pills
const char ZOMBIE('Z'); // Zombies

// Defining the command letters to move the blob on the maze
const int UP(72); // Up arrow
const int DOWN(80); // Down arrow
const int RIGHT(77); // Right arrow
const int LEFT(75); // Left arrow

// Defining the other command letters
const char QUIT('Q'); // End the game
const char PLAY('P'); // Play the game
const char INFORMATION('I'); // Open help menu

// Data structure to store data for a grid item

struct Item {

	const char symbol;	// Symbol on grid
	int x, y;	// Coordinates
	int render; // Alive or not

};

struct ZombieItem {

	const char symbol;	// Symbol on grid
	int x, y, orginalx, orginaly; // Coordinates
	int render; // Alive or not

};

//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Start of main
//
//---------------------------------------------------------------------------
int main() {

	// Function declarations (prototypes)
	void createTitleScreen(bool showhelp);
	void initialiseGame(char grid[][SIZEX], Item& spot, vector<Item> &holes, vector<Item> &powerpills, vector<ZombieItem> &zombies);
	bool wantToQuit(int k);
	bool wantToPlay(int k);
	bool wantToOpenInformation(int k);
	bool wantToFreeze(int k);
	bool wantToExterminate(int key);
	bool wantToEat(int key);
	bool isArrowKey(int k);
	int getKeyPress();
	void updateGame(char g[][SIZEX], Item& sp, int k, string& mess, vector<Item> &holes, vector<Item> &powerpills, vector<ZombieItem> &zombies, int &spotLives, int &pillsLeft, int &zombiesLeft, bool Freeze);
	void renderGame(const char g[][SIZEX], string mess, int spotLives, int zombiesLeft, int pillsLeft, string name, int oldhighscore);
	void endProgram(string quitMessage);
	void exterminateAllZombies(vector<ZombieItem> &zombies, bool Exterminate, char gr[][SIZEX]);
	void eatAllPills(vector<Item> &pills, char gr[][SIZEX]);
	void printInfo();
	string getPlayerName();
	void clearScreen();
	// Local variable declarations 
	int spotLives = 5;
	int pillsLeft = 8;
	int zombiesLeft = 4;


	bool Freeze, Exterminate, Eat;
	Freeze = Exterminate = Eat = false;

	char grid[SIZEY][SIZEX]; // Grid for display

	Item spot = { SPOT }; // Spot's symbol and position (0, 0) 
	spot.render = true;

	vector<Item> holes;
	vector<Item> powerpills;
	vector<ZombieItem> zombies;

	int key(' '); // Create key to store keyboard events

	string playername = ""; // Holds the players name

	int oldhighscore; // Holds the last high score of the player

	bool exitmenu = false; // Holds if the menu should be exited or not

	while (!exitmenu) {

		clearScreen(); // Clear the screen
		createTitleScreen(true); // Display the title screen with help

		SelectBackColour(clBlack);
		SelectTextColour(clWhite);

		key = getKeyPress();

		while (!wantToPlay(key) && !wantToOpenInformation(key) && !wantToQuit(key)) {
			
			// Display invalid key if the key was not an I, P or Q (or lower case of each)

			Gotoxy(50, 20);
			cout << "INVALID KEY!   ";

			key = getKeyPress(); // Keep listening until the player enters a valid key

		}

		if (wantToQuit(key)) { // Determine if the quit key was inputted
			// Quit the game
			endProgram("PLAYER QUITS!   ");
			return 0;
		}
		else if (wantToOpenInformation(key)) { // Determin if the information key was inputted

			clearScreen(); // Clear screen
			printInfo(); // Display game info screen

			while (key != 13) // Exit enter screen when the enter key (ASCII value 13) is inputted
				key = getKeyPress();

		} else {

			// If neither quit or information key, then the play key was inputted

			ifstream name;

			SelectBackColour(clBlack);
			clearScreen(); // Clear the screen

			playername = getPlayerName(); // Ask the player for their name

			name.open(playername + ".src", ios::in); // Open up the score file for this name

			if (name.fail()) { // Check to see if this failed (if it exists)

				ofstream newName;
				newName.open(playername + ".src", ios::out); // Create the file
				newName << -1; // Print -1 to the file
				oldhighscore = -1; // Set the old high score to -1 as they have never played before

			} else { // Else the file exists
				name >> oldhighscore; // Load the score saved in their score file
			}

			SelectBackColour(clBlack);
			clearScreen();

			exitmenu = true; // Exit the menu and start the game

		}

	}

	string message("LET'S START... "); // Current message to player

	initialiseGame(grid, spot, holes, powerpills, zombies); // Initialise grid (incl. walls and spot)

	do {

		renderGame(grid, message, spotLives, zombiesLeft, pillsLeft, playername, oldhighscore); // Render game state on screen

		message = "                                     ";

		key = getKeyPress(); // Read in next keyboard event
		
		if (isArrowKey(key)) // Check if an arrow key was pressed
			updateGame(grid, spot, key, message, holes, powerpills, zombies, spotLives, pillsLeft, zombiesLeft, Freeze);
		else if (wantToFreeze(key)) // Check if the cheat key freeze was pressed
			Freeze = !Freeze; // Toggle the value of freeze
		else if (wantToEat(key)) { // Check if the cheat key eat was pressed
			Eat = !Eat; // Togle the value of eat

			if (Eat) { // If eat is now true
				eatAllPills(powerpills, grid); // Remove all power pills from the grid
				spotLives += pillsLeft; // Increase spot lives by this amount
				pillsLeft = 0; // Set remaining pills to 0
			}

		} else if (wantToExterminate(key)) { // Check if the cheat key exterminate was pressed

			Exterminate = !Exterminate; // Toggle the value of exterminate
			zombiesLeft = Exterminate == true ? 0 : 4; // Set the amount of zombies left to 0 if Exterminate is true & 4 if Exterminate is false

			exterminateAllZombies(zombies, Exterminate, grid); // Either place back or remove all the zombies from the grid depending on the value of Exterminate

		} else
			message = "INVALID KEY! "; // Set 'Invalid key' message

	} while (!wantToQuit(key) && spotLives > 0 && !(zombiesLeft == 0 && pillsLeft == 0)); // While user does not want to quit & the win/lose conditions are not met

	if (spotLives > 0) { // Check if the player has any lifes left (check if they lost)

		if (zombiesLeft == 0 && pillsLeft == 0) { // Check if the player has won the game

			if (spotLives > oldhighscore) { // Update the score saved in file only if the current score is greater than the old high score

				ofstream score;
				score.open(playername + ".src", ios::out);
				score << spotLives; // Set to the new high score

			}

			string endMessage;	// Final string to pass to endProgram
			ostringstream convert;	// streams used for the conversion of int to string
			convert << spotLives;	// puts textual representation of spotLivesi nto stream

			endMessage = convert.str();	// sets endMessage to conents of stream
			clearScreen(); // Clear the screen
			createTitleScreen(false); // Create the title screen again, without showing the help
			endMessage = "PLAYER WINS WITH: " + endMessage + " LIVES REMAINING"; // formats endMessage
			endProgram(endMessage); // Display the end game message

		} else {
			clearScreen(); // Clear the screen
			createTitleScreen(false); // Create the title screen again, without showing the help
			endProgram("PLAYER QUITS! "); // Display final message
		}

	} else {

		if (spotLives > oldhighscore) { // Update the score saved in file only if the current score is greater than the old high score

			ofstream score;
			score.open(playername + ".src", ios::out);
			score << spotLives; // Set to the new high score

		}

		clearScreen(); // Clear the screen
		createTitleScreen(false); // Create the title screen again, without showing the help
		endProgram("YOU LOST!"); // Display the end game message

	}

	return 0;

} //end Main

//---------------------------------------------------------------------------
// Start of updateGame
//
// Updates spots location
// Updates the zombies locations
// Updates the rest of the grid
//---------------------------------------------------------------------------
void updateGame(char grid[][SIZEX], Item& spot, int key, string& message, vector<Item> &holes, vector<Item> &powerpills, vector<ZombieItem> &zombies, int &spotLives, int &pillsLeft, int &zombiesLeft, bool Freeze) {

	void updateSpotCoordinates(const char g[][SIZEX], Item& spot, int key, string& mess, int &spotLives, vector<Item> &powerpills, int &pillsLeft);
	void updateZombieCoordinates(const char g[][SIZEX], vector<ZombieItem> &zombies, Item& sp, int &spotLives, int &zombiesLeft, bool Freeze);
	void updateGrid(char g[][SIZEX], Item spot, vector<Item> &holes, vector<Item> &powerpills, vector<ZombieItem> &zombies);

	updateSpotCoordinates(grid, spot, key, message, spotLives, powerpills, pillsLeft); // Update spot coordinates
	updateZombieCoordinates(grid, zombies, spot, spotLives, zombiesLeft, Freeze); // Update all zombie coordinates

	// According to key

	updateGrid(grid, spot, holes, powerpills, zombies); // Update grid information

} // End of updateGame

//---------------------------------------------------------------------------
// Start of initialiseGame
//
// Initialise grid
// Create and place zombies in the 4 corners
// Place spot
// Create and place the holes
// Create and place the pills
//---------------------------------------------------------------------------
void initialiseGame(char grid[][SIZEX], Item& spot, vector<Item> &holes, vector<Item> &powerpills, vector<ZombieItem> &zombies) { 

	void setGrid(char[][SIZEX]);

	void createZombies(char gr[][SIZEX], vector<ZombieItem> &zombies);
	void placeZombies(char gr[][SIZEX], vector<ZombieItem> &zombies);

	void setSpotInitialCoordinates(Item& spot, char grid[][SIZEX]);
	void placeSpot(char gr[][SIZEX], Item spot);

	void createHoles(char gr[][SIZEX], vector<Item> &holes);
	void placeHoles(char gr[][SIZEX], vector<Item> &holes);

	void createPills(char gr[][SIZEX], vector<Item> &powerpills);
	void placePills(char gr[][SIZEX], vector<Item> &powerpills);

	Seed(); // Seed random number generator

	setGrid(grid); // Reset empty grid

	setSpotInitialCoordinates(spot, grid); // Initialise spots starting position

	createZombies(grid, zombies); // Create zombies
	placeZombies(grid, zombies); // Places zombies into grid

	placeSpot(grid, spot); // Place spot into grid

	createHoles(grid, holes); // Create holes
	placeHoles(grid, holes); // Place holes into grid

	createPills(grid, powerpills); // Create pills
	placePills(grid, powerpills); // Place pills into grid

} // End of initialiseGame

//---------------------------------------------------------------------------
// Start of setSpotInitialoordinates
//
// Sets spots starting place, ensuring its placed over a tunnel only
//---------------------------------------------------------------------------
void setSpotInitialCoordinates(Item& spot, char grid[][SIZEX]) { //set spot coordinates inside the grid at random at beginning of game

	bool getGridClearAt(char[][SIZEX], int x, int y);

	do {
		spot.y = Random(SIZEY - 2); // Vertical coordinate in range [1..(SIZEY - 2)]
		spot.x = Random(SIZEX - 2); // Horizontal coordinate in range [1..(SIZEX - 2)]
	} while (!getGridClearAt(grid, spot.x, spot.y));

} // End of setSpotInitialoordinates

//---------------------------------------------------------------------------
// Start of setGrid
//
// Reset the empty grid configuration
//---------------------------------------------------------------------------
void setGrid(char grid[][SIZEX]) {

	for (int row(0); row < SIZEY; ++row) { // For each column
		for (int col(0); col < SIZEX; ++col) { // For each col

			if ((row == 0) || (row == SIZEY - 1)) // Top and bottom walls
				grid[row][col] = WALL; // Draw a wall symbol
			else
				if ((col == 0) || (col == SIZEX - 1)) // Left and right walls
					grid[row][col] = WALL; // Draw a wall symbol
				else
					grid[row][col] = TUNNEL; // Draw a space

		} // End of row-loop
	} // End of col-loop

} // End of setGrid

//---------------------------------------------------------------------------
// Start of placeSpot
//
// Place spot at its new position in the grid
//---------------------------------------------------------------------------
void placeSpot(char gr[][SIZEX], Item spot) {

	gr[spot.y][spot.x] = spot.symbol;

} // End of placeSpot

//---------------------------------------------------------------------------
// Start of placeHoles
//
// Places the holes into the grid
//---------------------------------------------------------------------------
void placeHoles(char gr[][SIZEX], vector<Item> &holes) { // Place all the holes onto the grid

	for (Item hole : holes)
		gr[hole.y][hole.x] = hole.symbol;

} // End of placeHoles

//---------------------------------------------------------------------------
// Start of createHoles
//
// Creates the holes, ensuring they are placed over a tunnel
// then adds them to a vector
//---------------------------------------------------------------------------
void createHoles(char gr[][SIZEX], vector<Item> &holes) {

	bool getGridClearAt(char[][SIZEX], int x, int y);

	int holeon = 0;

	while (holeon < 12) {

		int y = Random(SIZEY - 2); //vertical coordinate in range [1..(SIZEY - 2)]
		int x = Random(SIZEX - 2); //horizontal coordinate in range [1..(SIZEX - 2)]

		if (getGridClearAt(gr, x, y)) {

			Item hole = { HOLE };

			hole.x = x;
			hole.y = y;

			hole.render = true;

			holes.push_back(hole);

			holeon++;

		} // Else grid is not a tunnel at cords

	}

} // End of createHoles

//---------------------------------------------------------------------------
// Start of placePills
//
// Places the un-collected pills into the game
//---------------------------------------------------------------------------
void placePills(char gr[][SIZEX], vector<Item> &powerpills) {

	for (Item pill : powerpills) { // Go through every pill

		if (pill.render != 0) // Check if we should render this pill
			gr[pill.y][pill.x] = pill.symbol; // If render is not false(0) then place it into the grid

	}

} // End of placePills

//---------------------------------------------------------------------------
// Start of deletePillAt
//
// Removes a certain pill from the game by setting the render value to 0
//---------------------------------------------------------------------------
void deletePillAt(vector<Item> &powerpills, int x, int y) {

	for (int i = 0; i < 8; i++) // Go through all the pills
		if (powerpills.at(i).x == x && powerpills.at(i).y == y) // Check if the X/Y of this pill match the passed in X/Y
			powerpills.at(i).render = 0; // Set the render to false(0) if they match, this will stop the pill appearing on the grid

} // End of deletePillAt

//---------------------------------------------------------------------------
// Start of deletePillAt
//
// Removes a certain pill from the game by setting the render value to 0
//---------------------------------------------------------------------------
bool isThereAPillAt(vector<Item> &powerpills, int x, int y) {

	for (int i = 0; i < 8; i++) // Go through all the pills
		if (powerpills.at(i).x == x && powerpills.at(i).y == y) // Check if the X/Y of this pill match the passed in X/Y
			if (powerpills.at(i).render != 0) // Check if this pill is being rendered
				return true; // Return true, there is a pill there

	return false; // Return false, there is not a pull there

} // End of deletePillAt

//---------------------------------------------------------------------------
// Start of createPills
//
// Creates the pills, ensuring they are created over a tunnel
// then places them into the vector
//---------------------------------------------------------------------------
void createPills(char gr[][SIZEX], vector<Item> &powerpills) {

	bool getGridClearAt(char[][SIZEX], int x, int y);

	int pillon = 0;

	while (pillon < 8) {

		int y = Random(SIZEY - 2); //vertical coordinate in range [1..(SIZEY - 2)]

		int x = Random(SIZEX - 2); //horizontal coordinate in range [1..(SIZEX - 2)]

		if (getGridClearAt(gr, x, y)) { // Ensure the grid is clear

			Item pill = { PILL };

			pill.x = x;
			pill.y = y;

			pill.render = 1; // Set the render to true

			powerpills.push_back(pill); // Add the pill to the vector

			pillon++; // Increase the pillson by 1, this ensures that 8 pills are always placed by forcing an empty space to be found

		} // Else gird is not a tunnel at cords

	}

} // End of createPills

//---------------------------------------------------------------------------
// Start of placeZombies
//
// Places the zombies into the grid
//---------------------------------------------------------------------------
void placeZombies(char gr[][SIZEX], vector<ZombieItem> &zombies) {

	for (ZombieItem zombie : zombies) {	// Go through all the zombies stored in the vector

		if (zombie.render != 0)	// Check to see if the zombie is alive
			gr[zombie.y][zombie.x] = zombie.symbol; // Place the zombies symbol into the grid at zombies coordinates

	}

} // End of placeZombies

//---------------------------------------------------------------------------
// Start of eatAllPills
//
// Removes all pills from the game
//---------------------------------------------------------------------------
void eatAllPills(vector<Item> &pills, char gr[][SIZEX]) {

	for (int i = 0; i < 8; i++) { // Go through every pill

		if (pills.at(i).render != 0) { // Check if the pill is being rendered 
			pills.at(i).render = 0; // Set the pill to not being rendered
			gr[pills.at(i).y][pills.at(i).x] = TUNNEL; // Set the current place of the pill to a tunnel on the grid
		}

	}

} // End of eatAllPills

//---------------------------------------------------------------------------
// Start of exterminateAllZombies
//
// Kills all remaining zombies
//---------------------------------------------------------------------------
void exterminateAllZombies(vector<ZombieItem> &zombies, bool Exterminate, char gr[][SIZEX]) {

	for (int i = 0; i < 4; i++) { // Go through every zombie

		int render = Exterminate == false ? 1 : 0; // Set render to 1 if Exterminate is false, else set render to 1

		if (zombies.at(i).render != render)	{ // Check to see if the zombie is alive/dead
			zombies.at(i).render = render; // Set the zombie to be dead/alive
			gr[zombies.at(i).y][zombies.at(i).x] = Exterminate == true ? TUNNEL : ZOMBIE; // Place a tunnel if Exterminate is true, else place a zombie
		}

	}

} // End of exterminateAllZombies

//---------------------------------------------------------------------------
// Start of createZombies
//
// Creates the zombies & places them into the vector
//---------------------------------------------------------------------------
void createZombies(char gr[][SIZEX], vector<ZombieItem> &zombies) {

	{ // Create Zombie 1 (top left corner)
		ZombieItem zombie = { ZOMBIE };

		zombie.render = 1;
		zombie.x = 1;
		zombie.y = 1;

		zombie.orginalx = zombie.x;
		zombie.orginaly = zombie.y;

		zombies.push_back(zombie); // Add to vector
	}

	{ // Create Zombie 2 (top right corner)
		ZombieItem zombie = { ZOMBIE };

		zombie.render = 1;
		zombie.x = SIZEX - 2;
		zombie.y = 1;

		zombie.orginalx = zombie.x;
		zombie.orginaly = zombie.y;

		zombies.push_back(zombie); // Add to vector
	}

	{ // Create Zombie 3 (bottom left corner)
		ZombieItem zombie = { ZOMBIE };

		zombie.render = 1;
		zombie.x = 1;
		zombie.y = SIZEY - 2;

		zombie.orginalx = zombie.x;
		zombie.orginaly = zombie.y;

		zombies.push_back(zombie); // Add to vector
	}

	{ // Create Zombie 4 (bottom right corner)
		ZombieItem zombie = { ZOMBIE };

		zombie.render = 1;
		zombie.x = SIZEX - 2;
		zombie.y = SIZEY - 2;

		zombie.orginalx = zombie.x;
		zombie.orginaly = zombie.y;

		zombies.push_back(zombie); // Add to vector
	}

} // End of createZombies

//---------------------------------------------------------------------------
// Start of updateGrid
//
// Updates the grid state by updating the grid configuration for each move
//---------------------------------------------------------------------------
void updateGrid(char grid[][SIZEX], Item spot, vector<Item> &holes, vector<Item> &powerpills, vector<ZombieItem> &zombies) {

	void setGrid(char[][SIZEX]);
	void placeSpot(char g[][SIZEX], Item spot);
	void placeZombies(char gr[][SIZEX], vector<ZombieItem> &zombies);
	void placeHoles(char gr[][SIZEX], vector<Item> &holes);
	void placePills(char gr[][SIZEX], vector<Item> &powerpills);

	setGrid(grid);	// Redraws the grid with new positions

	placeHoles(grid, holes); // Set holes in grid
	placePills(grid, powerpills); // Set pills in grid
	placeZombies(grid, zombies); // Set Zombies in grid
	placeSpot(grid, spot);	// Set spot in grid

} // End of updateGrid

//---------------------------------------------------------------------------
// Start of getGridClearAt
//
// Gets if there is a tunnel at a certain coordinate on the grid
//---------------------------------------------------------------------------
bool getGridClearAt(char grid[][SIZEX], int x, int y) {

	if (grid[y][x] == TUNNEL) // Check if a tunnel is found at these X/Y
		return true; // return true if there is a tunnel here

	return false; // return false if there is not a tunnel here

} // End of getGridClearAt

//---------------------------------------------------------------------------
// Start of updateZombieCoordinates
//
// Updates the zombies locations
//---------------------------------------------------------------------------
void updateZombieCoordinates(const char g[][SIZEX], vector<ZombieItem> &zombies, Item& sp, int &spotLives, int &zombiesLeft, bool Freeze) {

	void checkColide(const char g[][SIZEX], vector<ZombieItem> &zombies, Item& sp, int &spotLives);

	int spotX = sp.x;
	int spotY = sp.y;

	for (int i = 0; i < 4; i++) {

		if (Freeze) // Check if freeze is true
			break; // Stop here as zombies are currently frozen

		ZombieItem zombie = zombies.at(i);

		if (zombie.render != 0) {

			int zombieX = zombie.x;
			int zombieY = zombie.y;

			int dx(0), dy(0);

			if (zombieX > spotX)
				dx--;
			else if (zombieX < spotX)
				dx++;

			if (zombieY > spotY)
				dy--;
			else if (zombieY < spotY)
				dy++;

			const int targetY(zombies.at(i).y + dy);
			const int targetX(zombies.at(i).x + dx);

			switch (g[targetY][targetX]) {

			case TUNNEL: // Can move

				zombies.at(i).y += dy; // Go in that Y direction
				zombies.at(i).x += dx; // Go in that X direction

				break;

			case PILL: // Can move

				zombies.at(i).y += dy; // Go in that Y direction
				zombies.at(i).x += dx; // Go in that X direction

				break;

			case HOLE: // Kill zombie

				zombies.at(i).render = 0;
				zombiesLeft--;

				zombies.at(i).y = zombie.orginaly;
				zombies.at(i).x = zombie.orginalx;

				break;

			case SPOT: // Can move

				zombies.at(i).y += dy; // Go in that Y direction
				zombies.at(i).x += dx; // Go in that X direction

				break;

			}

		}

	}

	checkColide(g, zombies, sp, spotLives);

} // End of updateZombieCoordinates

//---------------------------------------------------------------------------
// Start of checkColide
//
// Checks for collision between zombies, other zombies and players
//---------------------------------------------------------------------------
void checkColide(const char g[][SIZEX], vector<ZombieItem> &zombies, Item& sp, int &spotLives) {

	// Get spots current coordinates
	int spotX = sp.x;
	int spotY = sp.y;

	for (int i = 0; i < 4; i++) { // Go through all the zombies
		
		ZombieItem zombie = zombies.at(i); // Get the zombie on at the moment

		if (zombie.render != 0) { // Ensure that this zombie is alive

			// Get the coordinates of this zombie
			int zombieX = zombie.x;
			int zombieY = zombie.y;

			if (spotX == zombieX && spotY == zombieY) { // Check to see if spots coordinates match the zombies coordinates

				// If they do, then reset the zombies coordinates to its orginal coordinates
				zombies.at(i).y = zombie.orginaly;
				zombies.at(i).x = zombie.orginalx;

				spotLives--; // Decrease spots lives by 1

			} else {

				for (int i2 = 0; i2 < 4; i2++) { // Check for collision with other zombies

					if (i == i2) // Ensure it's not the same zombie
						continue;

					ZombieItem zombie2 = zombies.at(i2);

					if (zombie2.render != 0) { // Ensure that the zombie checking against is alive

						// Get the coordinates of this second zombie
						int zombie2X = zombie2.x;
						int zombie2Y = zombie2.y;

						if (zombie2X == zombieX && zombie2Y == zombieY) { // Check for collision with other zombies
							
							// Reset both zombies to their corners

							zombies.at(i).y = zombie.orginaly;
							zombies.at(i).x = zombie.orginalx;

							zombies.at(i2).y = zombie2.orginaly;
							zombies.at(i2).x = zombie2.orginalx;

						}

					}

				}

			}

		}

	}

} // End of checkColide

//---------------------------------------------------------------------------
// Start of updateSpotCoordinates
//
// Moves spot in the required direction
//---------------------------------------------------------------------------
void updateSpotCoordinates(const char g[][SIZEX], Item& sp, int key, string& mess, int &spotLives, vector<Item> &powerpills, int &pillsLeft) {

	void setKeyDirection(int k, int& dx, int& dy);
	void deletePillAt(vector<Item> &powerpills, int x, int y);
	bool isThereAPillAt(vector<Item> &powerpills, int x, int y);

	// Calculate direction of movement required by key - if any

	int dx(0), dy(0);

	setKeyDirection(key, dx, dy); // Find direction indicated by key

	// Check new target position in grid 
	// And update spot coordinates if move is possible

	const int targetY(sp.y + dy);
	const int targetX(sp.x + dx);

	switch (g[targetY][targetX]) {	//...depending on what's on the target position in grid...

	case TUNNEL: // Can move

		sp.y += dy; // Go in that Y direction
		sp.x += dx; // Go in that X direction

		break;

	case HOLE: // Can move

		sp.y += dy; // Go in that Y direction
		sp.x += dx; // Go in that X direction

		spotLives--; // Decrease lives by 1

		break;

	case PILL: // Can move

		sp.y += dy; // Go in that Y direction
		sp.x += dx; // Go in that X direction

		deletePillAt(powerpills, targetX, targetY); // Delete the pill

		spotLives++; // Increase lives by 1
		pillsLeft--; // Decrease pill amount by 1

		break;

	case ZOMBIE: // Can move

		sp.y += dy; // Go in that Y direction
		sp.x += dx; // Go in that X direction

		if (isThereAPillAt(powerpills, targetX, targetY)) { // See if there was a pill where the zombie was standing
			deletePillAt(powerpills, targetX, targetY); // Delete the pill
			spotLives++; // Increase lives by 1
			pillsLeft--; // Decrease pill amount by 1
		}

		break;

	case WALL: // Hit a wall and stay there

		cout << '\a'; // Beep the alarm 
		mess = "CANNOT GO THERE! ";

		break;

	}

} // End of updateSpotCoordinates

//---------------------------------------------------------------------------
// Start of setKeyDirection
//
// Process the key
//---------------------------------------------------------------------------
void setKeyDirection(int key, int& dx, int& dy) {

	switch (key) { // Check for the key that was pressed

	case LEFT: // When LEFT arrow pressed...

		dx = -1; // Decrease the X coordinate
		dy = 0;

		break;

	case RIGHT: // When RIGHT arrow pressed...

		dx = +1; // Increase the X coordinate
		dy = 0;

		break;

	case UP: // When up is pressed 

		dx = 0;
		dy = -1; // Decrease the Y coordinate

		break;

	case DOWN: // When down is pressed 

		dx = 0;
		dy = 1; // Increase the Y coordinate

		break;

	}

} // End of setKeyDirection

//---------------------------------------------------------------------------
// Start of getKeyPress
//
// Get key or command selected by user
//---------------------------------------------------------------------------
int getKeyPress() { 

	int keyPressed;
	keyPressed = getch(); // Read in the selected arrow key or command letter

	while (keyPressed == 224) // Ignore symbol following cursor key
		keyPressed = getch();

	return(keyPressed);

} // End of getKeyPress

//---------------------------------------------------------------------------
// Start of isArrowKey
//
// Check if the key pressed is an arrow key (also accept 'K', 'M', 'H' and 'P')
//---------------------------------------------------------------------------
bool isArrowKey(int key) { 
	return ((key == LEFT) || (key == RIGHT) || (key == UP) || (key == DOWN));
} // End of isArrowKey

//---------------------------------------------------------------------------
// Start of wantToQuit
//
// Check if the key pressed is 'Q' or 'q'
//---------------------------------------------------------------------------
bool wantToQuit(int key) { 
	return (key == QUIT || key == tolower(QUIT));
} // End of wantToQuit

//---------------------------------------------------------------------------
// Start of wantToPlay
//
// Check if the key pressed is 'P' or 'p'
//---------------------------------------------------------------------------
bool wantToPlay(int key) {
	return (key == PLAY || key == tolower(PLAY));
} // End of wantToPlay

//---------------------------------------------------------------------------
// Start of wantToOpenInformation
//
// Check if the key pressed is 'I' or 'i'
//---------------------------------------------------------------------------
bool wantToOpenInformation(int key) {
	return (key == INFORMATION || key == tolower(INFORMATION));
} // End of wantToOpenInformation

//---------------------------------------------------------------------------
// Start of wantToFreeze
//
// Check if the key pressed is 'F'
//---------------------------------------------------------------------------
bool wantToFreeze(int key) {
	return (key == 'f' || key == 'F');
} // End of wantToFreeze

//---------------------------------------------------------------------------
// Start of wantToExterminate
//
// Check if the key pressed is 'X'
//---------------------------------------------------------------------------
bool wantToExterminate(int key) {
	return (key == 'x' || key == 'X');
} // End of wantToExterminate

//---------------------------------------------------------------------------
// Start of wantToEat
//
// Check if the key pressed is 'E'
//---------------------------------------------------------------------------
bool wantToEat(int key) {
	return (key == 'e' || key == 'E');
} // End of wantToEat

//---------------------------------------------------------------------------
// Start of clearMessage
//
// Clear message area on screen
//---------------------------------------------------------------------------
void clearMessage() { 

	SelectBackColour(clBlack);
	SelectTextColour(clWhite);

	Gotoxy(40, 8);

	string str(20, ' ');

	cout << str; // Display blank message

} // End of clearMessage

//---------------------------------------------------------------------------
// Start of renderGame
//
// Display game title, messages, maze, spot and apples on screen
//---------------------------------------------------------------------------
void renderGame(const char gd[][SIZEX], string mess, int spotLives, int zombies, int pills, string name, int oldhighscore) {

	void paintGrid(const char g[][SIZEX]);
	void showTitle();
	void showOptions();
	void showMessage(string);
	void displayGameInfo(int spotLives, int zombies, int pills, string name, int oldhighscore);

	Gotoxy(0, 0);

	// Display grid contents
	paintGrid(gd);

	// Display game title
	showTitle();

	// Display menu options available
	showOptions();

	// Display game info
	displayGameInfo(spotLives, zombies, pills, name, oldhighscore);

	// Display message if any
	showMessage(mess);

} //end of renderGame

//---------------------------------------------------------------------------
// Start of paintGrid
//
// Display grid content on screen
//---------------------------------------------------------------------------
void paintGrid(const char g[][SIZEX]) { 

	SelectBackColour(clBlack);
	SelectTextColour(clWhite);

	Gotoxy(0, 2);

	for (int row(0); row < SIZEY; ++row) { // For each row (vertically)
		for (int col(0); col < SIZEX; ++col) { // For each column (horizontally)
			cout << g[row][col]; // Output cell content
		} // End of col-loop
		cout << endl;
	} // End of row-loop

} // End of paintGrid

//---------------------------------------------------------------------------
// Start of showTitle
//
// Display game title
//---------------------------------------------------------------------------
void showTitle() { 
	void createTime(int x, int y);
	SelectTextColour(clYellow);

	Gotoxy(0, 0);
	cout << " ZOMBIES GAME BASIC VERSION \n" << endl;

	SelectBackColour(clWhite);
	SelectTextColour(clRed);

	//Gotoxy(40, 0);
	//cout << "Pascale Vacher: March 15";
	createTime(40, 1);

} // End of showTitle

//---------------------------------------------------------------------------
// Start of showOptions
//
// Show game options
//---------------------------------------------------------------------------
void showOptions() {

	SelectBackColour(clRed);
	SelectTextColour(clYellow);

	Gotoxy(40, 5);
	cout << "TO MOVE USE KEYBOARD ARROWS ";

	Gotoxy(40, 6);
	cout << "TO QUIT ENTER 'Q' ";

} // End of showOptions

//---------------------------------------------------------------------------
// Start of displayGameInfo
//
// Display game stats, remaining lives, zombies and pills
//---------------------------------------------------------------------------
void displayGameInfo(int spotLives, int zombies, int pills, string name, int oldhighscore) {

	SelectBackColour(clBlack);

	SelectTextColour(clWhite);

	Gotoxy(40, 10);
	cout << "REMAINING LIVES: " << spotLives << "   ";

	Gotoxy(40, 11);
	cout << "REMAINING ZOMBIES: " << zombies;

	Gotoxy(40, 12);
	cout << "REMAINING PILLS: " << pills;

	Gotoxy(40, 17);
	cout << "PLAYER'S NAME: ";
	SelectTextColour(clYellow);
	cout << name;

	Gotoxy(40, 18);
	SelectTextColour(clWhite);
	cout << "PLAYER'S HIGH SCORE: ";
	SelectTextColour(clGreen);
	cout << oldhighscore;

} // End of displayGameInfo

//---------------------------------------------------------------------------
// Start of showMessage
//
// Print auxiliary messages if any
//---------------------------------------------------------------------------
void showMessage(string m) { 

	SelectBackColour(clBlack);
	SelectTextColour(clWhite);

	Gotoxy(40, 8);

	cout << m;	// Display current message

} // End of showMessage

//---------------------------------------------------------------------------
// Start of endProgram
//
// End program with appropriate message
//---------------------------------------------------------------------------
void endProgram(string quitMessage) { 

	void clearMessage();
	clearMessage();

	SelectBackColour(clBlack);

	SelectTextColour(clYellow);

	Gotoxy(40, 8);

	cout << quitMessage;

	// Hold output screen until a keyboard key is hit

	Gotoxy(40, 9);

	system("pause");

} // End of endProgram

//---------------------------------------------------------------------------
// Start of createTitleScreen
//
// Display the title screen to the user
//---------------------------------------------------------------------------
void createTitleScreen(bool showhelp) {

	void createTime(int x, int y);
	createTime(50, 5);

	Gotoxy(10, 5);
	SelectBackColour(clWhite);
	SelectTextColour(clRed);

	cout << "--------------------";
	Gotoxy(10, 6);
	cout << ": SPOT AND ZOMBIES :";
	Gotoxy(10, 7);
	cout << "--------------------";

	Gotoxy(10, 12);
	cout << "GROUP 1RR "; // Display group

	// Display group members

	Gotoxy(10, 13);
	cout << "Hamish Mackay     : b4014566";

	Gotoxy(10, 14);
	cout << "James Kirk        : b4012447";

	Gotoxy(10, 15);
	cout << "Robert Jefferies  : b4016187";

	if (showhelp) { // Check if we should display the help to the user

		SelectBackColour(clBlack);
		SelectTextColour(clYellow);

		Gotoxy(50, 13);
		cout << "START GAME: ";
		SelectTextColour(clCyan); // Print the letter required in a different colour
		cout << "P";

		Gotoxy(50, 14);
		SelectTextColour(clYellow);
		cout << "HOW TO PLAY: ";
		SelectTextColour(clCyan); // Print the letter required in a different colour
		cout << "I";

		Gotoxy(50, 16);
		SelectTextColour(clRed);
		cout << "QUIT: ";
		SelectTextColour(clCyan); // Print the letter required in a different colour
		cout << "Q";

	}

} // End of createTitleScreen

//---------------------------------------------------------------------------
// Start of getPlayerName
//
// Asks for and returns the name inputted by the user
//---------------------------------------------------------------------------
string getPlayerName() {

	string name;

	SelectBackColour(clBlack);
	SelectTextColour(clWhite);

	Gotoxy(10, 16);
	cout << "Enter Your Name: ";

	SelectTextColour(clYellow);
	cin >> name;

	while (name.length() > 20) { // Keep asking for a name if it's longer than 20 letters

		Gotoxy(10, 18);
		SelectTextColour(clRed);
		cout << "Name too long (Should be 20 or letters or less)";

		Gotoxy(10, 16);
		cout << "                                                ";
		Gotoxy(10, 16);
		SelectTextColour(clWhite);
		cout << "Enter Your Name: ";
		SelectTextColour(clYellow);
		cin >> name;

	}

	return name; // Return the players name

} // End of getPlayerName

//---------------------------------------------------------------------------
// Start of clearScreen
//
// Clear everything currently on the screen
//---------------------------------------------------------------------------
void clearScreen() {
	system("cls");
} // End of clearScreen

//---------------------------------------------------------------------------
// Start of createTime
//
// Display the date & time at an passed in X/Y 
//---------------------------------------------------------------------------
void createTime(int x,int y) {

	time_t currentTime;
	struct tm *localTime;

	time(&currentTime);                   // Get the current time
	localTime = localtime(&currentTime);  // Convert the current time to the local time

	int Day = localTime->tm_mday;		  // Get the current day of the month
	int Month = localTime->tm_mon + 1;    // Get the curent month
	int Year = localTime->tm_year + 1900; // Get the currnt year

	// Get the current time
	int Hour = localTime->tm_hour;		 
	int Min = localTime->tm_min;
	int Sec = localTime->tm_sec;

	Gotoxy(x, y);
	SelectBackColour(clWhite);
	SelectTextColour(clBlack);
	
	// Display the date, filling in single letter days/months with an extra 0 (e.g. 4 changes to 04)
	cout << "DATE: " << setfill('0') << setw(2) << Day << "/" << setfill('0') << setw(2) << Month << "/" << Year;

	Gotoxy(x, y+1);

	// Display the time, filling in single letter hours/min/secs with an extra 0 (e.g. 8 changes to 08)
	cout << "TIME: " << setfill('0') << setw(2) << Hour << ":" << setfill('0') << setw(2) << Min << ":" << setfill('0') << setw(2) << Sec;

} // End of createTime

//---------------------------------------------------------------------------
// Start of printInfo
//
// Display game help screen to the user
//---------------------------------------------------------------------------
void printInfo() {

	SelectBackColour(clBlack);
	SelectTextColour(clGrey);

	// Display basic info of the game

	Gotoxy(5, 3);
	cout << "SCORE CALCULATED BY AMOUNT OF LIVES LEFT AFTER ZOMBIES HAVE DIED."; 

	Gotoxy(5, 4);
	cout << "AVOID ZOMBIES(Z) AND HOLES(0). COLLECT PILLS(*) TO INCREASE LIVES.";

	Gotoxy(5, 5);
	cout << "ZOMBIES DIE WHEN THEY FALL DOWN HOLES (SO WILL YOU)";

	SelectTextColour(clYellow);

	// Display cheats

	Gotoxy(5, 8);
	cout << "CHEATS:";

	Gotoxy(5, 9);
	SelectTextColour(clCyan);
	cout << "F";
	SelectTextColour(clYellow); // Change the colour
	cout << " TO FREEZE THE ZOMBIES";


	Gotoxy(5, 10);
	SelectTextColour(clRed);
	cout << "X";
	SelectTextColour(clYellow); // Change the colour
	cout << " TO TOGGLE THE ZOMBIES";


	Gotoxy(5, 11);
	SelectTextColour(clGreen);
	cout << "E";
	SelectTextColour(clYellow); // Change the colour
	cout << " TO EAT THE PILLS";

	Gotoxy(5, 13);
	cout << "Press Enter to return to the home screen";
	
} // End of printInfo