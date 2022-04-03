/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game_object.h"

GameObject::GameObject()
    : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false), left(true), leftpress(false), right(true), rightpress(false), up(true), uppress(false), down(true), downpress(false), move(true), dir(0), mv(20) {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
    : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), IsSolid(false), Destroyed(false), left(true), leftpress(false), right(true), rightpress(false), up(true), uppress(false), down(true), downpress(false), move(true), dir(0), mv(20) {}

void GameObject::Draw(SpriteRenderer &renderer, glm::vec2 player_position, int light) {
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color, player_position, light);
}