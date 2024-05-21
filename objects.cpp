#include"objects.h"
sf::Texture TexturesandSounds::background_texture;
sf::Texture TexturesandSounds::alien_texture;
sf::Texture TexturesandSounds::missile_texture;
sf::Texture TexturesandSounds::bomb_texture;
sf::Texture TexturesandSounds::cannon_texture;
sf::SoundBuffer TexturesandSounds::bomb_explo_sound;
sf::SoundBuffer TexturesandSounds::explo_sound;
sf::Clock Constants::clock;
sf::Font Constants::font;
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
AlienMunition::AlienMunition()
{

}
AlienMunition::~AlienMunition(){}

Cannon::Cannon(int hp)
{
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    this->setPosition(Constants::width/2,Constants::height*0.8);
    this->setScale(0.5*Constants::width/1920.0,0.5*Constants::height/1080.0);
    this->hp=hp;
    this->setTexture(TexturesandSounds::cannon_texture);
}
Cannon::~Cannon(){}
bool Cannon::update(sf::Time &elapsed)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && this->getPosition().x > 0)
    {
        this->move(-elapsed.asSeconds()*0.05*Constants::width,0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && this->getPosition().x < Constants::width-this->getGlobalBounds().width)
    {
        this->move(elapsed.asSeconds()*0.05*Constants::width,0);
    }
    
    return true;
}
void Cannon::hit()
{
    hp--;
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
Alien::~Alien()
{
    grid[grid_posy][grid_posx] = DESTROYED;
    enemies_left--;
}
bool Alien::dropped_bomb()
{
    return droppedbomb;
}
int Alien::alientype()
{
    return alien_type;
}
void Alien::hit()
{
    alien_hp--;
}
int Alien::hp()
{
    return alien_hp;
}
sf::Time Alien::last_spawn;
int Alien::grid[5][8]={{2,2,0,0,0,0,2,2},{2,0,0,0,0,0,0,2},{2,0,0,0,0,0,0,2},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};
int Alien::position=0;
int Alien::enemies_left=32;
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
    this->move(0,-frametime.asSeconds()*0.5*Constants::height);
    return true;
}
void Missile::Fire(std::vector<Missile*> &missiles ,Cannon &cannon)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if ((Constants::clock.getElapsedTime() - Missile::last_fired()).asSeconds() > 1)
        {
            Missile* mis = new Missile(cannon.getPosition().x + cannon.getGlobalBounds().width/2,cannon.getPosition().y);
            mis->move(-mis->getGlobalBounds().width/2,0);
            missiles.push_back(mis);
        }
    }
}
bool Missile::collision(std::vector<Alien*> &aliens)
{

    for(int i = 0; i < aliens.size(); i++)
    {
        if(this->getGlobalBounds().intersects(aliens[i]->getGlobalBounds()))
        {
            aliens[i]->hit();
            if (aliens[i]->hp() == 0)
            {
                //remove alien from vector
                delete aliens[i];
                aliens.erase(aliens.begin()+i--);
            }
            return true;
        }
    }
    return false;
}
Bomb::Bomb()
{
    this->setTexture(TexturesandSounds::bomb_texture);
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    this->setScale(0.1*Constants::width/1920.0,0.1*Constants::height/1080.0);
    this->birthtime=Constants::clock.getElapsedTime();
}
bool Bomb::collision(Cannon &cannon)
{
    if(this->getGlobalBounds().intersects(cannon.getGlobalBounds()))
    {
        cannon.hit();
        return true;
    }
    return false;
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
}
void Bomb::Spawn(std::vector<Alien*> &aliens, std::vector<AlienMunition*> &AlienMunitions)
{
    int r = rand() % (1000/aliens.size());
    if (r == 0)
    {
        int alien_index = rand() % aliens.size();
        if (!aliens[alien_index]->dropped_bomb())
        {
            aliens[alien_index]->bomb_dropped();
            if (aliens[alien_index]->alientype() == 1)
            {
                Bomb *b = new UnguidedBomb(aliens[alien_index]->getPosition().x,aliens[alien_index]->getPosition().y);
                AlienMunitions.push_back(b);
            }
            else
            {
                Bomb *b = new GuidedBomb(aliens[alien_index]->getPosition().x,aliens[alien_index]->getPosition().y);
                AlienMunitions.push_back(b);
            }
        }
    }
}