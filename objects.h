#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include<vector>
#include<iostream>
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
     static sf::Clock clock;
     static int width, height;
};
class GraphicalObject : public sf::Sprite
{
    sf::Texture texture_;

public:
    GraphicalObject();
    ~GraphicalObject();
    // virtual void update() = 0;
    
};
class Munition : public GraphicalObject
{
    public:
     Munition();
     ~Munition();
     virtual bool update(sf::Time &frametime) = 0;
};
class Cannon : public GraphicalObject
{
    static int hp;
    public:
     static int health();
     Cannon(int hp);
     ~Cannon();
     bool update(std::vector<Munition*> &munitions, sf::Time &elapsed);
};
class Alien : public GraphicalObject
{
    protected:
     static int grid[5][8];
     int alien_hp=1;
     int phase = 1;
     int alien_type=1;
     int grid_posx = -1, grid_posy = -1;
     bool dropped_bomb;
     double last_drop;
    public:
     static sf::Time last_spawn;
     Alien();
     ~Alien();
     void update(sf::Time &frametime);

};

class Missile : public Munition
{
    private:
     static sf::Time lastfired;
    public:
     Missile(int posx, int posy);
     static sf::Time last_fired();
     bool update(sf::Time &frametime);

};
class Bomb : public Munition
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
     virtual bool update() = 0;

};
class GuidedBomb : public Bomb
{
    protected:
     int dam=1;
    public:
     GuidedBomb(int posx, int posy);
     bool update();

};
class UnguidedBomb : public Bomb
{
    protected:
     int dam=5;
    public:
     UnguidedBomb(int posx, int posy);
     bool update();
};
class Background : public GraphicalObject
{
    public:
};