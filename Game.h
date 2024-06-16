#pragma once
#include "Main.h"


void logicMain(registry& reg, entity& player);
void timerEnd(std::chrono::steady_clock::time_point& startTime);
void updateRockets(const entity& player,
	registry& reg, int& rocketCooldown);
void addRockets(const entity& player, registry& reg, int& rocketCooldown);
void addEnemies(int& enemyNum, registry& reg);
entity initEnemy(registry& reg, float posX, float posY);



