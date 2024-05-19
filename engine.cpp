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
    LoadTextures();
    LoadSounds();
}
Engine::~Engine()
{
    window_.close();
}


int Engine::highscore;
int Engine::score;
int Engine::enemies_left;
int Engine::enemies_to_spawn;
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
    TexturesandSounds::bomb_explo_sound.loadFromFile("resources/bomb.wav");
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
    window_.clear(sf::Color(240,240,220));
    std::cout << "RunGame()" << std::endl;
    std::vector<GraphicalObject*> objects;
    Cannon cannon(100);
    objects.push_back(&cannon);
    enemies_left = 40;
    enemies_to_spawn = 40;
    
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
