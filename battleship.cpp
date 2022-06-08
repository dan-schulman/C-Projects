#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iomanip>

using namespace std;

const int SIZE = 10; // size of the board
const char BLANK = '~'; // DO NOT SET THIS TO A SHIP CHARACTER
const char UNKNOWN = '?'; // DO NOT SET THIS TO A SHIP CHARACTER
const char HIT = 'X'; // DO NOT SET THIS TO A SHIP CHARACTER
const char MISS = 'O'; // DO NOT SET THIS TO A SHIP CHARACTER

class Submarine {
private:
	string name;
    int length;
    // positions are the top left point of the piece
    int rowPosition;
    int columnPosition;
    bool amIHorizontal;
    char boardCharacter;
public:
    Submarine()
    {
        name = "Battleship";
        length = 4;
        boardCharacter = 'B';
    }
    
    Submarine(string n, char c, int l)
    {
		name=n;
		boardCharacter = c;
		length=l;
	}

	// fills in the rowPosition, columnPosition and amIHorozontal variables based on the input
	// (rowPosition and columnPositing are the top and left coordinate respectively)
    void setPiece(int row, int column, char direction)
    {
        if(direction == 'u')
        {
            rowPosition = row-(length-1);
            columnPosition = column;
            amIHorizontal = false;
        }
        else if(direction == 'd')
        {
            rowPosition = row;
            columnPosition = column;
            amIHorizontal = false;
        }
        else if(direction == 'l')
        {
            rowPosition = row;
            columnPosition = column - (length-1);
            amIHorizontal = true;
        }
        else if(direction == 'r')
        {
            rowPosition = row;
            columnPosition = column;
            amIHorizontal = true;
        }
        else
        {
            rowPosition = -1;
            columnPosition = -1;
        }
    }

    bool isHorizontal() const
    {
        return amIHorizontal;
    }

    int getTopLeftRow() const
    {
        return rowPosition;
    }

    int getTopLeftColumn() const
    {
        return columnPosition;
    }

    int getLength() const
    {
        return length;
    }

    string getName() const
    {
        return name;
    }

    char getBoardCharacter() const
    {
        return boardCharacter;
    }
};

class BattleshipGame {
public:
	// THE MIGHTY CONSTRUCTOR
	// meekly makes both boards blank and initialize pieces
	BattleshipGame()
    {
        for(int i =0; i < SIZE; i++)
        {
            for(int j=0; j < SIZE; j++)
            {
                playerBoard[i][j] = BLANK;
            }
        }

        for(int i =0; i < SIZE; i++)
        {
            for(int j=0; j < SIZE; j++)
            {
                computerBoard[i][j] = UNKNOWN;
            }
        }

        playerSub = Submarine();
        computerSub = Submarine();
    }
    
    // query user where they wish to place their pieces
    void placePlayerPieces()
    {
        bool correctPlacement;
        do{
            displayBoards();
            correctPlacement = placePiece(playerSub);
            if(!correctPlacement)
            {
                cout << "\nPlease enter a valid position and direction\n";
            }
        }while(!correctPlacement);

        addPieceToBoard(playerBoard, playerSub);
    }

	// places the computers pieces
	// if input string is "", then it places them randomly in a valid position
	// if the input string is not blank, it will read from that file
    void placeComputerPieces(string s)
    {
		ifstream in;
		if(s != "")
		{
			in.open(s.c_str());
			if(in.fail())
			{
				cout << "Incorrect file, aborting protocol!\n";
				exit(404);
			}
		}

        char tempBoard[SIZE][SIZE];
        for(int i=0; i<SIZE; i++)
        {
            for(int j=0; j<SIZE; j++)
            {
                tempBoard[i][j] = BLANK;
            }
        }

        bool correctPlacement;
        do{
			int randomColumn, randomRow;
			string randomDirection;
			if(s == "") // random generate
			{
				randomColumn = rand()%SIZE;
				randomRow = rand()%SIZE;

				int randomDirectionInt = rand()%4;
				randomDirection = "";
				switch(randomDirectionInt)
				{
					case 0:
						randomDirection = "up";
						break;
					case 1:
						randomDirection = "down";
						break;
					case 2:
						randomDirection = "left";
						break;
					case 3:
						randomDirection = "right";
						break;
					default:
						randomDirection = "";
						break;
				}
			}
			else
			{
				if(in.eof())
				{
					cout << "Error, not enough valid ships in file!\n";
					exit(1);
				}
				
				char type;
				string name;
				int length;
				in >> type >> randomRow >> randomColumn >> randomDirection;
				if (type=='A')
				{
					name="Aircraft Carrier";
					length=5;
				}
				if (type=='B')
				{
					name="Battleship";
					length=4;
				}
				if (type=='S')
				{
					name="Submarine";
					length=3;
				}
				if (type=='C')
				{
					name="Cruiser";
					length=3;
				}
				if (type=='D')
				{
					name="Destroyer";
					length=2;
				}
				
				computerSub=Submarine(name, type, length);
			}

            computerSub.setPiece(randomRow, randomColumn, getFirstChar(randomDirection));
            correctPlacement = isValidPlacement(tempBoard, computerSub);
        }while(!correctPlacement);

        addPieceToBoard(tempBoard, computerSub);
        in.close();
    }

	// query player for where they wish to bombard
	// returned string is the coordinate with result (hit/miss/sink)
    string playerMove()
    {
        int attackRow;
        int attackColumn;
        string coordinate;

        do
        {
            cout << "Which coordinate would you like to bombard?\n";
            coordinate = toLower(requeststring());
            attackRow = rowCharToInt(getFirstChar(coordinate));
            attackColumn = getFirstInt(coordinate)-1;
        }while( (attackRow < 0 || attackRow > SIZE-1) || (attackColumn < 0 || attackColumn > SIZE-1));

        int row = computerSub.getTopLeftRow();
        int column = computerSub.getTopLeftColumn();
        for(int i = 0; i < computerSub.getLength(); i++)
        {
            if(row == attackRow && column == attackColumn)
            {
                computerBoard[attackRow][attackColumn] = HIT;
                if(isSunk(computerBoard,computerSub))
                {
					string result = computerSub.getName();
					result +=" sunk at ";
					result +=toupper(getFirstChar(coordinate));
					result +=('0'+getFirstInt(coordinate));
                    return result;
                }
                string result = "Hit at ";
				result +=toupper(getFirstChar(coordinate));
				result +=('0' + getFirstInt(coordinate));
				return result;
            }

            if(computerSub.isHorizontal())
            {
                column++;
            }
            else
            {
                row++;
            }
        }

        computerBoard[attackRow][attackColumn] = MISS;
        string result = "Miss at ";
		result +=toupper(getFirstChar(coordinate));
		result +=('0' + getFirstInt(coordinate));
		return result;
    }

	// computer shoots a random square it has not already
	// the returned string tells where (coordinate) and the result (hit/miss/sink)
    string computerMove()
    {
        int attackRow = rand()%SIZE;
        int attackColumn = rand()%SIZE;

        string coordinate = "";
        coordinate+=(attackRow+'A');
        coordinate+=(attackColumn+'1');

        int row = playerSub.getTopLeftRow();
        int column = playerSub.getTopLeftColumn();
        for(int i = 0; i < playerSub.getLength(); i++)
        {
            if(row == attackRow && column == attackColumn)
            {
                playerBoard[attackRow][attackColumn] = HIT;
                if(isSunk(playerBoard,playerSub))
                {
					string result = playerSub.getName();
					result += " sunk at ";
					result += coordinate;
                    return result;
                }
                string result = "Hit at ";
                result += coordinate;
                return result;
            }

            if(playerSub.isHorizontal())
            {
                column++;
            }
            else
            {
                row++;
            }
        }

        playerBoard[attackRow][attackColumn] = MISS;
        
        string result="Miss at ";
        result += coordinate;
        return result;
    }

	// is the game over yet?
	// 1 = computer wins
	// 2 = you win
	// 3 = you tie (rare!)
    int gameOver()
    {
        int result = 0;

        int computerShipCoordinateTotal = 0;
        computerShipCoordinateTotal += computerSub.getLength();

        int playerShipCoordinateTotal = 0;
        playerShipCoordinateTotal += playerSub.getLength();


        int playerBoardHitTotal = 0;
        for(int i=0; i<SIZE; i++)
        {
            for(int j=0; j<SIZE; j++)
            {
                if(playerBoard[i][j] == HIT)
                {
                    playerBoardHitTotal++;
                }
            }
        }
        if(playerBoardHitTotal == playerShipCoordinateTotal)
        {
            result+=1;
        }

        int computerBoardHitTotal = 0;
        for(int i=0; i<SIZE; i++)
        {
            for(int j=0; j<SIZE; j++)
            {
                if(computerBoard[i][j] == HIT)
                {
                    computerBoardHitTotal++;
                }
            }
        }
        if(computerBoardHitTotal == computerShipCoordinateTotal)
        {
            result+=2;
        }

        return result;
    }

	// show boards WIHTOUT movement info (i.e. when placing pieces)
    void displayBoards()
    {
        displayBoards("", "", playerBoard, computerBoard);
    }

	// show boards WITH movement info (hit/miss/sink)
    void displayBoards(string move1, string move2)
    {
        displayBoards(move1, move2, playerBoard, computerBoard);
    }

	// shows both boards
    void displayBoards(string move1, string move2, char leftBoard[SIZE][SIZE], char rightBoard[SIZE][SIZE])
    {
        // first row(
        cout << "  ";
        for(int i = 0; i < SIZE; i++)
        {
            cout << setw(2) << i+1 << " ";
        }
        cout <<"                    ";
        cout << "   ";
        for(int i = 0; i < SIZE; i++)
        {
            cout << setw(2) << i+1 << " ";
        }
        cout << "\n";

        //rest of rows
        // (int)'A' = 65
        for(int i = 0; i < SIZE; i++)
        {
            char row = 'A'+i;
            cout << row << "  ";
            for(int j = 0; j < SIZE; j++)
            {
                cout << leftBoard[i][j] << "  ";
            }
			cout <<"                    ";
            cout << row << "  ";
            for(int j = 0; j < SIZE; j++)
            {
                cout << rightBoard[i][j] << "  ";
            }
            cout << "\n";
        }

        if(move1 != "")
        {
            cout << "Your move:\n";
            cout << move1 << endl;
        }
        if(move2 != "")
        {
            cout << "Computer's move:\n";
            cout << move2 << endl;
        }
    }
    
    // character to number conversion (for indexing)
    int rowCharToInt(char c)
    {
        // (int)'a' = 97
        return c - 'a';
    }


private:
    char playerBoard[SIZE][SIZE];
    char computerBoard[SIZE][SIZE];
    Submarine playerSub;
    Submarine computerSub;

	// query the user for where they want to put the "piece" and modify the class's variables to reflect this
    bool placePiece(Submarine& piece)
    {
        cout << "What coordinate do you want to place the " << toLower(piece.getName()) << " (length "<<piece.getLength()<<") at?\n";
        string coordinate = toLower(requeststring());
        cout << "What direction do you want the piece to go? (up, down, left or right)\n";
        string direction = toLower(requeststring());

        piece.setPiece(rowCharToInt(getFirstChar(coordinate)), getFirstInt(coordinate)-1, getFirstChar(direction));

        return isValidPlacement(playerBoard, piece);
    }

	// actually write the "piece" to the "board"
    void addPieceToBoard(char board[SIZE][SIZE], const Submarine& piece)
    {
        int row = piece.getTopLeftRow();
        int column = piece.getTopLeftColumn();

        for(int i =0; i < piece.getLength(); i++)
        {
            board[row][column] = piece.getBoardCharacter();

            if(piece.isHorizontal())
            {
                column++;
            }
            else
            {
                row++;
            }
        }
    }

	// checks to see if a certain "piece" is sunk or not (i.e. true if all spots of this "piece" are hit)
    bool isSunk(char board[SIZE][SIZE], const Submarine& piece)
    {
        int row = piece.getTopLeftRow();
        int column = piece.getTopLeftColumn();
        for(int i=0; i<piece.getLength(); i++)
        {
            if(board[row][column] != HIT)
            {
                return false;
            }

            if(piece.isHorizontal())
            {
                column++;
            }
            else
            {
                row++;
            }
        }

        return true;
    }

	// checks to see whether the input "piece" is a valid board position
    bool isValidPlacement(char board[SIZE][SIZE], const Submarine& piece)
    {
        int row = piece.getTopLeftRow();
        int column = piece.getTopLeftColumn();

        // check and see if the piece is actually on the board
        if(column<0 || (column+piece.getLength()-1>SIZE-1 && piece.isHorizontal()))
        {
            return false;
        }
        if(row<0 || (row+piece.getLength()-1>SIZE-1 && !piece.isHorizontal()))
        {
            return false;
        }

        // check to see if another ship is present
        for(int i =0; i < piece.getLength(); i++)
        {
            if(board[row][column] != BLANK)
            {
                return false;
            }

            if(piece.isHorizontal())
            {
                column++;
            }
            else
            {
                row++;
            }
        }

        return true;
    }
    
    // lazy... don't ask
	string requeststring()
	{
		string temp;
		getline(cin, temp);

		return temp;
	}
	
	// converts a string to all lower case letters
	string toLower(string s)
	{
		for(int i=0; i < static_cast<int>(s.length()); i++)
		{
			s[i] = tolower(s[i]);
		}
		
		return s;
	}

	// returns the first character in the input string
	char getFirstChar(string temp)
	{
		int i=0;
		char c;
		if(temp.length()==0)
		{
			c = 0;
		}
		else
		{
			while(i < static_cast<int>(temp.length()) && !isalpha(temp[i]))
			{
				i++;
			}
			if(i < static_cast<int>(temp.length()))
			{
				c = temp[i];
			}
			else
			{
				c = 0;
			}
		}

		return c;
	}

	// returns the first integer value in the input string
	int getFirstInt(string temp)
	{        
		int i =0;
		if(temp.length()==0)
		{
			return 0;
		}

		while(i < static_cast<int>(temp.length()) && !isdigit(temp[i]))
		{
			i++;
		}

		string number = temp.substr(i);

		if(number == "")
		{
			return 0;
		}

		return atoi(number.c_str());
	}
};


int main()
{
	srand(time(0));
	BattleshipGame game;
	cout << "Welcome to Battleship v23.1\n";

	string fromFile;
	cout << "Do you wish the computer to generate a random board or from file? (r or f)\n";
	getline(cin, fromFile);

	// computer board generation
	if('f' == tolower(fromFile[0]))
	{
		cout << "What file do you wish to read?\n";
		getline(cin,fromFile);
		game.placeComputerPieces(fromFile);
	}
	else
	{
		game.placeComputerPieces("");
	}
	
	// place your pieces
	game.placePlayerPieces();

	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	game.displayBoards();
	do{
		string playerMove = game.playerMove();
		string computerMove = game.computerMove();
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
		game.displayBoards(playerMove, computerMove);            
	}while(game.gameOver() == 0);

	int gameOver = game.gameOver();
	if(gameOver == 1)
	{
		cout << "You lost!  Practice more.\n";
	}
	else if(gameOver == 2)
	{
		cout << "You won!  Have a cookie.\n";
	}
	else if(gameOver == 3)
	{
		cout << "Oh my gosh, a tie!\n";
	}
}
