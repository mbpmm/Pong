#ifndef JUEGO_H
#define JUEGO_H

namespace App
{
	namespace Game
	{
		enum Winner
		{
			nobody, p1Wins, p2Wins
		};

		void InitValues();
		void UpdateFrame();
		void Draw();
		void ResetValues();

		extern Winner winner;
		extern int pointsP1;
		extern int pointsP2;
	}
}
#endif // !JUEGO_H



