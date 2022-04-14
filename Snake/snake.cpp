//snake, builds on movingsprites
//also apparently classes need to be before the place they're called lol

#include "raylib.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


int main(void) { 
    //declare vars

        //screen resolution
            int screenwidth = 600;
            int screenheight = 400;
        
        //bounding box data
            int boundingBoxWidth = 400; //bounding box data
            int boundingBoxHeight = 400; 

        //framerate
            SetTargetFPS(30); //helps with limiting speed

        //playerdata
            //coords of the head
            int headposx = boundingBoxWidth/2;
            int headposy = boundingBoxHeight/2;
            //direction
            int headdir = 4; //headdir 1 = left, 2 = up, 3 = right, 4 = down
            //body
            int snake[400]; //stored as 1xy where 1 is there to add spacing
            int length = 1; //length of the snake
            int segmentx, segmenty; //segment xys
            int lasthead;

        //gamedata
            int applex, appley;
            bool isAppleUsed = true; //check if apple is eaten and a new one should be generated
            

        //other
            //game states
            int frametime = 0;  //limits the movments over time while maintaining a high fps
            bool lose = false; //used to check if game is over and updates should occur
            bool gameFinished = false;

        //experimental
            int nextdir; //next direction to be compared to current direction
            int keypressed; //used for keypress handling

            //high scores var
                int topScore = 0;
                string topScoreString;

                /*
                //high scores file
                    //make a file
                    ofstream highscore;
                    highscore.open("highscores.txt");
                    

                    
                    ifstream highscores; //make file read/writable
                    highscores.open("highscores.txt", ios::in | ios::out); //create .txt for storing int values
                    
                    //test value
                    highscore << 5;

                    
                    //load last value from highscore to topscore
                    getline(highscores,topScoreString);
                    cout << topScoreString << "\n";
                    */
                    
                    
                    

        //create window
        InitWindow(screenwidth, screenheight, "raylib - snake");

    //create main loop
    while (!WindowShouldClose()) {
        //get current keypress if available
            
            //DEPRECATED OLD SMELLY KEYPRESS HANDLING
            //move in direction of the last keypress if valid (2 linked to 4, 1 linked to 3)
          /*  if (IsKeyPressed(KEY_W))
                if (headdir != 2)
                headdir = 4;
            if (IsKeyPressed(KEY_A))
                if (headdir != 3)
                headdir = 1;
            if (IsKeyPressed(KEY_S))
                if (headdir != 4)
                headdir = 2;
            if (IsKeyPressed(KEY_D))
                if (headdir != 1)
                headdir = 3;    */
        
            //experimental keypress handling
            //can only take one key if valid
            keypressed = GetKeyPressed();
                //convert keypress to a direction if valid
                switch (keypressed) {
                    case (KEY_W):
                    if (headdir != 2)
                    nextdir = 4;
                    break;

                    case (KEY_A):
                    if (headdir != 3)
                    nextdir = 1;
                    break;

                    case (KEY_S):
                    if (headdir != 4)
                    nextdir = 2;
                    break;
                    
                    case (KEY_D):
                    if (headdir != 1)
                    nextdir = 3;
                    break;
                }
             
        

        //update positions (every 7th frame)
        frametime++;
        if (frametime % 7 == 0) {
            //take last headpos for later
            lasthead = 1000000 + (headposx*100) + headposy;

            if (lose == false) {     //check if anything should be updated
            
            headdir = nextdir;  //update actual direction before position updates
            
            //update head position
            switch (headdir)
            {
                case (1):
                headposx -= 20; //decrements if direction left
                break;
        
                case (2):
                headposy += 20; //increments if up
                break;

                case (3):
                headposx += 20; //increments if right
                break;

                 default:
                headposy -= 20; //decrements if down
                 break;
            }
            
            //body updates
                for (int i = length; i > 0; i--) {
                    
                    snake[i] = snake[i - 1]; 
                    //cout << snake[i] << " length = " << length << " Segment = " << i << "\n"; //debug
                    snake[0] = lasthead;  //1000000 + (headposx*100) + headposy; //new head position
                    //cout << snake[0] << "\n"; //debug
                }
            }
        }

        //apple handling
            //create random value that's not within the head or body already
            if (isAppleUsed == true) {
                applex = 20 * (rand() % 20);
                appley = 20 * (rand() % 20);
                isAppleUsed = false;
            }

        //check if outside bounding box or if collided
            //bounding box collision
            if ((headposx > boundingBoxWidth) || (headposx < 0) || (headposy > boundingBoxHeight) || (headposy < 0))
                lose = true;

            //check snake collision
            for (int i = length - 1; i > 1; i--) {
                segmenty = (snake[i] - 1000000) % 1000;
                segmentx = ((snake[i] - 1000000) - segmenty) / 100;
                if (segmentx == headposx && segmenty == headposy) {
                    lose = true;
                }
            }
            
            //check apple collision
            if (headposx == applex && headposy == appley) {
                isAppleUsed = true;
                length++;
            }

            //highscore handling
                //if current score is higher than the last load it to the .txt
                //if (length > topScore) {
                //highscore << to_string(length);
                
                
                //}
                



        //render
        BeginDrawing();
            //clr screen
            ClearBackground(LIGHTGRAY);
            
            //draw body
            if (length == 2) { //fixes a rendering issue when lengths are = 2
                DrawRectangle((((lasthead - 1000000) - ((lasthead - 1000000) % 1000)) / 100), (lasthead - 1000000) % 1000, 20, 20, LIME);
                DrawRectangleLines((((lasthead - 1000000) - ((lasthead - 1000000) % 1000)) / 100), (lasthead - 1000000) % 1000, 20, 20, DARKGREEN);
            }
            else {
                for (int i = length - 1; i > 0; i--) { //starts from the last length and grabs the pos of the segment in front of it
                    segmenty = (snake[i] - 1000000) % 1000; //isolate y
                    segmentx = ((snake[i] - 1000000) - segmenty) / 100; //isolate x

                    //debug
                    //cout << segmentx << " = segmentx\n" << segmenty << " = segmenty\n";
                
                    DrawRectangle(segmentx, segmenty, 20, 20, LIME);
                    DrawRectangleLines(segmentx, segmenty, 20, 20, DARKGREEN);
                }
            }

            // draw head
            DrawRectangle(headposx, headposy, 20, 20, LIME);
            DrawRectangleLines(headposx, headposy, 20, 20, DARKGREEN);

            //draw apple
            DrawRectangle(applex, appley, 20, 20, RED);

            //check if you lost window should be displayed
            if (lose == true) {
                DrawRectangle((screenwidth/2) - 50, (screenheight/2) - 50, 100, 100, WHITE);
                DrawRectangleLines((screenwidth/2) - 50, (screenheight/2) - 50, 100, 100, BLACK);
                DrawText("You lost", (screenwidth/2) - 40, (screenheight/2) - 40, 20, BLACK);
                
                DrawText(TextFormat("Score \n %i", length), (screenwidth/2) - 40, (screenheight/2) - 10, 20, BLACK);
            }

            //draw info pannel
                DrawRectangle(boundingBoxWidth, 0, 200, 400, DARKGRAY);
                //draw logo
                    DrawText("SNAKE", 440, 20, 40, WHITE);
                    DrawText("SNAKE", 441, 21, 40, RED);
                //draw score
                    DrawText(TextFormat("SCORE: %i", length), 460, 60, 20, WHITE);
                //draw topscore
                    
                    //DrawText("HIGHSCORE: ", 410, 80, 20, WHITE);
                //draw instructions
                    DrawText("Press WASD to move \n  press ESC to exit", 450, 300, 10, WHITE);

        EndDrawing();

        
    }

    //exit
    CloseWindow();
    //highscores.close();
    return 0;
}
