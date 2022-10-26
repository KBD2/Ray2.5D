#define OLC_PGE_APPLICATION

#include <utility>
#include <vector>
#include <string>
#include <random>

#include "olcPixelGameEngine.h"
#include "vec2.hpp"
#include "wall.hpp"

const float MOVE_UNITS_PER_SECOND = 400.0f;
const int MAX_DRAW_DISTANCE = 2000;

class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "Game";
	}

	olc::Sprite wall;
	olc::Sprite background;
	olc::Sprite staticEffect;

	float playerAngle;
	float mouseXHold;
	Coords pos;

	double cameraDistance;

	std::vector<Wall> walls;

public:
	bool OnUserCreate() override
	{
		wall = olc::Sprite("Resources/wall.png");
		background = olc::Sprite("Resources/background.png");
		staticEffect = olc::Sprite("Resources/static.png");

		walls = {
			Wall(Coords(-100, 100), Coords(-100, 300), &wall),
			Wall(Coords(-100, 300), Coords(100, 300), &wall),
			Wall(Coords(100, 300), Coords(100, 100), &wall)
		};

		playerAngle = 0;
		POINT mousePos;
		GetCursorPos(&mousePos);
		mouseXHold = (float)mousePos.x;

		cameraDistance = -(ScreenWidth() / 2) / std::tan(0.7854);

		return true;
	}

	void draw(float fElapsedTime)
	{
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> distHoriz(0, staticEffect.width - ScreenWidth());
		std::uniform_int_distribution<std::mt19937::result_type> distVert(0, staticEffect.height - ScreenHeight());

		static double counter = 0;
		counter += fElapsedTime;

		DrawSprite(0, 0, &background);

		Coords cameraPoint = Coords(0, cameraDistance).rotate(playerAngle) + pos;

		for (int x = 0; x < ScreenWidth(); x++)
		{
			double nearest = 1e10;
			CollisionResult hold = {
				false,
				Coords(),
				0.0,
				NULL
			};

			Coords screenPoint = Coords(x - ScreenWidth() / 2, 0).rotate(playerAngle) + pos;

			Vec2 ray = (screenPoint - cameraPoint).unit();
			//Vec2 ray = Vec2(0, 1).rotate(playerAngle + 1.570796 * (double)(ScreenWidth() / 2 - x) / ScreenWidth());

			for (auto& wall : walls)
			{
				CollisionResult collision = wall.getCollision(pos, ray);
				double distance = collision.collisionPoint.dist(pos);
				if (!collision.didCollide || distance > MAX_DRAW_DISTANCE) continue;
				else if (distance < nearest)
				{
					nearest = distance;
					hold = collision;
				}
			}

			if (hold.didCollide)
			{
				double adjustedDistance = pos.dist(hold.collisionPoint) * std::cos(std::atan2(x - ScreenWidth() / 2, -cameraDistance));

				int drawHeight = (int32_t)(ScreenHeight() * ScreenHeight() / std::max(1.0, adjustedDistance));

				int blank = (ScreenHeight() - drawHeight) / 2;

				for (int y = blank; y <= ScreenHeight() - blank; y++)
				{
					olc::Pixel pixel = hold.sprite->GetPixel(
						(int)(hold.sprite->width * hold.texturePosition),
						(y - blank) * hold.sprite->height / (drawHeight + 2)
					);
					Draw(x, y, pixel / (float)std::max(1.0, adjustedDistance / 1000.0));
				}
			}
		}

		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialSprite(0, 0, &staticEffect, distHoriz(rng), distVert(rng), ScreenWidth(), ScreenHeight(), 1, 0);
		SetPixelMode(olc::Pixel::NORMAL);
		if (counter >= 0.16) {
			counter = 0;
		}
		
		//DrawString(0, 0, std::to_string(playerAngle * (180.0 / 3.14159)) + " " + std::to_string(playerX) + " " + std::to_string(playerY));
	}

	void update(float fElapsedTime)
	{
		POINT mousePos;
		GetCursorPos(&mousePos);
		playerAngle -= (mousePos.x - mouseXHold) / 100;
		mouseXHold = (float)mousePos.x;

		double sint = MOVE_UNITS_PER_SECOND * fElapsedTime * std::sin(playerAngle);
		double cost = MOVE_UNITS_PER_SECOND * fElapsedTime * std::cos(playerAngle);

		if (GetKey(olc::Key::W).bHeld)
		{
			pos.x -= sint;
			pos.y += cost;
		}
		if (GetKey(olc::Key::S).bHeld)
		{
			pos.x += sint;
			pos.y -= cost;
		}
		if (GetKey(olc::Key::A).bHeld)
		{
			pos.x -= cost;
			pos.y -= sint;
		}
		if (GetKey(olc::Key::D).bHeld)
		{
			pos.x += cost;
			pos.y += sint;
		}
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		draw(fElapsedTime);

		update(fElapsedTime);

		return true;
	}
};

int main()
{
	Game game;
	if (game.Construct(320, 200, 3, 3))
		game.Start();
	return 0;
}