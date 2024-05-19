#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<vector>
#include<string>
#include"engine.h"

int main()
{
    Engine engine;
    std::vector<GraphicalObject*> objects;
    engine.LoadTextures();
    engine.LoadSounds();
    engine.InitWindow();
    engine.StartMenu();
    engine.RunGame();
    
    return 0;
}

/*
copyright:
    backgrounf image:
    https://www.freepik.com/free-vector/explore-beauty-night-space-outdoor-events_39455975.htm by starline on freepik.com
    alien spaceship image:
    https://www.freepik.com/free-vector/sticker-template-with-unidentified-flying-object-ufo-isolated_16254210.htm by brgfx on freepik.com
*/