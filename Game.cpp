#include "Game.h"
#include "Movement.h"


entity initEnemy(registry& reg, float posX, float posY)
{
	entity enemy = reg.create();

	Position& pos = reg.emplace<Position>(enemy);
	Velocity& v = reg.emplace<Velocity>(enemy);
	Drawable& draw = reg.emplace<Drawable>(enemy);
	Npc& identifier = reg.emplace<Npc>(enemy);


	pos = { posX, posY };
	v = { 0.f, 0.f };
	draw.color = BLUE;
	draw.radius = 5.f;
	draw.drawFunc = DrawCircleV;

	return enemy;
}

void addEnemies(int& enemyNum, registry& reg)
{

	if (enemyNum >= 8)
		return;

	Position enemyPos{};
	int corner = rand() % 4;
	float offset = (float)(((rand() + 1) % 20) * 100);

	if (corner == 0)
	{
		enemyPos = { offset , 100.f };
	}
	else if (corner == 1) {
		enemyPos = { 1900.f, offset };
	}
	else if (corner == 2) {
		enemyPos = { offset, 1900.f };
	}
	else if (corner == 3) {
		enemyPos = { 100.f, offset };
	}
	entity newEnemy = initEnemy(reg, enemyPos.x, enemyPos.y);
}



void addRockets(const entity& player, registry& reg, int& rocketCooldown)
{

	if (rocketCooldown > 0)
	{
		rocketCooldown -= 1;
		return;
	}
	if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		return;

	Vector2 mousePosition;
	windowToMap(GetMousePosition(), mousePosition);
	if (!inBounds(mousePosition))
		return;

	const Position& playerPos = reg.get<Position>(player);
	entity newRocket = reg.create();

	TargetDest& dest = reg.emplace<TargetDest>(newRocket);
	Position& rocketPos = reg.emplace<Position>(newRocket);
	Velocity& rocketV = reg.emplace<Velocity>(newRocket);
	Drawable& draw = reg.emplace<Drawable>(newRocket);
	Rocket& identifer = reg.emplace<Rocket>(newRocket);

	dest.x = mousePosition.x;
	dest.y = mousePosition.y;
	rocketV.y = -atan2(playerPos.x - mousePosition.x, playerPos.y - mousePosition.y);
	rocketV.x = 200.f / physics_tick;
	rocketPos.x = playerPos.x;
	rocketPos.y = playerPos.y;
	//	draw.center.x = mousePosition.x;
	//	draw.center.y = mousePosition.y;
	draw.radius = 3.f;
	draw.color = RED;
	draw.drawFunc = DrawCircleLinesV;

	rocketCooldown = 30;   // 0.5 * physics_tick

}


void updateRockets(const entity& player,
	registry& reg, int& rocketCooldown)
{


	addRockets(player, reg, rocketCooldown);


	float rocketSpeed = 200.f / physics_tick;

	//auto view = reg.view<Position, Velocity, TargetDest>();
	auto view = reg.view<Rocket>();
	for (entity e : view)
	{
		Position& rocketPos = reg.get<Position>(e);
		Velocity& rocketV = reg.get<Velocity>(e);
		TargetDest& dest = reg.get<TargetDest>(e);

		if (distance(rocketPos, dest) < rocketSpeed)  // detonate on uncertainty margin
		{

			rocketPos.x = dest.x;
			rocketPos.y = dest.y;
			// explosion
			auto enemyView = reg.view<Npc>();
			for (entity enemyId : enemyView)
			{
				Position& enemyPos = reg.get<Position>(enemyId);
				if ((distance(enemyPos, rocketPos)) < 30.f)
					reg.destroy(enemyId);

			}
			reg.destroy(e);
			continue;
		}
		addVelocityToPos(rocketV, rocketPos);
	}



}



void timerEnd(std::chrono::steady_clock::time_point& startTime)
{

	auto endTime = std::chrono::high_resolution_clock::now();
	auto delta = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
	std::cout << "time in microseconds " << delta << "\n";

}



void logicMain(registry& reg, entity& player)
{

	float tickRate = (1.f / physics_tick) * 1000;
	auto start_time = std::chrono::high_resolution_clock::now();
	int rocketCooldown = 0;
	int count = 0;
	int enemyNum = 0;
	while (1)
	{
		auto next_time = std::chrono::high_resolution_clock::now();
		auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(next_time - start_time).count();

		if (delta >= tickRate)
		{

			//auto timerStart = std::chrono::high_resolution_clock::now();


			/***************
				Main loop
			****************/

			playerMovement(player, reg, &getMovementKeys);
			updateRockets(player, reg, rocketCooldown);
			start_time = std::chrono::high_resolution_clock::now();
			count += 1;

			if ((count % 90) == 0)
			{
				addEnemies(enemyNum, reg);
				auto view = reg.view<Npc>();
				int num = 0;
				for (entity e : view)
				{
					num += 1;
				}

			}


			if (count == 1000)
				count = 0;

			/***************
				End Main
			****************/

			//timerEnd(timerStart);


		}
	}
}
