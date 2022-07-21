#include "GameLoop.h"

//System

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

std::chrono::system_clock::time_point a = std::chrono::system_clock::now();
std::chrono::system_clock::time_point b = std::chrono::system_clock::now();

const int actionArrayLength = 3;	//Number of 'actions' player can do
const int movementArrayLength = 2;	//Player's movement commands ammount

const int alienLimitL = 2;	//Alien's limits in the screen on the left side
const int alienLimitR = 34;	//Alien's limits in the screen on the right side		65

const int intervalChangeInMov = 3;	//How often does the "intervalMovement" change

int gameSpeed = 20;	//How fast does the game run (in milliseconds)
int intervalMovement = 50;	//How often do the aliens move
int currIntervalChangeInMov = 0;	//Current progress in the interval
int intervalBulletSpeed = 5;	//How often does the bullets move

const char upL = 218;	//Character for upper left corner
const char upR = 191;	//Character for upper right corner
const char horLine = 196;	//Character for horizontal line
const char verLine = 179;	//Character for vertical line
const char downL = 192;	//Character for lower left corner
const char downR = 217;	//Character for lower right corner

int movementKeys[movementArrayLength]{75,77 };	//Characters for player's movement
char actionKeys[actionArrayLength]{ ' ','p','P' };	//Characters for player's actions

//Enemies
const int enemyAmountMax = 55;	//Amount of starting enemies
const int differenceBtwn = 2;	//Space between each enemy
const int totalAlienBullets = 3;

int aliensAlive = enemyAmountMax;	//How many aliens are alive

const char alienProjectile = 118;	//Character for alien's projectile

bool movingRight = true;	//Are enemies moving to the right

Enemy enemyArray[enemyAmountMax+1];	//Access to all existing enemies

//Player

Player player{ 15,27 };	//Current player

int playerScore = 0;

//Deltatime es el current time en segundos - el tiempo previo en segundos
void GameLoop(string playerName)
{

	playerScore = 0;

	bool stillPlaying = true;	//Player is still playing

	Enemy desiredAlien;	//Alien that shoots a bullet
	Enemy spaceShip;
	spaceShip.alive = false;
	spaceShip.alienType = EnemyTypes::SpaceShip;
	enemyArray[enemyAmountMax] = spaceShip;
	spaceShip.alive = false;  //this is wrong
	
	Bullet bullet;	//Player's bullet
	Bullet alienBullet{0,0,false,alienProjectile,false};	//Alien's bullet
	
	Cover playerCovers[4];
	
	ScreenCoordinates scrnCoord;	//Screen's data and HUD coordinates

	PlayConfigs playConfig{ playerName };	//Configurations for current game

	GameStates currState;	//Is game currently running, in pause or finished

	int currIntervalMovement = 0;	//Current count used in movement's interval
	int currIntervalBulletSpeed = 0;	//Current count used in bulletSpeed's interval

	HeadsUpDisplay(playConfig, scrnCoord);	//Print HUD

	CreateAliens(scrnCoord);	//Create enemies

	PrintAliens();	//Print enemies

	PrintPlayer(player);	//Print player

	PrintCovers(playerCovers, scrnCoord);

	while (stillPlaying)	//While player is still playing
	{
		currState = WinConditions();	//Determine if game is over

		switch (currState)	//Switch based on current state of the game
		{
			case GameStates::StillPlaying:	//If player is still playing
			{

				BulletWork(bullet, alienBullet,playerCovers,player, scrnCoord, playConfig);	//Make bullet work

				// Maintain designated frequency of 5 Hz (200 ms per frame)
				a = std::chrono::system_clock::now();
				std::chrono::duration<double, std::milli> work_time = a - b;

				if (work_time.count() < gameSpeed) 
				{
					std::chrono::duration<double, std::milli> delta_ms(gameSpeed - work_time.count());
					auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
					std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
				}

				b = std::chrono::system_clock::now();
				std::chrono::duration<double, std::milli> sleep_time = b - a;

				// Your code here
				currIntervalMovement++;	//Time has passed
				currIntervalBulletSpeed++;	//Time has passed

				if (currIntervalMovement >= intervalMovement)	//Enough time has passed for aliens to start continue moving
				{
					if (!alienBullet.alive)	//There is not a bullet on screen
					{
						desiredAlien = AliensAttack(alienBullet);	//Select an alien from alien pool
					}

					MoveAliens();	//Move aliens

					if (!enemyArray[enemyAmountMax].alive)
					{
						trySpawnSpaceShip();
					}
					

					currIntervalMovement = 0;	//Restart interval
				}

				if (currIntervalBulletSpeed >= intervalBulletSpeed)	//Enough time has passed for bullets to continue moving
				{
					BulletWork(alienBullet, bullet, playerCovers, desiredAlien, scrnCoord, playConfig);	//Move alien bullets

					currIntervalBulletSpeed = 0;	//Restart interval
				}


				TakeInput(player, bullet);	//Player's input registration
			}
			break;

			case GameStates::Lost:	//Player has lost
				LoseScreen();	//Go to lose screen
				stillPlaying = false;	//Player is no longer playing
				break;
			case GameStates::Won:	//Player has won
				WinScreen();	//Go to win screen
				stillPlaying = false;	//Player is no longer playing
				break;
			default:
				break;
		}
		
	}
}

//Has game ended
GameStates WinConditions()
{
	bool allAliensDead = true;
	bool aliensReachedBottom = false;

	for (int i = 0; i < enemyAmountMax; i++)
	{
		if (enemyArray[i].alive)
		{
			allAliensDead = false;
		}
	}

	if (allAliensDead)
	{
		return GameStates::Won;
	}
	else
	{
		for (int i = 0; i < enemyAmountMax; i++)
		{
			if (enemyArray[i].y == player.y && enemyArray[i].alive)
			{
				aliensReachedBottom = true;
				break;
			}
		}
		if (player.lifes <= 0)
		{
			return GameStates::Lost;
		}
		else if (aliensReachedBottom)
		{
			return GameStates::Lost;
		}
		else
		{
			return GameStates::StillPlaying;
		}
	}
}

//Prints win screen
void WinScreen()
{
	system("CLS");
	cout << "YOU WON" << endl;
	char ans = _getch();
}

//Prints lose screen
void LoseScreen()
{
	system("CLS");
	cout << "YOU LOST" << endl;
	char ans = _getch();
}

//Prints UI
void HeadsUpDisplay(PlayConfigs playConfig, ScreenCoordinates scrnCoord)
{
	ScreenBorder(scrnCoord);

	StatsWriter(playConfig, scrnCoord);
}

//Prints border of the screen 
void ScreenBorder(ScreenCoordinates scrnCoord)
{
	gotoxy(0, 0);
	cout << upL;

	for (int i = 0; i < scrnCoord.screenSizeX; i++)
	{
		cout << horLine;
	}

	cout << upR;

	cout << '\n';

	for (int i = 1; i < scrnCoord.screenSizeY; i++)
	{
		cout << verLine;
		gotoxy(scrnCoord.screenSizeX + 1, i);
		cout << verLine;
		cout << '\n';
	}

	cout << downL;
	for (int i = 0; i < scrnCoord.screenSizeX; i++)
	{
		cout << horLine;
	}
	cout << downR;
}

//Writes player's stats onto the screen
void StatsWriter(PlayConfigs playConfig, ScreenCoordinates scrnCoord)
{
	gotoxy(scrnCoord.nameX, scrnCoord.nameY);
	cout << playConfig.playerName;
	
	gotoxy(scrnCoord.lifesX, scrnCoord.lifesY);
	cout << "Lifes: " << player.lifes;

	gotoxy(scrnCoord.ScoreX, scrnCoord.ScoreY);
	cout << "Score: " << playerScore;
}

//Create enemies for the game
void CreateAliens(ScreenCoordinates scrnCoord)
{
	int currentY = 2;
	int rowCount = 0;
	bool newLine = false;

	for (int i = 0; i < enemyAmountMax; i++)
	{
		
		if (i == 0)
		{
			enemyArray[i].x = scrnCoord.ScreenStartX;
		}
		else
		{
			if (0 == i % 11)
			{
				currentY += 3;
				newLine = true;
				rowCount++;
			}

			if (!newLine)
			{
				int startPoint = enemyArray[i - 1].x;
				enemyArray[i].x = startPoint + differenceBtwn;
			}
			else
			{
				enemyArray[i].x = scrnCoord.ScreenStartX;
			}
			
		}

		enemyArray[i].y = currentY;
		newLine = false;

		//Defining type and score it gives
		switch (rowCount)
		{
		case 0:
			enemyArray[i].alienType = EnemyTypes::UpperAlien;
			enemyArray[i].icon = (char)6;
			break;
		case 1:
		case 2:
			enemyArray[i].alienType = EnemyTypes::MiddleAlien;
			enemyArray[i].icon = (char)4;
			break;
		case 3:
		case 4:
			enemyArray[i].alienType = EnemyTypes::LowerAlien;
			enemyArray[i].icon = (char)190;
			break;
		}
	}
}

//Makes a given bullet move across the screen depending its orientation
void BulletWork(Bullet& bullet, Bullet& alienBullet, Cover playerCovers[], Player origin, ScreenCoordinates scrnCoord, PlayConfigs playConfig)
{
	bool hit = false;

	if (bullet.alive)
	{
		

		if (bullet.goesUp)
		{
			bullet.y--;
		}
		else
		{
			bullet.y++;
		}

		if (bullet.y <= 1)
		{
			bullet.alive = false;
			
			hit = true;
		}
		else if (bullet.y >= scrnCoord.screenSizeY-1)
		{
			bullet.alive = false;

			hit = true;
		}

		if (bullet.goesUp)		//If it's player's bullet
		{
			if (alienBullet.x == bullet.x && alienBullet.y == bullet.y && alienBullet.alive)
			{
				gotoxy(bullet.x, bullet.y);
				SetConsoleTextAttribute(hConsole, 0);
				cout << ' ';
				SetConsoleTextAttribute(hConsole, 7);
				hit = true;
				bullet.alive = false;
				alienBullet.alive = false;
				
			}
			else
			{
				for (int i = 0; i <= enemyAmountMax; i++)	//Check if bullet has hit an alien
				{
					if (enemyArray[i].x == bullet.x && enemyArray[i].y == bullet.y && enemyArray[i].alive)
					{
						HitAlien(bullet, playConfig, scrnCoord, i, hit);
						aliensAlive--;
						break;
					}
					else if (enemyArray[i].x == bullet.x && enemyArray[i].y == bullet.y + 1 && enemyArray[i].alive)
					{
						HitAlien(bullet, playConfig, scrnCoord, i, hit);
						aliensAlive--;
						break;
					}
				}
			}
		}
		else
		{
			bool hasHitPlayer = false;

			for (int i = player.x - 1; i <= player.x + 1; i++)
			{
				if (bullet.x == i && bullet.y == player.y)
				{
					hasHitPlayer = true;
					break;
				}
			}
			if (hasHitPlayer)
			{
				bullet.alive = false;
				player.lifes--;
				hit = true;
				PlayerHitEffect(scrnCoord,playerCovers,playConfig);
				StatsWriter(playConfig, scrnCoord);
			}
		}
		bool bulletCrashedWithCover = false;
		for (int i = 0; i < 4; i++)
		{
			if (bulletCrashedWithCover)
			{
				break;
			}
			for (int j = 0; j < 4; j++)
			{
				if (playerCovers[i].walls[j].x == bullet.x && playerCovers[i].walls[j].y == bullet.y && playerCovers[i].walls[j].life > 0)
				{
					playerCovers[i].walls[j].life--;
					if (playerCovers[i].walls[j].life == 0)
					{
						gotoxy(playerCovers[i].walls[j].x, playerCovers[i].walls[j].y);
						SetConsoleTextAttribute(hConsole, 0);
						cout << ' ';
						SetConsoleTextAttribute(hConsole, 7);
						hit = true;
						bullet.alive = false;
						bulletCrashedWithCover = true;
						break;
					}
					else
					{
						gotoxy(playerCovers[i].walls[j].x, playerCovers[i].walls[j].y);
						if (playerCovers[i].walls[j].y != scrnCoord.CoverY)
						{
							cout << (char)242;
						}
						else
						{
							cout << playerCovers[i].walls[j].halfLife;
						}
						hit = true;
						bullet.alive = false;
						bulletCrashedWithCover = true;
						break;
					}
				}
			}
		}
		

		if (!hit)
		{
			PrintBullet(bullet, origin);
		}
		else
		{
			if (bullet.goesUp)
			{
				if (!bulletCrashedWithCover)
				{
					gotoxy(bullet.x, bullet.y);
					SetConsoleTextAttribute(hConsole, 0);
					cout << ' ';
					SetConsoleTextAttribute(hConsole, 7);
				}
				
				if (origin.y != bullet.y + 1 && bullet.goesUp)
				{
					gotoxy(bullet.x, bullet.y + 1);
					SetConsoleTextAttribute(hConsole, 0);
					cout << ' ';
					SetConsoleTextAttribute(hConsole, 7);
				}
				else if (origin.y != bullet.y - 1 && !bullet.goesUp)
				{
					gotoxy(bullet.x, bullet.y - 1);
					SetConsoleTextAttribute(hConsole, 0);
					cout << ' ';
					SetConsoleTextAttribute(hConsole, 7);
				}
			}
			else
			{
				if (origin.y != bullet.y + 1 && bullet.goesUp)
				{
					gotoxy(bullet.x, bullet.y + 1);
					SetConsoleTextAttribute(hConsole, 0);
					cout << ' ';
					SetConsoleTextAttribute(hConsole, 7);
				}
				else if (origin.y != bullet.y - 1 && !bullet.goesUp)
				{
					gotoxy(bullet.x, bullet.y - 1);
					SetConsoleTextAttribute(hConsole, 0);
					cout << ' ';
					SetConsoleTextAttribute(hConsole, 7);
				}
			}
		}
	}
}				

void HitAlien(Bullet& bullet, PlayConfigs playConfig, ScreenCoordinates scrnCoord, int index, bool& hit)
{
	bullet.alive = false;
	enemyArray[index].alive = false;

	currIntervalChangeInMov++;

	if (currIntervalChangeInMov == intervalChangeInMov)
	{
		intervalMovement-=4;
		currIntervalChangeInMov = 0;
	}

	hit = true;
	gotoxy(enemyArray[index].x, enemyArray[index].y);
	SetConsoleTextAttribute(hConsole, 0);
	cout << ' ';
	SetConsoleTextAttribute(hConsole, 7);

	playerScore += (int)enemyArray[index].alienType;
	StatsWriter(playConfig, scrnCoord);
}

//Move every alien alive in the screen
void MoveAliens()
{
	int currentY;
	bool moveY = false;

	for (int i = 0; i < enemyAmountMax; i++)
	{
		if (enemyArray[i].x >= alienLimitR && enemyArray[i].alive)
		{
			moveY = true;
			movingRight = false;
		}
		else if(enemyArray[i].x <= alienLimitL && enemyArray[i].alive)
		{ 
			moveY = true;
			movingRight = true;
		}
	}


	for (int i = 0; i < enemyAmountMax; i++)
	{
		if (enemyArray[i].alive)
		{
			gotoxy(enemyArray[i].x, enemyArray[i].y);
			SetConsoleTextAttribute(hConsole, 0);
			cout << ' ';
			SetConsoleTextAttribute(hConsole, 7);

			if (moveY)
			{
				enemyArray[i].y++;
			}

			
			if (movingRight)
			{
				enemyArray[i].x++;
			}
			else
			{
				enemyArray[i].x--;
			}

		}
		
	}

	PrintAliens();

	if (enemyArray[enemyAmountMax].alive)
	{
		if (enemyArray[enemyAmountMax].x == alienLimitR)
		{

		}
		else
		{
			gotoxy(enemyArray[enemyAmountMax].x, enemyArray[enemyAmountMax].y);
			SetConsoleTextAttribute(hConsole, 0);
			cout << ' ';
			SetConsoleTextAttribute(hConsole, 7);
			enemyArray[enemyAmountMax].x++;
			gotoxy(enemyArray[enemyAmountMax].x, enemyArray[enemyAmountMax].y);
			cout << enemyArray[enemyAmountMax].icon;
		}
		

	}
}

void trySpawnSpaceShip()
{
	if (aliensAlive <= enemyAmountMax / 2)
	{
		int randResult = rand() % 10;

		if (randResult == 5)
		{
			SpawnSpaceShip();
		}
	}
}

void SpawnSpaceShip()
{
	ScreenCoordinates scrnCoord;

	gotoxy(1, 1);
 	enemyArray[enemyAmountMax].alive = true;
	enemyArray[enemyAmountMax].x = 1;
	enemyArray[enemyAmountMax].y = 1;
	cout << enemyArray[enemyAmountMax].icon;
}

//Make a designated alien shoot a bullet
Enemy AliensAttack(Bullet& alienBullet)
{
	int aliensXpos[enemyAmountMax];
	int emptySlot = 200;

	Enemy desiredAlien;

	for (int i = 0; i < enemyAmountMax; i++)
	{
		aliensXpos[i] = emptySlot;
	}

	int counter = 0;

	for (int i = 0; i < enemyAmountMax; i++)
	{
		if (enemyArray[i].alive)
		{
			bool isRepeated = false;

			for (int j = 0; j < enemyAmountMax; j++)
			{
				if (enemyArray[i].x == aliensXpos[j] && enemyArray[j].x != emptySlot)
				{
					isRepeated = true;
					break;
				}
			}

			if (!isRepeated)
			{
				aliensXpos[counter] = enemyArray[i].x;
				counter++;
			}
		}
	}

	if (counter > 0)
	{
		int randPos = rand() % counter;

		int Ypos = 0;

		for (int i = 0; i < enemyAmountMax; i++)
		{
			if (enemyArray[i].x == aliensXpos[randPos] && Ypos < enemyArray[i].y)
			{
				Ypos = enemyArray[i].y;
			}
		}

		for (int i = 0; i < enemyAmountMax; i++)
		{
			if (enemyArray[i].x == aliensXpos[randPos] && enemyArray[i].y == Ypos)
			{
				desiredAlien = enemyArray[i];
			}
		}

		alienBullet.alive = true;
		alienBullet.x = desiredAlien.x;
		alienBullet.y = desiredAlien.y;

	}
	return desiredAlien;
}

//Recieve player's input
void TakeInput(Player& player, Bullet& bullet)
{

	if (_kbhit())
	{
		

		int ans = _getch();
		bool movementKey = false;
		bool actionKey = false;

		for (int i = 0; i < movementArrayLength; i++)
		{
			if (movementKeys[i] == ans)
			{
				movementKey = true;
			}
		}

		if (!movementKey)
		{
			for (int i = 0; i < actionArrayLength; i++)
			{
				if (actionKeys[i] == ans)
				{
					actionKey = true;
				}
			}
		}

		if (movementKey)
		{
			MovementKeys key = (MovementKeys)ans;
			ScreenCoordinates scrnCoord;
			switch (key)
			{
			case MovementKeys::Right:

				if (player.x < scrnCoord.screenSizeX-1)
				{
					for (int i = player.y - 1; i <= player.y + 1; i++)
					{
						gotoxy(player.x - 1, i);
						SetConsoleTextAttribute(hConsole, 0);
						cout << ' ';
						SetConsoleTextAttribute(hConsole, 7);
					}
					player.x++;
				}
				

				
				break;
			case MovementKeys::Left:

				if (player.x > 2)
				{
					for (int i = player.y - 1; i <= player.y + 1; i++)
					{
						gotoxy(player.x + 1, i);
						SetConsoleTextAttribute(hConsole, 0);
						cout << ' ';
						SetConsoleTextAttribute(hConsole, 7);
					}
					player.x--;
				}
				

				
				break;
			default:
				break;
			}
		}
		else if (actionKey)
		{
			ActionKeys key = (ActionKeys)ans;
			
			switch (key)
			{
			case ActionKeys::Shoot:

				if (!bullet.alive)
				{
					bullet.x = player.x;
					bullet.y = player.y-1;
					bullet.alive = true;
				}
				
				break;
			case ActionKeys::Pause:
				break;
			case ActionKeys::PauseAlt:
				break;
			default:
				break;
			}
		}

		if (movementKey)
		{
			
			PrintPlayer(player);

		}
		
		
	}
}

//Print bullet on the screen
void PrintBullet(Bullet bullet, Player origin)
{
	gotoxy(bullet.x, bullet.y);
	cout << bullet.icon;
	if (origin.y - 1 != bullet.y + 1 && bullet.goesUp)
	{
		gotoxy(bullet.x, bullet.y + 1);
		SetConsoleTextAttribute(hConsole, 0);
		cout << ' ';
		SetConsoleTextAttribute(hConsole, 7);
	}
	else if((origin.y != bullet.y - 1 && !bullet.goesUp))
	{
		gotoxy(bullet.x, bullet.y - 1);
		SetConsoleTextAttribute(hConsole, 0);
		cout << ' ';
		SetConsoleTextAttribute(hConsole, 7);
	}
	
}

//Print player on the screen
void PrintPlayer(Player player)
{
	SetConsoleTextAttribute(hConsole, 10);
	for (int j = player.x - 1; j <= player.x + 1; j++)
	{
		gotoxy(j, player.y);
		if (j == player.x) //Center up
		{

			cout << '^';
		}
		else
		{
			cout << (char)254;
		}
	}
	SetConsoleTextAttribute(hConsole, 7);
}

//Print enemies on the scren
void PrintAliens()
{
	for (int i = 0; i < enemyAmountMax; i++)
	{
		if (enemyArray[i].alive)
		{
			gotoxy(enemyArray[i].x, enemyArray[i].y);
			cout << enemyArray[i].icon;
		}
	}

}

void PrintCovers(Cover playerCovers[], ScreenCoordinates scrnCoord)
{
	int currentX = scrnCoord.CoverStartX;

	for (int j = 0; j < 4; j++)
	{
		int currentWall = 0;
		for (int i = currentX - 1; i <= currentX + 1; i++)
		{

			gotoxy(i, scrnCoord.CoverY);

			cout << (char)219;

			playerCovers[j].walls[currentWall].x = i;
			playerCovers[j].walls[currentWall].y = scrnCoord.CoverY;
			currentWall++;
		}
		gotoxy(currentX, scrnCoord.CoverY - 1);
		cout << (char)220;
		playerCovers[j].walls[currentWall].x = currentX;
		playerCovers[j].walls[currentWall].y = scrnCoord.CoverY-1;
		currentX += scrnCoord.DistanceBtwnCovers;
	}
}

void PlayerHitEffect(ScreenCoordinates scrnCoord, Cover playerCovers[], PlayConfigs playConfig)
{
	SetConsoleTextAttribute(hConsole, 12);
	HeadsUpDisplay(playConfig, scrnCoord);	//Print HUD
	Sleep(80);
	SetConsoleTextAttribute(hConsole, 7);
	HeadsUpDisplay(playConfig, scrnCoord);	//Print HUD
	
}

//Make console cursor go to designated x and y coordinates
void gotoxy(int x, int y) 
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { (short)x,(short)y });
}

//HWND window;
//window = FindWindowA(NULL, "Space Invaders by Ian Kuznicki");
//int x = cos(gameSpeed * 9 * 0.01745329f * 18);
//int y = sin(gameSpeed * 0.8f * 0.01745329f * 10);
//SetWindowPos(window, NULL, x, y, NULL, NULL, SWP_NOSIZE);