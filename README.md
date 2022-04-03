# AMaze

AMaze is a 2D Maze game made using OpenGL. The game has 3 levels with increasing difficulties. The Objective of the game is to make it to the end of the three Chambers by collecting all the gold orbs by also avoiding the smiley monsters that wander around the maze.

## Rules

The game consists of 3 levels with increasing difficulties. The Game has 4 outer walls and a single exit. The inner walls, gold coins and enemies are randomly spawned around the maze. You can go to the next chamber/level only after collecting all the gold coins in that particular level. Contact with any of the enemies leads to instant death. There is also an option to toggle lights, when the lights are off, the visibility of the player decreases, and any gold collected during this period has double the credit. 

## Controls

W → Move Up

A → Move Left

S → Move Down

D → Move Right

Esc → Quit

Space → Toggle lights on and off

## Setup

1. Clone the repository.
2. cd into the directory.
3. Now, run the following commands:

<aside>
💡 cmake .
make -j
./game

</aside>