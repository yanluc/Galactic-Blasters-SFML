#include"engine.hpp"
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
int Engine::enemies_to_spawn=32;
void Engine::LoadTextures()
{
    TexturesandSounds::background_texture.loadFromFile("resources/nightsky.png");
    TexturesandSounds::cannon_texture.loadFromFile("resources/cannon.png");
    TexturesandSounds::alien_texture.loadFromFile("resources/alien.png");
    // TexturesandSounds::missile_texture.loadFromFile("resources/missile.png");
    TexturesandSounds::bomb_texture.loadFromFile("resources/bomb.png");
    TexturesandSounds::heart_texture.loadFromFile("resources/heart.png");
    TexturesandSounds::shield_texture.loadFromFile("resources/shield.png");
    
    TexturesandSounds::wreckage_texture.loadFromFile("resources/wreckage.png");
    int texture_wreckage_w = 93;
    int texture_wreckage_h = 88;
    int texture_wreckage_frames = 81;
    for(int i = 0; i < 9;i++)
    {
        for (int j = 0; j < 9;j++)
        {
            sf::IntRect rect(j*texture_wreckage_w,i*texture_wreckage_h,texture_wreckage_w,texture_wreckage_h);
            Wreckage::load_frame(rect);
        }
    }
    TexturesandSounds::background_start_texture.loadFromFile("resources/start-background.png");
    TexturesandSounds::background_texture.loadFromFile("resources/nightsky.png");

    for (int i = 0; i < 10; i++)
    {
        char filename[50];
        sprintf(filename,"resources/missile/Missile_1_Flying_00%d.png",i);
        TexturesandSounds::missile_texture_animated[i].loadFromFile(filename);
    }
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
    while(file >> tmp1 >> tmp2)
    { 
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
    if (!Constants::font.loadFromFile("resources/COMICSANS.TTF"))
    {
        std::cout << "Font not found !" << std::endl;
    }
}
void Engine::StartMenu()
{
    sf::Music music;
    if (!music.openFromFile("resources/Retro Vibes Loop.ogg"))
    {
        std::cout << "Music not found !" << std::endl;
    }
    music.setLoop(true);
    music.play();
    bool up = false;
    bool down = false;
    StartMenuElements startmenu;
    startmenu.current_screen = StartMenuElements::screen::MAIN;
    Background menu_background(TexturesandSounds::background_start_texture);   
    while (window_.isOpen())
    {
        
        sf::Event event;
        while (window_.pollEvent(event))
        {
            if(event.type==sf::Event::Closed)
            {
                music.stop();
                window_.close();
                exit(0);
            }
        }
        if(startmenu.current_screen==StartMenuElements::screen::MAIN)
        {
            startmenu.start_menu(window_,menu_background, up, down);
        }
        else if(startmenu.current_screen==StartMenuElements::screen::LEADERBOARD)
        {
            startmenu.ViewLeaderboard(window_,menu_background, up, down);
        }
        
        if(startmenu.startgame())
        {
            music.stop();
            return;
        }
        window_.display();
    }
}
void Engine::RunGame()
{
    sf::Music music;
    if (!music.openFromFile("resources/The Good Fight (w intro).ogg"))
    {
        std::cout << "Music not found !" << std::endl;
    }
    music.setLoop(true);
    music.setVolume(50);
    music.play();
    bool p_down = false;
    bool q_down = false;
    bool gameend = false;
    window_.clear(sf::Color(240,240,220));
    std::cout << "RunGame()" << std::endl;
    Cannon cannon(30);
    Background background(TexturesandSounds::background_texture);
    std::vector<Alien*> aliens;
    std::vector<AlienMunition*> AlienMunitions;
    std::vector<Missile*> missiles;
    std::vector<Wreckage*> wreckages;
    PowerUp* power_up=nullptr;
    sf::Time elapsed = Constants::clock.getElapsedTime() - Constants::stop_time;
    window_.setFramerateLimit(60);
    Constants::start_time = Constants::clock.getElapsedTime() - Constants::stop_time;
    PowerUp::last_spawn = Constants::clock.getElapsedTime() - Constants::stop_time;
    while(!gameend)
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            if(!q_down)
            {
                q_down = true;
                if(music.getStatus()==sf::Music::Playing)
                {
                    music.pause();
                }
                else
                {
                    music.play();
                }
            }  
        }
        else q_down = false;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::P) && !p_down)
        {
            p_down = true;
            PauseGame(p_down);
        }
        else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::P))
        {
            p_down = false;
        }
        window_.draw(background);
        gameend = GameLoop(cannon,aliens,wreckages, AlienMunitions, missiles, power_up, elapsed, music);
        window_.display();
    }
    music.stop();
    if(gameend)
    {
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
void Engine::PauseGame(bool &p_down)
{
    sf::Time time = Constants::clock.getElapsedTime();
    sf::Text text;
    text.setFont(Constants::font);
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::Black);
    text.setString("Paused");
    text.setPosition(0.45 * Constants::width,0.5 * Constants::height);
    window_.draw(text);
    window_.display();
    while((!sf::Keyboard::isKeyPressed(sf::Keyboard::P) || p_down) && window_.isOpen())
    {
        sf::Event event;
        while(window_.pollEvent(event))
        {
            if(event.type==sf::Event::Closed)
            {
                window_.close();
                exit(0);
            }
        }
        if(!sf::Keyboard::isKeyPressed(sf::Keyboard::P))
        {
            p_down = false;
        }
        sf::sleep(sf::Time(sf::milliseconds(50)));
    }
    p_down = true;
    Constants::stop_time += Constants::clock.getElapsedTime() - time;
}
bool Engine::GameLoop(Cannon &cannon, std::vector<Alien*> &aliens,std::vector<Wreckage*> &wreckages, std::vector<AlienMunition*> &AlienMunitions, std::vector<Missile*> &missiles, PowerUp* &power_up, sf::Time &elapsed, sf::Music &music)
{
    sf::Time frametime=Constants::clock.getElapsedTime() - Constants::stop_time-elapsed;
    elapsed = Constants::clock.getElapsedTime() - Constants::stop_time;
    sf::Event event;
    while(window_.pollEvent(event))
    {
        if(event.type==sf::Event::Closed)
        {
            music.stop();
            window_.close();
            exit(0);
        }
    }
    
    Update(cannon,aliens,wreckages, AlienMunitions, missiles, power_up, frametime);
    DrawObjects(cannon,aliens, wreckages, AlienMunitions, missiles, power_up);
    Spawn(aliens,AlienMunitions, missiles,power_up, frametime);
    Collisions(cannon,aliens, wreckages, AlienMunitions, missiles, power_up, frametime);
    DrawGameElements(cannon.health());
    if(cannon.health()<=0 || Alien::enemies_left<=0)
    {
        return true;
    }
    return false;
}
void Engine::DrawObjects(Cannon &cannon, std::vector<Alien*> &aliens,std::vector<Wreckage*> &wreckages, std::vector<AlienMunition*> &AlienMunitions, std::vector<Missile*> &missiles, PowerUp* &power_up)
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
    for(auto &wreckage:wreckages)
    {
        window_.draw(*wreckage);
    }
    if(power_up->exists)
    {
        window_.draw(*power_up);
    }
}
void Engine::DrawGameElements(int health)
{
    sf::Text text;
    text.setFont(Constants::font);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::Black);
    text.setString("Score: " + std::to_string(Cannon::score));
    text.setPosition(0.85 * Constants::width,0.05 * Constants::height);
    window_.draw(text);
    text.setString("Enemies left: " + std::to_string(Alien::enemies_left));
    text.setPosition(0.85 * Constants::width,0.1 * Constants::height);
    window_.draw(text);
    text.setString("Health: " + std::to_string(health));
    text.setPosition(0.85 * Constants::width,0.15 * Constants::height);
    window_.draw(text);

}
void Engine::Update(Cannon &cannon, std::vector<Alien*> &aliens,std::vector<Wreckage*> &wreckages, std::vector<AlienMunition*> &AlienMunitions, std::vector<Missile*> &missiles, PowerUp* &power_up, sf::Time &frametime)
{
    Missile::Fire(missiles, cannon);
    cannon.update(frametime);
    if(power_up->exists)
    {
        if(!power_up->update(frametime)) delete power_up;
    }
    for(auto &alien:aliens)
    {
        alien->update(frametime);
    }
    AlienMunitions.erase(std::remove_if(AlienMunitions.begin(),AlienMunitions.end(),[&](AlienMunition* munition){
        return !munition->update(frametime, cannon.getPosition().x);
    }),AlienMunitions.end());

    missiles.erase(std::remove_if(missiles.begin(),missiles.end(),[&](Missile* missile){
        return !missile->update(frametime);
    }),missiles.end());

    wreckages.erase(std::remove_if(wreckages.begin(),wreckages.end(),[&](Wreckage* wreckage){
        return !wreckage->update(frametime);
    }),wreckages.end());
    double a = (Constants::clock.getElapsedTime() - Constants::stop_time).asSeconds();
    int b = 8;
    bool turn = bool(int(a/b)%2);
    if(turn) Alien::position = b - fmod(a,b)+1;
    else Alien::position = fmod(a,b)+1;
    
}
void Engine::Collisions(Cannon &cannon, std::vector<Alien*> &aliens,std::vector<Wreckage*> &wreckages, std::vector<AlienMunition*> &AlienMunitions, std::vector<Missile*> &missiles, PowerUp* &power_up, sf::Time &frametime)
{
    int size = aliens.size()+AlienMunitions.size()+missiles.size();
    for(int i = 0; i < aliens.size();i++)
    {
        if(aliens[i]->getGlobalBounds().intersects(cannon.getGlobalBounds()))
        {
            cannon.hit(5);
            wreckages.push_back(new Wreckage(aliens[i]->getPosition().x,aliens[i]->getPosition().y));
            delete aliens[i];
            aliens.erase(aliens.begin()+i--);
        }
    }
    for(auto &munition:AlienMunitions)
    {
        AlienMunitions.erase(std::remove_if(AlienMunitions.begin(),AlienMunitions.end(),[&](AlienMunition* munition){
            return munition->collision(cannon, wreckages);
        }),AlienMunitions.end());
    }
    
    for(auto &missile:missiles)
    {
        
        missiles.erase(std::remove_if(missiles.begin(),missiles.end(),[&](Missile* missile){
            if(!power_up->exists) return missile->collision(aliens, wreckages);
            else return missile->collision(aliens, wreckages) || missile->collision(power_up, cannon);
        }),missiles.end());
    }

    if(size!=aliens.size()+AlienMunitions.size()+missiles.size())
    {
        TexturesandSounds::explo.play();
    }
}
void Engine::Spawn(std::vector<Alien*> &aliens, std::vector<AlienMunition*> &AlienMunitions, std::vector<Missile*> &missiles,PowerUp* &power_up, sf::Time &frametime)
{
    //spawn aliens
    if(enemies_to_spawn>0 && (Constants::clock.getElapsedTime() - Constants::stop_time-Alien::last_spawn).asSeconds()>2)
    {
        aliens.push_back(new Alien());
        enemies_to_spawn--;
    }

    //spawn bombs
    AlienMunition::Spawn(aliens,AlienMunitions, frametime);

    //spawn powerups
    if((Constants::clock.getElapsedTime() - Constants::stop_time-PowerUp::last_spawn).asSeconds()>15)
    {
        power_up = new PowerUp(rand()%2);
    }
}
void Engine::GameOver()
{
    std::cout << "GameOver()" << std::endl;
    window_.draw(*(new Background(TexturesandSounds::background_start_texture)));
    sf::Font font=Constants::font;
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::White);
    text.setString("Game Over");
    text.setPosition(0.4 * Constants::width,0.45 * Constants::height);
    window_.draw(text);
    text.setString("Press escape to exit");
    text.setPosition(0.4 * Constants::width,0.5 * Constants::height);
    window_.draw(text);
    window_.display();
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
}
void Engine::GameWon()
{
    std::cout << "GameWon()" << std::endl;
    window_.draw(*(new Background(TexturesandSounds::background_start_texture)));
    sf::Font font=Constants::font;
    sf::Time time = Constants::clock.getElapsedTime() - Constants::stop_time - Constants::start_time;
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::White);
    text.setString("You Won!");
    text.setPosition(0.4 * Constants::width,0.45 * Constants::height);
    window_.draw(text);
    text.setString("Score: " + std::to_string(Cannon::score));
    text.setPosition(0.4 * Constants::width,0.5 * Constants::height);
    window_.draw(text);
    text.setString("Time: " + std::to_string(time.asSeconds()) + " seconds");
    text.setPosition(0.4 * Constants::width,0.55 * Constants::height);
    window_.draw(text);
    
    text.setString("Press escape to exit");
    text.setPosition(0.4 * Constants::width,0.6 * Constants::height);
    window_.draw(text);
    window_.display();
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
    std::ofstream file;
    file.open("resources/scores.txt",std::ios::app);
    file << time.asSeconds() << " " << Cannon::score << std::endl;
    file.close();
}
