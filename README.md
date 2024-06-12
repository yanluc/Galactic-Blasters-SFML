# Galactic-Blasters-SFML

### Compile with:

> g++ namespaces.cpp objects.cpp menu.cpp engine.cpp main.cpp -o game -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -O3

## Instruction:

### Controls:
#### Start Menu:
Use up and down arrows to navigate and enter to select option.
To go back press escape.

#### Game:
Use right and left arrows to move, press space to shoot.
To pause or unpause press P key.
To pause or unpause music press Q key.

### Game mechanics:
#### Aliens:
At the beginning screen will be empty, but every 2 seconds new alien will spawn. They will keep coming until all 32 ships have been spawned.
Randomly enemy ships will drop bombs - either standard or guided. Also, sometimes they will conduct kamikaze attacks, that will deal more damage.

#### Powerups:
Every 15 seconds powerup will appear. Depending on the type, if you manage to shoot it down you will get either additional hp point or shield, protecting you from any damage for 6 seconds.

#### Score:
You will get points for shooting down enemy ships and powerups, but will loose points for getting hit.

#### Game end
Game ends when either all the aliens are destroyed or your hp drops to 0.
