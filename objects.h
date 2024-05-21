#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include<vector>
#include<cmath>
#include <algorithm>
// #include "functions.h"
#define DESTROYED 2
#define ALIVE 1
#define EMPTY 0
class TexturesandSounds
{
    public:
     static sf::SoundBuffer explo_sound;
     static sf::SoundBuffer bomb_explo_sound;
     static sf::Texture background_texture;
     static sf::Texture cannon_texture;
     static sf::Texture alien_texture;
     static sf::Texture missile_texture;
     static sf::Texture bomb_texture;
};
class Constants
{
    public:
     static sf::Font font;
     static sf::Clock clock;
     static int width, height;
     static sf::Time start_time;
};
class GraphicalObject : public sf::Sprite
{
    sf::Texture texture_;

public:
    GraphicalObject();
    ~GraphicalObject();
    // virtual void update() = 0;
    
};
class Alien : public GraphicalObject
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
class Cannon : public GraphicalObject
{
    protected:
     int hp;
    public:
     void hit(int damage);
     int health();
     Cannon(int hp);
     ~Cannon();
     bool update(sf::Time &elapsed);
};
class AlienMunition : public GraphicalObject
{
    public:
     AlienMunition();
     ~AlienMunition();
     virtual bool update(sf::Time &frametime) = 0;
     virtual bool collision(Cannon &cannon)=0;
};


class Missile : public GraphicalObject
{
    private:
     static sf::Time lastfired;
    public:
     Missile(int posx, int posy);
     static sf::Time last_fired();
     bool update(sf::Time &frametime);
     static void Fire(std::vector<Missile*> &missiles, Cannon &cannon);
     bool collision(std::vector<Alien*> &aliens);

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
     virtual bool update(sf::Time &frametime) = 0;
     static void Spawn(std::vector<Alien*> &aliens, std::vector<AlienMunition*> &AlienMunitions);
     bool collision(Cannon &cannon);

};
class GuidedBomb : public Bomb
{
    protected:
     int dam=1;
    public:
     GuidedBomb(int posx, int posy);
     bool update(sf::Time &frametime);

};
class UnguidedBomb : public Bomb
{
    protected:
     int dam=5;
    public:
     UnguidedBomb(int posx, int posy);
     bool update(sf::Time &frametime);
};
class Background : public GraphicalObject
{
    public:
};