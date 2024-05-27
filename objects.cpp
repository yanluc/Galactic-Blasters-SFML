#include"objects.h"

std::vector<sf::IntRect> Wreckage::frames;
Wreckage::Wreckage(int posx, int posy)
{
    this->frame=0;
    this->setTextureRect(frames[0]);
    this->setTexture(TexturesandSounds::wreckage_texture);
    this->setScale(Constants::width/1920.0,Constants::height/1080.0);
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    this->setPosition(posx,posy);
}
void Wreckage::load_frame(const sf::IntRect &frame)
{
    frames.push_back(frame);
}
bool Wreckage::update(sf::Time &frametime)
{
    //change frame in rate of 1 frame per 0.02 seconds
    if((Constants::clock.getElapsedTime()-last_frame).asSeconds()>0.02)
    {
        frame++;
        last_frame=Constants::clock.getElapsedTime();
    }
    if(frame>=frames.size())
    {
        delete this;
        return false;
    }
    this->setTextureRect(frames[frame]);
    return true;
}
int Cannon::health()
{
    return hp;
}
AlienMunition::AlienMunition()
{

}
AlienMunition::~AlienMunition(){}
int Cannon::score=0;
Cannon::Cannon(int hp)
{
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    this->setPosition(Constants::width/2,Constants::height*0.85);
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
    if(shield && (Constants::clock.getElapsedTime()-shield_time).asSeconds()>3)
    {
        shield=false;
    }
    return true;
}
void Cannon::hit(int damage)
{
    if(!shield)
    {
        hp-=damage;
        score-=damage*5;
    }
}
void Cannon::shield_on()
{
    shield=true;
    score+=5;
    shield_time=Constants::clock.getElapsedTime();
}
Alien::Alien()
{
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
    setPosition(-Constants::width*0.05,Constants::height*0.1);

}
Alien::~Alien()
{
    grid[grid_posy][grid_posx] = DESTROYED;
    enemies_left--;
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
double Alien::position=0;
int Alien::enemies_left=32;
sf::Time Alien::lastdrop()
{
    return last_drop;
}
void Alien::bomb_dropped()
{
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
            int r = rand() % int(80/frametime.asSeconds());
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
            missiles.push_back(new Missile(cannon.getPosition().x + cannon.getGlobalBounds().width/2,cannon.getPosition().y));
        }
    }
}
bool Missile::collision(std::vector<Alien*> &aliens, std::vector<Wreckage*> &wreckages)
{

    for(int i = 0; i < aliens.size(); i++)
    {
        if(this->getGlobalBounds().intersects(aliens[i]->getGlobalBounds()))
        {
            aliens[i]->hit();
            if (aliens[i]->hp() == 0)
            {
                Cannon::score+=aliens[i]->alientype()*10;
                wreckages.push_back(new Wreckage(aliens[i]->getPosition().x,aliens[i]->getPosition().y));
                //remove alien from vector
                delete aliens[i];
                aliens.erase(aliens.begin()+i--);
            }
            delete this;
            return true;
        }
    }
    return false;
}
bool Missile::collision(PowerUp* &power_up, Cannon &cannon)
{
    if(this->getGlobalBounds().intersects(power_up->getGlobalBounds()))
    {
        if(power_up->get_type()==PowerUp::HEALTH)cannon.hit(-1);
        else if (power_up->get_type()==PowerUp::SHIELD)cannon.shield_on();
        delete power_up;
        power_up=NULL;
        delete this;
        return true;
    }
    return false;
}
Bomb::Bomb()
{
    double a = Constants::clock.getElapsedTime().asSeconds();
    int b = 8;
    int turn =(int(a/b)%2);
    vel_x = -(turn*2-1) * Constants::width * 0.05;
    this->setTexture(TexturesandSounds::bomb_texture);
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    this->setScale(0.1*Constants::width/1920.0,0.1*Constants::height/1080.0);
    this->birthtime=Constants::clock.getElapsedTime();
}
bool Bomb::collision(Cannon &cannon, std::vector<Wreckage*> &wreckages)
{
    if(this->getGlobalBounds().intersects(cannon.getGlobalBounds()))
    {
        cannon.hit(1);
        int add_height=0;
        if(this->getPosition().y<Constants::height*0.85) add_height=Constants::height*0.05;
        wreckages.push_back(new Wreckage(this->getPosition().x,this->getPosition().y+add_height));
        delete this;
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
bool UnguidedBomb::update(sf::Time &frametime, double target)
{
    if(this->getPosition().y>Constants::height)
    {
        TexturesandSounds::explo.play();
        delete this;
        return false;
    }
    this->move(vel_x*frametime.asSeconds(),frametime.asSeconds()*0.2*Constants::height);
    return true;
}
GuidedBomb::GuidedBomb(int posx, int posy)
{
    this->setTexture(TexturesandSounds::bomb_texture);
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    this->setPosition(posx,posy);
    this->birthtime=Constants::clock.getElapsedTime();
}
bool GuidedBomb::update(sf::Time &frametime, double target)
{
    if(this->getPosition().y>Constants::height)
    {
        TexturesandSounds::explo.play();
        delete this;
        return false;
    }

    
    if(getPosition().y < 0.4 * Constants::height)
    {
        vel_y = Constants::height * 0.05  * (3+2*(Constants::clock.getElapsedTime()-birthtime).asSeconds());
    }
    else
    {
        double rem_y = Constants::height - getPosition().y;
        double delta_x = (target+0.01*Constants::width*0.5-getPosition().x) * (vel_y/rem_y);
        if (delta_x > 0)
            vel_x = std::min(delta_x, Constants::width * 0.08);
        else
            vel_x = -(std::min(-delta_x, Constants::width * 0.08));
    }
    if(vel_y==0) vel_y=Constants::height*0.05;
    move(vel_x*frametime.asSeconds(), vel_y*frametime.asSeconds());
    return true;
}
void Bomb::Spawn(std::vector<Alien*> &aliens, std::vector<AlienMunition*> &AlienMunitions, sf::Time &frametime)
{
    int i;
    for (i = 1; i * i < aliens.size(); i++){}
    int r = rand() % int(5.0/(frametime.asSeconds()*i)+1);
    if (r == 0)
    {
        int alien_index = rand() % aliens.size();
        aliens[alien_index]->bomb_dropped();
        if(rand()%2==0)
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
Background::Background(sf::Texture &texture)
{
    this->setTexture(texture);
    const double x = texture.getSize().x;
    const double y = texture.getSize().y;
    this->setScale(Constants::width/x,Constants::height/y);
    this->setPosition(0,0);
}
bool PowerUp::exists=false;
sf::Time PowerUp::last_spawn;
PowerUp::PowerUp(int type)
{
    last_spawn=Constants::clock.getElapsedTime();
    if(type==HEALTH) this->setTexture(TexturesandSounds::heart_texture);
    else if(type==SHIELD) this->setTexture(TexturesandSounds::shield_texture);
    this->setOrigin(this->getGlobalBounds().width/2,this->getGlobalBounds().height/2);
    this->setScale(0.05*Constants::width/1920.0,0.05*Constants::height/1080.0);
    this->setPosition(0,Constants::height*0.1);
    exists=true;
    this->type=type;
}
PowerUp::~PowerUp()
{
    exists=false;
}
bool PowerUp::update(sf::Time &frametime)
{
    if(this->getPosition().x>Constants::width)
    {
        return false;
    }
    // double new_y=(this->getPosition().x+0.1*Constants::width)*(this->getPosition().x-1.1*Constants::width);
    double new_y=Constants::height*0.4;
    this->setPosition(this->getPosition().x+0.1*Constants::width*frametime.asSeconds(), new_y);
    return true;
}
int PowerUp::get_type()
{
    return type;
}