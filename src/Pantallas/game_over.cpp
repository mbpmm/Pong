#include "game_over.h"

#include <raylib.h>

#include "game.h"
#include "app.h"

using namespace app;
using namespace game;

namespace app
{
	namespace gameover
	{
		int text1PositionX = 0;
		int text1PositionY = 0;
		int text2PositionX = 0;
		int text2PositionY = 0;
		int winnerNumber = 0;
		int sizeText1 = 0;
		int sizeText2 = 0;

		void InitGameover()
		{
			sizeText1 = (GetScreenWidth() * 50) / 800;
			sizeText2 = (GetScreenWidth() * 20) / 800;
			text1PositionX = GetScreenWidth() / 2 - GetScreenWidth() * 0.2625;
			text1PositionY = GetScreenHeight() / 2 - GetScreenHeight() * 0.13333333;
			text2PositionX = GetScreenWidth() * 0.1875;
			text2PositionY = GetScreenHeight() / 2 + GetScreenHeight() * 0.13333333;
		}

		void UpdateGameOver()
		{
			if (IsKeyPressed(KEY_SPACE))
			{
				currentScreen = Menu;
				ResetValues();
			}
			if (IsKeyPressed(KEY_ESCAPE))
			{
				exit = true;
			}
		}

		void DrawGameOver()
		{
			if (winner == p1Wins)
			{
				winnerNumber = 1;
			}
			if (winner == p2Wins)
			{
				winnerNumber = 2;
			}

			ClearBackground(BLACK);
			DrawText(FormatText("Gano el jugador %01i!!", winnerNumber), text1PositionX, text1PositionY, sizeText1, GRAY);
			DrawText("Apreta ESPACIO para volver al menu, ESC para salir", text2PositionX, text2PositionY, sizeText2, GRAY);
		}


	}
}
