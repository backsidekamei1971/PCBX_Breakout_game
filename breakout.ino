#include <Wire.h>

#include <Adafruit_GFX.h>

#include <Adafruit_SSD1306.h>



#define SCREEN_WIDTH 128

#define SCREEN_HEIGHT 64

#define OLED_RESET -1

#define PADDLE_WIDTH 20

#define PADDLE_HEIGHT 2

#define BALL_SIZE 2

#define BLOCK_ROWS 3

#define BLOCK_COLS 6

#define BLOCK_WIDTH 20

#define BLOCK_HEIGHT 6



// Button pin definitions

#define BTN_LEFT 2

#define BTN_RIGHT 3



Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



// Game object

struct {

 int paddleX;

 float ballX, ballY;

 float ballSpeedX, ballSpeedY;

 bool blocks[BLOCK_ROWS][BLOCK_COLS];

 int score;

 bool gameOver;

} game;



void setup() {

 // Initialize button pins

 pinMode(BTN_LEFT, INPUT_PULLUP);

 pinMode(BTN_RIGHT, INPUT_PULLUP);



 // Initialize display

 if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {

  for(;;); // If initialization fails, enter an infinite loop

 }

 display.clearDisplay();



 // Initialize game

 initGame();

}



void initGame() {

 // Initialize paddle position

 game.paddleX = (SCREEN_WIDTH - PADDLE_WIDTH) / 2;



 // Initialize ball position and velocity

 game.ballX = SCREEN_WIDTH / 2;

 game.ballY = SCREEN_HEIGHT - 10;

 game.ballSpeedX = 2;

 game.ballSpeedY = -2;



 // Initialize blocks

 for(int i = 0; i < BLOCK_ROWS; i++) {

  for(int j = 0; j < BLOCK_COLS; j++) {

   game.blocks[i][j] = true;

  }

 }



 game.score = 0;

 game.gameOver = false;

}



void updateGame() {

 // Read button inputs

 if(!digitalRead(BTN_LEFT) && game.paddleX > 0) {

  game.paddleX -= 4;

 }

 if(!digitalRead(BTN_RIGHT) && game.paddleX < SCREEN_WIDTH - PADDLE_WIDTH) {

  game.paddleX += 4;

 }



 if(!game.gameOver) {

  // Update ball position

  game.ballX += game.ballSpeedX;

  game.ballY += game.ballSpeedY;



  // Detect wall collisions

  if(game.ballX <= 0 || game.ballX >= SCREEN_WIDTH - BALL_SIZE) {

   game.ballSpeedX = -game.ballSpeedX;

  }

  if(game.ballY <= 0) {

   game.ballSpeedY = -game.ballSpeedY;

  }



  // Detect paddle collision

  if(game.ballY >= SCREEN_HEIGHT - PADDLE_HEIGHT - BALL_SIZE) {

   if(game.ballX >= game.paddleX && game.ballX <= game.paddleX + PADDLE_WIDTH) {

    game.ballSpeedY = -game.ballSpeedY;

    // Adjust the rebound angle based on the hit position

    float hitPosition = (game.ballX - game.paddleX) / PADDLE_WIDTH;

    game.ballSpeedX = (hitPosition - 0.5) * 4;

   }

  }



  // Game over detection

  if(game.ballY >= SCREEN_HEIGHT) {

   game.gameOver = true;

  }



  // Detect block collisions

  int blockX = game.ballX / BLOCK_WIDTH;

  int blockY = game.ballY / BLOCK_HEIGHT;



  if(blockX >= 0 && blockX < BLOCK_COLS && blockY >= 0 && blockY < BLOCK_ROWS) {

   if(game.blocks[blockY][blockX]) {

    game.blocks[blockY][blockX] = false;

    game.ballSpeedY = -game.ballSpeedY;

    game.score += 10;

   }

  }

 }

}



void drawGame() {

 display.clearDisplay();



 // Draw blocks

 for(int i = 0; i < BLOCK_ROWS; i++) {

  for(int j = 0; j < BLOCK_COLS; j++) {

   if(game.blocks[i][j]) {

    display.fillRect(j * BLOCK_WIDTH, i * BLOCK_HEIGHT, 

            BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1, WHITE);

   }

  }

 }



 // Draw paddle

 display.fillRect(game.paddleX, SCREEN_HEIGHT - PADDLE_HEIGHT, 

         PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);



 // Draw ball

 display.fillRect(game.ballX, game.ballY, BALL_SIZE, BALL_SIZE, WHITE);



 // Display score

 display.setTextSize(1);

 display.setTextColor(WHITE);

 display.setCursor(0, 0);

 display.print("Score:");

 display.print(game.score);



 // If game over, display game over message

 if(game.gameOver) {

  display.setTextSize(2);

  display.setCursor(10, 30);

  display.print("Game Over!");

 }



 display.display();

}



void loop() {

 updateGame();

 drawGame();

 delay(2); // Control game speed



 // If game over and both buttons are pressed, restart the game

 if(game.gameOver && !digitalRead(BTN_LEFT) && !digitalRead(BTN_RIGHT)) {

  delay(500); // Debounce

  initGame();

 }

}

