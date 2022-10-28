#define OLC_PGE_APPLICATION

#include <utility>
#include <vector>
#include <string>
#include <random>

#include "defs.hpp"
#include "olcPixelGameEngine.h"
#include "vec2.hpp"
#include "wall.hpp"
#include "collisionobject.hpp"

class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "Game";
	}

	olc::Sprite wall;
	olc::Sprite wallStone;

	olc::Sprite background;
	olc::Sprite staticEffect;

	float playerAngle;
	float mouseXHold;

	CollisionObject player;

	double cameraDistance;

	std::vector<Wall> walls;

public:
	bool OnUserCreate() override
	{
		wall = olc::Sprite("Resources/wall.png");
		wallStone = olc::Sprite("Resources/wall_stone.png");

		background = olc::Sprite("Resources/background.png");
		staticEffect = olc::Sprite("Resources/static.png");

		walls = {
			Wall{ Coords(-100, 100), Coords(-100, 300), &wall },
			Wall{ Coords(-100, 300), Coords(100, 300), &wall },
			Wall{ Coords(100, 300), Coords(100, 100), &wall },
			Wall{ Coords(100, 100), Coords(200, 0), &wallStone },
			Wall{ Coords(200, 0), Coords(300, 300), &wallStone }
		};

		player = CollisionObject(Coords(), 40.0);

		playerAngle = 0;
		POINT mousePos;
		GetCursorPos(&mousePos);
		mouseXHold = (float)mousePos.x;

		cameraDistance = -(ScreenWidth() / 2) / std::tan(0.7854);

		return true;
	}

	void draw(float fElapsedTime)
	{
		static double counter = 0;
		counter += fElapsedTime;

		static int staticX, staticY;

		std::random_device dev;
		std::mt19937 rng(dev());

		std::uniform_int_distribution<std::mt19937::result_type> distHoriz(0, staticEffect.width - ScreenWidth());
		std::uniform_int_distribution<std::mt19937::result_type> distVert(0, staticEffect.height - ScreenHeight());

		DrawSprite(0, 0, &background);

		Coords cameraPoint = Coords(0, cameraDistance).rotate(playerAngle) + player.pos;

		for (int x = 0; x < ScreenWidth(); x++)
		{
			double nearest = 1e10;
			CollisionResult hold = {
				false,
				Coords(),
				0.0,
				NULL
			};

			Coords screenPoint = Coords(x - ScreenWidth() / 2, 0).rotate(playerAngle) + player.pos;

			Vec2 ray = (screenPoint - cameraPoint).unit();

			for (auto& wall : walls)
			{
				CollisionResult collision = wall.getCollision(player.pos, ray);
				double distance = collision.collisionPoint.dist(player.pos);
				if (!collision.didCollide || distance > MAX_DRAW_DISTANCE) continue;
				else if (distance < nearest)
				{
					nearest = distance;
					hold = collision;
				}
			}

			if (hold.didCollide)
			{
				double adjustedDistance = player.pos.dist(hold.collisionPoint) * std::cos(std::atan2(x - ScreenWidth() / 2, -cameraDistance));

				int drawHeight = (int32_t)(ScreenHeight() * ScreenHeight() / std::max(1.0, adjustedDistance));

				int blank = (ScreenHeight() - drawHeight) / 2;

				for (int y = std::max(0, blank); y <= std::min(ScreenHeight(), ScreenHeight() - blank); y++)
				{
					olc::Pixel pixel = hold.sprite->GetPixel(
						(int)(hold.sprite->width * hold.texturePosition),
						(y - blank) * hold.sprite->height / (drawHeight + 2)
					);
					Draw(x, y, pixel / (float)std::max(1.0, adjustedDistance / 300.0));
				}
			}
		}

		//SetPixelMode(olc::Pixel::ALPHA);
		//DrawPartialSprite(0, 0, &staticEffect, staticX, staticY, ScreenWidth(), ScreenHeight(), 1, 0);
		//SetPixelMode(olc::Pixel::NORMAL);
		//if (counter >= 0.016) {
			//staticX = distHoriz(rng);
			//staticY = distVert(rng);
			//counter = 0;
		//}

		DrawString(0, 0, std::to_string(player.pos.x) + " " + std::to_string(player.pos.y));
	}

	void update(float fElapsedTime)
	{
		POINT mousePos;
		GetCursorPos(&mousePos);
		playerAngle -= (mousePos.x - mouseXHold) / 100;
		mouseXHold = (float)mousePos.x;

		double sint = std::sin(playerAngle);
		double cost = std::cos(playerAngle);

		player.setVelocity(0, 0);
		if (GetKey(olc::Key::W).bHeld) player.setVelocity(player.velocity + Vec2(-sint, cost));
		if (GetKey(olc::Key::S).bHeld) player.setVelocity(player.velocity + Vec2(sint, -cost));
		if (GetKey(olc::Key::A).bHeld) player.setVelocity(player.velocity + Vec2(-cost, -sint));
		if (GetKey(olc::Key::D).bHeld) player.setVelocity(player.velocity + Vec2(cost, sint));

		player.updatePhysics(fElapsedTime, walls);
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