#define OLC_PGE_APPLICATION

#include <utility>
#include <vector>
#include <string>

#include "olcPixelGameEngine.h"
#include "vec2.hpp"
#include "wall.hpp"

const float MOVE_UNITS_PER_SECOND = 400.0f;

class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "Game";
	}

	float playerAngle;
	float mouseXHold;
	Coords pos;

	std::vector<Wall> walls = {
		Wall(Coords(-150, 100), Coords(-150, 400), 0x00, 0xFF, 0xFF),
		Wall(Coords(-150, 400), Coords(150, 400), 0xFF, 0x00, 0x00),
		Wall(Coords(150, 400), Coords(150, 100), 0x00, 0xFF, 0x00)
	};

public:
	bool OnUserCreate() override
	{
		playerAngle = 0;
		POINT mousePos;
		GetCursorPos(&mousePos);
		mouseXHold = (float)mousePos.x;
		return true;
	}

	void draw()
	{
		FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::BLACK);

		Coords cameraPoint = Coords(0, -128).rotate(playerAngle) + pos;

		for (int x = 0; x < ScreenWidth(); x++)
		{
			double nearest = 1e10;
			Coords holdCoords;
			Wall* hold = NULL;

			Coords screenPoint = Coords(x - ScreenWidth() / 2, 0).rotate(playerAngle) + pos;

			Vec2 ray = (screenPoint - cameraPoint).unit();

			for (auto& wall : walls)
			{
				Coords collision;
				bool didCollide = wall.getCollision(pos, ray, collision);
				if (didCollide && collision.dist(pos) < nearest)
				{
					nearest = collision.dist(pos);
					hold = &wall;
					holdCoords = collision;
				}
			}

			if (hold)
			{
				double adjusted = screenPoint.dist(holdCoords);

				int height = std::min(ScreenHeight(), (int32_t)(ScreenHeight() * ScreenHeight() / adjusted));

				int blank = (ScreenHeight() - height) / 2;

				for (int y = blank; y <= ScreenHeight() - blank; y++)
				{
					Draw(x, y, olc::Pixel(hold->red, hold->green, hold->blue) / (float)(adjusted / 100.0));
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
		draw();

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