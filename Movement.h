#pragma once
#include "Main.h"
float distance(const Vector2& v1, const Vector2& v2);
void windowToMap(const Vector2& window_position, Vector2& position);
void mapToWindow(const Vector2& position, Vector2& window_position);
void polarToBoard(const Vector2& polar_position, Vector2& position);
bool inBounds(const Vector2& pos);
void addVelocityToPos(const Velocity& velocity, Position& position);
void enemyMovement(bool& W, bool& A, bool& S, bool& D);
void getMovementKeys(bool& W, bool& A, bool& S, bool& D);
void getMovementDirection(const bool& W, const bool& A, const bool& S, const bool& D,
	float& movementAngle);
	void playerMovement(entity & player, registry & reg, void (*movementFunc)(bool&, bool&, bool&, bool&));

