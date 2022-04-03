#ifndef GAME_H
#define GAME_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "game_level.h"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_LOSE,
    GAME_WIN,
    GAME_LVL2
};
// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100.0f, 100.0f);
// Initial velocity of the player paddle
const float PLAYER_VELOCITY(500.0f);

class Game {
   public:
    GameState State;
    bool Keys[1024];
    unsigned int Width, Height;
    std::vector<GameLevel> Levels;
    unsigned int Level;
    int score;
    int numleftcoins;
    int lvl1coins;
    int lvl2coins;
    int lvl3coins;
    int totcoins;
    bool newlevel;
    int light;
    glm::vec2 playerposition;
    Game(unsigned int width, unsigned int height);
    ~Game();

    void Init();
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void DoCollisions();
    void Assigndirections();
};

#endif