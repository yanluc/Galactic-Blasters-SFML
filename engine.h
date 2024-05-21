#pragma once
#include"objects.h"
// #include<format>
class Engine
{
    public:
    static int highscore, score, enemies_to_spawn;
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
    bool GameLoop(Cannon &cannon, std::vector<Alien*> &aliens, std::vector<AlienMunition*> &AlienMunitions, std::vector<Missile*> &missiles,sf::Time &elapsed);
    void DrawObjects(Cannon &cannon, std::vector<Alien*> &aliens, std::vector<AlienMunition*> &AlienMunitions, std::vector<Missile*> &missiles);
    void DrawGameElements();
    void Update(Cannon &cannon, std::vector<Alien*> &aliens, std::vector<AlienMunition*> &AlienMunitions, std::vector<Missile*> &missiles, sf::Time &frametime);
    void Collisions(Cannon &cannon, std::vector<Alien*> &aliens, std::vector<AlienMunition*> &AlienMunitions, std::vector<Missile*> &missiles, sf::Time &frametime);
    void Spawn(std::vector<Alien*> &aliens, std::vector<AlienMunition*> &AlienMunitions, std::vector<Missile*> &missiles);
    void GameOver();
    void GameWon();
};