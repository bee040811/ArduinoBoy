
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

#include <TFT.h>
#include <SPI.h>
#include <SnakeGame.h>


// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8  


// Game List
#define PPGAME 3
#define SNAKE 2

TFT TFTscreen = TFT(cs, dc, rst);
int myWidth = TFTscreen.width();
int myHeight = TFTscreen.height();

Game snake_game = Game(myWidth,myHeight);

// variables for the position of the ball and paddle
int paddleX = myWidth/2;
int paddleY = myHeight-20;
int oldPaddleX=-1, oldPaddleY=-1;
int ballDirectionX = 1;
int ballDirectionY = 1;
char* gameName[] = {"game1","game2","game3"};

int ballSpeed = 20; // lower numbers are faster

int ballX, ballY, oldBallX, oldBallY;

// define ArduinoBoy(1) => menu state(2) => game(3) 

int state = 1;

int menuOrder = -1;

int game = -1;
int selected = 1;
int type = 0;

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

}

void loop() {
    //DrawingBoundary();  
    switch(state) {
        case 1:
            // ArduinoBoy
            welcome(10);
            state = 2;
            break;
        case 2:
            //menu
            if(type == 0) {
                // black background
                TFTscreen.background(0,0,0);
                menu(INITIAL);
                type = 1;
            } else {
                menu(START);
            }
            break;
        case 3:
            if(game == PPGAME) {
                PPGame();
            } else if(game == SNAKE) {
                snake_game.SnakePlay(state,TFTscreen);
            }
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


void numberPrint(int val,int x,int y) {
    switch(val) {
        case 1:
            TFTscreen.text("1",x,y);
            break;
        case 2:
            TFTscreen.text("2",x,y);
            break;
    }
}

void menu(int stat) {

    bool change = false;
    if(digitalRead(PinUP) == 0) {
        selected -=1;
        change = true;
    } else if(digitalRead(PinDOWN) == 0){
        selected +=1;
        change = true;
    }
    if(selected > 3) {
        selected = 3;
    } else if(selected < 1) {
        selected = 1;
    }
    if(change || stat == INITIAL) {
        DrawingMenu(selected);
    } 
    delay(100);
    int key = digitalRead(PinLEFT);
    //Serial.println(digitalRead(PinBtn));
    if(key == 0) {
        state = 3;
        game = selected;
        TFTscreen.background(0,0,0);
        reset();
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

void PPGame() {
    // save the width and height of the screen
    //    TFTscreen.background(0,0,0);
    TFTscreen.stroke(0,0,0);
    // map the paddle's location to the position of the potentiometers
    int shiftx = 512;
    bool change=false;
    TFTscreen.fill(255,255,255);  
    TFTscreen.rect(paddleX, paddleY, 20, 5);
    oldPaddleX = paddleX;
    oldPaddleY = paddleY;
    while(1){
      change = false;
      if(digitalRead(PinLEFT)==0)
      {
        if(shiftx < 950)
        {
          shiftx+=1;
        }
        else
        {
          shiftx = 950;
        }
        change = true;
      }else if(digitalRead(PinRIGHT)==0)
      {
        if(shiftx > 70)
        {
          shiftx-=1;
        }
        else
        {
          shiftx = 70;
        }
        change = true;
      }
      paddleX = map(shiftx, 0, 1024, 0, myWidth) - 20/2;
      paddleY = myHeight-20;
      // set the fill color to black and erase the previous
      // position of the paddle if different from present
      TFTscreen.fill(0,0,0);
  
      if ((oldPaddleX != paddleX || oldPaddleY != paddleY) && change) {
          TFTscreen.fill(0,0,0);
          TFTscreen.rect(oldPaddleX, oldPaddleY, 20, 5);
          TFTscreen.fill(255,255,255);  
          TFTscreen.rect(paddleX, paddleY, 20, 5);
          oldPaddleX = paddleX;
          oldPaddleY = paddleY;
      }
  
      // draw the paddle on screen, save the current position
      // as the previous.
      
  
      // update the ball's position and draw it on screen
      if (millis() % ballSpeed < 2) {
          moveBall();
      }
  }
}

// this function determines the ball's position on screen
void moveBall() {
    // if the ball goes offscreen, reverse the direction:
    bool in=false;
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
        ballX += ballDirectionX*5;
        ballY += ballDirectionY*5;
        in = true;
    }

    // update the ball's position
    ballX += ballDirectionX;
    ballY += ballDirectionY;
    // erase the ball's previous position
    

    if ((oldBallX != ballX || oldBallY != ballY) ) {
        TFTscreen.fill(0,0,0);
        TFTscreen.rect(oldBallX, oldBallY, 5, 5);
        
    }


    // draw the ball's current position
    TFTscreen.fill(255,255,255);
    TFTscreen.rect(ballX, ballY, 5, 5);

    // check if the ball and the paddle occupy the same space on screen
    if (in) {
        TFTscreen.fill(255,255,255);
        TFTscreen.rect(oldPaddleX, oldPaddleY, 20, 5);
    }

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




void reset() {
    // reset
    selected = 1;
    type = 0;
}



