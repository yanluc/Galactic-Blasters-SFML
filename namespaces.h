#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<SFML/Window.hpp>
namespace TexturesandSounds
{
     extern sf::SoundBuffer explo_sound;
     extern sf::SoundBuffer bomb_explo_sound;
     extern sf::Texture background_texture;
     extern sf::Texture background_start_texture;
     extern sf::Texture cannon_texture;
     extern sf::Texture alien_texture;
     extern sf::Texture wreckage_texture;
     extern sf::Texture missile_texture;
     extern sf::Texture bomb_texture;
     extern sf::Texture heart_texture;
     extern sf::Texture shield_texture;
     extern sf::Sound explo;
};
namespace Constants
{
     extern sf::Font font;
     extern sf::Clock clock;
     extern int width, height;
     extern sf::Time start_time;
};