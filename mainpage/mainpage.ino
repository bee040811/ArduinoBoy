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
// PIN mode
#define PinUP 4
#define PinDOWN 5
#define PinLEFT 6
#define PinRIGHT 7

// Game List
#define PPGAME 3
#define SNAKE  1

// Game Status
#define INITIAL 1
#define START   2
// pin definition for the Leonardo
// #define cs   7
// #define dc   0
// #define rst  1

TFT TFTscreen = TFT(cs, dc, rst);
int myWidth = TFTscreen.width();
int myHeight = TFTscreen.height();

// variables for the position of the ball and paddle
int paddleX = 0;
int paddleY = 0;
int oldPaddleX, oldPaddleY;
int ballDirectionX = 1;
int ballDirectionY = 1;
char* gameName[] = {"game1","game2","game3"};

int ballSpeed = 10; // lower numbers are faster

int ballX, ballY, oldBallX, oldBallY;

// define ArduinoBoy(1) => menu state(2) => game(3) 

int state = 1;
int gameState = 1;

int menuOrder = -1;

int game = 0;
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
  DrawingBoundary();  
  /*
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
                SnakeGame();
            }
            break;
        default:
            break;    
    }
*/
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

void menu(int status) {
    
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
    if(change || status == INITIAL) {
        DrawingMenu(selected);
    } 
    delay(100);
    int key = digitalRead(PinLEFT);
    //Serial.println(digitalRead(PinBtn));
    if(key == 0) {
        state = 3;
        
        reset();
        
        game = selected;
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

void PPGame() {
    // save the width and height of the screen
    //    TFTscreen.background(0,0,0);
    TFTscreen.stroke(0,0,0);
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

void DrawingSnakeInitial() {
    // set the font color to white
    TFTscreen.stroke(255,192,203);
    // set the font size
    TFTscreen.setTextSize(2);
    // write the text to the top left corner of the screen
    TFTscreen.text("SNAKE",55,20);
    // erase the ball's previous position
    DrawingSnakeMenu(1);    
}

void DrawingSnakeMenu(int selected) {
    int y = 50;
    for(int i = 1 ; i <= 2; i++) {
        y = y + (i-1) * 25 ;

        if(i == selected) {
            TFTscreen.fill(255,0,0);
            TFTscreen.stroke(255,255,255);
        } else {
            TFTscreen.fill(255,255,255);
            TFTscreen.stroke(0,0,0);
        }

        // set the font size
        TFTscreen.setTextSize(1);
        // write the text to the top left corner of the screen
        TFTscreen.rect(65, y, 40, 20);

        if(i == 1) {
            TFTscreen.text("START",70,y + 7);
        } else {
            TFTscreen.text("LEAVE",70,y + 7);
        }

    }
}

void SnakeMenu() {
    bool change = false;
    if(digitalRead(PinUP) == 0) {
        selected -=1;
        change = true;
    } else if(digitalRead(PinDOWN) == 0){
        selected +=1;
        change = true;
    }
    if(selected > 2) {
        selected = 2;
    } else if(selected < 1) {
        selected = 1;
    }
    if(change) {
        DrawingSnakeMenu(selected);
    }
    if(digitalRead(PinLEFT) == 0 ) {
        if(selected == 1) {
            gameState = START;
            // black background
            TFTscreen.background(0,0,0);
        } else if(selected == 2) {
            reset();
            state -= 1;
            gameState = INITIAL;
        }
    }
}

void reset() {
    // reset
    selected = 1;
    type = 0;
}

void SnakeGame() {
    if(type == 0) {
        TFTscreen.background(58,135,173);
        DrawingSnakeInitial();
        type = 1;
    } else {
        switch(gameState){
            case INITIAL:
                SnakeMenu();        
                break;
            case START:
                break;
            default:
                break;
        }

    }
}

void DrawingBoundary() {
        
    TFTscreen.fill(0,200,255);
    TFTscreen.rect(5, 20, 150, 2);
    TFTscreen.rect(5, 20, 2, 98);
    TFTscreen.rect(myWidth-7, 20, 2, 98);
    TFTscreen.rect(5, myHeight-12, 150, 2);

    // set the font color to white
    TFTscreen.stroke(255,255,255);
    // set the font size
    TFTscreen.setTextSize(1);
    TFTscreen.text("Score",5,10);
    
    delay(2000);
}

bool checkBoundary(int x,int y) {
    if( x > 5 && x < myWidth - 7 && y > 20 && y < myHeight-12 ) {
        return true;
    }
    return false
}
