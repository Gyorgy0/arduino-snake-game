#define XPIN A1
#define YPIN A0

const int row[8] = {
  2, 7, 19, 5, 13, 18, 12, 16
};

const int col[8] = {
  6, 11, 10, 3, 17, 4, 8, 9
};

int frame[8][8];
int apple[2] = {random(8),random(8)};
int snake[63][2];
int newhead[2];
int rotation[4][2] = {
           {+1, 0}, // down
					 {0, +1},	// right
					 {-1, 0},	// up
					 {0, -1}};// left

int direction = 0;
int length = 1;
int serialinput;
bool pause = false;
bool start = false;
unsigned long previousMillis = 0UL;
unsigned long speed = 500UL;

void turn() {
  int vrx = analogRead(XPIN);
  int vry = analogRead(YPIN);
  if (direction == 0) {
    if (vry < 100) {
      direction = 1;
    }
    if (vry > 920) {
      direction = 3;
    }
  }
  if (direction == 1) {
    if (vrx > 920) {
      direction = 0;
    }
    if (vrx < 100) {
      direction = 2;
    }
  }
  if (direction == 2) {
    if (vry < 100) {
      direction = 1;
    }
    if (vry > 920) {
      direction = 3;
    }
  }
  if (direction == 3) {
    if (vrx < 100) {
      direction = 2;
    }
    if (vrx > 920) {
      direction = 0;
    }
  }
}

void snakeMovement() {
  for (int i = 0; i < length; i++) {
    snake[length-1][0] = newhead[0];
    snake[length-1][1] = newhead[1];
    snake[i-1][0] = snake[i][0];
    snake[i-1][1] = snake[i][1];
    if (i == 0) {
      newhead[0] += rotation[direction][0];
      newhead[1] += rotation[direction][1];
      for (int k = 0; k < length; k++) {
        if (newhead[0] == snake[k][0] && newhead[1] == snake[k][1]) {
          start = true;
          pause = true;
          setup();
        }
      }
    }
    if (newhead[0]==8) {
      newhead[0] = 0;
    }
    if (newhead[0]==-1) {
      newhead[0] = 7;
    }
    if (newhead[1]==8) {
      newhead[1] = 0;
    }
    if (newhead[1]==-1) {
      newhead[1] = 7;
    }
    if ( i == 0 && (newhead[0] == apple[0] && newhead[1] == apple[1])) {
      length++;
      snake[length-1][0] = newhead[0];
      snake[length-1][1] = newhead[1];
      snake[i-1][0] = snake[i][0];
      snake[i-1][1] = snake[i][1];
      apple[0] = random(8);
      apple[1] = random(8);
      frame[apple[0]][apple[1]] = 0;
    }
    if (length == 1) {
      frame[newhead[0]][newhead[1]] = 0;
    }
    else {
      frame[snake[i][0]][snake[i][1]] = 0;
    }
  }
  Serial.println(length);
}

void setup() {
  Serial.begin(9600);
  for (int thisPin = 0; thisPin < 8; thisPin++) {
    pinMode(col[thisPin], OUTPUT);
    pinMode(row[thisPin], OUTPUT);
  }
  for (int i = 0; i < length; i++) {
    snake[i][0] = 0;
    snake[i][1] = 0;
    clear();
  }
  clear();
  pause = false;
  apple[0] = random(8);
  apple[1] = random(8);
  newhead[0] = random(8);
  newhead[1] = random(8);
  setupMatrix();
  length = 1;
  direction = random(4);
}

void clear() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (i == apple[0] && j == apple[1]) {
          frame[i][j] = 0;
      }
      else {
        frame[i][j] = 1;
      }
    }
  }
}

void loop() {
  int vrx = analogRead(XPIN);
  int vry = analogRead(YPIN);
  if(Serial.available())
   {
      char serialinput = Serial.read();
      if (serialinput == 1) { 
        start = true;
      }
      if (serialinput == 2) { 
        pause = true; 
      }
      if (serialinput == 3) { 
        pause = false;
        start = true; 
      }
   }
   if (start && !pause) {
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > speed) {
      clear();
      snakeMovement();
      previousMillis = currentMillis;
    }
    turn();
   }
  displayLedPattern();
}

void displayLedPattern(){
  for (int thisRow = 0; thisRow < 8; thisRow++) {
    digitalWrite(row[thisRow], HIGH);
    for (int thisCol = 0; thisCol < 8; thisCol++) {
      int thisPixel = frame[thisRow][thisCol];
      digitalWrite(col[thisCol], thisPixel);
      if (thisPixel == LOW) {
        digitalWrite(col[thisCol], HIGH);
      }
    }
    digitalWrite(row[thisRow], LOW);
  }
}

void setupMatrix(){
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      if (frame[x][y] = 0){
        frame[x][y] = LOW;
      }
      if (frame[x][y] = 1) {
        frame[x][y] = HIGH;
      }
    }
  }
}