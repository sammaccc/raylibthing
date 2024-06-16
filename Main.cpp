#include "Main.h"
#include "Render.h"
#include "Game.h"


entity initPlayer(registry& reg)
{
	entity player = reg.create();

	Position& playerPos = reg.emplace<Position>(player);
	playerPos.x = 1000.f;
	playerPos.y = 1000.f;

	Velocity& playerV = reg.emplace<Velocity>(player);
	playerV.x = 0.f;
	playerV.y = 0.f;
		
	Pc& identifier = reg.emplace<Pc>(player);

	Drawable& playerDraw = reg.emplace<Drawable>(player);
	playerDraw.drawFunc = DrawCircleV;
	playerDraw.color = GREEN;
	playerDraw.radius = 5.f;
	playerDraw.center = playerPos;

	return player;
}

int main()
{

	
	SetConfigFlags(FLAG_WINDOW_UNDECORATED);
	InitWindow(GetScreenWidth(), GetScreenHeight(), "heLoO");
	SetTargetFPS(60);
	DisableCursor();
	//ToggleFullscreen();

	registry reg;
	entity player = initPlayer(reg);
	

	std::thread logic_thread(logicMain, std::ref(reg), std::ref(player));
	
	while (!WindowShouldClose())
	{
		render(reg);
	}

	CloseWindow();
	logic_thread.join();




}
