#include "Arduino.h"
#include "SnakeGame.h"

Game::Game(int w,int h){
    // Serial screen
    for (int i=0;i < MAXLEN;i++){
        posx[i] = 0;
        posy[i] = 0;
    }
    n = 0;
    selected = 1;
    myWidth = w;
    myHeight = h;
    gameState = INITIAL;
    type = 0;
    eatself = false;
    Serial.begin(9600);
}
void Game::test1111(void){
}
void Game::initial(void){
}
void Game::DrawingScore(int val,TFT& TFTscreen) {

    TFTscreen.fill(0,0,0);
    TFTscreen.stroke(0,0,0);
    TFTscreen.rect(35, 8, 60, 10);
    // set the font color to white
    TFTscreen.stroke(255,255,255);
    // set the font size
    TFTscreen.setTextSize(1);
    String b = String(val);
    const char *a = b.c_str();
    TFTscreen.text(a,40,10);
}
void Game::SnakePlay(int& state,TFT& TFTscreen) {
    if(type == 0) {
        TFTscreen.background(58,135,173);
        DrawingSnakeInitial(TFTscreen);
        type = 1;
    } else {
        switch(gameState){
            case INITIAL:
                SnakeMenu(state,TFTscreen);        
                break;
            case START:
                snake(TFTscreen);
                break;
            default:
                break;
        }

    }
}

void Game::DrawingBoundary(TFT& TFTscreen) {

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

}

int Game::calculateScore(int val,int count) {
    val += 5*count;
    return val;
}

bool Game::checkBoundary(int x,int y) {
    if( x >= 7 && x <= myWidth - 10 && y >= 22 && y <= myHeight-14 ) {
        return true;
    }
    return false;
}

void Game::SnakeMenu(int& state,TFT& TFTscreen) {
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
        DrawingSnakeMenu(selected,TFTscreen);
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

void Game::DrawingSnakeMenu(int selected,TFT& TFTscreen) {
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

void Game::DrawingSnakeInitial(TFT& TFTscreen) {
    // set the font color to white
    TFTscreen.stroke(255,192,203);
    // set the font size
    TFTscreen.setTextSize(2);
    // write the text to the top left corner of the screen
    TFTscreen.text("SNAKE",55,20);
    // erase the ball's previous position
    DrawingSnakeMenu(1,TFTscreen);    
}

void Game::DrawingEnd(TFT& TFTscreen) {
    // set the font color to white
    TFTscreen.stroke(125,192,203);
    // set the font size
    TFTscreen.setTextSize(2);
    // write the text to the top left corner of the screen
    TFTscreen.text("Game Over",myHeight/3,myWidth/3);
    // erase the ball's previous position
    delay(200);
}


void Game::snake(TFT& TFTscreen)
{
    int score = 0;
    int eatTimes = 0;
    int currentx = 25;
    int currenty = 25;
    int Move = 0;
    int direx=5;
    int direy=0;
    int snakelen = 3;
    int lastx = 0;
    int lasty = 0;

    TFTscreen.background(0,0,0);
    genBall(TFTscreen);
    DrawingBoundary(TFTscreen);
    DrawingScore(0,TFTscreen);
    n = 0;
    for (int i=0;i < MAXLEN;i++){
        posx[i] = 0;
        posy[i] = 0;
    }

    while (true){
        changeDirection(direx,direy,Move);
        if(n < snakelen){      
            posx[n] = currentx;
            posy[n] = currenty;
            n += 1;
        }else{
            posx[n-1] = currentx;
            posy[n-1] = currenty;
        }   
        currentx += direx;
        currenty += direy;
        for(int i=0;i<n;i++){
            if(currentx == posx[i] && currenty == posy[i]){    
                eatself = true;
            }
        }
        if(eatself){
            DrawingEnd(TFTscreen);
            delay(2000);
            gameState = INITIAL;
            reset();
            break;
        }
        if ( !checkBoundary(currentx, currenty)) {
            DrawingEnd(TFTscreen);
            delay(2000);
            gameState = INITIAL;
            reset();
            break;
        }

        if(currentx == foodx && currenty == foody){
            //TFTscreen.stroke(0,0,0);
            //TFTscreen.rect(foodx,foody,5,5);
            int prefoodx;
            int prefoody;
            prefoodx = foodx;
            prefoody = foody;
            TFTscreen.stroke(0,0,0);
            TFTscreen.fill(255,0,0);
            TFTscreen.rect(prefoodx,prefoody,5,5);
            genBall(TFTscreen);
            if(snakelen < 15)
            {
                snakelen+=1;
                score = calculateScore(score, ++eatTimes);
                DrawingScore(score,TFTscreen);
            }
        }
        TFTscreen.stroke(0,255,0);
        TFTscreen.fill(0,255,0);
        TFTscreen.rect(currentx,currenty,5,5);

        if(n == snakelen){
            lastx = posx[0];
            lasty = posy[0];
            for(int i = 1;i <= n;i++){
                posx[i-1] = posx[i];
                posy[i-1] = posy[i];
            }
            TFTscreen.stroke(0,0,0);
            TFTscreen.fill(0,0,0);
            TFTscreen.rect(lastx,lasty,5,5);
        }
        delay(100);
    }
}

void Game::changeDirection(int &direx, int &direy,int &Move)
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
            if(direy == 0){
                direy = -5;
                direx = 0;
            }      
            break;
        case 2://down
            if(direy == 0){
                direy = 5;
                direx = 0;
            }    
            break;
        case 3://right
            if(direx == 0){
                direx = -5;
                direy = 0;
            }
            break;
        case 4://left
            if(direx == 0){
                direx = 5;
                direy = 0;
            }
            break;
    }
}

void Game::genBall(TFT& TFTscreen)
{
    int mx,my;
    foodx = random(5 + 20,myWidth - 7 - 2);
    foody = random(20 + 20,myHeight - 14 - 2);
    mx = foodx/5;
    my = foody/5;
    foodx = mx * 5;
    foody = my * 5;
    if(!checkball(foodx,foody)){
        genBall(TFTscreen);
    }
    if(!checkBoundary(foodx,foody)) {
        genBall(TFTscreen);
    }
    TFTscreen.stroke(255,255,255);
    TFTscreen.rect(foodx,foody,5,5);
}

bool Game::checkball(int ballx,int bally)
{
    int x = 0;
    for(x = 0;x < MAXLEN;x++)
    {
        if(ballx == posx[x] && bally == posy[x])
        {
            return false;
        }else{
            return true;
        }

    }

}

void Game::reset() {
    // reset
    selected = 1;
    type = 0;
}
