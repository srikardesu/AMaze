#include "game.h"

#include <bits/stdc++.h>
#include <unistd.h>

#include <ctime>
#include <fstream>
#include <iostream>

#include "game_level.h"
#include "game_object.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "text_renderer.h"
#include "time.h"

SpriteRenderer *Renderer;
GameObject *Player;
GameObject *Monster_lvl1[2];
GameObject *Monster_lvl2[3];
GameObject *Monster_lvl3[4];
TextRenderer *Text;
int START;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height) {
}

Game::~Game() {
    // delete Renderer;
    delete Player;
}
float RandomFloat(float a, float b) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}
void Game::Init() {
    srand(static_cast<unsigned int>(time(0)));
    this->light = 0;
    START = time(NULL);
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("fonts/OCRAEXT.TTF", 24);
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    // ResourceManager::LoadTexture("textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("textures/game.jpg", false, "background");
    ResourceManager::LoadTexture("textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("textures/block.png", false, "block");
    ResourceManager::LoadTexture("textures/block_solid.png", false, "block_solid");
    // ResourceManager::LoadTexture("textures/paddle.png", true, "paddle");
    ResourceManager::LoadTexture("textures/paddle.png", false, "paddle");
    ResourceManager::LoadTexture("textures/lose.jpg", false, "lose");
    ResourceManager::LoadTexture("textures/win.jpg", false, "win");
    // load levels

    GameLevel one;
    GameLevel two;
    GameLevel three;
    int mat[20][20];
    int cnt = 0;
    // take 20 bricks ig?
    int arr[20];
    for (int i = 0; i < 20; i++) {
        arr[i] = rand() % 360;  // 0 to 360
    }
    int coins[10];
    for (int i = 0; i < 10; i++) {
        coins[i] = rand() % 360;
    }
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            mat[i][j] = 0;
        }
    }
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            int val = i * 20 + j;
            bool f = false;
            bool g = false;
            for (int i = 0; i < 20; i++) {
                if (val == arr[i]) f = true;
            }
            for (int i = 0; i < 10; i++) {
                if (val == coins[i] && !f) {
                    g = true;
                }
            }
            if (f) {
                mat[i][j] = 1;
            }
            if (g) {
                mat[i][j] = 4;
            }
        }
    }
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (i == 0 || i == 19 || j == 0 || j == 19) mat[i][j] = 1;
        }
    }
    mat[19][19] = 0;
    mat[19][18] = 0;
    mat[18][19] = 0;
    mat[0][0] = 0;
    mat[0][1] = 0;
    mat[1][0] = 0;
    mat[1][1] = 0;

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (mat[i][j] == 4) cnt++;
        }
    }

    this->score = 0;
    this->numleftcoins = cnt;
    this->totcoins = cnt;
    this->newlevel = false;
    this->lvl1coins = cnt;
    float x, y;
    float h, w;

    std::freopen("levels/one.lvl", "w", stdout);
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            std::cout << mat[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::fclose(stdout);
    one.Load("levels/one.lvl", this->Width, this->Height);
    int cnt2 = 0;
    int mat2[20][20];
    // take 30 bricks ig?
    int arr2[30];
    for (int i = 0; i < 30; i++) {
        arr2[i] = rand() % 360;  // 0 to 360
    }
    int coins2[15];
    for (int i = 0; i < 15; i++) {
        coins2[i] = rand() % 360;
    }
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            mat2[i][j] = 0;
        }
    }
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            int val = i * 20 + j;
            bool f = false;
            bool g = false;
            for (int i = 0; i < 30; i++) {
                if (val == arr2[i]) f = true;
            }
            for (int i = 0; i < 15; i++) {
                if (val == coins2[i] && !f) {
                    g = true;
                }
            }
            if (f) {
                mat2[i][j] = 1;
            }
            if (g) {
                mat2[i][j] = 4;
            }
        }
    }

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (i == 0 || i == 19 || j == 0 || j == 19) mat2[i][j] = 1;
        }
    }
    mat2[19][19] = 0;
    mat2[19][18] = 0;
    mat2[18][19] = 0;
    mat2[0][0] = 0;
    mat2[0][1] = 0;
    mat2[1][0] = 0;
    mat2[1][1] = 0;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (mat2[i][j] == 4) cnt2++;
        }
    }
    std::freopen("levels/two.lvl", "w", stdout);
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            std::cout << mat2[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::fclose(stdout);
    two.Load("levels/two.lvl", this->Width, this->Height);
    int sz = one.Bricks.size();
    this->lvl2coins = cnt2;
    this->numleftcoins += cnt2;
    this->totcoins += cnt2;

    int cnt3 = 0;
    int mat3[20][20];
    // take 35 bricks ig?
    int arr3[35];
    for (int i = 0; i < 35; i++) {
        arr3[i] = rand() % 360;  // 0 to 360
    }
    int coins3[20];
    for (int i = 0; i < 20; i++) {
        coins3[i] = rand() % 360;
    }
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            mat3[i][j] = 0;
        }
    }
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            int val = i * 20 + j;
            bool f = false;
            bool g = false;
            for (int i = 0; i < 35; i++) {
                if (val == arr3[i]) f = true;
            }
            for (int i = 0; i < 20; i++) {
                if (val == coins3[i] && !f) {
                    g = true;
                }
            }
            if (f) {
                mat3[i][j] = 1;
            }
            if (g) {
                mat3[i][j] = 4;
            }
        }
    }

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (i == 0 || i == 19 || j == 0 || j == 19) mat3[i][j] = 1;
        }
    }
    mat3[19][19] = 0;
    mat3[19][18] = 0;
    mat3[18][19] = 0;
    mat3[0][0] = 0;
    mat3[0][1] = 0;
    mat3[1][0] = 0;
    mat3[1][1] = 0;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (mat3[i][j] == 4) cnt3++;
        }
    }
    std::freopen("levels/three.lvl", "w", stdout);
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            std::cout << mat3[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::fclose(stdout);
    three.Load("levels/three.lvl", this->Width, this->Height);
    ////
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);

    this->lvl3coins = cnt3;
    this->numleftcoins += cnt3;
    this->totcoins += cnt3;

    glm::vec2 playerPos = glm::vec2(0, 0);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    // x = RandomFloat(0, this->Width - PLAYER_SIZE.x);
    // y = RandomFloat(0, this->Height - PLAYER_SIZE.y);
    // while (1) {
    //     x = RandomFloat(0, this->Width - PLAYER_SIZE.x);
    //     y = RandomFloat(0, this->Height - PLAYER_SIZE.y);
    //     bool f = false;
    //     for (GameObject &box : this->Levels[1].Bricks) {
    //         if (!box.Destroyed) {
    //             if ((float)x >= box.Position.x - box.Size.x / 1.5f && (float)x <= box.Position.x + box.Size.x / 1.5f) {
    //                 f = true;
    //             }
    //             if ((float)y >= box.Position.y - box.Size.y / 1.5f && (float)y <= box.Position.y + box.Size.y / 1.5f) {
    //                 f = true;
    //             }
    //         }
    //     }
    //     if (!f) break;
    // }

    h = this->Levels[this->Level].height;
    w = this->Levels[this->Level].width;
    while (1) {
        int xx, yy;
        xx = rand() % 20;
        yy = rand() % 20;
        if (mat[xx][yy] == 1)
            continue;
        else {
            x = xx;
            y = yy;
            break;
        }
    }
    glm::vec2 Monster_lvl1Pos = glm::vec2((float)x * w, (float)y * h);
    Monster_lvl1[0] = new GameObject(Monster_lvl1Pos, PLAYER_SIZE, ResourceManager::GetTexture("face"));
    Monster_lvl1[0]->Velocity = glm::vec2(1, 1);

    h = this->Levels[this->Level].height;
    w = this->Levels[this->Level].width;
    while (1) {
        int xx, yy;
        xx = rand() % 20;
        yy = rand() % 20;
        if (mat[xx][yy] == 1)
            continue;
        else {
            x = xx;
            y = yy;
            break;
        }
    }
    Monster_lvl1Pos = glm::vec2((float)x * w, (float)y * h);
    Monster_lvl1[1] = new GameObject(Monster_lvl1Pos, PLAYER_SIZE, ResourceManager::GetTexture("face"));
    Monster_lvl1[1]->Velocity = glm::vec2(1, 1);

    h = this->Levels[1].height;
    w = this->Levels[1].width;
    while (1) {
        int xx, yy;
        xx = rand() % 20;
        yy = rand() % 20;
        if (mat2[xx][yy] == 1)
            continue;
        else {
            x = xx;
            y = yy;
            break;
        }
    }
    Monster_lvl1Pos = glm::vec2((float)x * w, (float)y * h);
    Monster_lvl2[0] = new GameObject(Monster_lvl1Pos, PLAYER_SIZE, ResourceManager::GetTexture("face"));
    Monster_lvl2[0]->Velocity = glm::vec2(1, 1);

    h = this->Levels[1].height;
    w = this->Levels[1].width;
    while (1) {
        int xx, yy;
        xx = rand() % 20;
        yy = rand() % 20;
        if (mat2[xx][yy] == 1)
            continue;
        else {
            x = xx;
            y = yy;
            break;
        }
    }
    Monster_lvl1Pos = glm::vec2((float)x * w, (float)y * h);
    Monster_lvl2[1] = new GameObject(Monster_lvl1Pos, PLAYER_SIZE, ResourceManager::GetTexture("face"));
    Monster_lvl2[1]->Velocity = glm::vec2(1, 1);

    h = this->Levels[1].height;
    w = this->Levels[1].width;
    while (1) {
        int xx, yy;
        xx = rand() % 20;
        yy = rand() % 20;
        if (mat2[xx][yy] == 1)
            continue;
        else {
            x = xx;
            y = yy;
            break;
        }
    }
    Monster_lvl1Pos = glm::vec2((float)x * w, (float)y * h);
    Monster_lvl2[2] = new GameObject(Monster_lvl1Pos, PLAYER_SIZE, ResourceManager::GetTexture("face"));
    Monster_lvl2[2]->Velocity = glm::vec2(1, 1);

    h = this->Levels[2].height;
    w = this->Levels[2].width;
    while (1) {
        int xx, yy;
        xx = rand() % 20;
        yy = rand() % 20;
        if (mat3[xx][yy] == 1)
            continue;
        else {
            x = xx;
            y = yy;
            break;
        }
    }
    Monster_lvl1Pos = glm::vec2((float)x * w, (float)y * h);
    Monster_lvl3[0] = new GameObject(Monster_lvl1Pos, PLAYER_SIZE, ResourceManager::GetTexture("face"));
    Monster_lvl3[0]->Velocity = glm::vec2(1, 1);

    h = this->Levels[2].height;
    w = this->Levels[2].width;
    while (1) {
        int xx, yy;
        xx = rand() % 20;
        yy = rand() % 20;
        if (mat3[xx][yy] == 1)
            continue;
        else {
            x = xx;
            y = yy;
            break;
        }
    }
    Monster_lvl1Pos = glm::vec2((float)x * w, (float)y * h);
    Monster_lvl3[1] = new GameObject(Monster_lvl1Pos, PLAYER_SIZE, ResourceManager::GetTexture("face"));
    Monster_lvl3[1]->Velocity = glm::vec2(1, 1);

    h = this->Levels[2].height;
    w = this->Levels[2].width;
    while (1) {
        int xx, yy;
        xx = rand() % 20;
        yy = rand() % 20;
        if (mat3[xx][yy] == 1)
            continue;
        else {
            x = xx;
            y = yy;
            break;
        }
    }
    Monster_lvl1Pos = glm::vec2((float)x * w, (float)y * h);
    Monster_lvl3[2] = new GameObject(Monster_lvl1Pos, PLAYER_SIZE, ResourceManager::GetTexture("face"));
    Monster_lvl3[2]->Velocity = glm::vec2(1, 1);

    h = this->Levels[2].height;
    w = this->Levels[2].width;
    while (1) {
        int xx, yy;
        xx = rand() % 20;
        yy = rand() % 20;
        if (mat3[xx][yy] == 1)
            continue;
        else {
            x = xx;
            y = yy;
            break;
        }
    }
    Monster_lvl1Pos = glm::vec2((float)x * w, (float)y * h);
    Monster_lvl3[3] = new GameObject(Monster_lvl1Pos, PLAYER_SIZE, ResourceManager::GetTexture("face"));
    Monster_lvl3[3]->Velocity = glm::vec2(1, 1);
}

bool CheckCollision(GameObject &one, GameObject &two)  // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x > two.Position.x &&
                      two.Position.x + two.Size.x > one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y > two.Position.y &&
                      two.Position.y + two.Size.y > one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

void Game::DoCollisions() {
    bool f_l = false;
    bool f_r = false;
    bool f_u = false;
    bool f_d = false;
    for (GameObject &box : this->Levels[this->Level].Bricks) {
        if (!box.Destroyed) {
            if (CheckCollision(*Player, box)) {
                if (!box.IsSolid) {
                    box.Destroyed = true;
                    if (!this->light)
                        this->score += 10;
                    else
                        this->score += 20;
                    this->numleftcoins--;
                } else {
                    if (Player->leftpress) {
                        f_l = true;
                        Player->Position.x = box.Position.x + Player->Size.x + box.Size.x / 4.0f;
                    }
                    if (Player->rightpress) {
                        f_r = true;
                        Player->Position.x = box.Position.x - Player->Size.x;
                    }
                    if (Player->uppress) {
                        f_u = true;
                        Player->Position.y = box.Position.y + Player->Size.y;
                    }
                    if (Player->downpress) {
                        f_d = true;
                        Player->Position.y = box.Position.y - Player->Size.y;
                    }
                }
            }
            if (box.IsSolid) {
                if (this->Level == 0) {
                    if (CheckCollision(*Monster_lvl1[0], box)) {
                        if (Monster_lvl1[0]->dir == 0) {  // right
                            Monster_lvl1[0]->dir = 2;
                            Monster_lvl1[0]->Position.x = box.Position.x - Monster_lvl1[0]->Size.x;
                        } else if (Monster_lvl1[0]->dir == 1) {  // up
                            Monster_lvl1[0]->dir = 3;
                            Monster_lvl1[0]->Position.y = box.Position.y + Monster_lvl1[0]->Size.y;
                        } else if (Monster_lvl1[0]->dir == 2) {  // left
                            Monster_lvl1[0]->dir = 0;
                            Monster_lvl1[0]->Position.x = box.Position.x + Monster_lvl1[0]->Size.x + box.Size.x / 4.0f;
                        } else if (Monster_lvl1[0]->dir == 3) {  // down
                            Monster_lvl1[0]->dir = 1;
                            Monster_lvl1[0]->Position.y = box.Position.y - Monster_lvl1[0]->Size.y;
                        }
                    }
                    if (CheckCollision(*Monster_lvl1[1], box)) {
                        if (Monster_lvl1[1]->dir == 0) {
                            Monster_lvl1[1]->dir = 2;
                            Monster_lvl1[1]->Position.x = box.Position.x - Monster_lvl1[1]->Size.x;
                        } else if (Monster_lvl1[1]->dir == 1) {
                            Monster_lvl1[1]->dir = 3;
                            Monster_lvl1[1]->Position.y = box.Position.y + Monster_lvl1[1]->Size.y;
                        } else if (Monster_lvl1[1]->dir == 2) {
                            Monster_lvl1[1]->dir = 0;
                            Monster_lvl1[1]->Position.x = box.Position.x + Monster_lvl1[1]->Size.x + box.Size.x / 4.0f;
                        } else if (Monster_lvl1[1]->dir == 3) {
                            Monster_lvl1[1]->dir = 1;
                            Monster_lvl1[1]->Position.y = box.Position.y - Monster_lvl1[1]->Size.y;
                        }
                    }
                } else if (this->Level == 1) {
                    for (int i = 0; i < 3; i++) {
                        if (CheckCollision(*Monster_lvl2[i], box)) {
                            if (Monster_lvl2[i]->dir == 0) {
                                Monster_lvl2[i]->dir = 2;
                                Monster_lvl2[i]->Position.x = box.Position.x - Monster_lvl2[i]->Size.x;
                            } else if (Monster_lvl2[i]->dir == 1) {
                                Monster_lvl2[i]->dir = 3;
                                Monster_lvl2[i]->Position.y = box.Position.y + Monster_lvl2[i]->Size.y;
                            } else if (Monster_lvl2[i]->dir == 2) {
                                Monster_lvl2[i]->dir = 0;
                                Monster_lvl2[i]->Position.x = box.Position.x + Monster_lvl2[i]->Size.x + box.Size.x / 4.0f;
                            } else if (Monster_lvl2[i]->dir == 3) {
                                Monster_lvl2[i]->dir = 1;
                                Monster_lvl2[i]->Position.y = box.Position.y - Monster_lvl2[i]->Size.y;
                            }
                        }
                    }
                } else if (this->Level == 2) {
                    for (int i = 0; i < 4; i++) {
                        if (CheckCollision(*Monster_lvl3[i], box)) {
                            if (Monster_lvl3[i]->dir == 0) {
                                Monster_lvl3[i]->dir = 2;
                                Monster_lvl3[i]->Position.x = box.Position.x - Monster_lvl3[i]->Size.x;
                            } else if (Monster_lvl3[i]->dir == 1) {
                                Monster_lvl3[i]->dir = 3;
                                Monster_lvl3[i]->Position.y = box.Position.y + Monster_lvl3[i]->Size.y;
                            } else if (Monster_lvl3[i]->dir == 2) {
                                Monster_lvl3[i]->dir = 0;
                                Monster_lvl3[i]->Position.x = box.Position.x + Monster_lvl3[i]->Size.x + box.Size.x / 4.0f;
                            } else if (Monster_lvl3[i]->dir == 3) {
                                Monster_lvl3[i]->dir = 1;
                                Monster_lvl3[i]->Position.y = box.Position.y - Monster_lvl3[i]->Size.y;
                            }
                        }
                    }
                }
            }
        }
    }
    if (f_l) {
        Player->left = false;
    }
    if (f_r) {
        Player->right = false;
    }
    if (f_u) {
        Player->up = false;
    }
    if (f_d) {
        Player->down = false;
    }
    if (!f_l && !f_r && !f_d && !f_u) {
        Player->left = true;
        Player->right = true;
        Player->up = true;
        Player->down = true;
    }
}

void Game::Assigndirections() {
    if (this->Level == 0) {
        if (Monster_lvl1[0]->mv >= 0)
            Monster_lvl1[0]->mv--;
        else {
            for (int i = 0; i < 2; i++) {
                int val = rand() % 4;
                Monster_lvl1[i]->dir = val;
            }
            Monster_lvl1[0]->mv = 20;
        }
    } else if (this->Level == 1) {
        if (Monster_lvl2[0]->mv >= 0)
            Monster_lvl2[0]->mv--;
        else {
            for (int i = 0; i < 3; i++) {
                int val = rand() % 4;
                Monster_lvl2[i]->dir = val;
            }
            Monster_lvl2[0]->mv = 20;
        }
    } else if (this->Level == 2) {
        if (Monster_lvl3[0]->mv >= 0)
            Monster_lvl3[0]->mv--;
        else {
            for (int i = 0; i < 3; i++) {
                int val = rand() % 4;
                Monster_lvl3[i]->dir = val;
            }
            Monster_lvl3[0]->mv = 20;
        }
    }
}

void Game::Update(float dt) {
    this->Assigndirections();
    float velocity = PLAYER_VELOCITY * dt;
    if (this->State == GAME_ACTIVE) {
        this->playerposition = Player->Position;
    }
    if (this->Level == 0) {
        for (int i = 0; i < 2; i++) {
            if (Monster_lvl1[i]->dir == 0) {
                Monster_lvl1[i]->Position.x += velocity;
            } else if (Monster_lvl1[i]->dir == 1) {
                Monster_lvl1[i]->Position.y -= velocity;
            } else if (Monster_lvl1[i]->dir == 2) {
                Monster_lvl1[i]->Position.x -= velocity;
            } else if (Monster_lvl1[i]->dir == 3) {
                Monster_lvl1[i]->Position.y += velocity;
            }
            // Monster_lvl1[i]->Position = Monster_lvl1[i]->Position + Monster_lvl1[i]->Velocity * velocity;
            if (Monster_lvl1[i]->Position.x <= 0.0f) {
                Monster_lvl1[i]->Velocity.x = -Monster_lvl1[i]->Velocity.x;
                Monster_lvl1[i]->Position.x = 0.0f;
            } else if (Monster_lvl1[i]->Position.x + Monster_lvl1[i]->Size.x >= this->Width) {
                Monster_lvl1[i]->Velocity.x = -Monster_lvl1[i]->Velocity.x;
                Monster_lvl1[i]->Position.x = this->Width - Monster_lvl1[i]->Size.x;
            }
            if (Monster_lvl1[i]->Position.y <= 0.0f) {
                Monster_lvl1[i]->Velocity.y = -Monster_lvl1[i]->Velocity.y;
                Monster_lvl1[i]->Position.y = 0.0f;
            } else if (Monster_lvl1[i]->Position.y + Monster_lvl1[i]->Size.y >= this->Height) {
                Monster_lvl1[i]->Velocity.y = -Monster_lvl1[i]->Velocity.y;
                Monster_lvl1[i]->Position.y = this->Height - Monster_lvl1[i]->Size.y;
            }
        }
    } else if (this->Level == 1) {
        for (int i = 0; i < 3; i++) {
            if (Monster_lvl2[i]->dir == 0) {
                Monster_lvl2[i]->Position.x += velocity;  // right
            } else if (Monster_lvl2[i]->dir == 1) {
                Monster_lvl2[i]->Position.y -= velocity;  // up
            } else if (Monster_lvl2[i]->dir == 2) {
                Monster_lvl2[i]->Position.x -= velocity;  // left
            } else if (Monster_lvl2[i]->dir == 3) {
                Monster_lvl2[i]->Position.y += velocity;  // down
            }
            // Monster_lvl1[i]->Position = Monster_lvl1[i]->Position + Monster_lvl1[i]->Velocity * velocity;
            if (Monster_lvl2[i]->Position.x <= 0.0f) {
                Monster_lvl2[i]->Velocity.x = -Monster_lvl2[i]->Velocity.x;
                Monster_lvl2[i]->Position.x = 0.0f;
            } else if (Monster_lvl2[i]->Position.x + Monster_lvl2[i]->Size.x >= this->Width) {
                Monster_lvl2[i]->Velocity.x = -Monster_lvl2[i]->Velocity.x;
                Monster_lvl2[i]->Position.x = this->Width - Monster_lvl2[i]->Size.x;
            }
            if (Monster_lvl2[i]->Position.y <= 0.0f) {
                Monster_lvl2[i]->Velocity.y = -Monster_lvl2[i]->Velocity.y;
                Monster_lvl2[i]->Position.y = 0.0f;
            } else if (Monster_lvl2[i]->Position.y + Monster_lvl2[i]->Size.y >= this->Height) {
                Monster_lvl2[i]->Velocity.y = -Monster_lvl2[i]->Velocity.y;
                Monster_lvl2[i]->Position.y = this->Height - Monster_lvl2[i]->Size.y;
            }
        }
    } else if (this->Level == 2) {
        for (int i = 0; i < 4; i++) {
            if (Monster_lvl3[i]->dir == 0) {
                Monster_lvl3[i]->Position.x += velocity;  // right
            } else if (Monster_lvl3[i]->dir == 1) {
                Monster_lvl3[i]->Position.y -= velocity;  // up
            } else if (Monster_lvl3[i]->dir == 2) {
                Monster_lvl3[i]->Position.x -= velocity;  // left
            } else if (Monster_lvl3[i]->dir == 3) {
                Monster_lvl3[i]->Position.y += velocity;  // down
            }
            // Monster_lvl1[i]->Position = Monster_lvl1[i]->Position + Monster_lvl1[i]->Velocity * velocity;
            if (Monster_lvl3[i]->Position.x <= 0.0f) {
                Monster_lvl3[i]->Velocity.x = -Monster_lvl3[i]->Velocity.x;
                Monster_lvl3[i]->Position.x = 0.0f;
            } else if (Monster_lvl3[i]->Position.x + Monster_lvl3[i]->Size.x >= this->Width) {
                Monster_lvl3[i]->Velocity.x = -Monster_lvl3[i]->Velocity.x;
                Monster_lvl3[i]->Position.x = this->Width - Monster_lvl3[i]->Size.x;
            }
            if (Monster_lvl3[i]->Position.y <= 0.0f) {
                Monster_lvl3[i]->Velocity.y = -Monster_lvl3[i]->Velocity.y;
                Monster_lvl3[i]->Position.y = 0.0f;
            } else if (Monster_lvl3[i]->Position.y + Monster_lvl3[i]->Size.y >= this->Height) {
                Monster_lvl3[i]->Velocity.y = -Monster_lvl3[i]->Velocity.y;
                Monster_lvl3[i]->Position.y = this->Height - Monster_lvl3[i]->Size.y;
            }
        }
    }
    this->DoCollisions();
    // if (this->numleftcoins == 0) {
    //     this->State = GAME_LVL2;
    // }
}

void Game::ProcessInput(float dt) {
    if (this->State == GAME_ACTIVE) {
        float velocity = PLAYER_VELOCITY * dt;
        if (this->Level == 0) {
            for (int i = 0; i < 2; i++) {
                // if (!Monster_lvl1[i]->left) {
                //     Monster_lvl1[i]->Position.x += velocity;
                //     // Monster_lvl1[i]->Position += velocity * Monster_lvl1[i]->Velocity;
                //     if (Monster_lvl1[i]->Position.x >= this->Width - Monster_lvl1[i]->Size.x) {
                //         Monster_lvl1[i]->left = !(Monster_lvl1[i]->left);
                //     }
                // } else {
                //     Monster_lvl1[i]->Position.x -= velocity;
                //     // Monster_lvl1[i]->Position -= velocity * Monster_lvl1[i]->Velocity;
                //     if (Monster_lvl1[i]->Position.x <= 0.0f) {
                //         Monster_lvl1[i]->left = !(Monster_lvl1[i]->left);
                //     }
                // }
                // Monster_lvl1[i]->Position = Monster_lvl1[i]->Position + Monster_lvl1[i]->Velocity * dt;
                if (this->Level == 0) {
                    if (CheckCollision(*(Monster_lvl1[i]), *Player)) {
                        this->State = GAME_LOSE;
                    }
                }
            }
        } else if (this->Level == 1) {
            for (int i = 0; i < 3; i++) {
                // if (!Monster_lvl1[i]->left) {
                //     Monster_lvl1[i]->Position.x += velocity;
                //     // Monster_lvl1[i]->Position += velocity * Monster_lvl1[i]->Velocity;
                //     if (Monster_lvl1[i]->Position.x >= this->Width - Monster_lvl1[i]->Size.x) {
                //         Monster_lvl1[i]->left = !(Monster_lvl1[i]->left);
                //     }
                // } else {
                //     Monster_lvl1[i]->Position.x -= velocity;
                //     // Monster_lvl1[i]->Position -= velocity * Monster_lvl1[i]->Velocity;
                //     if (Monster_lvl1[i]->Position.x <= 0.0f) {
                //         Monster_lvl1[i]->left = !(Monster_lvl1[i]->left);
                //     }
                // }
                // Monster_lvl1[i]->Position = Monster_lvl1[i]->Position + Monster_lvl1[i]->Velocity * dt;
                if (this->Level == 1) {
                    if (CheckCollision(*(Monster_lvl2[i]), *Player)) {
                        this->State = GAME_LOSE;
                    }
                }
            }
        } else if (this->Level == 2) {
            for (int i = 0; i < 4; i++) {
                // if (!Monster_lvl1[i]->left) {
                //     Monster_lvl1[i]->Position.x += velocity;
                //     // Monster_lvl1[i]->Position += velocity * Monster_lvl1[i]->Velocity;
                //     if (Monster_lvl1[i]->Position.x >= this->Width - Monster_lvl1[i]->Size.x) {
                //         Monster_lvl1[i]->left = !(Monster_lvl1[i]->left);
                //     }
                // } else {
                //     Monster_lvl1[i]->Position.x -= velocity;
                //     // Monster_lvl1[i]->Position -= velocity * Monster_lvl1[i]->Velocity;
                //     if (Monster_lvl1[i]->Position.x <= 0.0f) {
                //         Monster_lvl1[i]->left = !(Monster_lvl1[i]->left);
                //     }
                // }
                // Monster_lvl1[i]->Position = Monster_lvl1[i]->Position + Monster_lvl1[i]->Velocity * dt;
                if (this->Level == 2) {
                    if (CheckCollision(*(Monster_lvl3[i]), *Player)) {
                        this->State = GAME_LOSE;
                    }
                }
            }
        }

        // move playerboard
        if (this->Keys[GLFW_KEY_LEFT] && Player->left) {
            if (Player->Position.x >= 0.0f)
                Player->Position.x -= velocity;
            Player->leftpress = true;
            Player->rightpress = false;
            Player->uppress = false;
            Player->downpress = false;
        } else if (this->Keys[GLFW_KEY_RIGHT] && Player->right) {
            if (Player->Position.x <= this->Width - Player->Size.x)
                Player->Position.x += velocity;
            Player->rightpress = true;
            Player->leftpress = false;
            Player->uppress = false;
            Player->downpress = false;
        } else if (this->Keys[GLFW_KEY_UP] && Player->up) {
            if (Player->Position.y >= 0.0f)
                Player->Position.y -= velocity;
            Player->uppress = true;
            Player->leftpress = false;
            Player->rightpress = false;
            Player->downpress = false;
        } else if (this->Keys[GLFW_KEY_DOWN] && Player->down) {
            if (Player->Position.y <= this->Height - Player->Size.y) {
                Player->Position.y += velocity;
            } else {
                if (this->numleftcoins == this->totcoins - this->lvl1coins) {
                    if (!this->Level)
                        this->Level = 1;
                    this->newlevel = false;
                } else if (this->numleftcoins == this->totcoins - this->lvl1coins - this->lvl2coins) {
                    if (this->Level == 1) this->Level = 2;
                    this->newlevel = false;
                } else if (this->numleftcoins == 0) {
                    this->State = GAME_WIN;
                }
            }
            Player->downpress = true;
            Player->leftpress = false;
            Player->uppress = false;
            Player->rightpress = false;
        }
    }
}

void Game::Render() {
    if (this->State == GAME_ACTIVE) {
        if (!this->newlevel) {
            glm::vec2 playerPos = glm::vec2(0, 0);
            Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
            this->newlevel = true;
        }
        // draw background
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(1.0f), playerposition, light);
        // draw level
        this->Levels[this->Level].Draw(*Renderer, playerposition, light);
        // draw player
        Player->Draw(*Renderer, playerposition, light);
        // Renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width/2, this->Height/2), 0.0f);
        if (this->Level == 0) {
            Monster_lvl1[0]->Draw(*Renderer, playerposition, light);
            Monster_lvl1[1]->Draw(*Renderer, playerposition, light);
        } else if (this->Level == 1) {
            for (int i = 0; i < 3; i++) {
                Monster_lvl2[i]->Draw(*Renderer, playerposition, light);
            }
        } else if (this->Level == 2) {
            for (int i = 0; i < 4; i++) {
                Monster_lvl3[i]->Draw(*Renderer, playerposition, light);
            }
        }
        std::stringstream sc, tm;
        sc << this->score;
        tm << (time(NULL) - START);
        Text->RenderText("Score: " + sc.str(), 5.0f, 5.0f, 2.0f);
        Text->RenderText("Time since start: " + tm.str(), 5.0f, 100.0f, 2.0f);
    }
    if (this->State == GAME_LOSE) {
        Renderer->DrawSprite(ResourceManager::GetTexture("lose"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(1.0f), playerposition, 0);

    }
    if (this->State == GAME_WIN) {
        Renderer->DrawSprite(ResourceManager::GetTexture("win"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(1.0f), playerposition, 0);
    }
}