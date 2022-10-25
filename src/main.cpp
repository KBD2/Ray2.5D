#define OLC_PGE_APPLICATION

#include <utility>
#include <vector>
#include <string>

#include "olcPixelGameEngine.h"
#include "vec2.hpp"
#include "wall.hpp"

const float MOVE_UNITS_PER_SECOND = 300.0f;

class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "Game";
	}

	float playerAngle;
	float mouseXHold;
	double playerX, playerY;

	std::vector<Wall> walls = {
		Wall(Vec2(-100, 100), Vec2(-100, 200), 0x00, 0xFF, 0xFF),
		Wall(Vec2(-100, 200), Vec2(100, 200), 0xFF, 0x00, 0x00),
		Wall(Vec2(100, 200), Vec2(100, 100), 0x00, 0xFF, 0x00)
	};

public:
	bool OnUserCreate() override
	{
		playerAngle = 0;
		playerX = 0;
		playerY = 0;
		POINT mousePos;
		GetCursorPos(&mousePos);
		mouseXHold = (float)mousePos.x;
		return true;
	}

	void draw()
	{
		FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::BLACK);

		Vec2 viewerPoint = Vec2(0, -128).rotate(playerAngle) + Vec2(playerX, playerY);

		for (int x = 0; x < ScreenWidth(); x++)
		{
			double nearest = 1e10;
			Wall* hold = NULL;

			Vec2 screenPoint = Vec2(x - ScreenWidth() / 2, 0).rotate(playerAngle) + Vec2(playerX, playerY);

			Vec2 ray = (screenPoint - viewerPoint).unit();

			for (auto& wall : walls)
			{
				double distance = wall.getCollision(screenPoint, ray);
				if (distance > 0 && distance < nearest)
				{
					nearest = distance;
					hold = &wall;
				}
			}

			if (hold)
			{
				for (int y = (int)(nearest / 10); y < ScreenHeight() - nearest / 10; y++)
				{
					Draw(x, y, olc::Pixel(hold->red, hold->green, hold->blue) / (float)(nearest / 100.0));
				}
			}
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
			playerX -= sint;
			playerY += cost;
		}
		if (GetKey(olc::Key::S).bHeld)
		{
			playerX += sint;
			playerY -= cost;
		}
		if (GetKey(olc::Key::A).bHeld)
		{
			playerX -= cost;
			playerY -= sint;
		}
		if (GetKey(olc::Key::D).bHeld)
		{
			playerX += cost;
			playerY += sint;
		}
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		draw();

		update(fElapsedTime);

		return true;
	}
};

int main()
{
	Game game;
	if (game.Construct(256, 256, 2, 2))
		game.Start();
	return 0;
}