#include "Front.h"
#include <iostream>
#include <cstdlib>
#include <windows.h>

void ClearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR)' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

void Front::draw_game(const Game& g) const
{
	using std::cout;
	using std::wcout;
	using std::endl;
	
	ClearScreen();
	
	if (g != Game::progressing)
	{
		switch (g)
		{
		case Game::black_win:
			wcout << "Black Win.";
			break;
		case Game::white_win:
			wcout << "White Win.";
			break;
		case Game::error:
			wcout << "EORROR";
			break;
		}
		return;
	}

	cout << "xy|00|11|22|33|44|55|66|77|88|\n";
	for (size_t i = 0; i < 19; i++)
	{
		if (i % 2 == 1)
		{
			cout << i/2 << " ";
			for (size_t j = 0; j < 9; j++)
			{
				switch (g[(i-1)/2][j])
				{
				case Game::black:
					wcout << "|¡õ";
					break;
				case Game::white:
					wcout << "|¡ö";
					break;
				case Game::air:
					wcout << "|  ";
					break;
				}
			}
			wcout << "|" << endl;
		}
		else
		{
			wcout << "------------------------------" << endl;
		}
	}
}
