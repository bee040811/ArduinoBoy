
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
#include <PingPongGame.h>


// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8  


// Game List
#define PPGAME 1
#define SNAKE 2

TFT TFTscreen = TFT(cs, dc, rst);
int myWidth = TFTscreen.width();
int myHeight = TFTscreen.height();

Game snake_game = Game(myWidth,myHeight);
PP PingPGame = PP(myWidth,myHeight);

char* gameName[] = {"Ping Pong","Snake","game3"};

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
                TFTscreen.background(100,20,50);
                menu(INITIAL);
                type = 1;
            } else {
                menu(START);
            }
            break;
        case 3:
            if(game == PPGAME) {
                PingPGame.PPGame(state, TFTscreen);
            } else if(game == SNAKE) {
                snake_game.SnakePlay(state, TFTscreen);
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
    if(selected > 2) {
        selected = 2;
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
    int x = 50;
    int y = -30;
    TFTscreen.stroke(255,255,255);
    TFTscreen.setTextSize(2);
    TFTscreen.text("Arduino Boy",15,15);

    for(int i = 1 ; i <= 2 ; i++) {
        int nowy = y + (i+1) * 40;
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
        TFTscreen.text(gameName[i-1],x+5,nowy+12);

    }
}

void reset() {
    // reset
    selected = 1;
    type = 0;
}

