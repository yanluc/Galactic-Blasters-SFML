#include "menu.hpp"
int StartMenuElements::highscore=-10000;
std::vector<std::pair<double,int>> StartMenuElements::GetLeaderboard(char const *filename)
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
bool StartMenuElements::startgame()
{
    return start_game;
}
void StartMenuElements::start_menu(sf::RenderWindow &window, Background &start_background, bool &up, bool &down)
{
    sf::Color color(200,200,200);
    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && down){down=false;}
    else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && up){ up=false;}

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !down){ selected++; down=true;}
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !up){ selected--; up=true;}
    if(selected<0)selected=0;
    if(selected>2)selected=2;

    window.draw(start_background);
    leaderboard = GetLeaderboard("resources/scores.txt");
    sf::Text text;
    text.setFont(Constants::font);
    text.setCharacterSize(50);
    
    text.setFillColor(sf::Color::White);
    text.setPosition(Constants::width*0.4,Constants::height*0.1);
    text.setString("Galactic Blasters");
    window.draw(text);
    text.setCharacterSize(30);
    text.setPosition(Constants::width*0.4,Constants::height*0.3);
    if(selected == 0) text.setFillColor(color);
    else text.setFillColor(sf::Color::White);
    text.setString("Start");
    window.draw(text);
    text.setPosition(Constants::width*0.4,Constants::height*0.4);
    if(selected == 1) text.setFillColor(color);
    else text.setFillColor(sf::Color::White);
    text.setString("View previous scores");
    window.draw(text);
    text.setPosition(Constants::width*0.4,Constants::height*0.5);
    if(selected == 2) text.setFillColor(color);
    else text.setFillColor(sf::Color::White);
    text.setString("Exit");
    window.draw(text);

    if(selected==0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) start_game=true;
    if(selected==1 && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) current_screen=LEADERBOARD;
    if(selected==2 && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)){ window.close(); exit(0);}
}
void StartMenuElements::ViewLeaderboard(sf::RenderWindow &window, Background &start_background , bool &up, bool &down)
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) current_screen=MAIN;
    std::vector<std::pair<double,int>> scores = GetLeaderboard("leaderboard.txt");
    sf::Text text;
    window.draw(start_background);
    text.setFont(Constants::font);
    text.setCharacterSize(Constants::height/20);
    text.setFillColor(sf::Color::White);
    text.setPosition(Constants::width*0.4,Constants::height*0.1);
    text.setString("Previous scores");
    window.draw(text);

    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && down){down=false;}
    else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && up){ up=false;}

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
            text.setPosition(0.4 *Constants::width,0.2*Constants::height+0.05*Constants::height*(i-score_pos));
            window.draw(text);
            text.setString(std::to_string(leaderboard[i].first));
            text.setPosition(0.5 * Constants::width,0.2*Constants::height+0.05*Constants::height*(i-score_pos));
            window.draw(text);
        }
}