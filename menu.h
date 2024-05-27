#include"objects.h"
class StartMenuElements
{
    protected:
     int score_pos=0;
     int selected=0;
     static int highscore;
     double best_time;
     std::vector<std::pair<double,int>> leaderboard;
     std::vector<std::pair<double,int>> GetLeaderboard(char const *filename);
     bool start_game=false;
    public:
     bool startgame();
     void ViewLeaderboard(sf::RenderWindow &window, Background &start_background, bool &up, bool &down);
     enum screen{MAIN, LEADERBOARD, GAME};
     int current_screen;
     void start_menu(sf::RenderWindow &window, Background &start_background, bool &up, bool &down);

};