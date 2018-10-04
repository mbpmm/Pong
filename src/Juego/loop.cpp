#include <raylib.h>
#include "juego.h"
#include "menu.h"
#include "game_over.h"
using namespace Game;
using namespace Menu;
using namespace GameOver;

namespace Loop
{
	bool exit = false;
	Screens currentScreen = Title;

	void ResetValues()
	{
		pointsP1 = 0;
		pointsP2 = 0;
		winner = nobody;
		finishScreen = 0;
	}

	void exe()
	{
		SetExitKey(0);
		InitWindow(screenWidth, screenHeight, "PONG");
		HideCursor();
		//InitAudioDevice();
		SetTargetFPS(60);
		// Main game loop
		while (!exit)    // Detect window close button or ESC key
		{
			switch (currentScreen)
			{
			case Title:
				UpdateMenu();
				if (FinishScreen() == 1)
				{
					currentScreen = Gameplay;
					InitGame();
				}
				break;
			case Gameplay:
				UpdateFrame();
				if (winner == p1Wins || winner == p2Wins)
				{
					currentScreen = Gameover;
				}
				break;
			case Gameover:
				UpdateGameOver();
				break;
			default:
				break;
			}

			BeginDrawing();
			switch (currentScreen)
			{
			case Title:
				DrawMenu();
				break;
			case Gameplay:
				Draw();
				break;
			case Gameover:
				DrawGameOver();
			default:
				break;
			}
			EndDrawing();
		}
		CloseWindow();
		//CloseAudioDevice();
	}
}
