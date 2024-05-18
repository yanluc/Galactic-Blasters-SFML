#include"objects.h"
#include"engine.h"
GraphicalObject::GraphicalObject()
{

}
GraphicalObject::~GraphicalObject(){}
Cannon::Cannon(int hp)
{
    this->hp=hp;
    this->setTexture(Engine::cannon_texture);
}
Cannon::~Cannon(){}
void Cannon::update()
{

}
Alien::Alien()
{
    this->setTexture(Engine::alien_texture);
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    int min_dist = 1000;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (Engine::grid[i][j] == EMPTY)
            {
                int dist = abs(3 - i) + abs(4 - j);
                if (dist < min_dist)
                {
                    min_dist = dist;
                    grid_posy = i;
                    grid_posx = j;
                }
            }
        }
    }
    Engine::grid[grid_posy][grid_posx] = ALIVE;
    dropped_bomb = false;
    last_spawn=Engine::clock.getElapsedTime();
    setPosition(Engine::width*0.05,Engine::height*0.1);

}
Alien::~Alien(){}
sf::Time Alien::last_spawn;
void Alien::update()
{

}
Missile::Missile(int posx,int posy)
{
    this->setTexture(Engine::missile_texture);
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    this->setPosition(posx,posy);
}
Missile::~Missile()
{
    
}
void Missile::update()
{

}
Bomb::Bomb()
{
    this->setTexture(Engine::bomb_texture);
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    this->birthtime=Engine::clock.getElapsedTime();
}
UnguidedBomb::UnguidedBomb(int posx, int posy)
{
    this->setTexture(Engine::bomb_texture);
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    this->setPosition(posx,posy);
    this->birthtime=Engine::clock.getElapsedTime();
}
void UnguidedBomb::update()
{

}
GuidedBomb::GuidedBomb(int posx, int posy)
{
    this->setTexture(Engine::bomb_texture);
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    this->setPosition(posx,posy);
    this->birthtime=Engine::clock.getElapsedTime();
}
void GuidedBomb::update()
{

}