#pragma once
#include "menu.hpp"
// #include<format>
class Engine
{
    public:
    static int highscore, enemies_to_spawn;
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
    void PauseGame(bool &p_down);
    bool GameLoop(Cannon &cannon, std::vector<Alien*> &aliens,std::vector<Wreckage*> &wreckages, std::vector<AlienMunition*> &AlienMunitions, std::vector<Missile*> &missiles, PowerUp* &power_up, sf::Time &elapsed, sf::Music &music);
    void DrawObjects(Cannon &cannon, std::vector<Alien*> &aliens,std::vector<Wreckage*> &wreckages, std::vector<AlienMunition*> &AlienMunitions, std::vector<Missile*> &missiles,  PowerUp* &power_up);
    void DrawGameElements(int health);
    void Update(Cannon &cannon, std::vector<Alien*> &aliens, std::vector<Wreckage*> &wreckages, std::vector<AlienMunition*> &AlienMunitions, std::vector<Missile*> &missiles,PowerUp* &power_up, sf::Time &frametime);
    void Collisions(Cannon &cannon, std::vector<Alien*> &aliens,std::vector<Wreckage*> &wreckages, std::vector<AlienMunition*> &AlienMunitions, std::vector<Missile*> &missiles,PowerUp* &power_up, sf::Time &frametime);
    void Spawn(std::vector<Alien*> &aliens, std::vector<AlienMunition*> &AlienMunitions, std::vector<Missile*> &missiles, PowerUp* &power_up, sf::Time &frametime);
    void GameOver();
    void GameWon();
};