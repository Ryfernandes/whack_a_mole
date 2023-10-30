const int solenoidPin1 = 3;
const int solenoidPin2 = 4;
const int solenoidPin3 = 5;
const int button1 = 6;
const int button2 = 7;
const int button3 = 8;
const int buttonStart = 9;

int score = 0;
bool play = false;
long startTime = 0;
long programStart;

int gameDurSec = 30;

int numUp = 0;

int pistonExtends[3] = {0, 0, 0};
long pistonStarts[3] = {0, 0, 0};

long lastSpawn = 0;

double pistonLife = 2.0;

long spawnSpace;
double spawnBase = 2;

int totalUp = 0;

int pistonDelays[3] = {0, 0, 0};
long pistonDelayStarts[3] = {0, 0, 0};

double delayTime = 0.3;

//writes values in stores lists of pistons to the solenoids
void updatePistons() {
  digitalWrite(solenoidPin1, pistonExtends[0]);
  digitalWrite(solenoidPin2, pistonExtends[1]);
  digitalWrite(solenoidPin3, pistonExtends[2]);
}

//resets variables at the start of a new game when the button is pressed
void startGame() {
  score = 0;
  play = true;
  startTime = millis();
  numUp = 0;
  spawnSpace = spawnBase * 1000 + (random(0, 1000) - 500);
  pistonExtends[0] = 0;
  pistonExtends[1] = 0;
  pistonExtends[2] = 0;

  pistonDelays[0] = 0;
  pistonDelays[1] = 0;
  pistonDelays[2] = 0;

  updatePistons();
}

//prints the final score at the end of the game
void endGame() {
  Serial.print("Score: ");
  Serial.print(score);
  Serial.print("\n");

  play = false;

  pistonExtends[0] = 0;
  pistonExtends[1] = 0;
  pistonExtends[2] = 0;
  
  pistonDelays[0] = 0;
  pistonDelays[1] = 0;
  pistonDelays[2] = 0;

  //puts all of the pistons down
  digitalWrite(solenoidPin1, LOW);
  digitalWrite(solenoidPin2, LOW);
  digitalWrite(solenoidPin3, LOW);

  updatePistons();
}

//extends the given piston and updates the lists of pistons to indicate that the piston is now up
void extendPiston(int pistonNum) {
  pistonExtends[pistonNum] = 1;
  pistonStarts[pistonNum] = millis();
  totalUp += 1;

  updatePistons();
}

//spawns a random piston based on the pistons that are still down
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

//opens the serial port to print to the computer and sets each of the pins for the solenoids/buttons
void setup() {
  Serial.begin(9600);

  pinMode(solenoidPin1, OUTPUT);
  pinMode(solenoidPin2, OUTPUT);
  pinMode(solenoidPin3, OUTPUT);
  pinMode(button1, INPUT);
  digitalWrite(button1, HIGH);
  pinMode(button2, INPUT);
  digitalWrite(button2, HIGH);
  pinMode(button3, INPUT);
  digitalWrite(button3, HIGH);
  pinMode(buttonStart, INPUT);
  digitalWrite(buttonStart, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);
}

//gameplay loop
void loop() {
  //triggered when button is pressed
  if(play) {
    digitalWrite(LED_BUILTIN, HIGH);

    //tracks when to end the game
    if((millis() - startTime) > (gameDurSec * 1000) || digitalRead(buttonStart) == LOW) {
      delay(2000);
      endGame();
    }

    updatePistons();

    //updates piston life/time the piston stays up based on the current score (makes game more difficult)
    pistonLife = 2.0 - (0.15 * score);
    spawnBase = 2.0 - (0.075 * score);

    if(pistonLife < 0.5) {
      pistonLife = 0.5;
    }
    if(spawnBase < 1.10) {
      spawnBase = 1.10;
    }

    //checks if it is time to spawn a new mole and if so, raises a piston
    if(numUp < 3) {
      if((millis() - lastSpawn) > spawnSpace) {
        spawnPiston();
        lastSpawn = millis();
        spawnSpace = int(spawnBase * 1000) + (random(0, 1000) - 500);
        numUp += 1;
      }
    }

    //Check pistons
    if(pistonExtends[0] == 1) {
      //Check hits
      if(digitalRead(button1) == LOW) {
        score += 1;
        Serial.print(score);
        Serial.print("\n");
        pistonExtends[0] = 0;
        updatePistons();
        pistonDelays[0] = 1;
        pistonDelayStarts[0] = millis();
      } else {
        //Check drop down
        if((millis() - pistonStarts[0]) > pistonLife * 1000) {
          pistonExtends[0] = 0;
          updatePistons();
          pistonDelays[0] = 1;
          pistonDelayStarts[0] = millis();
        }
      }
    }
    if(pistonExtends[1] == 1) {
      //Check hits
      if(digitalRead(button2) == LOW) {
        score += 1;
        Serial.print(score);
        Serial.print("\n");
        pistonExtends[1] = 0;
        updatePistons();
        pistonDelays[1] = 1;
        pistonDelayStarts[1] = millis();
      } else {
      //Check drop down
      if((millis() - pistonStarts[1]) > pistonLife * 1000) {
        pistonExtends[1] = 0;
        updatePistons();
        pistonDelays[1] = 1;
        pistonDelayStarts[1] = millis();
      }
    }
  }
  if(pistonExtends[2] == 1) {
    //Check hits
    if(digitalRead(button3) == LOW) {
      score += 1;
      Serial.print(score);
      Serial.print("\n");
      pistonExtends[2] = 0;
      updatePistons();
      pistonDelays[2] = 1;
      pistonDelayStarts[2] = millis();
    } else {
      //Check drop down
      if((millis() - pistonStarts[2]) > pistonLife * 1000) {
        pistonExtends[2] = 0;
        updatePistons();
        pistonDelays[2] = 1;
        pistonDelayStarts[2] = millis();
      }
    }
  }

  //put down pistons that have been up for long enough
  if(pistonDelays[0] == 1) {
    if((millis() - pistonDelayStarts[0]) > delayTime * 1000) {
      pistonDelays[0] = 0;
      numUp -=1;
    }
  }
  if(pistonDelays[1] == 1) {
    if((millis() - pistonDelayStarts[1]) > delayTime * 1000) {
      pistonDelays[1] = 0;
      numUp -=1;
    }
  }
  if(pistonDelays[2] == 1) {
    if((millis() - pistonDelayStarts[2]) > delayTime * 1000) {
      pistonDelays[2] = 0;
      numUp -=1;
    }
  }
  } else {
    //checks for game start
    digitalWrite(LED_BUILTIN, LOW);

    if(digitalRead(buttonStart) == LOW) {
      delay(2000);
      startGame();
    }
  }
}
