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

	POINT mousePosHold;

	olc::Sprite wall;
	olc::Sprite wallStone;

	olc::Sprite background;
	olc::Sprite staticEffect;

	float playerAngle;

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
				// Fisheye correction from https://www.playfuljs.com/a-first-person-engine-in-265-lines/
				// Improved to get rid of the atan2 and cos calls
				double ratio = -cameraDistance / cameraPoint.dist(screenPoint); // cos(t)=a/h
				double adjustedDistance = player.pos.dist(hold.collisionPoint) * ratio;

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

	bool update(float fElapsedTime)
	{
		POINT mousePos;
		if (IsFocused())
		{
			RECT windowPos;
			GetCursorPos(&mousePos);
			GetWindowRect(GetForegroundWindow(), &windowPos);
			int middleX = windowPos.left + (windowPos.right - windowPos.left) / 2;
			int middleY = windowPos.top + (windowPos.bottom - windowPos.top) / 2;
			SetCursorPos(middleX, middleY);
			
			playerAngle -= (float)(mousePos.x - middleX) / 100;
		}
		
		double sint = std::sin(playerAngle);
		double cost = std::cos(playerAngle);

		player.setVelocity(0, 0);
		if (GetKey(olc::Key::W).bHeld) player.setVelocity(player.velocity + Vec2(-sint, cost));
		if (GetKey(olc::Key::S).bHeld) player.setVelocity(player.velocity + Vec2(sint, -cost));
		if (GetKey(olc::Key::A).bHeld) player.setVelocity(player.velocity + Vec2(-cost, -sint));
		if (GetKey(olc::Key::D).bHeld) player.setVelocity(player.velocity + Vec2(cost, sint));
		if (GetKey(olc::Key::ESCAPE).bHeld) return false;

		player.updatePhysics(fElapsedTime, walls);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		draw(fElapsedTime);

		return update(fElapsedTime);
	}
};

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInsPrev, PSTR cmdline, int cmdshow)
{
	Game game;
	while (ShowCursor(false) >= 0);
	if (game.Construct(320, 200, 3, 3))
		game.Start();
	return 0;
}