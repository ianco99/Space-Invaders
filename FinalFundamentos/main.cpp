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
	string playerName;
	bool stillRunning = true;
	ScreenCoordinates scrnCoord;
	GameStats gameStats;
	int prevOption = 1;
	int currentOption = 0;
	bool selected = false;
	int currentChallenge = 1000;
	system("CLS");


	while (stillRunning)
	{
		if (prevOption != currentOption)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
			gotoxy(0, 0);
			cout << R"(
______                                  _     _         _                                   _                        
|  _  \                                | |   (_)       | |                                 | |                       
| | | | ___  ___   ___  ___  _ __    __| |    _  _ __  | |_  ___      _ __ ___    __ _   __| | _ __    ___  ___  ___ 
| | | |/ _ \/ __| / __|/ _ \| '_ \  / _` |   | || '_ \ | __|/ _ \    | '_ ` _ \  / _` | / _` || '_ \  / _ \/ __|/ __|
| |/ /|  __/\__ \| (__|  __/| | | || (_| |   | || | | || |_| (_) |   | | | | | || (_| || (_| || | | ||  __/\__ \\__ \
|___/  \___||___/ \___|\___||_| |_| \__,_|   |_||_| |_| \__|\___/    |_| |_| |_| \__,_| \__,_||_| |_| \___||___/|___/     )";
			
			
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			PrintPlay(currentOption);
			PrintStats(currentOption);
			PrintQuit(currentOption);

			prevOption = currentOption;

		}
		
		selected = MenuInput(currentOption);
		

		if (selected)
		{
			
			switch (currentOption)
			{
			case 0:
				gameStats.timesPlayed++;
				system("CLS");
				PointsExplanations(scrnCoord);
				Startup(gameStats);
				prevOption = 1;
				break;
			case 1:
				StatsMenu(gameStats);
				prevOption = 0;
				break;
			case 2:
				stillRunning = false;
				break;
			default:
				break;
			}
		}
	}
}


bool MenuInput(int& currentOption)
{
	MenuControls control;
	char ans = _getch();
	control = (MenuControls)ans;
	
	switch (control)
	{
	case MenuControls::LeftArrow:
		if (currentOption == 0)
		{
			currentOption = 2;
		}
		else
		{
			currentOption--;
		}
		break;
	case MenuControls::RightArrow:
		if (currentOption == 2)
		{
			currentOption = 0;
		}
		else
		{
			currentOption++;
		}
		break;
	case MenuControls::Spacebar:
		return true;
		break;
	default:
		break;
	}
	return false;
}
//41 7
void StatsMenu(GameStats gameStats)
{
	int currentY = 5;
	int currentX = 38;

	int boxLengthX = 30;
	int boxLengthY = 17;

	char upL = 218;
	char upR = 191;
	char verLine = 179;
	char horLine = 196;
	char downL = 192;
	char downR = 217;
	system("CLS");

	//Top of box
	gotoxy(currentX, currentY);
	cout << upL;
	for (int i = 0; i < boxLengthX; i++)
	{
		cout << horLine;
	}
	cout << upR;
	
	
	//Box body

	currentY++;
	gotoxy(currentX, currentY);

	for (int i = 0; i < boxLengthY; i++)
	{
		cout << verLine;
		for (int j = 0; j < boxLengthX; j++)
		{
			cout << ' ';
		}
		cout << verLine;
		currentY++;
		gotoxy(currentX, currentY);	//hehehehe
	}


	cout << downL;
	for (int i = 0; i < boxLengthX; i++)
	{
		cout << horLine;
	}
	cout << downR;


	gotoxy(45, 7);
	cout << "Current challenge: ";
	gotoxy(52, 8);
		cout << gameStats.lastHighScore;
	gotoxy(48, 11);
	cout << "Challenger:";
	gotoxy(52, 12);
	cout << gameStats.playerName;

	gotoxy(48, 16);
	cout << "Times played: ";
	gotoxy(52, 17);
	cout << gameStats.timesPlayed;

	gotoxy(46, 20);
	cout << "Press spacebar to";
	gotoxy(47, 21);
	cout << "return to menu";

	bool validInput = false;
	char ans;

	while (!validInput)
	{
		ans = _getch();
		if (ans == ' ')
		{
			system("CLS");
			validInput = true;
		}
	}
	
}

void PrintPlay(int currentOption)
{
	int currentRow = 9;
	if (currentOption == 0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 112);
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
	gotoxy(10, currentRow);
	cout << "  _____  _             ";
	currentRow++;
	gotoxy(10, currentRow);
	cout << " |  __ " << (char)92 << "| |            ";
	currentRow++;
	gotoxy(10, currentRow);
	cout << " | |__) | | __ _ _   _ ";
		currentRow++;
	gotoxy(10, currentRow);
	cout << " |  ___/| |/ _` | | | |";
	currentRow++;
	gotoxy(10, currentRow);
	cout << " | |    | | (_| | |_| |";
	currentRow++;
	gotoxy(10, currentRow);
	cout << " |_|    |_|" << (char)92 << "__,_|" << (char)92 << "__, |";
	currentRow++;
	gotoxy(10, currentRow);
	cout << "                  __/ |";
	currentRow++;
	gotoxy(10, currentRow);
	cout << "                 |___/ ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void PrintStats(int currentOption)
{
	int currentRow = 9;
	if (currentOption == 1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 112);
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
	gotoxy(75, currentRow);
	cout << "   _____ _        _       ";
	currentRow++;
	gotoxy(75, currentRow);
	cout << "  / ____| |      | |      ";
	currentRow++;
	gotoxy(75, currentRow);
	cout << " | (___ | |_ __ _| |_ ___ ";
	currentRow++;
	gotoxy(75, currentRow);
	cout << "  " << (char)92 << "___ " << (char)92 << "| __/ _` | __/ __|";
	currentRow++;
	gotoxy(75, currentRow);
	cout << "  ____) | || (_| | |_" << (char)92 << "__ " << (char)92;
	currentRow++;
	gotoxy(75, currentRow);
	cout << " |_____/ " << (char)92 << "__" << (char)92 << "__,_|" << (char)92 << "__|___/";
	currentRow++;
	gotoxy(75, currentRow);
	cout << "                          ";
	currentRow++;
	gotoxy(75, currentRow);
	cout << "                          ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);


}

void PrintQuit(int currentOption)
{
	int currentX = 40;
	int currentRow = 19;
	if (currentOption == 2)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 112);
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
	gotoxy(currentX, currentRow);
	cout << "     ____        _ _      ";
	currentRow++;
	gotoxy(currentX, currentRow);
	cout << "    / __ " << (char)92 << "      (_) |     ";
	currentRow++;
	gotoxy(currentX, currentRow);
	cout << "   | |  | |_   _ _| |_    ";
	currentRow++;
	gotoxy(currentX, currentRow);
	cout << "   | |  | | | | | | __|   ";
	currentRow++;
	gotoxy(currentX, currentRow);
	cout << "   | |__| | |_| | | |_    ";
	currentRow++;
	gotoxy(currentX, currentRow);
	cout << "    " << (char)92 << "___" << (char)92 << '_' << (char)92 << (char)92 << "__,_|_|" << (char)92 << "__|   ";
	currentRow++;
	gotoxy(currentX, currentRow);
	cout << "                          ";
	currentRow++;
	gotoxy(currentX, currentRow);
	cout << "                          ";
	if (currentOption == 3)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
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
	
	gotoxy(5, 5);
	cout << "SPACE INVADERS EDITION" << endl;

	gotoxy(10, 10);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	wcout << (char)6;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); 
	cout << " = 30 POINTS" << endl;
	gotoxy(10, 11);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	cout << (char)4;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); 
	cout << " = 20 POINTS" << endl;
	gotoxy(10, 12);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
	cout << (char)190;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); 
	cout << " = 10 POINTS" << endl;
	char ans = _getch();

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	
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