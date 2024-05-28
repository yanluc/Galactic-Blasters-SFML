#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<vector>
#include<string>
#include"engine.h"

int main()
{
    Engine engine;
    engine.LoadTextures();
    engine.LoadSounds();
    engine.InitWindow();
    engine.StartMenu();
    engine.RunGame();
    
    return 0;
}

/*
copyright:
    background images:
    https://www.freepik.com/free-vector/abstract-glowing-geometric-lines-background-design_136142409.htm
    https://www.freepik.com/free-vector/urban-city-background-video-conferencing_10137813.htm by freepik on freepik.com
    https://www.freepik.com/free-vector/explore-beauty-night-space-outdoor-events_39455975.htm by starline on freepik.com
    alien spaceship image:
    https://www.freepik.com/free-vector/sticker-template-with-unidentified-flying-object-ufo-isolated_16254210.htm by brgfx on freepik.com
    explosion:
    https://toppng.com/show_download/163425/explosion-sprite-png-2d-explosion-sprite-sheet
    
    Power ups and missile:
    https://craftpix.net/freebies/free-space-shooter-game-objects
*/