#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include<vector>
#include"objects.h"
// #include<format>
#define DESTROYED 2
#define ALIVE 1
#define EMPTY 0
class Engine
{
    public:
    static int highscore, score, enemies_left, enemies_to_spawn;
    double best_time;
    sf::RenderWindow window_;
    Engine();
    ~Engine();
    void LoadTextures();
    void LoadSounds();
    std::vector<std::pair<double,int>> Leaderboard(char const *filename);
    void InitWindow();
    void StartMenu();
    void RunGame();
    void GameLoop();

};