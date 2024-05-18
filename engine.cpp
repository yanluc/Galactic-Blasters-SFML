#include"engine.h"
#include<stdio.h>
#include<time.h>
#include<fstream>
#include<iostream>
Engine::Engine()
{
    srand(time(NULL));
    InitWindow();
    LoadTextures();
    LoadSounds();
}
Engine::~Engine()
{
    window_.close();
}
int Engine::grid[5][8];
sf::Clock Engine::clock;
int Engine::width=sf::VideoMode::getDesktopMode().width;
int Engine::height=sf::VideoMode::getDesktopMode().height;
sf::Texture Engine::background_texture;
sf::Texture Engine::alien_texture;
sf::Texture Engine::missile_texture;
sf::Texture Engine::bomb_texture;
sf::Texture Engine::cannon_texture;
sf::SoundBuffer Engine::bomb_explo_sound;
sf::SoundBuffer Engine::explo_sound;
int Engine::highscore;
int Engine::score;
int Engine::enemies_left;
int Engine::enemies_to_spawn;
void Engine::LoadTextures()
{
    background_texture.loadFromFile("resources/nightsky.png");
    cannon_texture.loadFromFile("resources/cannon.png");
    alien_texture.loadFromFile("resources/alien.png");
    missile_texture.loadFromFile("resources/missile.png");
    bomb_texture.loadFromFile("resources/bomb.png");

}
void Engine::LoadSounds()
{
    explo_sound.loadFromFile("resources/bomb.wav");
    bomb_explo_sound.loadFromFile("resources/bomb.wav");
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
    window_.create(sf::VideoMode(width,height),"Galactic Blasters");
}
void Engine::StartMenu()
{
    bool up = false;
    bool down = false;
    int score_pos=0;
    std::cout << "xd" << std::endl;
    std::vector<std::pair<double,int>> leaderboard = Leaderboard("resources/scores.txt");
    sf::Font font;
    if (!font.loadFromFile("resources/COMICSANS.TTF"))
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
        text.setFont(font);
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::Black);
        text.setString("space to shoot, arrows to control, press space to continue");
        text.setPosition(0.2 * width,0.7 * height);
        window_.draw(text);
        if(best_time!=9999999)
        {
            text.setCharacterSize(30);
            text.setFillColor(sf::Color::Black);
            text.setString("best time:");
            text.setPosition(0.2 * width,0.5*height);
            window_.draw(text);
            std::string tmp = "x";// std::format("{:.3f}",best_time);
            tmp = tmp + " seconds";
            text.setString(tmp);
            text.setPosition(0.3 * width,0.5*height);
            window_.draw(text);

            text.setCharacterSize(30);
            text.setFillColor(sf::Color::Black);
            text.setString("highscore:");
            text.setPosition(0.2 * width,0.55*height);
            window_.draw(text);
            tmp = std::to_string(highscore);
            text.setString(tmp);
            text.setPosition(0.3 * width,0.55*height);
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
            text.setPosition(0.8 * width,0.2*height+0.05*height*(i-score_pos));
            window_.draw(text);
            text.setString(std::to_string(leaderboard[i].first));
            text.setPosition(0.9 * width,0.2*height+0.05*height*(i-score_pos));
            window_.draw(text);
        }
        window_.display();
    }
}
void Engine::RunGame()
{
    // InitWindow();
    window_.clear(sf::Color(240,240,220));
    std::cout << "lmao" << std::endl;
    while(window_.isOpen())
    {
        window_.clear(sf::Color(240,240,220));
        GameLoop();
        window_.display();
    }
}
void Engine::GameLoop()
{
    sf::Event event;
    
    while(window_.pollEvent(event))
    {
        if(event.type==sf::Event::Closed)
        {
            window_.close();
        }
    }
    //core game loop
    
}
