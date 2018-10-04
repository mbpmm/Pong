#include "juego.h"

#include <raylib.h>
#include <cmath>

#include "game_over.h"
#include "menu.h"
#include "app.h"

using namespace App;
using namespace Game;
using namespace Menu;
using namespace GameOver;

namespace App
{
	namespace Game
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
		int velBallX = (screenWidth * (-300)) / 800;
		int velBallYmin = (screenWidth * (-150)) / 800;
		int velBallYmax = (screenWidth * (150)) / 800;
		int sizeText = 0;
		char text1[] = "Apreta ESPACIO para jugar";
		int textPositionX = 0;
		int textPositionY = 0;
		int textP1PositionX = 0;
		int textPointsPositionY = 0;
		int textP2PositionX = 0;
		Winner winner;
		bool init;
		Vector2 rectPosition1 = { initRect1, (float)screenHeight / 2 };
		Vector2 rectPosition2 = { initRect2, (float)screenHeight / 2 };
		Vector2 size = { wideRect,heightRect };
		Vector2 initBallPos = { screenWidth / 2,screenHeight / 2 };
		Vector2 initVel = { 0,0 };
		Rectangle player1;
		Rectangle player2;
		Rectangle limit1;
		Rectangle limit2;

		void InitValues()
		{
			winner = nobody;
			init = false;

			heightRect = (screenWidth* heightRect) / 800;
			wideRect = (screenWidth* wideRect) / 800;
			initRect1 = screenWidth * 0.0125;
			initRect2 = (screenWidth - wideRect) * 0.9875;

			pointsP1 = initPoints;
			pointsP2 = initPoints;

			player1.x = initRect1;
			player1.y = screenHeight / 2;
			player1.height = heightRect;
			player1.width = wideRect;

			player2.x = initRect2;
			player2.y = screenHeight / 2;
			player2.height = heightRect;
			player2.width = wideRect;

			limit1.x = 0;
			limit1.y = heightRect;
			limit1.height = 2;
			limit1.width = screenWidth;

			limit2.x = screenWidth / 2;
			limit2.y = 0;
			limit2.height = screenHeight;
			limit2.width = 1;

			ball.position = initBallPos;
			ball.speed = initVel;
			ball.radius = (screenWidth * 8) / 800;
			ball.active = false;

			sizeText = (screenWidth * 40) / 800;
			textPositionX = screenWidth / 2 - MeasureText(text1, sizeText) / 2;
			textPositionY = screenHeight / 2 + screenHeight * 0.1333333;
			textP1PositionX = screenWidth / 2 - screenWidth * 0.0625;
			textPointsPositionY = screenHeight * 0.025;
			textP2PositionX = screenWidth / 2 + screenWidth * 0.01;
			// lim
		}

		static void Input()
		{
			if (IsKeyDown(KEY_UP)) player2.y -= 300.0f* GetFrameTime();
			if ((player2.y - player2.height / 2) <= player2.height / 2)
				player2.y = player2.height;
			if (IsKeyDown(KEY_DOWN)) player2.y += 300.0f*GetFrameTime();
			if ((player2.y + player2.height / 2) >= screenHeight - player2.height / 2)
				player2.y = screenHeight - player2.height;
			if (IsKeyDown(KEY_W)) player1.y -= 300.0f*GetFrameTime();
			if ((player1.y - player1.height / 2) <= player1.height / 2)
				player1.y = player1.height;
			if (IsKeyDown(KEY_S)) player1.y += 300.0f*GetFrameTime();
			if ((player1.y + player1.height / 2) >= screenHeight - player1.height / 2)
				player1.y = screenHeight - player1.height;

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
				currentScreen = Title;
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
				PlaySound(hit);
				ball.position.x = player1.x +player1.width + ball.radius;
				ball.speed.x *= -1;
				ball.speed.x += 70;
				ball.speed.y = GetRandomValue(velBallYmin, velBallYmax);

			}

			if (CheckCollisionCircleRec(ball.position, ball.radius, player2))
			{
				PlaySound(hit);
				ball.position.x = player2.x - ball.radius ;
				ball.speed.x *= -1;
				ball.speed.x -= 70;
				ball.speed.y = GetRandomValue(velBallYmin, velBallYmax);

			}

			if (CheckCollisionCircleRec(ball.position, ball.radius, limit1))
			{
				ball.speed.y *= -1;

			}

			if (ball.position.y >= screenHeight-ball.radius)
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

			if (ball.position.x>screenWidth)
			{
				PlaySound(point);
				pointsP2++;
				ball.position = initBallPos;
				ball.speed = initVel;
				ball.active = false;
			}

			if (pointsP1 == maxPoints)
			{
				winner = p1Wins;
			}
			if (pointsP2 == maxPoints)
			{
				winner = p2Wins;
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
			finishScreen = 0;
		}
	}
}


