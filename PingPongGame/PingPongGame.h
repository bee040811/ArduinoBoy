#ifndef PingPongGame_h
#define PingPongGame_h

// Arduino versioning.
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <TFT.h>

// PIN mode
#define PinUP 4
#define PinDOWN 5
#define PinLEFT 6
#define PinRIGHT 7

class PP
{
    private:
        // variables for the position of the ball and paddle
        int paddleX;
        int paddleY;
        int oldPaddleX, oldPaddleY;
        int ballDirectionX;
        int ballDirectionY;
        int ballSpeed; // lower numbers are faster
        int ballX, ballY, oldBallX, oldBallY;
        int myHeight;
        int myWidth;
    public:
        PP(int,int);
        void PPGame(int&,TFT TFTscreen);
        void moveBall(int& ,TFT TFTscreen);
        bool checkEnd(int);
        boolean inPaddle(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight);
        void reset();
        void genBrick(TFT TFTscreen);
};
#endif
