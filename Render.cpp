
#include "Render.h"


void drawBorders()
{
	float length = ((float)GetScreenWidth() * 0.8f);
	float thickness = ((float)GetScreenWidth() / 400.f);
	Vector2 b_left{ 0.f, board_height };
	Vector2 t_left{ 0.f, 0.f };
	Vector2 t_right{ board_width, 0.f };
	Vector2 b_right{ board_width, board_height };
	Vector2 w_t_left;
	Vector2 w_b_left;
	Vector2 w_t_right;
	Vector2 w_b_right;

	mapToWindow(t_left, w_t_left);
	mapToWindow(b_right, w_b_right);
	mapToWindow(b_left, w_b_left);
	mapToWindow(t_right, w_t_right);

	DrawLineEx(w_t_left, w_b_left, thickness, RED);
	DrawLineEx(w_b_left, w_b_right, thickness, RED);
	DrawLineEx(w_b_right, w_t_right, thickness, RED);
	DrawLineEx(w_t_right, w_t_left, thickness, RED);



}


void render(const registry& reg)
{
	BeginDrawing();
	ClearBackground(BLACK);
	drawBorders();

	auto view = reg.view<Position, Drawable>();
	for (entity e : view)
	{
		Position pos = view.get<Position>(e);
		Drawable draw = view.get<Drawable>(e);
		mapToWindow(pos, draw.center);
		draw.drawFunc(draw.center, draw.radius, draw.color);
	}

	Vector2 mousePos;
	windowToMap(GetMousePosition(), mousePos);
	if (inBounds(mousePos))
		DrawCircleV(GetMousePosition(), 1.f, GREEN);


	EndDrawing();
}
