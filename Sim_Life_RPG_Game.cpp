#include <iostream>
#include <string>

using namespace std;

class Room {
private:
    // letters outline where the location is
    char mapLetter;

    // locations are rectangles....
    int topLeftX;
    int topLeftY;
    int bottomRightX;
    int bottomRightY;

    // all good things have a name, right?
    string roomName;

    // constructor to make a room!  need all the above information
public:
	Room();
	Room(string name, char symbol, int tlx, int tly, int brx, int bry);
    // Below: get functions to get all the information about a room

    string getName();
    char getLetter();
    int getTopLeftX();
    int getTopLeftY();
    int getBottomRightX();
    int getBottomRightY();
};

Room::Room()
{
	roomName="The Void";
	mapLetter='?';
	topLeftX=-1;
	topLeftY=-1;
	bottomRightX=-1;
	bottomRightY=-1;
}	

Room::Room(string name, char symbol, int tlx, int tly, int brx, int bry)
{
	roomName=name;
	mapLetter=symbol;
	topLeftX=tlx;
	topLeftY=tly;
	bottomRightX=brx;
	bottomRightY=bry;
}

string Room::getName()
{
	return roomName;
}

char Room::getLetter()
{
	return mapLetter;
}

int Room::getTopLeftX()
{
	return topLeftX;
}

int Room::getTopLeftY()
{
	return topLeftY;
}

int Room::getBottomRightX()
{
	return bottomRightX;
}

int Room::getBottomRightY()
{
	return bottomRightY;
}


/* ABOVE ROOM */
/* BELOW MAP */

class Map {
private:
    // size of the world
    static const int MAX_ROWS = 20;
    static const int MAX_COLUMNS = 40;

    // characters for blank space and you, the player
    static const char BLANK = ' ';
    static const char PLAYER = 'Y';

    // stores all the characters on the world
    char grid[MAX_ROWS][MAX_COLUMNS];
    
    // list of the possible locations to go to
    Room mapRooms[100];
    int usedRooms;

    // current place you are
    Room currentLocation;
    void generateMap();
    
public:
    Map();
    void addRoom(Room r);
    void displayMap();    // this simply shows the map on the screen...
    Room getPlayerLocation();
    int getLocations(Room changeMe[100]); // copies rooms into changeMe, returns the size
    void moveToRoom(Room destination);
};

Map::Map()
{
	usedRooms = 0;
	
	// initialize the map to be blank
	for(int i=0; i < MAX_ROWS; i++)
	{
		for(int j=0; j < MAX_COLUMNS; j++)
		{
			grid[i][j] = BLANK;
		}
	}

	// make 3 rooms
	Room outside("Outside", 'O', 0,0, 39,19);
	Room university("University", 'U', 5,5, 15,15);
	Room dorm("Dorm", 'D', 25,5, 30,10);

	// add them to our arrayList
	addRoom(outside);
	addRoom(university);
	addRoom(dorm);

	// start out in your room
	currentLocation = dorm;
	moveToRoom(currentLocation);

	// generate the map (rooms)
	generateMap();
}

void Map::addRoom(Room r)
{
	mapRooms[usedRooms] = r;
	usedRooms++;
}

void Map::generateMap()
{
	// for all the rooms we have in our list...
	for(int i=0; i < usedRooms; i ++)
	{
		Room place = mapRooms[i];
		// Fill in top and bottom of rectangle with characters
		for(int i = place.getTopLeftX(); i <= place.getBottomRightX(); i++)
		{
			grid[place.getTopLeftY()][i] = place.getLetter();
			grid[place.getBottomRightY()][i] = place.getLetter();
		}
		// fill in left and right side...
		for(int i = place.getTopLeftY(); i <= place.getBottomRightY(); i++)
		{
			grid[i][place.getTopLeftX()] = place.getLetter();
			grid[i][place.getBottomRightX()] = place.getLetter();
		}
	}
}

// this simply shows the map on the screen...
void Map::displayMap()
{
	for(int i=0; i < MAX_ROWS; i++)
	{
		for(int j=0; j < MAX_COLUMNS; j++)
		{
			cout << " " << grid[i][j];
		}
		cout << endl;
	}
}

// get where we are
Room Map::getPlayerLocation()
{
	return currentLocation;
}

// get the possible locations to go to
int Map::getLocations(Room changeMe[100])
{
	for(int i=0; i < usedRooms; i++)
	{
		changeMe[i] = mapRooms[i];
	}
	
	return usedRooms;
}

// update the map so the player symbol moves to the a new location
void Map::moveToRoom(Room destination)
{
	// if were somewhere before
	if(currentLocation.getTopLeftY() != -1) // if this is false, the room does not exist
	{
		// remove the mark from the center of the previous location
		grid[currentLocation.getTopLeftY() + (currentLocation.getBottomRightY() - currentLocation.getTopLeftY())/2]
				[currentLocation.getTopLeftX() +(currentLocation.getBottomRightX() - currentLocation.getTopLeftX())/2]
				= BLANK;
	}

	// update where we are
	currentLocation = destination;

	// add player mark to center of new location
	grid[currentLocation.getTopLeftY() + (currentLocation.getBottomRightY() - currentLocation.getTopLeftY())/2]
			[currentLocation.getTopLeftX() + (currentLocation.getBottomRightX() - currentLocation.getTopLeftX())/2]
			= PLAYER;			
			
}


class Player
{
private:
    // character has 3 stats: entertainment, energy and smartness
    Map world;
    int entertainment;
    int energy;
    int smartness;
	
public:
    Player();
    Player(Map world);
    int removeSmartness(int amount);
    void doAction(char action, int time);
    int addSmartness(int amount);
    int removeEnergy(int amount);
    int getSmartness();
    int addEnergy(int amount);
    int getEnergy();
    int removeEntertainment(int amount);
    int addEntertainment(int amount);
    int getEntertainment();
    void displayStats();
    void travel(int counter);
    char requestAction();
    void displayMap();

};

// Tell them where they are and what actions they can take (and ask them as the method says)
char Player::requestAction()
{
	// get where you are
	Room location = world.getPlayerLocation();

	// display where you are
	cout << "You are at the " + location.getName() + ".\n";

	// if at uni
	if(location.getName().compare("University") == 0)
	{
		cout << "Do you want to (A)ttend class or (S)leep through lecture?\n";
	}
	// if at dorm
	else if(location.getName().compare("Dorm") == 0)
	{
		cout << "Do you want to (S)leep, do (H)omeowrk or (W)atch some YouTube?\n";
	}
	// if outside
	else if(location.getName().compare("Outside") == 0)
	{
		cout << "Do you want to (S)ocialize or (P)lay Rugby?\n";
	}
	// if you are not in any of the above, we have a problem
	else
	{
		cout << "You are lost in the abyss...\n";
	}
	// they can also change locations by 'G'
	cout << "Do you want to (G)o to a different location?\n";

	// let them enter a choice
	string answer;
	getline(cin,answer);

	// if they entered something, give back the first character (capitalized)
	if(answer.length() > 0)
	{
		return toupper(answer[0]);
	}
	// otherwise they just hit enter (trying to crash me!), so they do nothing
	else
	{
		return ' ';
	}
}


// chage locations
void Player::travel(int counter)
{
	// destionations the names of possible destinations
	Room destinations[100];
	char r;
	int m;
	// to do this, we need to copy in each name manually
	int roomCount = world.getLocations(destinations);
	if (counter==0)
	{
		r='D';
		m=0;
	}
	else
	{
				m=1;
				// show the possible destinations
				cout << "Where do you want to travel to: (";
				if(roomCount>0)
				{
					cout << destinations[0].getLetter();
				}
				for(int i=1; i < roomCount; i++)
				{
					cout << ", " << destinations[i].getLetter(); 
				}
				cout <<")?\n";

				// read from the keyboard where they want to go
				string response;
				getline(cin, response);
				// if they didn't enter anything (shame on them) they go nowhere
				if(response.length() == 0)
				{
					cout << "You go nowhere...\n";
					return;
				}
				// otherwise pull out the first character
				r = toupper(response[0]);

				// find the room that matches this character
		}
	Room destination;
	for(int i=0; i < roomCount; i++)
	{
		Room place = destinations[i];
		if(place.getLetter() == r)
		{
			destination = place;
		}
	}

	// if we didnt find the room, they entered a bad character!
	if(destination.getBottomRightX() == -1 ) // not a valid room
	{
		cout << "Invalid location, you go nowhere...\n";
	}
	// otherwise go to that location
	else
	{
		if(m==0)
		{
		}
		else
		{
			cout << "You go to " + destination.getName() + ".\n";
		}
		world.moveToRoom(destination);            
	}
}

// display stats... yes what the methods says it does...
void Player::displayStats()
{
	cout << "Current stats are: ";
	cout << "Energy = " << energy <<", ";
	cout << "Entertainment = " << entertainment <<", ";
	cout << "Smartness = " << smartness <<".\n";
}


// start out at 70 in each stat
Player::Player()
{
	entertainment = 70;
	energy = 70;
	smartness = 70;
}
	
// hopefully you exist in a world...
Player::Player(Map world)
{
	entertainment = 70;
	energy = 70;
	smartness = 70;

	Player::world = world;
}

// list of possible actions....
void Player::doAction(char action, int time)
{
	// get where we are
	Room location = world.getPlayerLocation();
	string locationName = location.getName();

	// keep track of the effects of our actions...
	int entertainmentChange = 0;
	int energyChange = 0;
	int smartnessChange = 0;

	// long list of options below based on time/location! yay if/else
	if(action == 'G')
	{
		travel(1);
		energyChange = removeEnergy(4);
	}
	else if(locationName.compare("University") == 0)
	{
		if(8 < time && time < 16)
		{
			if(action == 'S')
			{
			
				cout << "You put your head down and snore very loudly.\n";
				energyChange = addEnergy(10);
				smartnessChange = addSmartness(1);
				entertainmentChange = removeEntertainment(2);
			}
			else if(action == 'A')
			{
				cout << "You take copious notes and pay close attention to the material.\n";
				energyChange = removeEnergy(4);
				smartnessChange = addSmartness(3);
				entertainmentChange = removeEntertainment(20);
			}
			else
			{
				cout << "Bad command, you do nothing for an hour.\n";
				energyChange = removeEnergy(2);
				entertainmentChange = removeEntertainment(2);
			}
		}
		else
		{
			cout << "There are no classes in session, you waste an hour of your time.\n";
			energyChange = removeEnergy(4);
			entertainmentChange = removeEntertainment(3);
		}
	}
	else if(locationName.compare("Dorm") == 0)
	{
		if(action == 'S')
		{
			if(8 < time && time < 20)
			{
				cout << "Since you are diurnal, you get an hour of unrestful sleep.\n";
				energyChange = addEnergy(4);
				entertainmentChange = removeEntertainment(5);
			}
			else
			{
				cout << "You get a good hour of sleep.\n";
				energyChange = addEnergy(10);
				entertainmentChange = removeEntertainment(5);
			}
		}
		else if(action == 'H')
		{
			cout << "You break out the books and pound through some problems.\n";
			energyChange = removeEnergy(3);
			smartnessChange = addSmartness(3);
			entertainmentChange = removeEntertainment(20);
		}
		else if(action == 'W')
		{
			cout << "OMG!! CATS!!!!\n";
			energyChange = removeEnergy(2);
			smartnessChange = removeSmartness(1);
			entertainmentChange = addEntertainment(20);
		}
		else
		{
			cout << "You stare at the ceiling for an hour...\n";
			energyChange = removeEnergy(1);
			entertainmentChange = removeEntertainment(3);
		}
	}
	else if(locationName.compare("Outside") == 0)
	{
		if(action == 'S')
		{
			cout << "Party hard!\n";
			energyChange = removeEnergy(8);
			entertainmentChange = addEntertainment(8);
		}
		else if(action == 'P')
		{
			if(8 < time && time < 20)
			{
				cout << "You run for almost an hour straight (when not flattened on the ground).\n";
				energyChange = removeEnergy(15);
				smartnessChange = removeSmartness(1);
				entertainmentChange = addEntertainment(12);
			}
			else
			{
				cout << "It is too dark and you end up falling flat on your face.\n";
				energyChange = removeEnergy(6);
				smartnessChange = removeSmartness(3);
				entertainmentChange = removeEntertainment(1);
			}
		}
		else
		{
			cout << "You wander around outside for an hour.\n";
			energyChange = removeEnergy(6);
			smartnessChange = removeSmartness(1);
			entertainmentChange = addEntertainment(3);
		}
	}
	else
	{
		cout << "You are currently lost, which is not a lot of fun\n";
		energyChange = removeEnergy(2);
		entertainmentChange = removeEntertainment(2);
	}

	// end list of actions based on time/location

	// tell the user the effects of actions
	cout << "From your actions changed your stats by... ";
	cout << "Energy: " << energyChange << ", ";
	cout << "Entertainment: " << entertainmentChange << ", ";
	cout << "Smartness: " << smartnessChange << ".\n";
}        

// below are methods for adding, removing and getting the value the 3 stats

// get the value of entertainment
int Player::getEntertainment()
{
	return entertainment;
}
// adds amount to "entertainment" and returns the change
int Player::addEntertainment(int amount)
{
	if (energy<=50)
	{
		amount/=2;
	}
	// remember the old value
	int oldEntertainment = entertainment;

	// add the entertainment amount
	entertainment = entertainment + amount;

	// don't let entertainment go over 100
	entertainment = min(entertainment, 100);

	return entertainment - oldEntertainment;
}

//subtracts amount from "entertainment" and returns the change
int Player::removeEntertainment(int amount)
{
	if (energy<=50)
	{
		amount*=2;
	}
	// remember the old value
	int oldEntertainment = entertainment;

	// add the entertainment amount
	entertainment = entertainment - amount;

	// don't let entertainment go under 0
	entertainment = max(entertainment, 0);

	return entertainment - oldEntertainment;
}

// get the value of energy
int Player::getEnergy()
{
	return energy;
}

// adds amount to "energy" and returns the change
int Player::addEnergy(int amount)
{
	if (energy<=50)
	{
		amount/=2;
	}
	// remember the old value
	int oldEnergy = energy;

	// add the energy amount
	energy = energy + amount;

	// don't let energy go over 100
	energy = min(energy, 100);

	return energy - oldEnergy;
}


// get the value of entertainment
int Player::getSmartness()
{
	return smartness;
}


//subtracts amount from "energy" and returns the change
int Player::removeEnergy(int amount)
{
	if(energy<=50)
	{
		amount*=2;
	}
	// remember the old value
	int oldEnergy = energy;

	// add the energy amount
	energy = energy - amount;

	// don't let energy go under 0
	energy = max(energy, 0);

	return energy - oldEnergy;
}
// adds amount to "smartness" and returns the change
int Player::addSmartness(int amount)
{
	if(energy<=50)
	{
		amount/=2;
	}
	// remember the old value
	int oldSmartness = smartness;

	// add the smartness amount
	smartness = smartness + amount;

	// don't let smartness go over 100
	smartness = min(smartness, 100);

	return smartness - oldSmartness;
}


//subtracts amount from "smartness" and returns the change
int Player::removeSmartness(int amount)
{
	if(energy<=50)
	{
		amount*=2;
	}
	// remember the old value
	int oldSmartness = smartness;

	// add the smartness amount
	smartness = smartness - amount;

	// don't let smartness go under 0
	smartness = max(smartness, 0);

	return smartness - oldSmartness;
}

// shows the map to the screen
void Player::displayMap()
{
	// cannot do this directly as map is private
	world.displayMap();
}

// the current time (0:00  - 23:59)
int dayTime = 8;

// you, the player
Player player;

// some useful functions...
void clearScreen();
void displayTime();
bool updatePlayer();

void displayTime()
{
	// display the time
	cout << "Current time: "<<dayTime<<":00.  \n";

	// add an hour (and loop so 24:00 -> 0:00)
	dayTime = (dayTime+1)%24;
}


bool updatePlayer()
{

	// default: keep on player
	bool gameOver = false;

	// all work and no play...
	if(player.getEntertainment() == 0)
	{
		cout << "Your life is sapped of color and you become a mindless cog in the population.\n";
		gameOver = true;
	}
	// dum dum dummmm!
	else if(player.getSmartness() == 0)
	{
		cout << "You drop out of college and are forced to drive taxis for the rest of your life.\n";
		gameOver = true;
	}

	return gameOver;
}

// hack!
void clearScreen()
{
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
}

int main() 
{
	// hey! its a map
	Map world;

	// player needs to know about the map
	player = Player(world);

	// not over yet!
	bool gameOver = false;
	
	while(!gameOver)
	{
		int n=1;
		int& counter=n;
		// methods names should be informative enough
		clearScreen();
		player.displayMap();
		player.displayStats();
		displayTime();
		// get the action after showing a list of options
		char action = player.requestAction();
		cout << "\n";
		// find out the effects of that action
		player.doAction(action, dayTime);
		// process what happened and end the game if we lose
		gameOver = updatePlayer();
		if (player.getEnergy()==0)
		{
			counter=0;
			double smart, ent;
			smart=player.getSmartness();
			ent=player.getEntertainment();
			player.removeSmartness(100);
			player.removeEntertainment(100);
			player.addSmartness(smart*0.6);
			player.addEntertainment(ent*0.6);
			player.addEnergy(80);
			cout<<endl<<"You pass out and awake in your dorm..."<<endl;
			cout<<"But you wake up with a bump on your head."<<endl;
			cout << "(Press enter to advance an hour)";
			string temp;
			getline(cin, temp);
			player.travel(counter);
			counter=1;
		}
		else
		{
			// wait so the person can
			cout << "(Press enter to advance an hour)";
			string temp;
			getline(cin, temp);
		}
	}

	// ruh roh
	cout << "Game Over!\n";
	return 0;
}
