#include "Executive.h"
#include <fstream>

Executive::Executive()
{
  m_turn = 1;
  m_player1 = nullptr;
  m_player2 = nullptr;
  
}

Executive::~Executive()
{
    delete m_player1;
    delete m_player2;
}

void Executive::saveGame(int turn, Admiral* player1, Admiral* player2, bool ai)
{
  //delete the files if they already exist. after that, save the latest save game info 
  fstream savedExists("saved.txt");
  fstream playerExists("player_info.txt");
  if(savedExists) 
  {
    remove("saved.txt");
  }

  if(playerExists) 
  {
    remove("player_info.txt");
  }

  //write AI and player turn info into player_info
  ofstream fileInfo;
  fileInfo.open("player_info.txt", ios::app); 

  fileInfo<<turn;
  fileInfo<<"\n";
  fileInfo<<ai;
  fileInfo<<"\n";

  string** board1 = nullptr; /*2D string array to get grid of player 1*/
  string** board2 = nullptr; /*2D string array to get grid of player 2*/

  //pass in 2D string array into the getPartialGrid to load the gird to board1/2 according to what the current player has
  player1->getBoard()->getPartialGrid(board1);
  player2->getBoard()->getPartialGrid(board2);

  writeBoard(board1, board2);

  for(int i = 0; i < m_BOARD_SIZE; i++){
    delete[] board1[i];
    delete[] board2[i];
  }
  delete[] board1;
  delete[] board2;
  board1 = nullptr;
  board2 = nullptr;
  
  fileInfo.close();
}

void Executive::writeBoard(string** player1_board, string** player2_board){
  ofstream gameFile;
  gameFile.open("saved.txt", ios::app); 

  string** board; /*2D string to write grid to file*/
  vector<Ship*>* fleet=nullptr; /*to access player's fleet*/
  string* coordsPtr=nullptr; /* to store 1D sting array coords from getCoords of fleet*/
  int sizeShips=0; /* to store size of ship to iterate and store all coordinates*/

  // //write board information to a text file, for 2 players

  // //contents of saved_txt are numbered below
  // //1. m_numShips
  gameFile<<m_numShips;
  gameFile<<"\n\n";

  for(int player = 1; player <= 2; player++){
      //if first player
      if(player==1)
      {
          board=player1_board;
          fleet=m_player1->getFleet();
      }
      //if second player
      if(player==2)
      {
          board=player2_board;
          fleet=m_player2->getFleet();
      }
      
      for(int j = 0; j<fleet->size(); j++)
      {
        coordsPtr=fleet->at(j)->getCoords();
        sizeShips=fleet->at(j)->getSize();

        //2. ship size: 
        gameFile<<sizeShips;
        gameFile<<"\n";

        for(int n=0; n<sizeShips; n++)
        {
          //3. coordsPtr[n]: coordinates of ship for example (3:8 4:8 for a 1x2 ship)
          gameFile<<coordsPtr[n];
          gameFile<<"\t";
        }

        //4. ship's status: true if the Ship is still afloat, else false
        gameFile<<"\n";
        gameFile<<fleet->at(j)->getStatus();

        //5. numHits: the number of hits the Ship has taken
        gameFile<<"\n";
        gameFile<<fleet->at(j)->getNumHits();
        gameFile<<"\n\n";

      } 

      //6. Print grid
      for(int i = 0; i < m_BOARD_SIZE; i++)
      {
          for(int j = 0; j < m_BOARD_SIZE; j++)
          {
              char temp = '\0';
              if(board[i][j].length() > 1)
              {
                  temp = board[i][j].at(1);
              }else
              {
                  temp = board[i][j].at(0);
              }
              if(((int)temp >= 48 && (int)temp <= 57) || temp == 'O' || temp == 'X')
              {
                  gameFile<< temp;
              }
              else
              {
                  gameFile<< "~";
              }
              gameFile<<"\t";
          }
          gameFile<<"\n";
      }
      gameFile<<"\n";
    }

    gameFile<<"\n";

    gameFile.close();
}

void Executive::readBoard()
{
  int turn=100; /* current player 1/2 */
  int ai=100; /* ai or pvp */
  int shipSize=0; /* current ship size */
  bool status=false;
  int hits=0;
  string* coordsPtr=nullptr;
  int sizeShips=0;
  vector<Ship*>* fleet=nullptr;
  string** board=nullptr; /*2D string to read grid from file*/
  
  /*---read in player info: current player turn and bool AI--*/
  /*---------------------------------------------------------*/ 
  ifstream playerInfo;
  playerInfo.open("player_info.txt"); 

  playerInfo>>turn;
  playerInfo>>ai;

  playerInfo.close();


  /*---------------------read in grid info:---------------------*/ 
  /*---numShips, shipSize, coordsPtr[n], status, numHits,grid---*/ 
  /*------------------------------------------------------------*/ 
  ifstream grid;
  grid.open("saved.txt");   

  //initial 2D with size rows
  board=new string*[m_BOARD_SIZE];
  string** player1_board=new string*[m_BOARD_SIZE];
  string** player2_board=new string*[m_BOARD_SIZE];
  for(int i = 0; i < m_BOARD_SIZE; i++)
  {
    //initial each row with 8 elements
    board[i] = new string[m_BOARD_SIZE];
    player1_board[i] = new string[m_BOARD_SIZE];
    player2_board[i] = new string[m_BOARD_SIZE];
  }  

  //1. read numShips
  grid>>m_numShips;

  //create two Admiral objects using given numShips
  m_player1= new Admiral(m_numShips);
  m_player2= new Admiral(m_numShips);

  for(int player = 1; player <= 2; player++)  //for 2 players
  {
    for(int n=0; n<m_numShips; n++) //for each ship
    {
      //2. shipSize
      grid>>shipSize;

      coordsPtr=new string[shipSize];

      for(int j = 0; j<shipSize; j++)
      {
        //3. coordsPtr[j]: coordinates of ship for example (3:8 4:8 for a 1x2 ship)
        grid>>coordsPtr[j];
      }

      //4. ship's status: true if the Ship is still afloat, else false
      grid>>status;

      //5. numHits: the number of hits the Ship has taken
      grid>>hits;
        
      //if player 1
      if(player==1)
      {
        //load a ship using constructor call
        m_player1->loadShip(shipSize, coordsPtr, status, hits);
      }
      
      //if player 2
      if(player==2)
      {
        //load a ship using constructor call
        m_player2->loadShip(shipSize, coordsPtr, status, hits);
      }

      delete[] coordsPtr;
    }
    //6. 2D string array grid
    for(int i = 0; i < m_BOARD_SIZE; i++)
    {
        for(int j = 0; j < m_BOARD_SIZE; j++)
        {
          grid>>board[i][j];
        }
    }

    //if player 1
      if(player==1)
      {
        //assign board
        for(int i = 0; i < m_BOARD_SIZE; i++)
        {
            for(int j = 0; j < m_BOARD_SIZE; j++)
            {
              player1_board[i][j]=board[i][j];
            }
        }
      }
      
      //if player 2
      if(player==2)
      {
        //assign board
        for(int i = 0; i < m_BOARD_SIZE; i++)
        {
            for(int j = 0; j < m_BOARD_SIZE; j++)
            {
              player2_board[i][j]=board[i][j];
            }
        }
      }
    
    
  } 

  grid.close();

  loadGame(turn, player1_board, player2_board, ai);

  for(int i = 0; i < m_BOARD_SIZE; i++)
  {
    //initial each row with 8 elements
    delete[] board[i];
  } 

  delete[] board;

  m_powerups.setAdmirals(m_player1,m_player2);
}


void Executive::loadGame(int turn, string** player1_board, string** player2_board, bool ai)
{
    //store boards
    m_player1->getBoard()->readGrid(player1_board);
    m_player2->getBoard()->readGrid(player2_board);

      for(int i = 0; i < m_BOARD_SIZE; i++)
    {
      //initial each row with 8 elements
      delete[] player1_board[i];
      delete[] player2_board[i];
    } 

    delete[] player1_board;
    delete[] player2_board;

    //set current turn
    m_turn=turn;

    //return in relaunchOption if AI or not
    m_ai=ai;
}

void Executive::placeAIShip(int n)
{
  for (int i = 1; i< n; i++)
  {
    int x = 1;
    if (i%2 ==0)
    {
      x+= rand()%4;
    }
    std::string r = std::to_string(x);
    std::string c = std::to_string(n);
    std::string* arr = new string[i];
    for (int j = 0; j< i; j++)
    {
      std::string coor = r+":"+c;
      arr[j] = coor;
    }
    m_player2->addShip(i, arr);
  }

}

void Executive::placeShip(int n, Admiral* player, bool ai)
{
  bool taken [9][9];
  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j<9; j++)
    {
        taken[i][j] = false;
    }
  }
  for (int size = 1; size<= n; size++)
  {
    std::string start = "";
    int r = 0;
    int c = 0;

    if(!ai)
    {
      player->getBoard()->printGrid(false);
      std::cout << "It's time to place your ship of size 1x" << size << ". Enter a starting coordinate\n";
      start = askCoord();
      r = std::stoi(start.substr(0,1));
      c = charCoordtoIntCoord(start.at(1));
    }
    else
    {
      int x = rand() % 8+1;
      int y = rand() % 8+ 1;
      start = std::to_string(x) + ":" + std::to_string(y);
      r = x;
      c = y;
    }
    
    while(taken[r][c] == true)
    {
      if(!ai)
      {
        std::cout << "Error you already placed a ship there. Try a different coordinate. ";
        start = askCoord();
        c = charCoordtoIntCoord(start.at(1));
        r = std::stoi(start.substr(0,1));
      }
      else
      {
        r = rand() % 8+1;
        c = rand() % 8+ 1;
      }
    }

    bool north = false;
    bool south = false;
    bool west = false;
    bool east = false;
    std::string option_n = "";
    std::string option_s = "";
    std::string option_w = "";
    std::string option_e = "";

    if (size != 1)
    {
      if(!ai)
      {
        std::cout << "Enter an ending coordinate. Possible ending coordinates for this ship are: ";
      }
      if (r+(size-1) <= 8)
      {
        bool flag = false;
        for (int j = 0; j< size; j++)
        {
          if (taken[j+r][c])
              flag = true;
        }
        if (!flag)
        {
          south = true;
          option_s = std::to_string(r+(size-1))+start.at(1);
          std::cout << r+(size-1) << start.at(1) << " ";
        }
      }
      if (r-(size-1) > 0)
      {
        bool flag = false;
        for (int j = 0; j< size; j++)
        {
          if (taken[r-j][c])
              flag = true;
        }
        if (!flag)
        {
          north = true;
          option_n = std::to_string(r-(size-1))+start.at(1);
          std::cout << r-(size-1) << start.at(1) << " ";
        }
      }
      if (c + (size-1) <= 8)
      {
        bool flag = false;
        for (int j = 0; j< size; j++)
        {
          if (taken[r][c+j])
              flag = true;
        }
        if (!flag)
        {
          east = true;
          char letter = char(64+c+(size-1));
          option_e = std::to_string(r)+letter;
          cout<< r <<  letter << " ";
        }
      }
      if (c-(size-1) > 0)
      {
        bool flag = false;
        for (int j = 0; j< size; j++)
        {
          if (taken[r][c-j])
              flag = true;
        }
        if (!flag)
        {
          west = true;
          char letter = char(64+(c-(size-1)));
          option_w = std::to_string(r)+letter;
          cout << r <<  letter << " ";
        }
      }
      std::string end = "";
      if(!ai)
      {
        std::cout << "\n";
        end = askCoord();
      }
      else
      {
        int randOpt = rand()%4;
        if (randOpt == 0)
        {
          if (east)
          {
            end = option_e;
          }
          else if (north)
          {
            end = option_n;
          }
          else if (west)
          {
            end = option_w;
          }
          else if (south)
          {
            end = option_s;
          }
        }
        if (randOpt == 1)
        {
          if(north)
          {
            end = option_n;
          }
          else if (west)
          {
            end = option_w;
          }
          else if (south)
          {
            end = option_s;
          }
          else if (east)
          {
            end = option_e;
          }
        }
        if (randOpt ==2)
        {
          if (south)
          {
            end = option_s;
          }
          else if (east)
          {
            end = option_e;
          }
          else if (north)
          {
            end = option_n;
          }
          else if (west)
          {
            end = option_w;
          }
        }
        if (randOpt == 3)
        {
          if(west)
          {
            end = option_w;
          }
          else if (south)
          {
            end = option_s;
          }
          else if (east)
          {
            end = option_e;
          }
          else if (north)
          {
            end = option_n;
          }
        }
      }
    
      while (end !=option_e && end!=option_n && end!=option_s && end!=option_w)
      {
        std::cout << "\n Error. Enter one of the coordinates listed above. ";
        end = askCoord();
      }

      if(south && (std::stoi(end.substr(0,1))== r+(size-1)))
      {
        std::string* arr = generateCoordsArr(3, r, c, size);
        int index = r;
        for(int i = 0; i<size; i++)
        {
          taken[index][charCoordtoIntCoord(start.at(1))] = true;
          index++;
        }
        player->addShip(size, arr);
        delete[] arr;
      }
      else if (north && (std::stoi(end.substr(0,1)) == r-(size-1)))
      {
        std::string* arr = generateCoordsArr(1, r, c, size);
        int index = r;
        for(int i = size-1; i>=0; i--)
        {
          taken[index][charCoordtoIntCoord(start.at(1))] = true;
          index--;
        }
        player->addShip(size, arr);
        delete[] arr;
      }
      else if (east && (end.at(1) > start.at(1)))
      {
        std::string* arr = generateCoordsArr(2, r, c, size);
        int index = c;
        for (int i = 0; i<size; i++)
        {
          taken[r][index] = true;
          index++;
        }
        player->addShip(size, arr);
        delete[] arr;
      }
      else if (west && (end.at(1) < start.at(1)))
      {
        std::string* arr = generateCoordsArr(4, r, c, size);
        int index = c;
        for (int i = size-1; i>=0; i--)
        {
          taken[r][index] = true;
          index--;
        }
        player->addShip(size, arr);
        delete[] arr;
      }
    }
    else //if size is 1x1 don't ask for ending coor
    {
      std::string* arr = generateCoordsArr(1, r, c, size);
      taken[r][c] = true;
      player->addShip(size, arr);
      delete[] arr;
    }
  }

  if(!ai)
  {
    player->getBoard()->printGrid(false);
  }
}

std::string Executive::askCoord()
{
    std::string coor = "";
    std::cout << "Please enter a coordinate in the form 1A: ";
    std::getline(std::cin, coor);
    bool valid = validPos(coor);
    while (!valid)
    {
        std::cout << "Invalid coordinate. Please enter a coordinate in the form 1A: ";
        std::cin >> coor;
        valid = validPos(coor);
    }
    return coor;
}


/// check length is 2, first coordinate, second coordinate.
bool Executive::validPos(std::string pos)
{
  if (pos.length() == 2)
  {
    if (pos[1] == 'A' || pos[1] == 'B' || pos[1] == 'C' || pos[1] == 'D' || pos[1] == 'E' || pos[1] == 'F' || pos[1] == 'G' || pos[1] == 'H')
    {
      if (pos[0] == '1' || pos[0] == '2' || pos[0] == '3' || pos[0] == '4' || pos[0] == '5' || pos[0] == '6' || pos[0] == '7' || pos[0] == '8')
      {
        return true;
      }
    }
  }
  return false;
}

void Executive::run()
{
  int winner = 0;
    int menu = mainMenu();
    if (menu == 4)
    {
      return;
    }
    if (menu == 2)
    {
      setupGame(false);
      winner = gameplay(false);
      printGameOver(winner);
    }
    if (menu == 3)
    {
      setupGame(true);
      winner = gameplay(true);
      printGameOver(winner);
    }
    if (menu == 5)
    {
      //setupGame(true);
      winner = gameplay(m_ai);
      printGameOver(winner);
    }
}

int Executive::mainMenu()
{
  bool menurun = true;
  std::string ship_num;
  std::string player_choice;
  while (menurun == true)
  {
    /// ASCII conversion done with online generator, http://patorjk.com/software/taag/.
    std::cout << "\n\n  ____        _   _   _           _     _       \n";
    std::cout << " |  _ \\      | | | | | |         | |   (_)      \n";
    std::cout << " | |_) | __ _| |_| |_| | ___  ___| |__  _ _ __  \n";
    std::cout << " |  _ < / _` | __| __| |/ _ \\/ __| '_ \\| | '_ \\ \n";
    std::cout << " | |_) | (_| | |_| |_| |  __/\\__ \\ | | | | |_) |\n";
    std::cout << " |____/ \\__,_|\\__|\\__|_|\\___||___/_| |_|_| .__/ \n";
    std::cout << "                                         | |    \n";
    std::cout << "                                         |_|    \n";
    std::cout << "\nMenu:\n 1) Instructions\n 2) Start Game: Player vs. Player\n 3) Start Game: Player vs. AI\n 4) Quit Game\n 5) Resume Saved Game\n\nEnter option (1-5): ";
    std::getline(std::cin,player_choice);
    if (player_choice == "1")
    {
      std::cout << "There are two game modes: a 1v1 game with two players, or a player vs. AI mode. In Player vs. AI mode, you can choose Easy, Medium, or Hard AI difficulty.\n";
      std::cout << "\nGoals of the game!: Sink all enemy ships\n\nHow to Play:\n - You, the player, will start by selecting how many ships you'd like to play with, 1 to 5 ships.";
      std::cout << "\n - You will walk through and place your ships and then take turns entering coordinates to attack the other players ships.\n - The game is over when all Enemy Ships have been sunk.";
      std::cout << "\n - Here is the list of symbols that will appear on the board: \n";
      std::cout << "\t~: Water \n\tO: Miss \n\tX: Hit \n\t5: 5x1 Carrier \n\t4: 4x1 Battleship \n\t3: 3x1 Destroyer \n\t2: 2x1 Submarine \n\t1: 1x1 Tug Boat\n";
    }
    else if (player_choice == "2")
    {
      return 2;
    }
    else if (player_choice == "3")
    {
      return 3;
    }
    else if (player_choice == "4")
    {
      std::cout << "\nHave a nice day!\n";
      return 4;
    }
    else if (player_choice == "5")
    {
      readBoard();
      return 5;
    }
    else
    {
      cout << "Please enter a valid input.\n";
      menurun = true;
    }
  }
}

void Executive::setNumShips()
{
  bool working = false;
  int ship_int = 0;
    std::string ship_num = "";
    while (working == false)
      {
        std::cout << "\nHow many ships would you like to play with? (1-5 ships): ";
        std::getline(std::cin,ship_num);
        if (ship_num == "1" || ship_num == "2" || ship_num == "3" || ship_num == "4" || ship_num == "5")
        {
          working = true;
          /// If input is good, cast to int for map creation and end loop for menu.
          ship_int = std::stoi(ship_num);
        }
        else
        {
          std::cout << "\nError with number of ships, please enter a valid number: 1, 2, 3, 4 or, 5\n";
        }
      }
    m_numShips = ship_int;
    m_player1->setNumShips(m_numShips);
    m_player1->setNumAfloat(m_numShips);
    m_player2->setNumShips(m_numShips);
    m_player2->setNumAfloat(m_numShips);
}

void Executive::switchTurn()
{
  if(m_turn == 1)
  {
    m_turn = 2;
  }
  else
  {
    m_turn = 1;
  }
}

bool Executive::handleTurn(const int player, const bool AI)
{
  string powerup;
  string targetCoord;
  string turnResult;
  if(AI)  // AI player
  {
    if (player ==1)
    {
      cout << "Player 1's turn!\n\n";
      printMaps(player);
      printEnemyAction(); 
      if(m_powerups.hasAPowerup(true))
      {
        powerup = askForPowerUp(1);
        usePowerUp(powerup, true);
      } 
      else //no power up
      {
        targetCoord = askForFireCoord(m_turn);
        turnResult = m_player2->incomingShot(targetCoord);
        receivedPowerUp(turnResult, true, targetCoord);
        printTurnResult(turnResult);
      }
        cout << "Next player's turn!\n";
        return(m_player2->getNumAfloat() < 1);
    }
    else
    {
        cout<<"\nAI Firing\n";
        targetCoord = m_player2->fire();
        cout<<"\nIncoming Shot\n";
        turnResult = m_player1->incomingShot(targetCoord);
        cout<<"\nTurn Result"<<turnResult<<"\n";
        return(m_player1->getNumAfloat() < 1);
    }
  }
  else //no AI, p v p
  {
   // std::this_thread::sleep_for(m_interval);
    clearConsole();
    
    if(player == 1)
    {
      cout<<"\nDo you want to save and quit the game? Hit S/s to save and quit. Hit C/c to continue.\n";
      cin>>quit_choice;
      if(quit_choice=='s'||quit_choice=='S')
      {
        saveGame(m_turn, m_player1, m_player2, AI);
        exit(0);
      }
      else
      {
        cout << "Player 1's turn!\n\n";
        printMaps(player);
        printEnemyAction();
        if(m_powerups.hasAPowerup(true))
        {
          powerup = askForPowerUp(1);
          usePowerUp(powerup, true);
        }
        else //no powerups
        {
            targetCoord = askForFireCoord(m_turn);
            turnResult = m_player2->incomingShot(targetCoord);
            receivedPowerUp(turnResult, true, targetCoord);
            printTurnResult(turnResult);
        }
        cout << "Next player's turn!\n";
        return(m_player2->getNumAfloat() < 1);
      }
    }
    else  // player 2
    {
      cout<<"\nDo you want to save and quit the game? \nHit S/s to save and quit. Hit C/c to continue.\n";
      cin>>quit_choice;
      if(quit_choice=='s'||quit_choice=='S')
      {
        saveGame(m_turn, m_player1, m_player2, AI);
        exit(0);
      }
      else
      {
      cout << "Player 2's turn!\n\n";
      printMaps(player);
      printEnemyAction(); 
      if(m_powerups.hasAPowerup(false))
      {
          powerup = askForPowerUp(2);
          usePowerUp(powerup, false);
      }
      else //no powerups
      {
        targetCoord = askForFireCoord(m_turn);
        turnResult = m_player1->incomingShot(targetCoord);
        receivedPowerUp(turnResult, false, targetCoord);
        printTurnResult(turnResult);
      }
      cout << "Next player's turn!\n";
      return(m_player1->getNumAfloat() < 1);
      }
    }
  }
}

int Executive::gameplay(const bool AI)
{
  bool gameOver = false;
  int winner = 0;
  while(!gameOver)
  {
    if(m_turn == 1)
    {
      gameOver = handleTurn(1, false);
    }
    else
    {
      gameOver = handleTurn(2, AI);
      cout<<"\nAI Turn Over\n";
    }

    if(gameOver)
    {
      if(m_turn == 1)
      {
        winner = 1;
      }
      else
      {
        if(AI)
        {
          winner = 3;
        }
        else
        {
          winner = 2;
        }
      }
    }
    switchTurn();
  }
  return(winner);
}

void Executive::placePowerUp(bool ai)
{
   Grid* p1 = m_player1->getBoard();
   Grid* p2 = m_player2->getBoard();
   string coor = "";
  //  coor = p2->randCoor(false);
  //  p2->setCoor("1:1", "T");
   coor = p2->randCoor(false);
   p2->setCoor(coor, "U");
   coor = p2->randCoor(false);
   p2->setCoor(coor, "S");
   coor = p2->randCoor(false);
   p2->setCoor(coor, "R");
   coor = p2->randCoor(false);
   if(!ai)
   {
      // p1->setCoor(coor, "T");
      // coor = p1->randCoor(false);
      p1->setCoor(coor, "U");
      coor = p1->randCoor(false);
      p1->setCoor(coor, "S");
      coor = p1->randCoor(false);
      p1->setCoor(coor, "R");
   }
   
}

string Executive::askForPowerUp(const int player)
{
  bool goodInput = false;
  vector<string> playerPUs = m_powerups.getPowerUps(player);
  vector<string>::iterator it = playerPUs.begin();
  string choice = "";
  do {
      cout << "Player " << player << ", would you like to use a powerup?\n";
      it = playerPUs.begin();
      for(int i = 0; i< playerPUs.size(); i++)
      {
        if(playerPUs.at(i) == "T")
        {
          cout << "Enter T to use your Torpedo.\n";
        }
        if(playerPUs.at(i)== "R")
        {
          cout << "Enter R to use your Radar.\n";
        }
        if(playerPUs.at(i) == "U")
        {
          cout << "Enter U to use your Uber Commander.\n";
        }
        if(playerPUs.at(i) == "S")
        {
          cout << "Enter S to use your Scattershot.\n";
        }
        ++it;
      }  
      cout << "Or enter N to not use a powerup this turn.\n";
      cout << "Your choice: ";
      cin >> choice;
      //check whether the choice is in the vector
      if(choice == "T"||choice == "R" || choice == "U" || choice =="S" || choice== "N")
      {
        goodInput = true;
      }
      // it = find(playerPUs.begin(), playerPUs.end(), choice);
      // if(it != playerPUs.end())
      // {
      //   goodInput = true;
      // }
  } while(!goodInput);
  return(choice);
}

string Executive::askForFireCoord(const int player)
{
  string coord = "";
  do {
    cout << "Player " << player << ", input a valid coordinate to fire on: ";
    cin >> coord;
  } while(!validPos(coord));
  string fire = "";
  fire = coord.substr(0,1) + ":" + to_string(charCoordtoIntCoord(coord.at(1)));
  return(fire);
}

void Executive::usePowerUp(string powerup, bool isPlayer1)
{
  Admiral* opponent;
  Admiral* self;
  if(isPlayer1)
  {
    opponent = m_player2;
    self = m_player1;
  }
  else
  {
    opponent = m_player1;
    self = m_player2;
  }
  
  string targetCoord = "";
  string turnResult = "";
  if (powerup == "N")
  {
    targetCoord = askForFireCoord(m_turn);
    turnResult = opponent->incomingShot(targetCoord);
    printTurnResult(turnResult);
  }
  else if (powerup == "T")
  {     
    cout << "The torpedo will automatically sink a ship if you hit one.";
    targetCoord = askForFireCoord(m_turn);
    string* shipCoords;
    int size = -1;
    turnResult = opponent->incomingShot(targetCoord);
    if (turnResult == "hit")
    {
      int index = opponent->findShipbyCoord(targetCoord);
      shipCoords = (opponent->getFleet())->at(index)->getCoords();
      size = (opponent->getFleet())->at(index)->getSize();
      m_powerups.useTorpedo(targetCoord, isPlayer1, size, shipCoords);
    }
    else
    {

      m_powerups.useTorpedo(targetCoord, isPlayer1, size, shipCoords);
    }
    self->getBoard()->printGrid(true);
  }
  else if (powerup == "R")
  {
    cout << "Radar will allow you to see a 3x3 part of the board. Water tiles will be shown as misses.";
    cout << "Enter the coordinate for the center of the box \n";
    targetCoord = askForFireCoord(m_turn);
    m_powerups.useRadar(targetCoord, isPlayer1);
    self->getBoard()->printGrid(true);
  }
  else if (powerup == "S")
  {
    cout << "Scatter shot fired at three random locations.";
    m_powerups.useScatterShot(isPlayer1);
    self->getBoard()->printGrid(true);
  }
  else //uber commander
  {
    cout << "UberCommander automatically sinks your opponents smallest remaining ship.\n";
    targetCoord = askForFireCoord(m_turn);
    m_powerups.useUberCommander(targetCoord, isPlayer1);
    self->getBoard()->printGrid(true);
  }
          
}

void Executive::receivedPowerUp(string turnResult, bool isPlayer1, string coord)
{
  if(turnResult == "T" || turnResult == "R" || turnResult == "U" || turnResult == "S")
  {
    m_powerups.addPowerUp(turnResult, isPlayer1);
    m_player1->getBoard()->setCoor(coord, "O");
  }
}
vector<string>* Executive::getPowerups(const int player) const
{
//   vector<string>* powerups = m_powerups.getPowerUps(player);
}

void Executive::printTurnResult(const string result) const
{
  if(result == "hit")
  {
    cout << "You hit their ship!\n";
  }
  else if(result == "sunk")
  {
    cout << "You hit and sank their ship!\n";
  }
  else if(result == "miss")
  {
    cout << "You missed.\n";
  }
  else if(result == "X")
  {
    cout << "You already fired on that location.\n";
  }
  else
  {
    cout << "You got a powerup!\n";
  }
}

// --- HELPER FUNCTIONS ---
int Executive::charCoordtoIntCoord(char c){
  return (int)c - 64;
}

string Executive::convertCoord(string orig)
{
  int row = charCoordtoIntCoord(orig.at(0));
  int col = charCoordtoIntCoord(orig.at(1));
  string newCoord = to_string(row) + ":" + to_string(col);
  return(newCoord);
}

void Executive::printGameOver(const int player) const
{
  cout << "GAME OVER!\n";
  if(player == 3)
  {
    cout << "The AI won the game.  Better luck next time!\n";
  }
  else
  {
    cout << "Congratulations player " << player << ", you are the winner!\n\n";
    cout << "                                  )___(\n";
    cout << "                           _______/__/_\n";
    cout << "                  ___     /===========|   ___\n";
    cout << " ____       __   [\\\\]___/____________|__[///]   __\n";
    cout << " \\   \\_____[\\\\]__/___________________________\\__[//]___\n";
    cout << "  \\448                                                 |\n";
    cout << "   \\                                                  /\n";
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n";
  }
}

void Executive::printMaps(const int player) const
{
  clearConsole();
  if(player == 1)
  {
    cout << "Your firing map:\n";
    m_player2->getBoard()->printGrid(true);
    cout << "Your ship map:\n";
    m_player1->getBoard()->printGrid(false);
  }
  else
  {
    m_player1->getBoard()->printGrid(true);
    m_player2->getBoard()->printGrid(false);
  }
}

void Executive::printEnemyAction() const
{
}

void Executive::setupGame(bool AI)
{
  m_powerups = PowerUps();
  if(AI)
  {
    bool valid = false;
    string difficulty = "";
    cout << "Starting a Player vs. AI game!\n";
    cout << "Choose a difficulty level for the AI.\n1) Easy\n2) Medium\n3) Hard\nYour choice: ";
    
    do
    {
      std::getline(std::cin, difficulty);
      if (difficulty == "1")
      {
        m_player1 = new Admiral();
        m_player2 = new EasyAI(m_player1->getBoard());
        valid = true;
      }
      else if (difficulty == "2")
      {
        m_player1 = new Admiral();
        m_player2 = new MedAI(m_player1->getBoard());
        valid = true;

      }
      else if (difficulty == "3")
      {
        m_player1 = new Admiral();
        m_player2 = new HardAI(m_player1);
        valid = true;
      }
      else
      {
        std::cout << "Error, enter a valid number from 1-3: ";
      }
    }while(!valid);

    
    setNumShips();
    std::cout<< "\nPlayer 1: It's time to place your ships.\n\n";
    placeShip(m_numShips, m_player1, false);

    //place AI ships
    placeAIShips(m_numShips);
    placePowerUp(true);
    std::cout << "Thanks for placing your ships. The AI's ships have been placed randomly. Time to start the game!\n";
  }
  else
  {
    cout << "Starting a Player vs. Player game!\n";
    m_player1 = new Admiral();
    m_player2 = new Admiral();
    setNumShips();
    std::cout<< "\nPlayer 1: It's time to place your ships.\n\n";
    placeShip(m_numShips, m_player1, false);
    std::cout << "Thanks for placing your ships, player 1! \nNow it's player 2's turn.";
    //std::this_thread::sleep_for(std::chrono::seconds(5));

    clearConsole();
    std::cout<< "Player 2: It's time to place your ships.\n\n";
    placeShip(m_numShips, m_player2, false);
    std::cout << "Thanks for placing your ships. Time to start the game!";
    placePowerUp(false);
    //std::this_thread::sleep_for(std::chrono::seconds(5));
  }
  m_powerups.setAdmirals(m_player1,m_player2);
}

void Executive::clearConsole() const
{
  for(int i = 0; i < 100; i++)
  {
    cout << '\n';
  }
}

string* Executive::generateCoordsArr(int direction, int startRow, int startCol, int size)
{
  string coor = "";
  string* coordsArr = new string[size];
  if(direction == 1)  // north
  {
    int rowIndex = startRow;
    for(int i = size-1; i>=0; i--)
    {
      coor = to_string(rowIndex) + ":" + to_string(startCol);
      coordsArr[i] = coor;
      rowIndex--;
    }
  }
  else if(direction == 2) // east
  {
    int colIndex = startCol;
    for(int i = 0; i < size; i++)
    {
      coor = to_string(startRow) + ":" + to_string(colIndex);
      coordsArr[i] = coor;
      colIndex++;
    }
  }
  else if(direction == 3) // south
  {
    int rowIndex = startRow;
    for(int i = 0; i < size; i++)
    {
      coor = to_string(rowIndex) + ":" + to_string(startCol);
      coordsArr[i] = coor;
      rowIndex++;
    }
  }
  else if(direction == 4) // west
  {
    int colIndex = startCol;
    for(int i = size-1; i >= 0; i--)
    {
      coor = to_string(startRow) + ":" + to_string(colIndex);
      coordsArr[i] = coor;
      colIndex--;
    }
  }
  else
  {
    cout << "ERROR in Exec::generateCoordsArr.\n";
  }
  return(coordsArr);
}

string* Executive::generateEndpoints(int startRow, int startCol, int size, bool** taken)
{
  string* endpoints = new string[4];

  if (startRow - (size-1) > 0)  // north
  {
    bool valid = true;
    for (int j = 0; j< size; j++)
    {
      if (taken[startRow - j][startCol])
      {
        valid = false;
      }
    }
    if (valid)
    {
      endpoints[0] = to_string(startRow - (size-1)) + ":" + to_string(startCol);
    }
    else
    {
      endpoints[0] = "";
    }
  }
  else
  {
    endpoints[0] = "";
  }

  if (startCol + (size-1) <= 8) // east
  {
    bool valid = true;
    for (int j = 0; j< size; j++)
    {
      if (taken[startRow][startCol + j])
      {
        valid = false;
      }
    }
    if (valid)
    {
      endpoints[1] = to_string(startCol + (size - 1)) + ":" + to_string(startRow);
    }
    else
    {
      endpoints[1] = "";
    }
  }
  else
  {
    endpoints[1] = "";
  }

  if (startRow + (size-1) <= 8) // south
  {
    bool valid = true;
    for (int j = 0; j< size; j++)
    {
      if (taken[j+startRow][startCol])
      {
        valid = false;
      }
    }
    if (valid)
    {
      endpoints[2] = to_string(startRow+(size-1)) + ":" + to_string(startCol);
    }
    else
    {
      endpoints[2] = "";
    }
  }
  else
  {
    endpoints[2] = "";
  }

  if (startCol-(size-1) > 0)  // west
  {
    bool valid = true;
    for (int j = 0; j< size; j++)
    {
      if (taken[startRow][startCol-j])
      {
        valid = false;
      }
    }
    if (valid)
    {
      endpoints[3] = to_string(startCol - (size - 1)) + ":" + to_string(startRow);
    }
    else
    {
      endpoints[3] = "";
    }
  }
  else
  {
    endpoints[3] = "";
  }

  return(endpoints);
}

int Executive::chooseRandDirection(string* endpoints)
{
  int randDirection = 0;
  bool valid = false;
  while(!valid)
  {
    randDirection = rand()%4 + 1;

    if(endpoints[randDirection - 1] != "")
    {
      valid  = true;
    }
  }
  return(randDirection);
}

int* Executive::generateStartCoord(bool** taken)
{
  int* startCoord = new int[2];
  bool valid = false;
  while(!valid)
  {
    startCoord[0] = rand() % 8 + 1;
    startCoord[1] = rand() % 8 + 1;
    if(taken[startCoord[0]][startCoord[1]])
    {
      valid = false;
    }
    else
    {
      valid = true;
    }
  }
  return(startCoord);
}

void Executive::updateTaken(bool** taken, int direction, int startRow, int startCol, int size, bool value)
{
  if(direction == 1)  // north
  {
    int rowIndex = startRow;
    for(int i = size-1; i>=0; i--)
    {
      taken[rowIndex][startCol] = value;
      rowIndex--;
    }
  }
  else if(direction == 2) // east
  {
    int colIndex = startCol;
    for(int i = 0; i < size; i++)
    {
      taken[startRow][colIndex] = value;
      colIndex++;
    }
  }
  else if(direction == 3) // south
  {
    int rowIndex = startRow;
    for(int i = 0; i < size; i++)
    {
      taken[rowIndex][startCol] = value;
      rowIndex++;
    }
  }
  else if(direction == 4) // west
  {
    int colIndex = startCol;
    for(int i = size-1; i >= 0; i--)
    {
      taken[startRow][colIndex] = value;
      colIndex--;
    }
  }
  else
  {
    cout << "ERROR in Exec::updateTaken.\n";
  }
}

void Executive::placeAIShips(const int numShips)
{
  bool** taken = new bool*[9];
  for(int i = 0; i < 9; i++)
  {
    taken[i] = new bool[9];
  }

  for(int i = 0; i < 9; i++)
  {
    for(int j = 0; j < 9; j++)
    {
      taken[i][j] = false;
    }
  }

  int** allStartCoords = new int*[numShips];
  for(int i = 0; i < 3; i++)
  {
    allStartCoords[i] = new int[3];
  }
  int* startCoord = nullptr;
  string* endpoints = nullptr;
  int direction = 0;
  string* coordsArr = nullptr;

  bool canPlaceShip = true;
  int i = 1;
  while(i <= numShips)
  {
    if(i < 1)
    {
      i = 1;
      cout << "Exec::placeAIShips: somehow the loop went below 1\n";
    }

    startCoord = generateStartCoord(taken);
    if(i > 1)
    {
      endpoints = generateEndpoints(startCoord[0], startCoord[1], i, taken);
      direction = chooseRandDirection(endpoints);
      if(direction != 0)
      {
        coordsArr = generateCoordsArr(direction, startCoord[0], startCoord[1], i);
        m_player2->addShip(i, coordsArr);
        canPlaceShip = true;
        updateTaken(taken, direction, startCoord[0], startCoord[1], i, true);
      }
      else
      {
        canPlaceShip = false;
      }
    }
    else
    {
      coordsArr = generateCoordsArr(1, startCoord[0], startCoord[1], i);
      canPlaceShip = true;
      updateTaken(taken, 1, startCoord[0], startCoord[1], i, true);
    }
    
    if(canPlaceShip)
    {
      allStartCoords[i - 1][0] = startCoord[0];
      allStartCoords[i - 1][1] = startCoord[1];
      allStartCoords[i - 1][2] = direction;
      i++;
    }
    else
    {
      m_player2->removeLastShip();
      
      updateTaken(taken, allStartCoords[i - 2][2], allStartCoords[i - 2][0], allStartCoords[i - 2][1], i, false);
      i--;
    }

    delete[] startCoord;
    delete[] endpoints;
    delete[] coordsArr;
  }
  
  for(int i = 0; i < 9; i++)
  {
    delete[] taken[i];
  }
  delete[] taken;

  for(int i = 0; i < 3; i++)
  {
    delete[] allStartCoords[i];
  }
  delete[] allStartCoords;
}