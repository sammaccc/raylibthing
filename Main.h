#pragma once
#include <vector>
#include <iostream>
#include <thread>
#include <cmath>
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <chrono>
#include <fstream>
#include <entt.hpp>

#define pi 3.14159265f
#define board_width 2000.f
#define board_height 2000.f
#define physics_tick 60.f

using entity = entt::entity;
using registry = entt::registry;


/* identifiers */
struct Npc { bool identifer; };
struct Pc { bool identifier; };
struct Rocket { bool identifier; };

/* components */
struct TargetDest : Vector2 {};
struct Position : Vector2 {};
struct Velocity : Vector2 {};
struct Drawable
{
	void (*drawFunc)(Vector2, float, Color);
	Color color;
	Position center;
	float radius;
};
