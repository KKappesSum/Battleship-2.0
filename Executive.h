#ifndef EXECUTIVE_H
#define EXECUTIVE_H
#include <string>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <stdio.h> 
#include <stdlib.h> 
#include "Admiral.h"
#include "PowerUps.h"
#include "EasyAI.h"
#include "MedAI.h"
#include "HardAI.h"
#include <chrono>
#include <thread>
using namespace std;

class Executive
{
    private:
        const int m_BOARD_SIZE = 8; /** The size of the board to be created.*/
        Admiral* m_player1; //Admiral object for player 1
        Admiral* m_player2; //Admiral object for player 2
        int m_numShips; //int of the number of ships used in the game
        int m_turn; /* 1 for player 1, 2 for player 2 */
        std::chrono::duration<unsigned long long> m_interval = std::chrono::seconds(3); //their timeout thing
        PowerUps m_powerups;    /* Contains both players' powerups and methods to use them. */
        char quit_choice;

        bool m_ai; /*bool value to check if AI or not to relaunch PvP or PvAI */
        
    public:
    /** A constructor that takes no parameters.
     * @post initializes the player turn to 1.
     */
    Executive();

    /** 
     * Destructor that deletes the 2 player Admiral objects
     * @param none
     */
    ~Executive();

    /**
     * Asks for a coordinate from the user in the form 1A. Checks the input to make sure it is 
     * valid on the board size 8x8 and in the correct syntax. 
     * @param none
     * @return std::string of the coordinate in the form 1A
     */
    std::string askCoord();

    /**
     * Places ships for a user. Asking the user the coordinate from the beginning, then displays the possible 
     * ending coordinates for the user. Then asks the user to input one of the ending coordinate from list. 
     * @param size is an int for the length of the ship being placed
     * @param player is an Admiral object for the player placing ship
     * @param AI true if player is AI (don't interact with user)
     * @return none
     * @post sends a string array of the coordinates of the ship to Admiral
     */
    void placeShip(int n, Admiral* player, bool ai);

    /**
     * Delete saved.txt if it already exists. Then, write the AI and turn info and call writeBoard()
     * @param turn is an int for the player's turn (1/2)
     * @param player1 is an Admiral object for player 1
     * @param player2 is an Admiral object for player 2
     * @param AI true if player is AI
     * @return none
     * @post writes bool ai and int turn into a file called player_info.txt
     */
    void saveGame(int turn, Admiral* player1, Admiral* player2, bool ai);

    /**
     * write grid info into a text file
     * @param player1_board 2D string array for player 1
     * @param player2_board 2D string array for player 2
     * @return none
     * @post writes grid info for both players into a file called saved.txt
     */
    void writeBoard(string** player1_board, string** player2_board);

    /**
     * set the board, turn and indicate if AI or not
     * @param turn indicates whose turn is next
     * @param player1_board 2D string array for player 1
     * @param player2_board 2D string array for player 2
     * @param ai if pvp or ai game
     * @return none
     * @post relaunch the game
     */
    void loadGame(int turn, string** player1_board, string** player2_board, bool ai);

    /**
     * read the text file to attain ship information and grid information
     * @post stores all grid info for both players from a file called saved.txt into buffer variables
     */
    void readBoard();

    /**
     * Runs the Battleship game
     * @param none
     * @return none
     */
    void run();

    /**
     * Runs the menu for the game
     * @param none
     * @return int of the user's selection on the menu: 1 for instructions, 2 to play a PvP game, 3 to play vs. AI, 4 to quit.
     */
    int mainMenu();

    /** Changes the player turn.
     * @post if the turn is 1, changes it to 2, and vice versa.
     */
    void switchTurn();

    /**
     * Translates a letter coordinate to a number coordinate
     * Only works for letters A-H
     * @param char c is the character of the letter you are changing to an int
     * @return int of the number equivalent
     */
    int charCoordtoIntCoord(char c);

    /**
     * Evaluates a coordinate and returns if it is valid or not
     * @param pos is a string of the coordinate in the form 1A
     * @return true if the position is valid on the 8x8 board
     */
    bool validPos(std::string pos);

    /**
     * Asks and sets the number of ships used for the game
     * @param none
     * @return none
     * @post sets the m_numShips to the number obtained from user
     */
    void setNumShips();

    /** Handles the given player or AI's turn: asks for firing targets & powerup usage, updates Grid & Ship data.
     * @param player 1 for player 1, 2 for player 2.
     * @param AI true if an AI is playing, else false.
     * @post updates the other player's game data as necessary, and prints messages to the user.
     * @return true if the game is over, else false.
     */
    bool handleTurn(const int player, const bool AI);

    /** Runs the gameplay portion of the program: switches turns and calls handleTurn for each player. 
     * @param AI true if there is an AI player, else false.
     * @return a number representing which player has won: 1 for player 1, 2 for player 2, 3 for an AI.
     */
    int gameplay(const bool AI);

    /** Asks the player whether they want to use a powerup.
     * @pre assumes this method is called only when the player has 1 or more powerups.
     * @param player 1 for player 1, 2 for player 2.
     * @return an initial representing the powerup chosen, or N if the player chooses not to use one.
     */
    string askForPowerUp(const int player);

    /** Gets the firing coordinate from the user.
     * @param playerNum the current player turn, 1 for player 1, 2 for player 2.
     * @return a validated coordinate input by the player.
     */
    string askForFireCoord(const int player);

    /** Gets the given player's current powerups.
     * @param player 1 for player 1, 2 for player 2.
     * @return a pointer to the array of the player's powerups.
     */
    vector<string>* getPowerups(const int player) const;

    /** Adds the given powerup to the current player's powerup vector. [NOT USED YET]
     * @param powerup a string representing the powerup.
     */
    void updatePowerups(const string powerup);

    /** Prints the result of the turn: hit, miss, powerup, etc.
     * @param result represents whether the turn result was a hit, miss, sink, or powerup.
     * @post Prints messages for the current player based on the given result
     */
    void printTurnResult(const string result) const;

    /** Helper function: converts coords from the form "1A" to the form "<row>:<col>".
     * @param orig the original coordinate.
     * @return the coordinate in "<row>:<col>" form.
     */
    string convertCoord(string orig);

    /**
     * @param player 1 for player 1, 2 for player 2.
     * @post prints the game over message.
     */
    void printGameOver(const int player) const;

    /** Prints the current player's firing map and ship map.
     * @param 1 for player 1, 2 for player 2.
     * @post prints the current player's firing map and ship map.
     */
    void printMaps(const int player) const;

    /** Prints a message if the other player has hit or sunk the current player's Ships.
     * @param
     */
    void printEnemyAction() const;

    /** Sets up the game.
     * @pre the player has chosen either PvP or PvAI gameplay.
     * @param AI true if the game includes an AI player, else false.
     * @post Gets number of Ships from the player, constructs Admiral or AI objects, and lets players place their Ships.
     */
    void setupGame(bool AI);

    /**
     * Places powerups on the board
     * Places them randomly, giving each user 1 of each
     * @param ai bool true if the p2 is an AI (gives them no powerups)
     */
    void placePowerUp(bool ai);

    /**
     * Lets user use a powerup
     * @param powerup is a string of the letter for the power (TRUS)
     * @param isPlayer1 is a bool true for player 1, false for player2
     */
    void usePowerUp(string powerup, bool isPlayer1);

    /**
     * Checks if the player received a powerup and adds it to the vector if so
     * @param turnResult is the value of the grid at the a coordinate point
     * @param isPlayer1 is a bool true for player 1, false for player2
     */
    void receivedPowerUp(string turnResult, bool isPlayer1, string coord);

    /** Clears the console by printing newlines.
     * @post prints 100 newlines to the console.
     */
    void clearConsole() const;

    /**
     * Place ships for AI
     * @param n = int of num ships
     */
    void placeAIShip(int n);

    /** Generates coordinates for a Ship.
     * @pre the Ship's start and end coordinates have already been validated.
     * @param direction the direction of the Ship's endpoint relative to its starting coordinate: 1 for north, 2 for east, 3 for south, 4 for west.
     * @param startRow the starting row coordinate.
     * @param startCol the starting column coordinate.
     * @param size the size of the Ship.
     * @return a pointer to an array of string coordinates that the Ship will occupy.
     */
    string* generateCoordsArr(int direction, int startRow, int startCol, int size);
};
#endif