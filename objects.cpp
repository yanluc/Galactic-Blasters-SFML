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
int Cannon::hp;
int Cannon::health()
{
    return hp;
}
Munition::Munition()
{

}
Munition::~Munition(){}

Cannon::Cannon(int hp)
{
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    this->setPosition(Constants::width/2,Constants::height*0.8);
    this->setScale(0.5*Constants::width/1920.0,0.5*Constants::height/1080.0);
    this->hp=hp;
    this->setTexture(TexturesandSounds::cannon_texture);
}
Cannon::~Cannon(){}
bool Cannon::update(std::vector<Munition*> &munitions, sf::Time &elapsed)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && this->getPosition().x > 0)
    {
        this->move(-elapsed.asSeconds()*0.05*Constants::width,0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && this->getPosition().x < Constants::width-this->getGlobalBounds().width)
    {
        this->move(elapsed.asSeconds()*0.05*Constants::width,0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if ((Constants::clock.getElapsedTime() - Missile::last_fired()).asSeconds() > 1)
        {
            Munition *m = new Missile(this->getPosition().x,this->getPosition().y);
        
            std::cout << m->getPosition().x << " " << m->getPosition().y << std::endl;
            munitions.push_back(m);
        }
    }
    return true;
}
Alien::Alien()
{
    droppedbomb = false;
    this->setTexture(TexturesandSounds::alien_texture);
    this->setScale(0.2*Constants::width/1920.0,0.2*Constants::height/1080.0);
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
    if(grid_posy==0)
    {
        alien_type=2;
        alien_hp=2;
    }
    else
    {
        alien_type=1;
        alien_hp=1;
    }
    Alien::grid[grid_posy][grid_posx] = ALIVE;
    last_spawn=Constants::clock.getElapsedTime();
    setPosition(Constants::width*0.05,Constants::height*0.1);

}
Alien::~Alien(){}
bool Alien::dropped_bomb()
{
    return droppedbomb;
}
int Alien::alientype()
{
    return alien_type;
}
sf::Time Alien::last_spawn;
int Alien::grid[5][8]={{2,2,0,0,0,0,2,2},{2,0,0,0,0,0,0,2},{2,0,0,0,0,0,0,2},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};
int Alien::position=0;
sf::Time Alien::lastdrop()
{
    return last_drop;
}
void Alien::bomb_dropped()
{
    droppedbomb = true;
    last_drop = Constants::clock.getElapsedTime();
}
int Alien::get_position()
{
    return position;
}
bool Alien::update(sf::Time &frametime)
{
    if (phase == 1)
    {
        move(Constants::width * 0.2 * frametime.asSeconds(), 0);
        if (getPosition().x>Constants::width*0.9)
            phase = 2;
        
    }
    else if (phase == 2 || phase == 3)
    {
        double delta_x = Constants::width * 0.1 * frametime.asSeconds();
        double delta_y = Constants::height * 0.2 * frametime.asSeconds();
        double tolerance = Constants::width*0.002;
        double target_y, target_x;
        if(phase == 2) 
        {
            target_x = Constants::width * 0.8 * grid_posx / 12  + position * Constants::width * 0.05 - Constants::width * 0.1;
            target_y = Constants::height * 0.5 * grid_posy / 8 - Constants::height * 0.15;
        }
        else 
        {
            target_x = Constants::width * 0.8 * grid_posx / 12  + position * Constants::width * 0.05;
            target_y = Constants::height * 0.5 * grid_posy / 8 + Constants::height * 0.05;
        }
        if (abs(getPosition().x - target_x) < tolerance && abs(getPosition().y - target_y) < tolerance)
        {
            phase = 3;
            setPosition(target_x, target_y);
        }
        else
        {
            double dist = sqrt((getPosition().x - target_x) * (getPosition().x - target_x) + (getPosition().y - target_y) * (getPosition().y - target_y));
            double vel_x = delta_x * (getPosition().x - target_x) / dist ;
            double vel_y = delta_y * (getPosition().y - target_y) / dist;
            move(-vel_x, -vel_y);
        }
        if(phase == 3 && getPosition().y == target_y)
        {
            int r = rand() % 4000;
            if (r == 0)
            {
                phase = 4;
            }
        }
    }
    else if (phase == 4)
    {
        double delta_x = Constants::width * 0.2 * frametime.asSeconds();
        double delta_y = Constants::height * 0.3 * frametime.asSeconds();
        double tolerance = Constants::width*0.002;
        double return_x = Constants::width * 0.8 * grid_posx / 12  + position * Constants::width * 0.05 - Constants::width * 0.1;
        double return_y = Constants::height * 0.5 * grid_posy / 8 + Constants::height * 0.05;
        double target_y = 0.9 * Constants::height;
        double target_x = sqrt((return_y - getPosition().y)*(return_y - getPosition().y)+ getPosition().x);
        if (getPosition().y > Constants::height * 0.9)
        {
            phase = 3;
        }
        else
        {
            double dist = sqrt((getPosition().x - target_x) * (getPosition().x - target_x) + (getPosition().y - target_y) * (getPosition().y - target_y));
            double vel_x = delta_x * (getPosition().x - target_x) / dist ;
            double vel_y = delta_y * (getPosition().y - target_y) / dist;
            move(-vel_x, -vel_y);
        }
    }
    return true;
}
Missile::Missile(int posx,int posy)
{
    lastfired=Constants::clock.getElapsedTime();
    this->setTexture(TexturesandSounds::missile_texture);
    this->setScale(0.3*Constants::width/1920.0,0.3*Constants::height/1080.0);
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    this->setPosition(posx,posy);
}
sf::Time Missile::lastfired;
sf::Time Missile::last_fired()
{
    return lastfired;
}
bool Missile::update(sf::Time &frametime)
{
    if(this->getPosition().y<0)
    {
        return false;
    }
    this->move(0,-frametime.asSeconds()*0.2*Constants::height);
    return true;
}
Bomb::Bomb()
{
    this->setTexture(TexturesandSounds::bomb_texture);
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    this->setScale(0.1*Constants::width/1920.0,0.1*Constants::height/1080.0);
    this->birthtime=Constants::clock.getElapsedTime();
}
UnguidedBomb::UnguidedBomb(int posx, int posy)
{
    this->setTexture(TexturesandSounds::bomb_texture);
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    this->setPosition(posx,posy);
    this->birthtime=Constants::clock.getElapsedTime();
}
bool UnguidedBomb::update(sf::Time &frametime)
{
    if(this->getPosition().y>Constants::height)
    {
        return false;
    }
    this->move(0,frametime.asSeconds()*0.2*Constants::height);
    return true;
}
GuidedBomb::GuidedBomb(int posx, int posy)
{
    this->setTexture(TexturesandSounds::bomb_texture);
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    this->setPosition(posx,posy);
    this->birthtime=Constants::clock.getElapsedTime();
}
bool GuidedBomb::update(sf::Time &frametime)
{
    if(this->getPosition().y>Constants::height)
    {
        return false;
    }
    this->move(0,frametime.asSeconds()*0.2*Constants::height);
    return true;
    return true;
}
void Bomb::Spawn(std::vector<Alien> &aliens, std::vector<Munition*> &munitions)
{
    int r = rand() % (1000/aliens.size());
    if (r == 0)
    {
        int alien_index = rand() % aliens.size();
        Alien &alien = aliens[alien_index];
        if (!alien.dropped_bomb())
        {
            alien.bomb_dropped();
            if (alien.alientype() == 1)
            {
                Bomb *b = new UnguidedBomb(alien.getPosition().x,alien.getPosition().y);
                munitions.push_back(b);
            }
            else
            {
                Bomb *b = new GuidedBomb(alien.getPosition().x,alien.getPosition().y);
                munitions.push_back(b);
            }
        }
    }
}