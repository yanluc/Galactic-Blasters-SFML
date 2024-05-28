#pragma once
#include<vector>
#include<cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "namespaces.h"
#define DESTROYED 2
#define ALIVE 1
#define EMPTY 0

class Alien : public sf::Sprite
{
    protected:
     static int grid[5][8];
     int alien_hp=1;
     int phase = 1;
     int alien_type=1;
     int grid_posx = -1, grid_posy = -1;
     sf::Time last_drop;
    public:
     static double position;
     void hit();
     int hp();
     int alientype();
     void bomb_dropped();
     sf::Time lastdrop();
     static int enemies_left;
     static int get_position();
     static sf::Time last_spawn;
     Alien();
     ~Alien();
     bool update(sf::Time &frametime);

};
class PowerUp : public sf::Sprite
{
    protected:
     int type;
     
    public:
     int get_type();
     static bool exists;
     static sf::Time last_spawn;
     enum types{HEALTH, SHIELD};
     PowerUp(int type);
     ~PowerUp();
     bool update(sf::Time &frametime);
};
class Wreckage : public sf::Sprite
{
    protected:
     static std::vector<sf::IntRect> frames;
     sf::Time last_frame;
    public:
     int frame;
     static void load_frame(const sf::IntRect &frame);
     Wreckage(int posx, int posy);
     bool update(sf::Time &frametime);
};
class Cannon : public sf::Sprite
{
    protected:
     int hp;
     bool shield=false;
     sf::Time shield_time;
    public:
     void shield_on();
     static int score;
     void hit(int damage);
     int health();
     Cannon(int hp);
     ~Cannon();
     bool update(sf::Time &elapsed);
};
class AlienMunition : public sf::Sprite
{
    protected:
     double vel_x;
    public:
     AlienMunition();
     ~AlienMunition();
     static void Spawn(std::vector<Alien*> &aliens, std::vector<AlienMunition*> &AlienMunitions, sf::Time &frametime);
     virtual bool update(sf::Time &frametime, double target) = 0;
     virtual bool collision(Cannon &cannon, std::vector<Wreckage*> &wreckages)=0;
};


class Missile : public sf::Sprite
{
    private:
     int frame=0;
     sf::Time last_frame;
     static sf::Time lastfired;
    public:
     Missile(int posx, int posy);
     static sf::Time last_fired();
     bool update(sf::Time &frametime);
     static void Fire(std::vector<Missile*> &missiles, Cannon &cannon);
     bool collision(std::vector<Alien*> &aliens, std::vector<Wreckage*> &wreckages);
     bool collision(PowerUp* &power_up, Cannon &cannon);

};
class Bomb : public AlienMunition
{
    protected:
     sf::Time birthtime;
     int velocity_y;
     int dam=1;
     int start_y;
    public:
     int damage();
     double birth();
     Bomb();
     virtual bool update(sf::Time &frametime, double target) = 0;
     bool collision(Cannon &cannon, std::vector<Wreckage*> &wreckages);

};
class GuidedBomb : public Bomb
{
    protected:
     int dam=1;
     double vel_y;
    public:
     GuidedBomb(int posx, int posy);
     bool update(sf::Time &frametime, double target);

};
class UnguidedBomb : public Bomb
{
    protected:
     int dam=5;
    public:
     UnguidedBomb(int posx, int posy);
     bool update(sf::Time &frametime, double target);
};
class Background : public sf::Sprite
{
    public:
    Background(sf::Texture &texture);
};
