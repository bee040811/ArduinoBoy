#include "PingPongGame.h"

PP::PP(int w,int h) {
    // variables for the position of the ball and paddle
    paddleX = w/2;
    paddleY = h-20;
    oldPaddleX = -1;
    oldPaddleY = -1;
    ballDirectionX = 1;
    ballDirectionY = 1;
    ballSpeed = 20; // lower numbers are faster
    myWidth = w;
    myHeight = h;
    Serial.begin(9600);
}

void PP::PPGame(int& state, TFT TFTscreen) {

    // save the width and height of the screen
    //    TFTscreen.background(0,0,0);
    reset();
    
    ballX = 5;
    ballY = 15;

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
            if(shiftx < 950) {
                shiftx+=1;
            } else {
                shiftx = 950;
            }
            change = true;
        } else if(digitalRead(PinRIGHT)==0) {
            if(shiftx > 70) {
                shiftx-=1;
            } else {
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
            moveBall(state,TFTscreen);
            if(checkEnd(ballY)) {
                state = 2;
                break;
            }
        }
    }
}

// this function determines the ball's position on screen
void PP::moveBall(int& state,TFT TFTscreen) {
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
        if(ballX < paddleX + 10) {
            ballDirectionX = -1;
        } else {
            ballDirectionX = 1;
        }

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
boolean PP::inPaddle(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight) {
    boolean result = false;

    if ((x >= rectX && x <= (rectX + rectWidth)) &&
            (y >= rectY && y <= (rectY + rectHeight))) {
        result = true;
    }

    return result;  
}

bool PP::checkEnd(int y) {
    if( (y > myHeight-5) && (y < myHeight) ) {
        return true;
    }
    return false;
}

void PP::reset() {
    paddleX = myWidth/2;
    paddleY = myHeight-20;
    oldPaddleX = -1;
    oldPaddleY = -1;
    ballDirectionX = 1;
    ballDirectionY = 1;
}