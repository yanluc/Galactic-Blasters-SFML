#include "namespaces.h"
namespace TexturesandSounds
{
    sf::SoundBuffer explo_sound;
    sf::SoundBuffer bomb_explo_sound;
    sf::Texture background_texture;
    sf::Texture background_start_texture;
    sf::Texture cannon_texture;
    sf::Texture alien_texture;
    sf::Texture wreckage_texture;
    sf::Texture missile_texture;
    sf::Texture bomb_texture;
    sf::Texture heart_texture;
    sf::Texture shield_texture;
    sf::Texture missile_texture_animated[10];
    sf::Sound explo;
};
namespace Constants
{
    sf::Font font;
    sf::Clock clock;
    sf::Time stop_time = sf::seconds(0);
    int width = sf::VideoMode::getDesktopMode().width, height = sf::VideoMode::getDesktopMode().height;
    sf::Time start_time;
};