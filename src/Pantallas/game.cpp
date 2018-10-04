#include "game.h"

#include <raylib.h>
#include <cmath>

#include "menu.h"
#include "app.h"

using namespace app;
using namespace menu;

namespace app
{
	namespace game
	{
		struct Ball
		{
			Vector2 position;
			Vector2 speed;
			int radius;
			bool active;
		};

		Ball ball;
		int initRect1 = 0;
		int initRect2 = 0;
		int heightRect = 70;
		int wideRect = 10;
		int pointsP1;
		int pointsP2;
		int initPoints = 0;
		int maxPoints = 10;
		int velBallX;
		int velBallYmin;
		int velBallYmax;
		int sizeText = 0;
		char text1[] = "Apreta ESPACIO para jugar";
		int textPositionX = 0;
		int textPositionY = 0;
		int textP1PositionX = 0;
		int textPointsPositionY = 0;
		int textP2PositionX = 0;
		Winner winner;
		bool init;
		Vector2 rectPosition1 ;
		Vector2 rectPosition2 ;
		Vector2 size = { wideRect,heightRect };
		Vector2 initBallPos;
		Vector2 initVel = { 0,0 };
		Rectangle player1;
		Rectangle player2;
		Rectangle limit1;
		Rectangle limit2;
		Sound hit;
		Sound point;
		Sound triumph;
		float impactPoint = 0.0f;
		float maxAngle = 0.05f;
		int multiplicadorAngulos = 10000; // el seno me daba un valor muy chico y la diferencia no se notaba

		static float Angles()
		{
			return (impactPoint - heightRect / 2)*maxAngle / (heightRect / 2);
		}

		void InitValues()
		{
			hit = LoadSound("res/sounds/hit.wav");
			point = LoadSound("res/sounds/point.wav");
			triumph = LoadSound("res/sounds/triumph.wav");
			winner = nobody;
			init = false;

			heightRect = (GetScreenWidth()* heightRect) / 800;
			wideRect = (GetScreenWidth()* wideRect) / 800;
			initRect1 = GetScreenWidth() * 0.0125;
			initRect2 = (GetScreenWidth() - wideRect) * 0.9875;
			rectPosition1 = { (float)initRect1, (float)GetScreenHeight() / 2 };
			rectPosition2 = { (float)initRect2, (float)GetScreenHeight() / 2 };

			pointsP1 = initPoints;
			pointsP2 = initPoints;

			player1.x = initRect1;
			player1.y = GetScreenHeight() / 2;
			player1.height = heightRect;
			player1.width = wideRect;

			player2.x = initRect2;
			player2.y = GetScreenHeight() / 2;
			player2.height = heightRect;
			player2.width = wideRect;

			limit1.x = 0;
			limit1.y = heightRect;
			limit1.height = 2;
			limit1.width = GetScreenWidth();

			limit2.x = GetScreenWidth() / 2;
			limit2.y = 0;
			limit2.height = GetScreenHeight();
			limit2.width = 1;

			initBallPos= { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
			velBallX = (GetScreenWidth() * (-300)) / 800;
			velBallYmin = (GetScreenWidth() * (-150)) / 800;
			velBallYmax = (GetScreenWidth() * (150)) / 800;
			ball.position = initBallPos;
			ball.speed = initVel;
			ball.radius = (GetScreenWidth() * 8) / 800;
			ball.active = false;

			sizeText = (GetScreenWidth() * 40) / 800;
			textPositionX = GetScreenWidth() / 2 - MeasureText(text1, sizeText) / 2;
			textPositionY = GetScreenHeight() / 2 + GetScreenHeight() * 0.1333333;
			textP1PositionX = GetScreenWidth() / 2 - GetScreenWidth() * 0.0625;
			textPointsPositionY = GetScreenHeight() * 0.025;
			textP2PositionX = GetScreenWidth() / 2 + GetScreenWidth() * 0.01;
		}

		static void Input()
		{
			if (IsKeyDown(KEY_UP)) player2.y -= 400.0f* GetFrameTime();
			if ((player2.y - player2.height / 2) <= player2.height / 2)
				player2.y = player2.height;
			if (IsKeyDown(KEY_DOWN)) player2.y += 400.0f*GetFrameTime();
			if ((player2.y + player2.height / 2) >= GetScreenHeight() - player2.height / 2)
				player2.y = GetScreenHeight() - player2.height;
			if (IsKeyDown(KEY_W)) player1.y -= 400.0f*GetFrameTime();
			if ((player1.y - player1.height / 2) <= player1.height / 2)
				player1.y = player1.height;
			if (IsKeyDown(KEY_S)) player1.y += 400.0f*GetFrameTime();
			if ((player1.y + player1.height / 2) >= GetScreenHeight() - player1.height / 2)
				player1.y = GetScreenHeight() - player1.height;

			if (!ball.active)
			{
				if (IsKeyPressed(KEY_SPACE))
				{
					ball.active = true;
					ball.speed.x = velBallX;
					ball.speed.y = GetRandomValue(velBallYmin, velBallYmax);
					init = true;
				}
			}

			if (IsKeyPressed(KEY_ESCAPE))
			{
				currentScreen = Menu;
				ResetValues();
			}
		}

		static void Update()
		{
			if (ball.active)
			{
				ball.position.x += ball.speed.x*GetFrameTime();
				ball.position.y += ball.speed.y*GetFrameTime();
			}
			else
			{
				ball.position = initBallPos;
			}

			if (CheckCollisionCircleRec(ball.position, ball.radius, player1))
			{
				impactPoint = ball.position.y - player1.y;
				PlaySound(hit);
				ball.position.x = player1.x + player1.width + ball.radius;
				ball.speed.x *= -1;
				ball.speed.x += 70;
				ball.speed.y = sin(Angles())*multiplicadorAngulos;
			}

			if (CheckCollisionCircleRec(ball.position, ball.radius, player2))
			{
				impactPoint = ball.position.y - player2.y;
				PlaySound(hit);
				ball.position.x = player2.x - ball.radius;
				ball.speed.x *= -1;
				ball.speed.x -= 70;
				ball.speed.y = sin(Angles()) * multiplicadorAngulos;

			}

			if (CheckCollisionCircleRec(ball.position, ball.radius, limit1))
			{
				ball.speed.y *= -1;

			}

			if (ball.position.y >= GetScreenHeight() - ball.radius)
			{

				ball.speed.y *= -1;

			}

			if (ball.position.x<0)
			{
				PlaySound(point);
				pointsP1++;
				ball.position = initBallPos;
				ball.speed = initVel;
				ball.active = false;
			}

			if (ball.position.x>GetScreenWidth())
			{
				PlaySound(point);
				pointsP2++;
				ball.position = initBallPos;
				ball.speed = initVel;
				ball.active = false;
			}

			if (pointsP1 == maxPoints)
			{
				PlaySound(triumph);
				winner = p1Wins;
				currentScreen = GameOver;
			}
			if (pointsP2 == maxPoints)
			{
				PlaySound(triumph);
				winner = p2Wins;
				currentScreen = GameOver;
			}
		}

		void UpdateFrame()
		{
			Input();
			Update();
		}

		void Draw()
		{
			ClearBackground(RAYWHITE);
			if (!init)
			{
				DrawText(text1, textPositionX, textPositionY, sizeText, GRAY);
			}
			DrawCircleV(ball.position, ball.radius, DARKPURPLE);
			DrawRectangleRec(player1, BLACK);
			DrawRectangleRec(player2, BLACK);
			DrawRectangleRec(limit1, BLACK);
			DrawRectangleRec(limit2, BLACK);
			DrawText(FormatText("%02i", pointsP1), textP1PositionX, textPointsPositionY, sizeText, GRAY);
			DrawText(FormatText("%02i", pointsP2), textP2PositionX, textPointsPositionY, sizeText, GRAY);
		}

		void ResetValues()
		{
			pointsP1 = 0;
			pointsP2 = 0;
			winner = nobody;
			init = false;
			ball.active = false;
		}
	}
}


