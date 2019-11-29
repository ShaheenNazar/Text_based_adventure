#include <iostream>
#include <ctime>
#include <string>
#include <cstdlib>
#include <cctype>
#include <utility>

//Feature 1--- Created my own standard template library
template <typename Item>
class IsNotVector
{
  private: 
    Item *_items;
    int _size;
  
  public:
    int size() const 
    {
      return _size;
    }
    Item& operator[](int i)
    {
      return _items[i];
    };
    const Item& operator[] (int i) const 
    {
     return _items[i];
    }
    Item* myData()
    {
      return _items;
    }
    const Item* myData() const 
    {
      return _items;
    }
    
    void sizeAgain(int size)
    {
      delete[] _items;
      _items = new Item[size];
      _size = size;
    }
    
    int setPosition()
    {
      int sizeNew = _size + 1;
      Item  *newItems = new Item [sizeNew];
      for (int i = 0; i < _size; i++)
      {
        newItems[i] = std::move(_items[i]);
      }
      delete[] _items;
      _items = newItems;
      int newI = _size;
      _size = sizeNew;
      return newI;
    }
    
    void getBack(const Item &src)
    {
      int i = setPosition();
      _items[i] = src;
    }
    
    void getBack(Item &&src)
    {
      int i = setPosition();
      _items[i] = std::move(src);
    }
    
    void clearData()
    {
      delete[] _items;
      _items = nullptr;
      _size = 0;
    }
    
    IsNotVector<Item>& operator=(const IsNotVector<Item> &src)
    {
      sizeAgain(src.size());
      for(int i= 0; i < _size; i++)
      {
        _items[i] = src[i];
      }
      return *this;
    }
    
    IsNotVector<Item>& operator = (IsNotVector<Item> &&src)
    {
      delete [] _items;
      _items = src._items;
      _size = src._size;
      src._items = nullptr;
      src._size = 0;
      return *this;
    }
    
    IsNotVector()
    {
      _items = nullptr;
      _size = 0;
    }
    
    IsNotVector(int size)
    {
      _items = nullptr;
      sizeAgain(size);
    }
    
    IsNotVector(const IsNotVector<Item> &src)
    {
      _items = nullptr;
      *this = src;
    }
    
    IsNotVector(IsNotVector &&src)
    {
      _items = nullptr;
      *this = std::move(src);
    }
    
    ~IsNotVector()
    {
      delete[] _items;
    }
};

inline int getLevelCount( int gameLevel)
{
  return (gameLevel / 2) + 2;
}

//function prototype
//Noun singular ("eye", true) -> "An eye"
std::string sing(const std::string &in, bool caps);

const int GAME_LIFE_LINES = 3;

class Chamber;
class randomMonster;

enum class gameStatus
{
  endOfGame, //when the game is over 
  playerMove, // when the player has made a move 
  noMoves // when the player has made no moves 
};

class GameLogic
{
  private:
    IsNotVector<Chamber>_chambers;
    int _lifeLines, _currChamber;
    
    //function prototypes
    const char* lifeLineDescription () const;
    gameStatus gameNav(int nav);
    //returns true if the player dies after a move 
    bool playMove(int chamber_id);
    //returns true if the game is over 
    bool player_turn();
    
    void showMap(int levelNum);
    void tablePosition(const randomMonster &_randomMonster,                     const IsNotVector <int> &newChamber_ids                    );
    void monsterPosition(int monstCount, 
                         const IsNotVector<int>&newChamber_ids,
                         const IsNotVector<randomMonster> &_randomMonsters, int gameLevel);
    void exitPoint(const IsNotVector<int> &newChamber_ids,
                   const randomMonster &finalRandMonster);
    //returns true if the player is successful in completing the game 
    bool switchChambers(int newChamb_id, int orgChamb_id);
    int moreChambers();
    IsNotVector<int> createChambers(const IsNotVector<int> &formerChamb_ids);
  public:
    void playGame(int lifelines, int levelNum);
};

//Game Level Difficulties
const int LEVEL_MIN = 1;
const int LEVEL_MAX = 10;

int main()
{
  std::srand(std::time(0));
  int userLevel; //prompt to ask the user for the desired difficulty level 
  //Feature 2---Differing levels depending on how tough the player wants the game to be 
  std::cout << " Please enter the level of difficulty that you would like (1 to 10): ";
  std::cin >> userLevel;
  while(std::cin.fail() or userLevel < LEVEL_MIN or userLevel> LEVEL_MAX)
  {
    std::cin.clear();
    std::cin.ignore(1000, '\n');
    std::cout << " Sorry Invalid Input! Please enter a level from 1 to 10: ";
    std::cin >> userLevel;
  }
  std::cin.ignore(1000, '\n');
  GameLogic game;
  game.playGame(GAME_LIFE_LINES, userLevel);
  return 0;
}

const int CHAMBERS_MIN = 2;
const int CHAMBERS_MAX = 4;

const int MONST_MIN = 2;
const int MONST_MAX = 3;

inline int randomRange (int minimum, int maximum)
{
  return minimum + (std::rand() % (maximum - minimum + 1));
}

const int NAV_COUNT = 4;
const char *NAV_STRINGS[] = {"north", "east", "west", "south"};
const char INVALID_INPUT = -1;

inline int getNavigationCommand(int n)
{
  return (n + 2) % 4;
}

int navigationInput(char navigator)
{
  switch(navigator)
  {
    case 'n': return 0;
    case 'e': return 1;
    case 'w': return 2;
    case 's': return 3;
    default: return INVALID_INPUT;
  }
}

const int PASS_DOOR = -1;
const int EXIT_POINT = -2;

class randomMonster
{
  private:
     std::string monstName,
            magicSpell,
            adj,
            noise,
            monstAttack,
            monstDead;
            
  public:
     const std::string& getName() const {return monstName;}
     const std::string& getMagicWord() const {return magicSpell;}
     
     //function prototypes
     std::string describePlayerAttack() const;
     std::string describeMonsterDeath() const;
     
     void setting(const std::string &name, const std::string &magicWord);
};

enum class attackPlayerScene
{
  hitPlayer,
  monsterDead,
  retreatPlayer
};

class gameMonster
{
  private:
      randomMonster monstType;
      bool monstAlive = false;
      
  public:
      const std::string& getName() const 
      {
        return monstType.getName();
      }
      const std::string& getMagicWord() const
      {
        return monstType.getMagicWord();
      }
      
  //function prototypes
  void monstEntry(const randomMonster &_randomMonster);
  bool monstIsAlive () const {return monstAlive;}
  attackPlayerScene playerAttack();
};

class gameWall
{
  private:
      std::string wallDecors, wallAdj, wallDoors;
      int chamberID;
      bool isTable;
      randomMonster _bookMonstType;
      
  public:
      gameWall();
      const std::string& getWallDecors() const
      {
        return wallDecors;
      }
      const std::string& getwallAdj() const 
      {
        return wallAdj;
      }
      const std::string& getWallDoors() const 
      {
        return wallDoors;
      }
      int getChamberID() const 
      {
        return chamberID;
      }
      const randomMonster& getbookMonstType() const 
      {
        return _bookMonstType;
      }
      bool wallHasDoor() const 
      {
        return chamberID != PASS_DOOR;
      }
      bool hasWallDecors() const 
      {
        return wallDecors.length();
      }
      bool isExitPoint() const 
      {
        return chamberID == EXIT_POINT;
      }
      bool hasATable() const 
      {
        return isTable;
      }
      bool doorIsOpen() const 
      {
        return not wallHasDoor() and not hasATable();
      }
      
      //function prototypes
      void createChambDoors(int chamber_id);
      void correctDoor(int orgChamb_id, const gameWall &corrDoor);
      void createTable(const randomMonster &bookMonstType);
};

struct Chamber
{
  std::string name, midDecors;
  gameWall gameWalls[NAV_COUNT];
  gameMonster monst;
  bool hasAppeared;
  
  //function prototypes
  Chamber();
  
  bool chambIsOccupied() const 
  {
    return monst.monstIsAlive();
  }
  std::string describeGame() const;
  bool hasOpenPathway() const;
};

//function prototypes
void generateRandMonsters(randomMonster gameMonst[], int levelNum);
void welcomeMessage();
char detailCommand(const std::string &userCommand);
void openSpellBook(const gameWall &gameWalls);

void GameLogic::playGame(int lifelines, int levelNum)
{
  showMap(levelNum);
  _lifeLines = lifelines;
  welcomeMessage();
  bool gameIsOver;
  do 
  {
    gameIsOver = player_turn();
  }while(not gameIsOver);
}

void welcomeMessage()
{
  //Feature 3--- Displays "WELCOME" in ASCII Art 
  std::cout << "***                     ***   ************   ***            ************   ************      *******        *******      ************\n";
  std::cout << "***                     ***   ************   ***            ************   ************     ********       *********     ************\n";
  std::cout << "***                     ***   ***            ***            ***      ***   ***      ***    ***     ***    ***     ***    ***\n";
  std::cout << "***                     ***   ***            ***            ***      ***   ***      ***   ***       ***  ***       ***   ***\n";
  std::cout << "***                     ***   ************   ***            ***            ***      ***   ***        ******        ***   ************\n";
  std::cout << "***       ******        ***   ************   ***            ***            ***      ***   ***                      ***   ************\n";
  std::cout << "***      ***  ***      ***    ***            ***            ***      ***   ***      ***   ***                      ***   ***\n";
  std::cout << " ***    ***    ***    ***     ***            ***            ***      ***   ***      ***   ***                      ***   ***\n";
  std::cout << "  ***  ***      ***  ***      ************   ************   ************   ************   ***                      ***   ************\n";
  std::cout << "   ***             ***        ************   ************   ************   ************   ***                      ***   ************\n";
  
  std::cout << std::endl;
  std::cout << "\nHello! Welcome to the Text Based Adventure Game!\n\n" << std::endl;
  std::cout << " You wake up from a deep sleep to see that you are lying at a corner in a myterious royal chamber of an enormous cursed palace. You've got no weapon to help yourself besides a magic wand, but you don't know how to use it since you have no spells that you know of! You find a way to escape but before that, you might want to look for a magic spell book. ";
}

bool GameLogic::player_turn()
{
  std::cout << std::endl;
  Chamber &chamber = _chambers[_currChamber];
  if(chamber.hasAppeared)
  {
    //Specification 3---Display room titles when the player subsequently visits the room 
    std::cout << " You are in " << sing(chamber.name, false) << std::endl;
  }
  else 
  {
    //Specification 2--- Display room title and room description when the player first enters the room 
    std::cout << chamber.describeGame();
    chamber.hasAppeared = true;
  }
  
  while(true)
  {
    //Specification 6--- Input Validation for command inputs from user 
    std::cout << " Type in your command (or type h for help): ";
    std::string navCommandString;
    std::getline(std::cin, navCommandString);
    char navCommand = detailCommand(navCommandString);
    switch(navCommand)
    {
      //Specification 3(a)---Only redisplay the room if the player enters "L" for look 
      case 'l':
         std::cout << chamber.describeGame();
         break;
      case 'h':
         std::cout << " Navigations: \n"
                 " L: Look around the room.\n"
                 " N, E, W, S: direction keys for moving north, east, west or south.\n";
         break;
      default:
          //Specification 1--- Move throughout the game using the cardinal directions N, E, W, S 
          int nav = navigationInput(navCommand);
          if(nav == INVALID_INPUT)
          {
            std::cout << " Sorry! Invalid Input! Please enter a valid command: ";
          }
          else 
          {
            gameStatus stats = gameNav(nav);
            switch(stats)
            {
              case gameStatus::endOfGame:
                   return true;
              case gameStatus::playerMove:
                   return false;
              case gameStatus::noMoves:
                   {}//reprompts the user incase of Invalid input from the user 
            }
          }
    }
  }
}

gameStatus GameLogic::gameNav(int nav)
{
  Chamber &chamber = _chambers[_currChamber];
  gameWall &wall = chamber.gameWalls[nav];
  if (wall.wallHasDoor())
  {
    if(wall.isExitPoint())
    {
      std::cout << "\nAs you keep moving, you find a slight ray of sunlight shining on you.\n"
        "You feel the wind blowing and gently touching you,\n"
        "with the pleasant fragrance of blossoms which refresh you."
        "You feel proud of yourself for you've got the fruit of your hardship!\n"
        "\nCongratulations! You made it!\n"
        "\n------End of Game------\n";
      return gameStatus::endOfGame;
    }
    bool isDead = playMove(wall.getChamberID());
    return isDead ? gameStatus::endOfGame : gameStatus::playerMove;
  }
  else if(wall.hasATable())
  {
    openSpellBook(wall);
  }
  else
  {
    std::cout << " It's empty! " << std::endl;
  }
  return gameStatus::noMoves;
}

bool GameLogic::playMove(int chamber_id)
{
  Chamber &chamber = _chambers[chamber_id];
  if (chamber.chambIsOccupied())
  {
    gameMonster &monst = chamber.monst;
    std::cout << " You enter the chamber, but before you make your next move, \n "
    << sing(monst.getName(),false) << " comes into your way! \n ";
    
    attackPlayerScene scene = monst.playerAttack();
    
    if (scene == attackPlayerScene::hitPlayer)
    {
      _lifeLines--;
      std::cout << lifeLineDescription() << std::endl;
      if (_lifeLines <= 0)
      {
        std::cout << "---It's Over!---\n";
        return true;
      }
      std::cout<< " You are thrown back into the chamber you intially came from!\n";
      std::cout << " The " << monst.getName() << " is not here.\n";
      return false;
    }
    else if (scene == attackPlayerScene::retreatPlayer)
    {
      return false;
    }//Monster gets killed 
  }
  _currChamber = chamber_id;
  return false;
}

const char* GameLogic:: lifeLineDescription() const 
{
  if(_lifeLines == 1)
  {
    return " Woah! That was a strong hit! You might not bear a second one!";
  }
  else if (_lifeLines <= 0)
  {
    return " Looks like you've reached the end of your life!";
  }
  else 
  {
    return " Good so far............";
  }
}

char detailCommand(const std::string &navCommand)
{
  if (navCommand.length() != 1)
  {
    return INVALID_INPUT;
  }
  return std::tolower(navCommand[0]);
}

//function prototype
void repeatMagicWord(const char *userPrompt, const std::string &magWord);

//Feature 4---Book of magic spells to gain magical powers
void openSpellBook(const gameWall &gameWalls)
{
  const randomMonster &_randomMonster = gameWalls.getbookMonstType();
  std::cout << " You find a book of magic spells on the table!\n"
  " you eagerly rush to the table hoping to get a solution. You open the book and turn every page until you come across the magic spell " << _randomMonster.getMagicWord() << "!" << " This magic word can be used to defend yourself against " << sing(_randomMonster.getName(),false) << ".\n";
  repeatMagicWord("It's time to practice the magic word now: ", _randomMonster.getMagicWord());
  repeatMagicWord("Good Job! Enter the magic word again: ", _randomMonster.getMagicWord());
  repeatMagicWord("Amazing! Enter the magic word one last time and you are all set! ", _randomMonster.getMagicWord());
  std::cout << " You leave the book on the table and set out to find your way! ";
  std::cout << " In case you forget the magic word, you can return here! " ;
}

void stringLowerCase(std:: string &input)
{
  for (unsigned int j = 0; j < input.length(); j++)
  {
    input[j] = static_cast<char>(std::tolower(input[j]));
  }
}

void repeatMagicWord(const char *userPrompt, const std::string &magWord)
{
  std::string inputSpell;
  std::cout << userPrompt;
  std::getline(std::cin, inputSpell);
  stringLowerCase(inputSpell);
  while (inputSpell != magWord)
  {
    std::cout << " Maybe you misspelt something, please try again: ";
    std::getline(std::cin,inputSpell);
  }
  for (int i=0; i<3; i++)
  {
    std::cout << ".\n";
  }
}

//Feature 5---Randomly Generated Game Map
void GameLogic::showMap(int levelNum)
{
  IsNotVector<randomMonster> _randomMonsters(levelNum);
  generateRandMonsters(_randomMonsters.myData(), levelNum);
  _chambers.clearData();
  
  //the room begins with the table 
  int firstChambId = moreChambers();
  Chamber &firstChamber = _chambers[firstChambId];
  _currChamber = firstChambId;
  IsNotVector<int> formerChamb_ids;
  formerChamb_ids.getBack(firstChambId);
  firstChamber.gameWalls[rand() % NAV_COUNT].createTable(_randomMonsters[0]);
  
  for (int gameLevel = 0; gameLevel < levelNum; gameLevel++)
  {
    IsNotVector<int> newChamber_ids;
    newChamber_ids = createChambers(formerChamb_ids);
    
    if (gameLevel != (levelNum - 1 ))
    {
      tablePosition(_randomMonsters[gameLevel + 1], newChamber_ids);
    }
    int newChambCount = newChamber_ids.size();
    int monstersMin = newChambCount < MONST_MIN ? newChambCount: MONST_MIN;
    int monstersMax = newChambCount < MONST_MAX ? newChambCount: MONST_MAX;
    int monstersNum = randomRange(monstersMin, monstersMax);
    monsterPosition(monstersNum, newChamber_ids, _randomMonsters, gameLevel);
    formerChamb_ids = std::move(newChamber_ids);
  }
  exitPoint(formerChamb_ids, _randomMonsters[levelNum - 1]);
}

//function prototype
bool hasOpenChambers (const IsNotVector<int> ids, const IsNotVector<Chamber> &chambers);

//Specification 5--- Minimum of 5 rooms in the game 
// Feature 6--- More than 5 rooms as levels increase
IsNotVector<int> GameLogic::createChambers(const IsNotVector<int> &formerChamb_ids)
{
  int newChambCount = randomRange(CHAMBERS_MIN, CHAMBERS_MAX);
  IsNotVector<int> newChamber_ids;
  for (int c=0; c<newChambCount; c++)
  {
    int orgChambCount = _chambers.size();
    int newChamb_id = moreChambers();
    int orgChamb_id;
    
    if(newChamber_ids.size() >= 1 && (std::rand()%3) > 0)
    {
      do 
      {
        int indNum  = std::rand()% newChamber_ids.size();
        orgChamb_id = newChamber_ids[indNum];
      } 
      while (not switchChambers(newChamb_id, orgChamb_id));
    }
    else if(hasOpenChambers(formerChamb_ids, _chambers))
    {
      do 
      {
        int indNum = std::rand()%formerChamb_ids.size();
        orgChamb_id = formerChamb_ids[indNum];
      }
      while (not switchChambers(newChamb_id, orgChamb_id));
    }
    else 
    {
      do 
      {
        orgChamb_id = std::rand() % orgChambCount;
      }
      while(not switchChambers(newChamb_id,orgChamb_id));
    }
    newChamber_ids.getBack(newChamb_id);
  }
  return newChamber_ids;
}

void GameLogic::tablePosition(const randomMonster &_randomMonster,const IsNotVector <int> &newChamber_ids)
{
  Chamber *chamber;
  
  do 
  {
    int chamber_id = newChamber_ids[std::rand() % newChamber_ids.size()];
    chamber = &(_chambers[chamber_id]);
  }
  while(not chamber->hasOpenPathway());
  
  gameWall *pathway;
  do 
  {
    int nav = std::rand() % NAV_COUNT;
    pathway = &(chamber->gameWalls[nav]);
  }
  while (not pathway-> doorIsOpen());
  
  pathway -> createTable(_randomMonster);
}

void GameLogic::monsterPosition(int monstCount,const IsNotVector<int>&newChamber_ids ,const IsNotVector<randomMonster> &_randomMonsters, int gameLevel)
{
  for(int l = 0; l<monstCount; l++)
  {
    //Specification 4---Random Monsters when the room is occupied 
    int monstLevel = l ? (std::rand() % (gameLevel + 1)): gameLevel;
    const randomMonster &_randomMonster = _randomMonsters[monstLevel];
    
    Chamber *chamber;
    do 
    {
      int chamber_id = newChamber_ids[std::rand()% newChamber_ids.size()];
      chamber = &(_chambers[chamber_id]);
    }
    while (chamber->chambIsOccupied());
    
    chamber-> monst.monstEntry(_randomMonster);
  }
}

void GameLogic::exitPoint(const IsNotVector<int> &newChamber_ids,const randomMonster &finalRandMonster)
{
  Chamber *chamber;
  do 
  {
    int chamber_id = newChamber_ids[std::rand()%newChamber_ids.size()];
    chamber = &(_chambers[chamber_id]);
  }
  while(not chamber-> chambIsOccupied() || not chamber->hasOpenPathway());
  
  chamber->monst.monstEntry(finalRandMonster);
  
  gameWall *pathway;
  do 
  {
    int nav = std::rand()%NAV_COUNT;
    pathway = &(chamber->gameWalls[nav]);
  }
  while(not pathway->doorIsOpen());
  
  pathway->createChambDoors(EXIT_POINT);
}

bool GameLogic::switchChambers(int newChamb_id, int orgChamb_id)
{
  Chamber &newChamber = _chambers[newChamb_id];
  Chamber &orgChamber = _chambers[orgChamb_id];
  
  if(not orgChamber.hasOpenPathway())
  {
    return false;
  }
  
  int nav;
  do 
  {
    nav = std::rand() % NAV_COUNT;
  }
  while(not orgChamber.gameWalls[nav].doorIsOpen());
  
  gameWall &orgDoorWall = orgChamber.gameWalls[nav];
    orgDoorWall.createChambDoors(newChamb_id);
    
    gameWall &newDoorWall = newChamber.gameWalls[getNavigationCommand(nav)];
    newDoorWall.correctDoor(orgChamb_id, orgDoorWall);
    
    return true;
}

bool hasOpenChambers(const IsNotVector<int> ids, const IsNotVector<Chamber> &chambers)
{
  for(int d = 0; d < ids.size(); d++)
    {
        int id = ids[d];
        if(chambers[id].hasOpenPathway())
        {
            return true;
        }
    }
    return false;
}

int GameLogic::moreChambers()
{
  return _chambers.setPosition();
}

const char *GAME_ADJECTIVES[] = 
{
  " dusty ", " shabby "," smoky ", " windowless ", " silent ", " grand ", " ancient ", " dismal ", " deserted ", " vaulted ", " mysterious "
};
const int GAME_ADJECTIVES_COUNT = 11;

const char *CHAMBER_TYPES[] = 
{
  " room ", " dungeon ", " porch ", " ballroom ", " hallway ", " chamber "," attic "," greathall "
};
const int CHAMBER_TYPES_COUNT = 8;

const char *CHAMBER_DOORS[] = 
{
  " glass door ", " silver door ", " gold door ", " ivory door ", " metal gate ", " rosewood door ", " Ogee archway "
};
const int CHAMBER_DOORS_COUNT = 7;

const char *CENTER_DECORS[] = 
{
  " marble statue ", " huge clock ", " lotus pond ", " teak desk "," giant flower vase "," grand harp ", " great throne ", " fountain "," great white pearl "
};
const int CENTER_DECORS_COUNT = 9;

const char *WALL_DECORS[] = 
{
  " huge chandelier hangs from above ", " big golden candle holder sits by ", " elegant dusty rocking chair sits by ", " bamboo torch with a reddish flame juts from the corner of ", " huge mirror hangs on ", " silky soft curtains fly from ", "painted window is set in ", " broken vase sits by "," broken jar lies ", " torch with a blue flame lights up from ", " small earthen lamp sits by ", " rusty sword hangs on", " broken window is set in ", " disorganized chess board sits by " 
};
const int WALL_DECORS_COUNT = 14;

const char *MONST_ADJ[] = 
{
  " colossal ", " grumpy ", " cruel ", " ugly ", " bushy ", " horned ", " three-toed ", " whiskered ", " eight-eyed ", " hairy ", " creepy ", " ferocious ", " gigantic ", " poisonous ", " bloodthirsty "
};
const int MONST_ADJ_COUNT = 14;

const char *MONST_NOISES[] =
{
  " grunts ", " rattles ", " clatters ", " roars ", " hisses ", " croaks ", " groans "
};
const char MONST_NOISES_COUNT = 7;

const char *MONST_ATTACKTYPES[] = 
{
  " pounces ", " plunges ", " leaps ", " smacks ", " slaps ", " claws "
};
const int MONST_ATTACKTYPES_COUNT = 6;

const char *MONST_DEATHTYPES [] = 
{
  " freezes and shatters into pieces ", " rapidly melts into a weird sticky fluid ", " catches fire and burns into ashes ", " disappears into thin air ", " turns into a stone statue and quickly wears off ", " crumbles into sand ", " gets trapped in a bubble and blasts along with the bubble ", " cracks and bursts off ", " has become extinct ", " pops up and vanishes ", " falls to the ground with a thud and turns into dust "
};
const int MONST_DEATHTYPES_COUNT = 11;

void randomMonster::setting(const std::string &name, const std::string &magicWord)
{
  monstName = name;
  magicSpell = magicWord;
  adj = MONST_ADJ[std::rand() % MONST_ADJ_COUNT];
  noise = MONST_NOISES[std::rand() % MONST_NOISES_COUNT];
  monstAttack = MONST_ATTACKTYPES[std::rand() % MONST_ATTACKTYPES_COUNT];
  monstDead = MONST_DEATHTYPES[std::rand() % MONST_DEATHTYPES_COUNT];
}

std::string randomMonster::describePlayerAttack() const 
{
  return std::string(" The ") + adj + " " + monstName + " " + noise + " " + " when it " + monstAttack + " at you! ";
}

std::string randomMonster::describeMonsterDeath() const 
{
  return std::string(" The ") + monstName + " " + monstDead + ".";
}

void gameMonster::monstEntry(const randomMonster &_randomMonster)
{
  monstType = _randomMonster;
  monstAlive = true;
}

//Feature 7---Enables player to practice and remember the magic words 
attackPlayerScene gameMonster::playerAttack()
{
  std::cout << monstType.describePlayerAttack() << std::endl;
  std::cout << " Enter the spell to cast it or enter \"withdraw\": ";
  std::string chant;
  std::getline(std::cin,chant);
  stringLowerCase(chant);
  if (chant == "withdraw")
  {
    std::cout << " You withdraw and return back! \n";
    return attackPlayerScene::retreatPlayer;
  }
  else if (chant == getMagicWord())
  {
    std::cout << monstType.describeMonsterDeath() << "\n\n";
    monstAlive = false;
    return attackPlayerScene::monsterDead;
  }
  else 
  {
    std::cout << " The " << getName() << " attacks you!\n";
    return attackPlayerScene::hitPlayer;
  }
}

bool Chamber::hasOpenPathway() const
{
  for(int p =0; p<NAV_COUNT; p++)
  {
    if(gameWalls[p].doorIsOpen())
    {
      return true;
    }
  }
  return false;
}

std::string Chamber::describeGame() const 
{
  std::string result = std::string(" You've arrived in ") + sing(name, false) + ".\n";
  if (midDecors.length())
  {
    result += std::string(" In the middle is ") + sing(midDecors,false) + ".\n";
  }
  for(int m = 0; m < NAV_COUNT; m++)
  {
    const gameWall &wall = gameWalls[m];
    if(wall.hasWallDecors())
    {
      result += sing(wall.getWallDecors(), true) + " the " + NAV_STRINGS[m] + " wall. \n";
    }
    else if(wall.wallHasDoor())
    {
      result += sing(wall.getwallAdj(), true) + wall.getWallDoors() + " is in the " + NAV_STRINGS[m] + ".\n";
    }
    else if (wall.hasATable())
    {
      result += std::string(" A table is at the ") + NAV_STRINGS[m] + ". (Might want to get there?)\n";
    }
  }
  return result;
}

//Feature 8--- Random descriptions of the rooms in the game 
gameWall::gameWall()
{
  chamberID = PASS_DOOR;
  isTable = false;
  if(std::rand()%2)
  {
    wallDecors = WALL_DECORS[std::rand() % WALL_DECORS_COUNT];
  }
}

void gameWall::createChambDoors(int chamber_id)
{
  chamberID = chamber_id;
  wallDecors = "";
  wallAdj = GAME_ADJECTIVES[std::rand() % GAME_ADJECTIVES_COUNT];
  wallDoors = CHAMBER_DOORS[std::rand() % CHAMBER_DOORS_COUNT];
}

void gameWall::correctDoor(int orgChamb_id, const gameWall &corrDoor)
{
   chamberID = orgChamb_id;
    wallDecors = "";
    wallAdj = corrDoor.wallAdj;
    wallDoors = corrDoor.wallDoors;
}

void gameWall::createTable(const randomMonster &bookMonstType)
{
  _bookMonstType= bookMonstType;
  isTable = true;
  wallDecors = "";
}

Chamber::Chamber()
{
  name = std::string(GAME_ADJECTIVES[std::rand()% GAME_ADJECTIVES_COUNT]) + " " + CHAMBER_TYPES[std::rand()%CHAMBER_TYPES_COUNT];
  hasAppeared = false;
  if(std::rand() % 3 == 0)
  {
    midDecors = CENTER_DECORS[std::rand() % CENTER_DECORS_COUNT];
  }
}

//Feature 9---Monster Names and Spells are generated randomly 
const char *GAME_VOWELS [] = {"a","e","i","o","u","aa","ee","oo"};
const int GAME_VOWELS_COUNT = 8;

const char C_VOW [] = {'a','e','i','o','u'};
const char C_VOW_COUNT = 5;

const char *GAME_CONSONANTS [] = {"b","c","d","f","g","h", "j", "k", "l", "m", "n", "p", "q", "r", "s", "t", "v", "w","x", "y", "z"}; 
const int GAME_CONSONANTS_COUNT = 21;

std::string generateRandString(int vowCount)
{
  std::string result;
  if (std::rand()%2)
  {
    result += GAME_CONSONANTS[std::rand() % GAME_CONSONANTS_COUNT];
  }
  while(true)
  {
    result += GAME_VOWELS[std::rand() % GAME_VOWELS_COUNT];
    vowCount--;
    if (std::rand() % 2)
    {
      result += GAME_CONSONANTS[std::rand() % GAME_CONSONANTS_COUNT];
    }
    if (vowCount <= 0)
    {
      break;
    }
    result += GAME_CONSONANTS[std::rand() % GAME_CONSONANTS_COUNT];
  }
  return result;
}

bool isAVowel(char l)
{
  l = std::tolower(l);
  for(int i = 0; i < C_VOW_COUNT; i++)
  {
    if(l == C_VOW[i])
    {
      return true;
    }
  }
  return false;
}

std::string sing(const std::string &in, bool caps)
{
  std::string result = caps ? "A" : "a";
  if(in.length())
  {
    if(isAVowel(in[0]))
    {
      result += 'n';
    }
  }
  return result + " " + in;
}

bool spellInputs(randomMonster _randomMonsters[], int levelNum, const std::string &name, const std::string &magicWord)
{
  for(int j = 0; j < levelNum; j++)
  {
    randomMonster &n = _randomMonsters[j];
    if(magicWord == n.getMagicWord() || magicWord == n.getName() || name == n.getMagicWord() || name == n.getName())
    {
      return true;
    }
  }
  return false;
}

//Feature 10---Monster varieties are randomly generated
void generateRandMonsters(randomMonster _randomMonsters[], int levelNum)
{
  for(int gameLevel = 0; gameLevel < levelNum; gameLevel++)
  {
    int levCount = getLevelCount(gameLevel);
    std::string name, magicWord;
    do 
    {
      name = generateRandString(levCount);
      magicWord = generateRandString(levCount);
    }while(spellInputs(_randomMonsters, gameLevel, name, magicWord));
    _randomMonsters[gameLevel].setting(name, magicWord);
  }
}
