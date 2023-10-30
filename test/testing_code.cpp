//For full comments on functionality of main code, see arduino folder

#include <iostream>
#include <chrono>

using namespace std;

int score = 0;
bool play = false;
long startTime = 0;
long programStart;

int gameDurSec = 80;

int numUp = 0;

int pistonExtends[3] = {0, 0, 0};
long pistonStarts[3] = {0, 0, 0};
string pistonRep[3] = {"_", "_", "_"};

long lastSpawn = 0;

double pistonLife = 2.0;

long spawnSpace;
double spawnBase = 2;

int totalUp = 0;

//-------------
int pistonDelays[3] = {0, 0, 0};
long pistonDelayStarts[3] = {0, 0, 0};

double delayTime = 0.3;

//simulates arduino random function
int random(int low, int high) {
  return rand() % (high - low) + low;
}

//simulates arduino millis function
long millis() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - programStart;
}

void displayPistons() {
  //DigitalWrites
  if(pistonExtends[0] == 1) {
    pistonRep[0] = "T";
  } else {
    pistonRep[0] = "_";
  }
  if(pistonExtends[1] == 1) {
    pistonRep[1] = "T";
  } else {
    pistonRep[1] = "_";
  }
  if(pistonExtends[2] == 1) {
    pistonRep[2] = "T";
  } else {
    pistonRep[2] = "_";
  }

  std::cout << pistonRep[0] + " ";
  std::cout << pistonRep[1] + " ";
  std::cout << pistonRep[2] + "\n";
}

void startGame() {
  score = 0;
  play = true;
  startTime = millis();
  numUp = 0;
  spawnSpace = spawnBase * 1000 + (random(0, 1000) - 500);
  pistonExtends[0] = 0;
  pistonExtends[1] = 0;
  pistonExtends[2] = 0;
  //----
  pistonDelays[0] = 0;
  pistonDelays[1] = 0;
  pistonDelays[2] = 0;
  std::cout << "start\n";
  displayPistons();
}

void endGame() {
  play = false;

  //pin1 low
  //pin2 low
  //pin3 low

  std::cout << "end\n";
  displayPistons();
}

void extendPiston(int pistonNum) {
  pistonExtends[pistonNum] = 1;
  pistonStarts[pistonNum] = millis();
  totalUp += 1;
  displayPistons();
}

void spawnPiston() {
  int piston = 0;

  if(numUp < 2) {
    piston = random(0, 3 - numUp);
  }

  for(int i = 0; i < 3; i++) {
    if(pistonExtends[i] == 0 && pistonDelays[i] == 0) {
      if(piston == 0) {
        extendPiston(i);
        return;
      } else {
        piston -= 1;
      }
    }
  }
}

//start of program
int main() {
  std::cout << "Hello\n";

  //sets start time
  programStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

  //only runs program for set test time
  while(millis() < 60000) {
    if(play) {
      if((millis() - startTime) > (gameDurSec * 1000)) {
        endGame();
      }

      pistonLife = 2.0 - (0.15 * score);
      spawnBase = 2.0 - (0.075 * score);

      if(pistonLife < 0.5) {
        pistonLife = 0.5;
      }
      if(spawnBase < 1.10) {
        spawnBase = 1.10;
      }

      if(numUp < 3) {
        if((millis() - lastSpawn) > spawnSpace) {
          std::cout << "PistonSpawned\n";
          spawnPiston();
          lastSpawn = millis();
          spawnSpace = int(spawnBase * 1000) + (random(0, 1000) - 500);
          numUp += 1;
        }
      }
  
      if(pistonExtends[0] == 1) {
        if((millis() - pistonStarts[0]) > pistonLife * 1000) {
          pistonExtends[0] = 0;
          displayPistons();
          pistonDelays[0] = 1;
          pistonDelayStarts[0] = millis();
        }
      }
      if(pistonExtends[1] == 1) {
        if((millis() - pistonStarts[1]) > pistonLife * 1000) {
          pistonExtends[1] = 0;
          displayPistons();
          pistonDelays[1] = 1;
          pistonDelayStarts[1] = millis();
        }
      }
      if(pistonExtends[2] == 1) {
        if((millis() - pistonStarts[2]) > pistonLife * 1000) {
          pistonExtends[2] = 0;
          displayPistons();
          pistonDelays[1] = 1;
          pistonDelayStarts[1] = millis();
        }
      }

      if(pistonDelays[0] == 1) {
        if((millis() - pistonDelayStarts[0]) > delayTime * 100) {
          pistonDelays[0] = 0;
          numUp -=1;
        }
      }
      if(pistonDelays[1] == 1) {
        if((millis() - pistonDelayStarts[1]) > delayTime * 100) {
          pistonDelays[1] = 0;
          numUp -=1;
        }
      }
      if(pistonDelays[2] == 1) {
        if((millis() - pistonDelayStarts[2]) > delayTime * 100) {
          pistonDelays[2] = 0;
          numUp -=1;
        }
      }
    } else {
      startGame();
    }
  }

  std::cout << totalUp;
  std::cout << "\n";

  return 0;
}
