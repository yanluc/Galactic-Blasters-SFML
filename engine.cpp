#include"engine.h"
#include<stdio.h>
#include<time.h>
#include<fstream>
#include<iostream>
#include<vector>
Engine::Engine()
{
    srand(time(NULL));
    InitWindow();
}
Engine::~Engine()
{
    window_.close();
}


int Engine::highscore;
int Engine::score;
int Engine::enemies_to_spawn=32;
void Engine::LoadTextures()
{
    TexturesandSounds::background_texture.loadFromFile("resources/nightsky.png");
    TexturesandSounds::cannon_texture.loadFromFile("resources/cannon.png");
    TexturesandSounds::alien_texture.loadFromFile("resources/alien.png");
    TexturesandSounds::missile_texture.loadFromFile("resources/missile.png");
    TexturesandSounds::bomb_texture.loadFromFile("resources/bomb.png");

}
void Engine::LoadSounds()
{
    TexturesandSounds::explo_sound.loadFromFile("resources/bomb.wav");
    TexturesandSounds::explo.setBuffer(TexturesandSounds::explo_sound);
}
std::vector<std::pair<double,int>> Engine::Leaderboard(char const *filename)
{
    std::vector<std::pair<double,int>> scores;
    std::ifstream file;
    file.open(filename);
    double tmp1;
    int tmp2;
    while(!file.eof())
    {
        file >> tmp1 >> tmp2;
        scores.push_back(std::make_pair(tmp1,tmp2));
        if(tmp1<best_time) best_time = tmp1;
        if(tmp2>highscore) highscore = tmp2;
    }
    file.close();
    return scores;
}
void Engine::InitWindow()
{
    window_.create(sf::VideoMode(Constants::width,Constants::height),"Galactic Blasters");
}
void Engine::StartMenu()
{
    bool up = false;
    bool down = false;
    int score_pos=0;
    std::cout << "StartMenu()" << std::endl;
    std::vector<std::pair<double,int>> leaderboard = Leaderboard("resources/scores.txt");
    
    if (!Constants::font.loadFromFile("resources/COMICSANS.TTF"))
    {
        std::cout << "Font not found !" << std::endl;
    }
    while (window_.isOpen())
    {
        if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && down){down=false;}
        else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && up){ up=false;}
        sf::Event event;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) return;
        while (window_.pollEvent(event))
        {
            if(event.type==sf::Event::KeyPressed)
            {
                if(event.key.code==sf::Keyboard::Space)
                {
                    return;
                }
            }
            if(event.type==sf::Event::MouseButtonPressed)
            {
                return;
            }
            if(event.type==sf::Event::Closed)
            {
                window_.close();
            }
        }
        window_.clear(sf::Color(150,150,150));
        sf::Text text;
        text.setFont(Constants::font);
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::Black);
        text.setString("space to shoot, arrows to control, press space to continue");
        text.setPosition(0.2 * Constants::width,0.7 * Constants::height);
        window_.draw(text);
        if(best_time!=9999999)
        {
            text.setCharacterSize(30);
            text.setFillColor(sf::Color::Black);
            text.setString("best time:");
            text.setPosition(0.2 * Constants::width,0.5*Constants::height);
            window_.draw(text);
            std::string tmp = "x";// std::format("{:.3f}",best_time);
            tmp = tmp + " seconds";
            text.setString(tmp);
            text.setPosition(0.3 * Constants::width,0.5*Constants::height);
            window_.draw(text);

            text.setCharacterSize(30);
            text.setFillColor(sf::Color::Black);
            text.setString("highscore:");
            text.setPosition(0.2 * Constants::width,0.55*Constants::height);
            window_.draw(text);
            tmp = std::to_string(highscore);
            text.setString(tmp);
            text.setPosition(0.3 * Constants::width,0.55*Constants::height);
            window_.draw(text);
        }
        if(leaderboard.size()>15)
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !down){ score_pos++; down=true;}
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !up){ score_pos--; up=true;}
            if(score_pos<0)score_pos=0;
            if(score_pos>leaderboard.size()-15)score_pos=leaderboard.size()-15;
        }
            
        for(int i = score_pos; i < std::min(score_pos+15, int(leaderboard.size()));i++)
        {
            text.setString(std::to_string(leaderboard[i].second));
            text.setPosition(0.8 *Constants::width,0.2*Constants::height+0.05*Constants::height*(i-score_pos));
            window_.draw(text);
            text.setString(std::to_string(leaderboard[i].first));
            text.setPosition(0.9 * Constants::width,0.2*Constants::height+0.05*Constants::height*(i-score_pos));
            window_.draw(text);
        }
        window_.display();
    }
}
void Engine::RunGame()
{
    bool gameend = false;
    window_.clear(sf::Color(240,240,220));
    std::cout << "RunGame()" << std::endl;
    Cannon cannon(100);
    std::vector<Alien*> aliens;
    std::vector<AlienMunition*> AlienMunitions;
    std::vector<Missile*> missiles;
    sf::Time elapsed = Constants::clock.getElapsedTime();
    window_.setFramerateLimit(60);
    Constants::start_time = Constants::clock.getElapsedTime();
    while(!gameend)
    {
        window_.clear(sf::Color(240,240,220));
        gameend = GameLoop(cannon,aliens,AlienMunitions, missiles, elapsed);
        window_.display();
    }
    if(gameend)
    {
        // std:: cout << gameend << std:: endl;
        if(cannon.health()<=0)
        {
            GameOver();
        }
        else if(Alien::enemies_left<=0)
        {
            GameWon();
        }
    }
}
bool Engine::GameLoop(Cannon &cannon, std::vector<Alien*> &aliens, std::vector<AlienMunition*> &AlienMunitions, std::vector<Missile*> &missiles, sf::Time &elapsed)
{
    sf::Time frametime=Constants::clock.getElapsedTime()-elapsed;
    elapsed = Constants::clock.getElapsedTime();
    sf::Event event;
    while(window_.pollEvent(event))
    {
        if(event.type==sf::Event::Closed)
        {
            window_.close();
            exit(0);
        }
    }
    Update(cannon,aliens,AlienMunitions, missiles, frametime);
    DrawObjects(cannon,aliens,AlienMunitions, missiles);
    Spawn(aliens,AlienMunitions, missiles);
    Collisions(cannon,aliens,AlienMunitions, missiles, frametime);
    DrawGameElements(cannon.health());
    if(cannon.health()<=0 || Alien::enemies_left<=0)
    {
        return true;
    }
    return false;
}
void Engine::DrawObjects(Cannon &cannon, std::vector<Alien*> &aliens, std::vector<AlienMunition*> &AlienMunitions, std::vector<Missile*> &missiles)
{
    window_.draw(cannon);
    for(auto &alien:aliens)
    {
        window_.draw(*alien);
    }
    for(auto &AlienMunition:AlienMunitions)
    {
        window_.draw(*AlienMunition);
    }
    for(auto &missile:missiles)
    {
        window_.draw(*missile);
    }
}
void Engine::DrawGameElements(int health)
{
    sf::Text text;
    text.setFont(Constants::font);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::Black);
    text.setString("Score: " + std::to_string(score));
    text.setPosition(0.8 * Constants::width,0.05 * Constants::height);
    window_.draw(text);
    text.setString("Enemies left: " + std::to_string(Alien::enemies_left));
    text.setPosition(0.8 * Constants::width,0.1 * Constants::height);
    window_.draw(text);
    text.setString("Health: " + std::to_string(health));
    text.setPosition(0.8 * Constants::width,0.15 * Constants::height);
    window_.draw(text);

}
void Engine::Update(Cannon &cannon, std::vector<Alien*> &aliens, std::vector<AlienMunition*> &AlienMunitions, std::vector<Missile*> &missiles, sf::Time &frametime)
{
    Missile::Fire(missiles, cannon);
    cannon.update(frametime);
    for(auto &alien:aliens)
    {
        alien->update(frametime);
    }
    AlienMunitions.erase(std::remove_if(AlienMunitions.begin(),AlienMunitions.end(),[&](AlienMunition* munition){
        return !munition->update(frametime);
    }),AlienMunitions.end());
    missiles.erase(std::remove_if(missiles.begin(),missiles.end(),[&](Missile* missile){
        return !missile->update(frametime);
    }),missiles.end());
    double a = Constants::clock.getElapsedTime().asSeconds();
    int b = 8;
    bool turn = bool(int(a/b)%2);
    if(turn) Alien::position = b - fmod(a,b)+1;
    else Alien::position = fmod(a,b)+1;
    
}
void Engine::Collisions(Cannon &cannon, std::vector<Alien*> &aliens, std::vector<AlienMunition*> &AlienMunitions, std::vector<Missile*> &missiles, sf::Time &frametime)
{
    int size = aliens.size()+AlienMunitions.size()+missiles.size();
    for(int i = 0; i < aliens.size();i++)
    {
        if(aliens[i]->getGlobalBounds().intersects(cannon.getGlobalBounds()))
        {
            cannon.hit(10);
            delete aliens[i];
            aliens.erase(aliens.begin()+i--);
        }
    }
    for(auto &munition:AlienMunitions)
    {
        AlienMunitions.erase(std::remove_if(AlienMunitions.begin(),AlienMunitions.end(),[&](AlienMunition* munition){
            return munition->collision(cannon);
        }),AlienMunitions.end());
    }
    for(auto &missile:missiles)
    {
        missiles.erase(std::remove_if(missiles.begin(),missiles.end(),[&](Missile* missile){
            return missile->collision(aliens);
        }),missiles.end());
    }
    if(size!=aliens.size()+AlienMunitions.size()+missiles.size())
    {
        TexturesandSounds::explo.play();
    }
}
void Engine::Spawn(std::vector<Alien*> &aliens, std::vector<AlienMunition*> &AlienMunitions, std::vector<Missile*> &missiles)
{
    //spawn aliens
    if(enemies_to_spawn>0 && (Constants::clock.getElapsedTime()-Alien::last_spawn).asSeconds()>2)
    {
        Alien* a = new Alien();
        aliens.push_back(a);
        enemies_to_spawn--;
    }

    //spawn bombs
    Bomb::Spawn(aliens,AlienMunitions);

}
void Engine::GameOver()
{
    std::cout << "GameOver()" << std::endl;
    window_.clear(sf::Color(240,240,220));
    sf::Font font;
    if (!font.loadFromFile("resources/COMICSANS.TTF"))
    {
        std::cout << "Font not found !" << std::endl;
    }
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::Black);
    text.setString("Game Over");
    text.setPosition(0.5 * Constants::width,0.5 * Constants::height);
    window_.draw(text);
    window_.display();
    sf::sleep(sf::seconds(3));
    window_.close();
}
void Engine::GameWon()
{
    std::cout << "GameWon()" << std::endl;
    window_.clear(sf::Color(240,240,220));
    sf::Font font;
    if (!font.loadFromFile("resources/COMICSANS.TTF"))
    {
        std::cout << "Font not found !" << std::endl;
    }
    sf::Time time = Constants::clock.getElapsedTime() - Constants::start_time;
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::Black);
    text.setString("You Won!");
    text.setPosition(0.5 * Constants::width,0.5 * Constants::height);
    window_.draw(text);
    text.setString("Score: " + std::to_string(score));
    text.setPosition(0.5 * Constants::width,0.55 * Constants::height);
    window_.draw(text);
    text.setString("Time: " + std::to_string(time.asSeconds()) + " seconds");
    text.setPosition(0.5 * Constants::width,0.6 * Constants::height);
    window_.draw(text);
    window_.display();
    text.setString("Press escape to exit");
    text.setPosition(0.5 * Constants::width,0.65 * Constants::height);
    window_.draw(text);
    while(!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && window_.isOpen())
    {
        sf::Event event;
        while(window_.pollEvent(event))
        {
            if(event.type==sf::Event::Closed)
            {
                window_.close();
            }
        }
    }
    window_.close();
    std::ofstream file;
    file.open("resources/scores.txt",std::ios::app);
    file << time.asSeconds() << " " << score << std::endl;
}