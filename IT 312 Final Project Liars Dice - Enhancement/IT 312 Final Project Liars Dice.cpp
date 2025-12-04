//******************************************
//* Tyler Lacroix
//* IT 312, Software Devel w/C++.Net 2024 &
//* CS 499, Computer Sci. Capstone 2025
//* November 21, 2025
//******************************************

//********************************************************************************************************************
//* Changelog:
//* - Updated and condensed language used for comments to be more accurate
//* - Updated rules.txt to be more brief and take up less real estate
//* - Added brief credits to rules.txt
//* - Update math to use less hardcoded values to allow for more modular game design
//* - Shrank dice display so each dice takes up less horizontal space to allow for more dice to be displayed at once
//* - Added support for default 5 dice games, as well as shorter 3 dice and longer 10 dice games
//********************************************************************************************************************


#include <iostream>
#include <string>
#include <time.h>
#include <fstream>

using namespace std;

// Class to define guesses to make management easier
class guesses
{
private:
    int maxDiceFaces = 0;  // I faced an error where this value was not being updated. Solved that by passing class by reference instead of value.

public:
    void setDice(int diceChoice)
    {
        maxDiceFaces = diceChoice;
        return;
    }

    int getDice()
    {
        return maxDiceFaces;
    }
};

// Container for guesses
struct guessTracker
{
    int diceChoice, guessTracker;
};

// Class to store individual player data
class player
{
public:
    int dice[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // array to store player dice.
    guessTracker guess; // guesses
    bool hasGuessed = false; // Used to ensure player has guessed before being called out.

};

// Init functions
void playerInit(player[], int);
int countDice(int, int, player[], int);
void callLiar(player[], int, int);
void displayMenu(player[], int, int, guesses, int);
void readRules();
void printDice(int, int[]);
int numInput();

guessTracker makeGuess(guesses&);

void playerInit(player player[], int numplayers) // Initialize each player's dice set
{
    for (int i = 1; i <= numplayers; i++) // Loop to iterate through players
    {

        for (int b = 0; b < size(player[i].dice); b++) // Loop to assign dice values
        {
            player[i].dice[b] = rand() % 6 + 1;
        }
    }
};

guessTracker makeGuess(guesses& guess) // Provide functionality to allow player to guess number of dice in play.
{
    // Declare variables, one for how many dice, and the second for the value of the dice, 
    // and a boolean that assists with making valid guesses.
    int numChoice, diceChoice;
    bool validGuess = false;

    // Loop back to asking for a guess if the player guesses the same or less than prior guesses.
    while (!validGuess)
    {
        cout << "The previous guess was " << guess.getDice() << " dice. You must guess more than that." << endl;
        cout << "How many dice are you guessing?" << endl;
        numChoice = numInput();

        // Ensure player guessed more than previous.
        if (numChoice > guess.getDice())
        {
            guess.setDice(numChoice);
            validGuess = true;
        }
        else
        {
            cout << "New guess cannot be lower than previous guess." << endl;
        }
    }

    cout << "What value are you guessing?" << endl;
    cin >> diceChoice;

    cout << "You guess " << numChoice << " of " << diceChoice << " are in play." << endl;

    guessTracker result = { numChoice, diceChoice }; // Assign to guess struct

    // Return struct value
    return result;

};

int countDice(int inputDiceValue, int numPlayers, player player[], int gameSize) // Function to count dice faces
{
    int count = 0;

    for (int i = 0; i <= numPlayers; i++) // loop to iterate through all players.
    {

        for (int b = 0; b < gameSize; b++) // loop to iterate through a players dice.
        {
            if (player[i].dice[b] == inputDiceValue)
            {
                count++;
                //cout << player[i].dice[b] << " ";
            }
        }

    }

    // Returns the calculated value of dice
    return count;
};

void readRules() // Reads gamerules from a file and prints it to the console
{
    // Open file and pass it to infile obj
    ifstream infile("rules.txt");

    if (infile.fail())
    {
        cerr << "Error opening file" << endl;
        exit(1); // Exit if file fails to open
    }

    string line;

    // Read file and print to console until eof
    while (!infile.eof())
    {
        getline(infile, line);
        cout << line << endl;
    }

    infile.close();
}

void callLiar(player players[], int numplayers, int gameSize) // Counts total dice in play, and determines the winner/loser
{
    int targetplayer;

    cout << "Which player would you like to call a liar?" << endl;
    cin >> targetplayer;

    // check to ensure guesses have been made
    if (players[targetplayer].hasGuessed == true)
    {

        // Display the previous guesses
        cout << "player " << targetplayer << " has made the following guesses: " << endl;
        cout << players[targetplayer].guess.diceChoice << " of " << players[targetplayer].guess.guessTracker << endl;

        // Display the total number of each dice in play amongst all players.
        cout << "There are a total of " << (numplayers * gameSize) << " dice in play." << endl << "Among them there are:" << endl;
        cout << "- " << countDice(1, numplayers, players, gameSize) << " 1s in play," << endl
             << "- " << countDice(2, numplayers, players, gameSize) << " 2s in play," << endl
             << "- " << countDice(3, numplayers, players, gameSize) << " 3s in play," << endl
             << "- " << countDice(4, numplayers, players, gameSize) << " 4s in play," << endl
             << "- " << countDice(5, numplayers, players, gameSize) << " 5s in play, and" << endl
             << "- " << countDice(6, numplayers, players, gameSize) << " 6s in play." << endl << endl;

        //return false;
        exit(0); // end program once a liar has been successfully called

    }
    else
    {
        // Display error and return to menu.
        cout << "This player has not made a guess and cannot be called out." << endl;
        //return true;
    }


};

// There's likely an easier way to handle this through recursion, however I for the life of me can't figure it out
void printDice(int numDice, int diceValues[]) // Print ascii art of dice to console
{
    //print the top layer of the dice in a row
    for (int i = 0; i < numDice; i++)
    {
        cout << "*******    ";
    }
    cout << endl;

    //print middle layer of dice
    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < numDice; y++) //prints inner contents of dice one row at a time
        {
            if (diceValues[y] == 1)
            {
                if (x == 0 || x == 2)
                {
                    cout << "*     *    ";
                }
                else
                {
                    cout << "*  1  *    ";
                }
            }
            else if (diceValues[y] == 2)
            {
                if (x == 0 || x == 2)
                {
                    cout << "*  2  *    ";
                }
                else
                {
                    cout << "*     *    ";
                }
            }
            else if (diceValues[y] == 3)
            {
                if (x == 0)
                {
                    cout << "*    3*    ";
                }
                else if (x == 1)
                {
                    cout << "*  3  *    ";
                }
                else
                {
                    cout << "*3    *    ";
                }
            }
            else if (diceValues[y] == 4)
            {
                if (x == 0 || x == 2)
                {
                    cout << "*4   4*    ";
                }
                else
                {
                    cout << "*     *    ";
                }
            }
            else if (diceValues[y] == 5)
            {
                if (x == 0 || x == 2)
                {
                    cout << "*5   5*    ";
                }
                else
                {
                    cout << "*  5  *    ";
                }
            }
            else if (diceValues[y] == 6)
            {
                cout << "*6   6*    ";
            }
            else
            {
                cout << "*     *    ";
            }

        }

        cout << endl; //breaks line for next row


    }

    //print the bottom layer of the dice in a row
    for (int i = 0; i < numDice; i++)
    {
        cout << "*******    ";
    }
    cout << endl << endl;

}

void displayMenu(player players[], int curPlayer, int numPlayer, guesses guess, int gameSize) // Main menu and primary game logic.
{
    int input;
    bool continueLoop = true;
    cout << "-------------------------------------------" << endl;
    cout << "                Liar's Dice                " << endl;
    cout << "-------------------------------------------" << endl;
    while (continueLoop) // loop until logic loop breaks
    {
        // Main menu display
        cout << "Player " << curPlayer << "'s turn"<< endl << endl;
        cout << "1: Display my dice." << endl;
        cout << "2: Make a guess." << endl;
        cout << "3: Call out a liar." << endl;
        cout << "4: Quit game" << endl;
        input = numInput();

        switch (input) // switch/case block less bulky than if/else block (and I think faster?)
        {
            case 1:
                printDice(gameSize, players[curPlayer].dice);
                cout << endl;

                // Return to menu
                break;

            case 2:
                // Call makeGuess function and mark the player as having made a guess.
                players[curPlayer].guess = makeGuess(guess);

                players[curPlayer].hasGuessed = true;

                // Iterate to the next player, or back to the first player if last player is making the guess.
                if (curPlayer < numPlayer)
                {
                    curPlayer++;
                }
                else
                {
                    curPlayer = 1;
                }

                // Flushes console to 'hide' the other player's dice and to (hopefully) improve legibility
                // cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;

                // Return to menu
                break;

            case 3:
                // Call out a player's guess, display how accurate they are, and end the game.
                callLiar(players, numPlayer, gameSize);

                break;

            case 4:

                continueLoop = false; // Exit game loop
                break;

            default:

                cout << "Invalid choice." << endl;

                // Return to menu
                break;
        }

    }
};

//input processing to make sure the program doesn't die
int numInput()
{
    int input;

    do
    {
        cin >> input;

        if (cin.fail()) { //verifies integer input.

            cout << "Invalid input! Expected an integer." << endl;

            // Clear the failbit and ignore the remaining input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else
        {
            break;
        }
    } while (true);
    return input;
}

int main()
{

    srand(time(NULL)); // Seed the random number generator

    int numplayers = 0, curPlayer = 1; // Initialize numPlayers to 0 for use in input validation loop

    guesses guess; // Create guess object from guesses class.

    int gameSize = 5; // sets default game size of 5

    readRules();

    while (numplayers < 2)
    {
        cout << "Please enter the number of players: " << endl;
        numplayers = numInput();
        cout << endl;

        if (numplayers < 2)
        {
            cout << "You must have a minimum of 2 players." << endl;
        }
    }

    while (true) // awful practice, I know...
    {
        cout << "Would you like to play with 3, 5, or 10 dice?" << endl << "(3 and 10 dice games are not standard, and gameplay may be awkward.)" << endl;
        gameSize = numInput();
        cout << endl;

        if (!((gameSize == 3 || gameSize == 5) || gameSize == 10))
        {
            cout << "Game size must be 3, 5, or 10." << endl;
        }
        else if (gameSize == 5) // no clue why but this has to be here or else it doesn't work
        {
            break;
        }
        else
        {
            break; // exit loop if game size is properly assigned
        }
    }

    // dynamically allocate memory to create array for player objects  
    player* playersList = new player[numplayers + 1]; // (the +1 is there to stop the compiled program from corrupting on exit)

    playerInit(playersList, numplayers); // Roll dice and assign them to players.

    displayMenu(playersList, curPlayer, numplayers, guess, gameSize); // Initiate main gameplay loop.

    return 0;
}