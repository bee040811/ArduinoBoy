/*

   TFT Pong

   This example for the Arduino screen reads the values
   of 2 potentiometers to move a rectangular platform
   on the x and y axes. The platform can intersect
   with a ball causing it to bounce.

   This example code is in the public domain.

   Created by Tom Igoe December 2012
   Modified 15 April 2013 by Scott Fitzgerald

http://arduino.cc/en/Tutorial/TFTPong

 */

#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8  
#define PinUP 4
#define PinDOWN 5
#define PinLEFT 6
#define PinRIGHT 7

#define PPGAME 3
#define SNAKE 2
#define MAXLEN 15


// pin definition for the Leonardo
// #define cs   7
// #define dc   0
// #define rst  1

TFT TFTscreen = TFT(cs, dc, rst);

// variables for the position of the ball and paddle
int paddleX = 0;
int paddleY = 0;
int oldPaddleX, oldPaddleY;
int ballDirectionX = 1;
int ballDirectionY = 1;
char* gameName[] = {"game1","game2","game3"};

int ballSpeed = 10; // lower numbers are faster

int ballX, ballY, oldBallX, oldBallY;
/*snake variable*/
int snakelen = 10;
int Move;
int direx;
int direy;
int currentx = 25;
int currenty = 25;
int lastx = 0;
int lasty = 0;
int foodx;
int foody; 
int posx[MAXLEN];
int posy[MAXLEN];
int n=0;

// define ArduinoBoy(1) => menu state(2) => game(3) 

int state = 1;

int menuOrder = -1;

int game = 2;
int selected = 1;

void setup() {
    // Serial screen
    Serial.begin(9600);
    // initialize the display
    TFTscreen.begin();
    // black background
    TFTscreen.background(0,0,0);
    pinMode(PinLEFT,INPUT);
    pinMode(PinRIGHT,INPUT);
    pinMode(PinUP,INPUT);
    pinMode(PinDOWN,INPUT);
    for (int i=0;i < MAXLEN;i++){
      posx[i] = 0;
      posy[i] = 0;
    }
}

void loop() {
    switch(state) {
        case 1:
            // ArduinoBoy
            welcome(10);
            state = 3;
            break;
        case 2:
            //menu        
            //menu();
            break;
        case 3:
            if(game == PPGAME)
            PPGame();
            else if(game ==SNAKE)
            snake();
            break;
        default:
            break;    
    }    
}

void welcome(int time) {
  // black background
  TFTscreen.background(0,0,0);
   // set the font color to white
  TFTscreen.stroke(255,25,255);
  // set the font size
  TFTscreen.setTextSize(2);
  // write the text to the top left corner of the screen
  TFTscreen.text("Welcome\n ArduinoBoy",0,0);
  delay(time);
  TFTscreen.background(0,0,0);
  
}

void menu() {
    if(digitalRead(PinUP) == 0) {
        selected -=1;
    } else if(digitalRead(PinDOWN) == 0){
        selected +=1;
    }
    if(selected > 3) {
        selected = 3;
    } else if(selected < 1) {
        selected = 1;
    }
    
    DrawingMenu(selected);
    Serial.println(digitalRead(PinDOWN));
    delay(200);
    int key = digitalRead(PinLEFT);
    //Serial.println(digitalRead(PinBtn));
    if(key == 0) {
        state = 3;
        game = SNAKE;
        TFTscreen.background(0,0,0);
    }
}

void DrawingMenu(int selected){

    // set the fill color to black and erase the previous
    // position of the paddle if different from present
    int x = 55;
    int y = -30;
    
    for(int i = 1 ; i <= 3 ; i++) {
        int nowy = y + i * 40;
        // draw the paddle on screen, save the current position
        // as the previous.
        if(selected == i ) {
            TFTscreen.fill(255,0,0);
            TFTscreen.stroke(255,255,255);
        } else {
            TFTscreen.fill(255,255,255);
            TFTscreen.stroke(0,0,0);
        }
        
        TFTscreen.rect(x, nowy, 60, 30);
        TFTscreen.setTextSize(1);
        TFTscreen.text(gameName[i-1],x+15,nowy+10);
 
   }
}
void snake()
{
  TFTscreen.background(0,0,0);
  
  
  while (true){
    changeDirection();
    if(n < snakelen){      
      posx[n] = currentx;
      posy[n] = currenty;
      n += 1;
    }else{
      posx[n] = currentx;
      posy[n] = currenty;
    }
    
    currentx += direx;
    currenty += direy;
    TFTscreen.stroke(255,255,255);
    TFTscreen.rect(currentx,currenty,5,5);
    
    
    if(n == snakelen){
      lastx = posx[0];
      lasty = posy[0];
      for(int i = 1;i <= n;i++){
        posx[i-1] = posx[i];
        posy[i-1] = posy[i];
      }
      TFTscreen.stroke(0,0,0);
      TFTscreen.rect(lastx,lasty,5,5);
    }

    delay(200);
  }
}

void changeDirection()
{
  if(digitalRead(PinUP) == 0)
    Move = 1;
  else if(digitalRead(PinDOWN) == 0)
    Move = 2;
  else if(digitalRead(PinLEFT) == 0)
    Move = 3;
  else if(digitalRead(PinRIGHT) == 0)
    Move = 4;
    switch(Move){
      case 1://up
        direy = 1;
        direx = 0;
        break;
      case 2://down
        direy = -1;
        direx = 0;
        break;
      case 3://right
        direx = 1;
        direy = 0;
        break;
      case 4://left
        direx = -1;
        direy = 0;
        break;
    }
}

void PPGame() {
    // save the width and height of the screen
//    TFTscreen.background(0,0,0);
    TFTscreen.stroke(0,0,0);
    int myWidth = TFTscreen.width();
    int myHeight = TFTscreen.height();
    // map the paddle's location to the position of the potentiometers  
    paddleX = map(analogRead(A0), 0, 1024, 0, myWidth) - 20/2;
    paddleY = map(450, 0, 512, 0, myHeight) - 5/2;
    // set the fill color to black and erase the previous
    // position of the paddle if different from present
    TFTscreen.fill(0,0,0);

    if (oldPaddleX != paddleX || oldPaddleY != paddleY) {
        TFTscreen.rect(oldPaddleX, oldPaddleY, 20, 5);
    }

    // draw the paddle on screen, save the current position
    // as the previous.
    TFTscreen.fill(255,255,255);

    TFTscreen.rect(paddleX, paddleY, 20, 5);
    oldPaddleX = paddleX;
    oldPaddleY = paddleY;

    // update the ball's position and draw it on screen
    if (millis() % ballSpeed < 2) {
        moveBall();
    }
}

// this function determines the ball's position on screen
void moveBall() {
    // if the ball goes offscreen, reverse the direction:
    if (ballX > TFTscreen.width() || ballX < 0) {
        ballDirectionX = -ballDirectionX;
    }

    if (ballY > TFTscreen.height() || ballY < 0) {
        ballDirectionY = -ballDirectionY;
    }  

    // check if the ball and the paddle occupy the same space on screen
    if (inPaddle(ballX, ballY, paddleX, paddleY, 20, 5)) {
        ballDirectionX = -ballDirectionX;
        ballDirectionY = -ballDirectionY;
    }

    // update the ball's position
    ballX += ballDirectionX;
    ballY += ballDirectionY;

    // erase the ball's previous position
    TFTscreen.fill(0,0,0);

    if (oldBallX != ballX || oldBallY != ballY) {
        TFTscreen.rect(oldBallX, oldBallY, 5, 5);
    }


    // draw the ball's current position
    TFTscreen.fill(255,255,255);
    TFTscreen.rect(ballX, ballY, 5, 5);

    oldBallX = ballX;
    oldBallY = ballY;

}

// this function checks the position of the ball
// to see if it intersects with the paddle
boolean inPaddle(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight) {
    boolean result = false;

    if ((x >= rectX && x <= (rectX + rectWidth)) &&
            (y >= rectY && y <= (rectY + rectHeight))) {
        result = true;
    }

    return result;  
}
