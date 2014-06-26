#ifndef SnakeGame_h
#define SnakeGame_h

// Arduino versioning.
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#define MAXLEN 15
// PIN mode
#define PinUP 4
#define PinDOWN 5
#define PinLEFT 6
#define PinRIGHT 7

// Game Status
#define INITIAL 1
#define START   2
// Game Screen Size
#define ShowW 160
#define ShowH 128
#define PADDING 10
// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8  

#include <TFT.h>
#include <SPI.h>


class Game
{
    private:
        /*snake variable*/
        int foodx;
        int foody; 
        int posx[MAXLEN];
        int posy[MAXLEN];
        int n;
        int selected;
        int gameState;
        int myWidth;
        int myHeight;
        int type;
    public:
        Game(int,int);
        void initial(void);
        void test1111(void);
        int calculateScore(int val,int count);
        bool checkBoundary(int x,int y);
        void changeDirection(int &direx, int &direy,int &Move);
        bool checkball(int ballx,int bally);
        bool eatself;
        void reset();
        
        void DrawingScore(int val,TFT&);
        void DrawingBoundary(TFT&);
        void SnakePlay(int&,TFT&);
        void SnakeMenu(int&,TFT&);
        void DrawingSnakeMenu(int selected,TFT&);
        void DrawingSnakeInitial(TFT&);
        void DrawingEnd(TFT&);
        void snake(TFT&);
        void genBall(TFT&);

};

#endif
