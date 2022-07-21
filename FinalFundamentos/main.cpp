#include "main.h"

int main()
{
	srand(time(NULL));
	SetConsoleTitle((L"Space Invaders by Ian Kuznicki"));
	hidecursor();

	Menu();


}

void Menu()
{
	SetFontSize();

	char ans;
	string playerName;
	bool stillRunning = true;
	ScreenCoordinates scrnCoord;

	cout << "Type your name" << endl;
	cin >> playerName;


	while (stillRunning)
	{
		system("CLS");
		cout << "1: Play \n2: Stats \n3: Quit" << endl;
		ans = _getch();
		if (ans == 49)
		{
			system("CLS");
			PointsExplanations(scrnCoord);
			GameLoop(playerName);
		}
		else if (ans == 51)
		{
			stillRunning = false;
		}
	}
}

void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void PointsExplanations(ScreenCoordinates scrnCoord)
{
	gotoxy(0, 0);

	for (int j = 0; j < scrnCoord.screenSizeY; j++)
	{
		for (int i = 0; i < scrnCoord.screenSizeX; i++)
		{
			cout << (char)254;
			
		}
		Sleep(1);
		cout << '\n';
	}

	gotoxy(0, 0);
	for (int j = 0; j < scrnCoord.screenSizeY; j++)
	{
		for (int i = 0; i < scrnCoord.screenSizeX; i++)
		{
			cout << (char)255;
			
		}
		Sleep(1);
		cout << '\n';
	}
	
	

	gotoxy(10, 10);
	wcout << (char)6 << " = 30 POINTS" << endl;
	gotoxy(10, 11);
	cout << (char)4 << " = 20 POINTS" << endl;
	gotoxy(10, 12);
	cout << (char)190 << " = 10 POINTS" << endl;
	char ans = _getch();


	gotoxy(0, 0);

	for (int j = 0; j < scrnCoord.screenSizeY; j++)
	{
		for (int i = 0; i < scrnCoord.screenSizeX; i++)
		{
			cout << (char)254;

		}
		Sleep(1);
		cout << '\n';
	}

	gotoxy(0, 0);
	for (int j = 0; j < scrnCoord.screenSizeY; j++)
	{
		for (int i = 0; i < scrnCoord.screenSizeX; i++)
		{
			cout << (char)255;

		}
		Sleep(1);
		cout << '\n';
	}
}

void SetFontSize()
{
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX info{ sizeof(CONSOLE_FONT_INFOEX) };
	info.dwFontSize = COORD{ (short)12,(short)20 };
	SetCurrentConsoleFontEx(output, false, &info);
}
//cout << R"(
//
//
//	  .
//	 /║\
//	 │▓│	
//	 │▓│	
//	/   \
//	├───┤ 
//	│   │
//	│   │    
//    │   │ 
//    └───┘
//
//
//
//
//	┌───┐
//	│   │ 
//	│   │
//	│   │    
//    ├───┤ 
//    \   /
//	 │ │
//	 │ │
//	 \ /
//	  V	
//)" << endl;