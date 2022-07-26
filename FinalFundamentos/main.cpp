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
	

	char ans;
	string playerName;
	bool stillRunning = true;
	ScreenCoordinates scrnCoord;

	while (stillRunning)
	{
		system("CLS");
		cout << R"(


______                                  _     _         _                                   _                        
|  _  \                                | |   (_)       | |                                 | |                       
| | | | ___  ___   ___  ___  _ __    __| |    _  _ __  | |_  ___      _ __ ___    __ _   __| | _ __    ___  ___  ___ 
| | | |/ _ \/ __| / __|/ _ \| '_ \  / _` |   | || '_ \ | __|/ _ \    | '_ ` _ \  / _` | / _` || '_ \  / _ \/ __|/ __|
| |/ /|  __/\__ \| (__|  __/| | | || (_| |   | || | | || |_| (_) |   | | | | | || (_| || (_| || | | ||  __/\__ \\__ \
|___/  \___||___/ \___|\___||_| |_| \__,_|   |_||_| |_| \__|\___/    |_| |_| |_| \__,_| \__,_||_| |_| \___||___/|___/                                                                                                                                                                         
            
              __      _____   _                           ___        _____  _          _         
             /_ | _  |  __ \ | |                         |__ \  _   / ____|| |        | |        
              | |(_) | |__) || |  __ _  _   _               ) |(_) | (___  | |_  __ _ | |_  ___  
              | |    |  ___/ | | / _` || | | |             / /      \___ \ | __|/ _` || __|/ __| 
              | | _  | |     | || (_| || |_| |            / /_  _   ____) || |_| (_| || |_ \__ \ 
              |_|(_) |_|     |_| \__,_| \__, |           |____|(_) |_____/  \__|\__,_| \__||___/ 
                                         __/ |  
                                        |___/                                           
                                
            
                                     _____      _____         _  _   
                                    |____ | _  |  _  |       (_)| |  
                                        / /(_) | | | | _   _  _ | |_ 
                                        \ \    | | | || | | || || __|
                                    .___/ / _  \ \/' /| |_| || || |_ 
                                    \____/ (_)  \_/\_\ \__,_||_| \__|
)" << endl;
		ans = _getch();
		if (ans == 49)
		{
			SetFontSize();
			system("CLS");
			PointsExplanations(scrnCoord);
			Startup();
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