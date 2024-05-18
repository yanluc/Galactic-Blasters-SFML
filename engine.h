#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include<vector>
// #include<format>
#define DESTROYED 2
#define ALIVE 1
#define EMPTY 0
class Engine
{
    public:
    static sf::Clock clock;
    static int grid[5][8];
    static int highscore, score, enemies_left, enemies_to_spawn;
    double best_time;
    static sf::SoundBuffer explo_sound;
    static sf::SoundBuffer bomb_explo_sound;
    static sf::Texture background_texture;
    static sf::Texture cannon_texture;
    static sf::Texture alien_texture;
    static sf::Texture missile_texture;
    static sf::Texture bomb_texture;
    static int width, height;
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