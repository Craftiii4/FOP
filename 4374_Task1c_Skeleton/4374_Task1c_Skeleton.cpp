
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
	void createTitleScreen();
	void initialiseGame(char grid[][SIZEX], Item& spot, vector<Item> &holes, vector<Item> &powerpills, vector<ZombieItem> &zombies);
	bool wantToQuit(int k);
	bool wantToFreeze(int k);
	bool wantToExterminate(int key);
	bool wantToEat(int key);
	bool isArrowKey(int k);
	int getKeyPress();
	void updateGame(char g[][SIZEX], Item& sp, int k, string& mess, vector<Item> &holes, vector<Item> &powerpills, vector<ZombieItem> &zombies, int &spotLives, int &pillsLeft, int &zombiesLeft, bool Freeze);
	void renderGame(const char g[][SIZEX], string mess, int spotLives, int zombiesLeft, int pillsLeft);
	void endProgram(string quitMessage);
	void createHoles(char gr[][SIZEX], vector<Item> &holes);
	void exterminateAllZombies(vector<ZombieItem> &zombies, bool Exterminate, char gr[][SIZEX]);
	void eatAllPills(vector<Item> &pills, char gr[][SIZEX]);

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

	string playername = "";

	createTitleScreen();

	while (playername == "") {



	}

	string message("LET'S START... "); // Current message to player

	initialiseGame(grid, spot, holes, powerpills, zombies); // Initialise grid (incl. walls and spot)

	int key(' '); // Create key to store keyboard events

	do {

		renderGame(grid, message, spotLives, zombiesLeft, pillsLeft); // Render game state on screen

		message = "                                     ";

		key = getKeyPress(); // Read in next keyboard event

		if (isArrowKey(key))
			updateGame(grid, spot, key, message, holes, powerpills, zombies, spotLives, pillsLeft, zombiesLeft, Freeze);
		else if (wantToFreeze(key))
			Freeze = !Freeze;
		else if (wantToEat(key)) {
			Eat = !Eat;
			if (Eat) {
				eatAllPills(powerpills, grid);
				spotLives += pillsLeft;
				pillsLeft = 0;
			}
		} else if (wantToExterminate(key)) {
			Exterminate = !Exterminate;
			zombiesLeft = Exterminate == true ? 0 : 4;
			exterminateAllZombies(zombies, Exterminate, grid);
		} else
			message = "INVALID KEY! "; // Set 'Invalid key' message

	} while (!wantToQuit(key) && spotLives > 0 && !(zombiesLeft == 0 && pillsLeft == 0)); // While user does not want to quit
	
	if (spotLives > 0) {
		if (zombiesLeft == 0 && pillsLeft == 0) {

			string endMessage;	// Final string to pass to endProgram
			ostringstream convert;	// streams used for the conversion of int to string
			convert << spotLives;	// puts textual representation of spotLivesi nto stream
			endMessage = convert.str();	// sets endMessage to conents of stream
			endMessage = "PLAYER WINS WITH: " + endMessage + " LIVES REMAINING"; // formats endMessage
			endProgram(endMessage);
			// TODO : Display how many lives left
		} else
			endProgram("PLAYER QUITS! "); // Display final message
	} else {

		void displayGameInfo(int spotLives, int zombies, int pills);
		displayGameInfo(spotLives, zombiesLeft, pillsLeft);

		endProgram("YOU LOST!");

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

	for (Item pill : powerpills) {

		if (pill.render != 0)
			gr[pill.y][pill.x] = pill.symbol;

	}

} // End of placePills

//---------------------------------------------------------------------------
// Start of deletePillAt
//
// Removes a certain pill from the game by setting the render value to 0
//---------------------------------------------------------------------------
void deletePillAt(vector<Item> &powerpills, int x, int y) {

	for (int i = 0; i < 8; i++)
		if (powerpills.at(i).x == x && powerpills.at(i).y == y)
			powerpills.at(i).render = 0;

} // End of deletePillAt

//---------------------------------------------------------------------------
// Start of deletePillAt
//
// Removes a certain pill from the game by setting the render value to 0
//---------------------------------------------------------------------------
bool isThereAPillAt(vector<Item> &powerpills, int x, int y) {

	for (int i = 0; i < 8; i++)
		if (powerpills.at(i).x == x && powerpills.at(i).y == y)
			if (powerpills.at(i).render != 0)
				return true;

	return false;

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

		if (getGridClearAt(gr, x, y)) {

			Item pill = { PILL };

			pill.x = x;
			pill.y = y;

			pill.render = 1;

			powerpills.push_back(pill);

			pillon++;

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

	for (int i = 0; i < 8; i++) {

		if (pills.at(i).render != 0)	{
			pills.at(i).render = 0; 
			gr[pills.at(i).y][pills.at(i).x] = TUNNEL;
		}

	}

} // End of eatAllPills

//---------------------------------------------------------------------------
// Start of exterminateAllZombies
//
// Kills all remaining zombies
//---------------------------------------------------------------------------
void exterminateAllZombies(vector<ZombieItem> &zombies, bool Exterminate, char gr[][SIZEX]) {

	for (int i = 0; i < 4; i++) {

		int render = Exterminate == false ? 1 : 0;

		if (zombies.at(i).render != render)	{ // Check to see if the zombie is alive/dead
			zombies.at(i).render = render; // Set the zombie to be dead/alive
			gr[zombies.at(i).y][zombies.at(i).x] = Exterminate == true ? TUNNEL : ZOMBIE;
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

		zombies.push_back(zombie);
	}

	{ // Create Zombie 2 (top right corner)
		ZombieItem zombie = { ZOMBIE };

		zombie.render = 1;
		zombie.x = SIZEX - 2;
		zombie.y = 1;

		zombie.orginalx = zombie.x;
		zombie.orginaly = zombie.y;

		zombies.push_back(zombie);
	}

	{ // Create Zombie 3 (bottom left corner)
		ZombieItem zombie = { ZOMBIE };

		zombie.render = 1;
		zombie.x = 1;
		zombie.y = SIZEY - 2;

		zombie.orginalx = zombie.x;
		zombie.orginaly = zombie.y;

		zombies.push_back(zombie);
	}

	{ // Create Zombie 4 (bottom right corner)
		ZombieItem zombie = { ZOMBIE };

		zombie.render = 1;
		zombie.x = SIZEX - 2;
		zombie.y = SIZEY - 2;

		zombie.orginalx = zombie.x;
		zombie.orginaly = zombie.y;

		zombies.push_back(zombie);
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
// Start of updateGrid
//
// Gets if there is a tunnel at a certain coordinate on the grid
//---------------------------------------------------------------------------
bool getGridClearAt(char grid[][SIZEX], int x, int y) {

	Gotoxy(0, 30);
	cout << grid[y][x];

	if (grid[y][x] == TUNNEL)
		return true;

	return false;

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

		if (Freeze)
			break;

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
// Start of setKeyDirection
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
// Check if the key pressed is 'Q'
//---------------------------------------------------------------------------
bool wantToQuit(int key) { 
	return (key == QUIT);
} // End of wantToQuit

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
void renderGame(const char gd[][SIZEX], string mess, int spotLives, int zombies, int pills) { 

	void paintGrid(const char g[][SIZEX]);
	void showTitle();
	void showOptions();
	void showMessage(string);
	void displayGameInfo(int spotLives, int zombies, int pills);

	Gotoxy(0, 0);

	// Display grid contents
	paintGrid(gd);

	// Display game title
	showTitle();

	// Display menu options available
	showOptions();

	// Display game info
	displayGameInfo(spotLives, zombies, pills);

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

	SelectTextColour(clYellow);

	Gotoxy(0, 0);
	cout << "___ZOMBIES GAME SKELETON___\n" << endl;

	SelectBackColour(clWhite);
	SelectTextColour(clRed);

	Gotoxy(40, 0);
	cout << "Pascale Vacher: March 15";

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
void displayGameInfo(int spotLives, int zombies, int pills) {

	SelectBackColour(clBlack);

	SelectTextColour(clWhite);

	Gotoxy(40, 10);
	cout << "REMAINING LIVES: " << spotLives << "   ";

	Gotoxy(40, 11);
	cout << "REMAINING ZOMBIES: " << zombies;

	Gotoxy(40, 12);
	cout << "REMAINING PILLS: " << pills;

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

void createTitleScreen() {

	void createTime();
	createTime();

	Gotoxy(10, 5);
	SelectBackColour(clWhite);
	SelectTextColour(clRed);

	cout << "--------------------";
	Gotoxy(10, 6);
	cout << ": SPOT AND ZOMBIES :";
	Gotoxy(10, 7);
	cout << "--------------------";

	Gotoxy(10, 12);
	cout << "GROUP 1RR ";

	Gotoxy(10, 13);
	cout << "Hamish Mackay     : b4014566";

	Gotoxy(10, 14);
	cout << "James Kirk        : b4012447";

	Gotoxy(10, 15);
	cout << "Robert Jefferies  : b4016187";

}

void createTime() {

	time_t currentTime;
	struct tm *localTime;

	time(&currentTime);                   // Get the current time
	localTime = localtime(&currentTime);  // Convert the current time to the local time

	int Day = localTime->tm_mday;
	int Month = localTime->tm_mon + 1;
	int Year = localTime->tm_year + 1900;
	int Hour = localTime->tm_hour;
	int Min = localTime->tm_min;
	int Sec = localTime->tm_sec;

	Gotoxy(50, 5);
	SelectBackColour(clWhite);
	SelectTextColour(clBlack);

	cout << "DATE: " << Day << "/" << Month << "/" << Year;

	Gotoxy(50, 6);

	cout << "TIME: " << Hour << ":" << Min << ":" << Sec;

}