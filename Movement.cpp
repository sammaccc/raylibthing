#include "movement.h"


float distance(const Vector2& v1, const Vector2& v2)
{

	float dx = abs(v1.x - v2.x);
	float dy = abs(v1.y - v2.y);
	return (float)std::sqrt((dx * dx) + (dy * dy));

}

void windowToMap(const Vector2& window_position, Vector2& position)
{

	float screen_width = (float)GetScreenWidth();
	float screen_height = (float)GetScreenHeight();


	float height_kept = 0.8f;
	float width_kept = height_kept * (screen_height / screen_width);
	float height_loss = (1.f - height_kept) / 2.f;
	float width_loss = (1.f - width_kept) / 2.f;

	float width_start = (width_loss * screen_width);
	float height_start = (height_loss * screen_height);

	float scaling_x = board_width / (screen_width * width_kept);
	float scaling_y = board_height / (screen_height * height_kept);

	position.x = scaling_x * (-width_start + window_position.x);
	position.y = scaling_y * (-height_start + window_position.y);
}



void mapToWindow(const Vector2& position, Vector2& window_position)
{

	float screen_width = (float)GetScreenWidth();
	float screen_height = (float)GetScreenHeight();


	float height_kept = 0.8f;
	float width_kept = height_kept * (screen_height / screen_width);
	float height_loss = (1.f - height_kept) / 2.f;
	float width_loss = (1.f - width_kept) / 2.f;

	float width_start = (width_loss * screen_width);
	float height_start = (height_loss * screen_height);


	float scaling_x = (screen_width * width_kept) / board_width;
	float scaling_y = (screen_height * height_kept) / board_height;

	window_position.x = width_start + (scaling_x * position.x);
	window_position.y = height_start + (scaling_y * position.y);

}

void polarToBoard(const Vector2& polar_position, Vector2& position)
{
	position.x = polar_position.x * sin(polar_position.y);
	position.y = polar_position.x * -cos(polar_position.y);
}


bool inBounds(const Vector2& pos)
{
	if ((pos.x < 0) || (pos.x > board_width)
		|| (pos.y < 0) || (pos.y > board_width))
		return false;

	return true;
}


void addVelocityToPos(const Velocity& velocity, Position& position)
{
	Velocity converted_v{};
	polarToBoard(velocity, converted_v);
	Position newPos{ position.x + converted_v.x, position.y + converted_v.y };
	if (inBounds(newPos))
		position = newPos;
}


void enemyMovement(bool& W, bool& A, bool& S, bool& D)
{
	W = false;
	A = false;
	S = false;
	D = false;
}



void getMovementKeys(bool& W, bool& A, bool& S, bool& D)
{
	W = IsKeyDown(KEY_W);
	A = IsKeyDown(KEY_A);
	S = IsKeyDown(KEY_S);
	D = IsKeyDown(KEY_D);
}


void getMovementDirection(const bool& W, const bool& A, const bool& S, const bool& D,
	float& movementAngle)
{
	if ((W && !A && !S && !D) || (W && A && !S && D))
		movementAngle = 0.f;
	else if ((W && !A && !S && D))
		movementAngle = 0.25f * pi;
	else if ((!W && !A && !S && D) || (W && !A && S && D))
		movementAngle = 0.5f * pi;
	else if ((!W && !A && S && D))
		movementAngle = 0.75f * pi;
	else if ((!W && !A && S && !D) || (!W && A && S && D))
		movementAngle = 1.f * pi;
	else if ((!W && A && S && !D))
		movementAngle = 1.25f * pi;
	else if ((!W && A && !S && !D) || (W && A && S && !D))
		movementAngle = 1.5f * pi;
	else if ((W && A && !S && !D))
		movementAngle = 1.75f * pi;

}


void playerMovement(entity& player, registry& reg, void (*movementFunc)(bool&, bool&, bool&, bool&))
{


	bool W, A, S, D;
	movementFunc(W, A, S, D);

	Position& playerPos = reg.get<Position>(player);
	Velocity& playerV = reg.get<Velocity>(player);


	float speed = 1000.f / physics_tick;   // units per second
	float slowDown = 0.1f * physics_tick;   // seconds to maxspeed

	Vector2 vectorAdd{ 0.f,0.f };

	if ((W && A && S && D) || (!W && !A && !S && !D)
		|| (W && !A && S && !D) || (!W && !A && !S && !D)
		)
	{
		playerV = { 0.f, 0.f };
		return;
	}

	getMovementDirection(W, A, S, D, vectorAdd.y);


	float kept_v = cos(vectorAdd.y - playerV.y);  // proportion of kept velocity on direction change
	// (0 on reversals, 0.5 * current v for evrerything else)
	if ((kept_v < 0) || (playerV.x == 0))
	{
		kept_v = 0;
	}
	else if (kept_v != 1) {
		kept_v = 0.5f;
	}
	playerV.y = vectorAdd.y;
	playerV.x = (playerV.x * kept_v);

	if (playerV.x < speed)
		playerV.x += (speed / slowDown);
	if (playerV.x > speed)
		playerV.x = speed;


	addVelocityToPos(playerV, playerPos);

}
