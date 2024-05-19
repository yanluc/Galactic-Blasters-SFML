#include"objects.h"
sf::Texture TexturesandSounds::background_texture;
sf::Texture TexturesandSounds::alien_texture;
sf::Texture TexturesandSounds::missile_texture;
sf::Texture TexturesandSounds::bomb_texture;
sf::Texture TexturesandSounds::cannon_texture;
sf::SoundBuffer TexturesandSounds::bomb_explo_sound;
sf::SoundBuffer TexturesandSounds::explo_sound;
sf::Clock Constants::clock;
int Constants::width=sf::VideoMode::getDesktopMode().width;
int Constants::height=sf::VideoMode::getDesktopMode().height;
GraphicalObject::GraphicalObject()
{

}
GraphicalObject::~GraphicalObject(){}
Cannon::Cannon(int hp)
{
    this->hp=hp;
    this->setTexture(TexturesandSounds::cannon_texture);
}
Cannon::~Cannon(){}
void Cannon::update()
{

}
Alien::Alien()
{
    this->setTexture(TexturesandSounds::alien_texture);
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    int min_dist = 1000;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (Alien::grid[i][j] == EMPTY)
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
    Alien::grid[grid_posy][grid_posx] = ALIVE;
    dropped_bomb = false;
    last_spawn=Constants::clock.getElapsedTime();
    setPosition(Constants::width*0.05,Constants::height*0.1);

}
Alien::~Alien(){}
sf::Time Alien::last_spawn;
int Alien::grid[5][8];
void Alien::update()
{

}
Missile::Missile(int posx,int posy)
{
    this->setTexture(TexturesandSounds::missile_texture);
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
    this->setTexture(TexturesandSounds::bomb_texture);
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    this->birthtime=Constants::clock.getElapsedTime();
}
UnguidedBomb::UnguidedBomb(int posx, int posy)
{
    this->setTexture(TexturesandSounds::bomb_texture);
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    this->setPosition(posx,posy);
    this->birthtime=Constants::clock.getElapsedTime();
}
void UnguidedBomb::update()
{

}
GuidedBomb::GuidedBomb(int posx, int posy)
{
    this->setTexture(TexturesandSounds::bomb_texture);
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    this->setPosition(posx,posy);
    this->birthtime=Constants::clock.getElapsedTime();
}
void GuidedBomb::update()
{

}