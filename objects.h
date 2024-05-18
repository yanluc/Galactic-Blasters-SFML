#pragma once
#include"engine.h"
class GraphicalObject : public sf::Sprite
{
    sf::Texture texture_;

public:
    GraphicalObject();
    ~GraphicalObject();
    virtual void update() = 0;
    
};
class Cannon : public GraphicalObject
{
    unsigned int hp;
    public:
     Cannon(int hp);
     ~Cannon();
     void update();
};
class Alien : public GraphicalObject
{
    protected:
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
     void update();

};
class Missile : public GraphicalObject
{
    private:
     static sf::Time lastfired;
    public:
     Missile(int posx, int posy);
     ~Missile();
     static double last_fired();
     void update();

};
class Bomb : public GraphicalObject
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
     virtual void update() = 0;

};
class GuidedBomb : public Bomb
{
    protected:
     int dam=1;
    public:
     GuidedBomb(int posx, int posy);
     void update();

};
class UnguidedBomb : public Bomb
{
    protected:
     int dam=5;
    public:
     UnguidedBomb(int posx, int posy);
     void update();
};
class Background : public GraphicalObject
{
    public:
};