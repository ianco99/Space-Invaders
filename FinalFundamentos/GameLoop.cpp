#include "GameLoop.h"

//System

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

//std::chrono::system_clock::time_point a = std::chrono::system_clock::now();
//std::chrono::system_clock::time_point b = std::chrono::system_clock::now();

auto curT = std::chrono::system_clock::now();
auto preT = std::chrono::system_clock::now();

std::chrono::duration<float> dT;
float _deltaTime;


//Deltatime es el current time en segundos - el tiempo previo en segundos

void Startup(GameStats& gameStats)
{
	Enemy enemyArray[56];	//Access to all existing enemies

	ScreenCoordinates scrnCoord;	//Screen's data and HUD coordinates

	PlayConfigs playConfig;	//Configurations for current game

	gameStats.aliensAlive = playConfig.enemyAmountMax;
	gameStats.lifes = 3;
	gameStats.playerScore = 0;

	HeadsUpDisplay(scrnCoord, gameStats);	//Print HUD

	CreateAliens(scrnCoord, playConfig, enemyArray);	//Create enemies

	GameLoop(playConfig, scrnCoord, gameStats, enemyArray);
}

void GameLoop(PlayConfigs& playConfig, ScreenCoordinates scrnCoord, GameStats& gameStats, Enemy enemyArray[])
{
	Enemy desiredAlien;	//Alien that shoots a bullet
	Enemy spaceShip{ 0,0,(char)203,1,false,false,EnemyTypes::SpaceShip,true };

	const int actionArrayLength = 3;	//Number of 'actions' player can do
	const int movementArrayLength = 2;	//Player's movement commands ammount

	int movementKeys[movementArrayLength]{ 75,77 };	//Characters for player's movement
	char actionKeys[actionArrayLength]{ ' ','q','Q' };	//Characters for player's actions

	enemyArray[playConfig.enemyAmountMax] = spaceShip;

	Bullet bullet;	//Player's bullet
	Bullet alienBullet{ 0,0,false,(char)173,false };	//Alien's bullet

	Cover playerCovers[4];

	bool stillPlaying = true;	//Player is still playing
	bool wantsQuit = false;

	GameStates currState;	//Is game currently running, in pause or finished

	Player player{ 15,27 };	//Current player

	StartPrint(playerCovers, scrnCoord, playConfig, player, enemyArray);


	while (stillPlaying)	//While player is still playing
	{
		currState = WinConditions(playConfig, gameStats, player, enemyArray);	//Determine if game is over
		if (wantsQuit)
		{
			currState = GameStates::Quit;
		}
		curT = std::chrono::system_clock::now();
		dT = curT - preT;
		_deltaTime = dT.count();

		if (_deltaTime < 1.0 / playConfig.gameSpeed)
		{
			continue;
		}
		preT = curT;


		switch (currState)	//Switch based on current state of the game
		{
		case GameStates::StillPlaying:	//If player is still playing
		{

			// Your code here
			gameStats.currIntervalMovement++;	//Time has passed
			gameStats.currIntervalBulletSpeed++;	//Time has passed
			gameStats.currIntervalPlayerBulletSpeed++;

			if (gameStats.currIntervalPlayerBulletSpeed >= playConfig.IntervalPlayerBulletSpeed)
			{
				
				BulletWork(bullet, alienBullet, playerCovers, { (short)player.x, (short)player.y }, scrnCoord, playConfig, gameStats, player, enemyArray);	//Make bullet work
				gameStats.currIntervalPlayerBulletSpeed = 0;

			}

			if (gameStats.currIntervalMovement >= playConfig.intervalMovement)	//Enough time has passed for aliens to start continue moving
			{
				CheckParticles(playConfig, enemyArray);
				if (!alienBullet.alive)	//There is not a bullet on screen
				{
					desiredAlien = AliensAttack(alienBullet, playConfig, enemyArray);	//Select an alien from alien pool
				}

				MoveAliens(playConfig, gameStats, scrnCoord, enemyArray);	//Move aliens

				if (!enemyArray[playConfig.enemyAmountMax].alive)
				{
					if (trySpawnSpaceShip(gameStats, playConfig))
					{
						SpawnSpaceShip(playConfig, gameStats, enemyArray);
					}
				}


				gameStats.currIntervalMovement = 0;	//Restart interval
			}

			if (gameStats.currIntervalBulletSpeed >= playConfig.intervalBulletSpeed)	//Enough time has passed for bullets to continue moving
			{
				BulletWork(alienBullet, bullet, playerCovers, { (short)desiredAlien.x, (short)desiredAlien.y }, scrnCoord, playConfig, gameStats, player, enemyArray);	//Move alien bullets

				gameStats.currIntervalBulletSpeed = 0;	//Restart interval

				MoveSpaceShip(enemyArray[playConfig.enemyAmountMax], scrnCoord);
			}


			wantsQuit = TakeInput(player, bullet, movementKeys, actionKeys);	//Player's input registration

		}
		break;

		case GameStates::Lost:	//Player has lost
			LoseScreen();	//Go to lose screen
			stillPlaying = false;	//Player is no longer playing
			break;
		case GameStates::Won:	//Player has won
			CheckScore(gameStats);
			WinScreen();	//Go to win screen
			stillPlaying = false;	//Player is no longer playing
			break;
		case GameStates::Quit:
			system("CLS");
			stillPlaying = false;
			break;
		default:
			break;
		}

	}
}

bool willQuit()
{
	int currentRow = 4;

	//Cartelito de will quit
	SetConsoleTextAttribute(hConsole, 96);
	for (int i = 0; i < 8; i++)
	{
		gotoxy(4, currentRow);
		for (int j = 0; j < 29; j++)
		{
			cout << (char)255;
		}
		currentRow++;
	}

	currentRow = 5;
	gotoxy(16, currentRow);
	cout << (char)218;
	for (int i = 0; i < 3; i++)
	{
		cout << (char)196;
	}
	cout << (char)191;
	currentRow++;
	gotoxy(16, currentRow);
	cout << (char)179 << ' ' << '!' << ' ' << (char)179;
	currentRow++;
	gotoxy(16, currentRow);
	cout << (char)192;
	for (int i = 0; i < 3; i++)
	{
		cout << (char)196;
	}
	cout << (char)217;


	currentRow++;
	gotoxy(7, currentRow);
	cout << "Are you sure you want";
	currentRow++;
	gotoxy(10, currentRow);
	cout << "to quit to menu ?";
	SetConsoleTextAttribute(hConsole, 7);
	char ans = _getch();
	if (ans == 'q')
	{
		system("CLS");
		return false;
	}
	else
	{
		system("CLS");
		//print
		return true;
	}
}

void StartPrint(Cover playerCovers[], ScreenCoordinates scrnCoord, PlayConfigs playConfig, Player player, Enemy enemyArray[])
{
	PrintAliens(playConfig, enemyArray);	//Print enemies

	PrintPlayer(player);	//Print player

	PrintCovers(playerCovers, scrnCoord);

	PrintControls(scrnCoord);
}

void PrintControls(ScreenCoordinates scrnCoord)
{

	gotoxy(scrnCoord.ControlsX - 4, scrnCoord.ControlsY);
	cout << "Move Left:    Move Right:";
	gotoxy(scrnCoord.ControlsX, scrnCoord.ControlsY + 1);

	cout << (char)218 << (char)196 << (char)196 << (char)196 << (char)191 << "       " << (char)218 << (char)196 << (char)196 << (char)196 << (char)191;

	gotoxy(scrnCoord.ControlsX, scrnCoord.ControlsY + 2);
	cout << (char)179 << ' ' << (char)17 << ' ' << (char)179 << "       " << (char)179 << ' ' << (char)16 << ' ' << (char)179;

	gotoxy(scrnCoord.ControlsX, scrnCoord.ControlsY + 3);
	cout << (char)192 << (char)196 << (char)196 << (char)196 << (char)217 << "       " << (char)192 << (char)196 << (char)196 << (char)196 << (char)217;


	gotoxy(scrnCoord.ControlsX, scrnCoord.ControlsY + 5);
	cout << "      Shoot:";
	gotoxy(scrnCoord.ControlsX, scrnCoord.ControlsY + 6);
	cout << (char)218 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)191;
	gotoxy(scrnCoord.ControlsX, scrnCoord.ControlsY + 7);
	cout << (char)179 << ' ' << ' ';

	SetConsoleTextAttribute(hConsole, 255);
	cout << (char)255 << (char)255 << (char)255 << (char)255 << (char)255 << (char)255 << (char)255 << (char)255 << (char)255 << (char)255 << (char)255;
	SetConsoleTextAttribute(hConsole, 7);

	cout << ' ' << ' ' << (char)179;
	gotoxy(scrnCoord.ControlsX, scrnCoord.ControlsY + 8);
	cout << (char)192 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)196 << (char)217;
}

//Has game ended
GameStates WinConditions(PlayConfigs& playConfig, GameStats& gameStats, Player player, Enemy enemyArray[])
{
	bool allAliensDead = true;
	bool aliensReachedBottom = false;

	for (int i = 0; i < playConfig.enemyAmountMax; i++)
	{
		if (enemyArray[i].alive)
		{
			allAliensDead = false;
		}
	}


	if (gameStats.aliensAlive == 0)
	{
		return GameStates::Won;
	}
	else if (gameStats.aliensAlive == 10)
	{
		playConfig.intervalMovement = 22;
		playConfig.intervalChangeInMov = 2;
		return GameStates::StillPlaying;
	}
	else
	{
		for (int i = 0; i < playConfig.enemyAmountMax; i++)
		{
			if (enemyArray[i].y == player.y && enemyArray[i].alive)
			{
				aliensReachedBottom = true;
				break;
			}
		}
		if (gameStats.lifes <= 0)
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

void CheckScore(GameStats& gameStats)
{
	string newName;
	char ans;
	bool validInput = false;

	if (gameStats.playerScore > gameStats.lastHighScore)
	{
		system("CLS");
		while (!validInput)
		{
			
			gotoxy(15, 10);
			cout << "CONGRATULATIONS, YOU GOT A NEW HIGHSCORE";
			gotoxy(18, 11);
			cout << "PLEASE INSERT YOUR NAME";
			gotoxy(20, 12);
			cin >> newName;
			if (newName.size() < 16)
			{
				validInput = true;
			}
			else
			{
				gotoxy(20, 12);
				for (int i = 0; i < newName.size(); i++)
				{
					cout << ' ';
				}
				gotoxy(20, 13);
				cout << "Name is too long, please keep it under 16 characters";
			}
		}
		

		gameStats.lastHighScore = gameStats.playerScore;
		gameStats.playerName = newName;
	}
}

//Prints win screen
void WinScreen()
{
	bool validInput = false;
	system("CLS");
	cout << R"(

 __     ______  _    _  __          ______  _   _ _ 
 \ \   / / __ \| |  | | \ \        / / __ \| \ | | |
  \ \_/ / |  | | |  | |  \ \  /\  / / |  | |  \| | |
   \   /| |  | | |  | |   \ \/  \/ /| |  | | . ` | |
    | | | |__| | |__| |    \  /\  / | |__| | |\  |_|
    |_|  \____/ \____/      \/  \/   \____/|_| \_(_)
                                                    
                                                    
                                                    
                                                    
		Press spacebar to continue
)" << endl;
	char ans;
	while (!validInput)
	{
		ans = _getch();

		if (ans == ' ')
		{
			validInput = true;
		}
	}
	system("CLS");
}

//Prints lose screen
void LoseScreen()
{
	bool validInput = false;
	system("CLS");
	cout << R"(

 __     ______  _    _   _      ____   _____ _______ 
 \ \   / / __ \| |  | | | |    / __ \ / ____|__   __|
  \ \_/ / |  | | |  | | | |   | |  | | (___    | |   
   \   /| |  | | |  | | | |   | |  | |\___ \   | |   
    | | | |__| | |__| | | |___| |__| |____) |  | |   
    |_|  \____/ \____/  |______\____/|_____/   |_|   
                                                                                                      
                                                    
                                                    
                                                    
		Press spacebar to continue
)" << endl;
	char ans;
	while (!validInput)
	{
		ans = _getch();

		if (ans == ' ')
		{
			validInput = true;
		}
	}
	system("CLS");
}

//Prints UI
void HeadsUpDisplay(ScreenCoordinates scrnCoord, GameStats gameStats)
{
	ScreenBorder(scrnCoord);

	StatsWriter(scrnCoord, gameStats);
}

//Prints border of the screen 
void ScreenBorder(ScreenCoordinates scrnCoord)
{
	const char upL = 218;	//Character for upper left corner
	const char upR = 191;	//Character for upper right corner
	const char horLine = 196;	//Character for horizontal line
	const char verLine = 179;	//Character for vertical line
	const char downL = 192;	//Character for lower left corner
	const char downR = 217;	//Character for lower right corner

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
void StatsWriter(ScreenCoordinates scrnCoord, GameStats gameStats)
{
	gotoxy(scrnCoord.lifesX, scrnCoord.lifesY);
	cout << "Lifes: " << gameStats.lifes;

	gotoxy(scrnCoord.ScoreX, scrnCoord.ScoreY);
	cout << "Score: " << gameStats.playerScore;
}

//Create enemies for the game
void CreateAliens(ScreenCoordinates scrnCoord, PlayConfigs playConfig, Enemy enemyArray[])
{
	int currentY = 2;
	int rowCount = 0;
	bool newLine = false;

	for (int i = 0; i < playConfig.enemyAmountMax; i++)
	{
		enemyArray[i].alive = true;
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
				enemyArray[i].x = startPoint + playConfig.differenceBtwn;
			}
			else
			{
				enemyArray[i].x = scrnCoord.ScreenStartX;
			}

		}

		enemyArray[i].y = currentY;
		newLine = false;
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
void BulletWork(Bullet& bullet, Bullet& alienBullet, Cover playerCovers[], COORD origin, ScreenCoordinates scrnCoord, PlayConfigs& playConfig, GameStats& gameStats, Player& player, Enemy enemyArray[])
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
		else if (bullet.y >= scrnCoord.screenSizeY - 1)
		{
			bullet.alive = false;

			hit = true;
		}

		if (bullet.goesUp)		//If it's player's bullet
		{
			if (alienBullet.x == bullet.x && alienBullet.y == bullet.y && alienBullet.alive)
			{
				hit = true;
				gotoxy(bullet.x, bullet.y);
				BulletWithBulletColl(bullet, alienBullet);
			}
			else if (alienBullet.x == bullet.x && alienBullet.y + 1 == bullet.y && alienBullet.alive)
			{
				hit = true;
				gotoxy(bullet.x, bullet.y - 1);
				BulletWithBulletColl(bullet, alienBullet);

			}
			else if (alienBullet.x == bullet.x && alienBullet.y - 1 == bullet.y && alienBullet.alive)
			{
				gotoxy(bullet.x, bullet.y + 1);
				hit = true;
				BulletWithBulletColl(bullet, alienBullet);

			}
			else
			{
				for (int i = 0; i <= playConfig.enemyAmountMax; i++)	//Check if bullet has hit an alien
				{
					if (i == playConfig.enemyAmountMax)
					{
						if (enemyArray[i].x == bullet.x && enemyArray[i].y == bullet.y + 1 && enemyArray[i].alive)
						{
							enemyArray[i].movingRight = true;
							hit = true;
							HitAlien(bullet, playConfig, gameStats, scrnCoord, enemyArray[i]);
							if (i != playConfig.enemyAmountMax)
							{
								gameStats.aliensAlive--;
							}
							break;
						}
						else if (enemyArray[i].x == bullet.x - 1 && enemyArray[i].y == bullet.y && enemyArray[i].alive)
						{
							enemyArray[i].movingRight = true;
							hit = true;
							HitAlien(bullet, playConfig, gameStats, scrnCoord, enemyArray[i]);
							if (i != playConfig.enemyAmountMax)
							{
								gameStats.aliensAlive--;
							}
							break;
						}
						else if (enemyArray[i].x == bullet.x + 1 && enemyArray[i].y == bullet.y && enemyArray[i].alive)
						{
							enemyArray[i].movingRight = true;
							hit = true;
							HitAlien(bullet, playConfig, gameStats, scrnCoord, enemyArray[i]);
							if (i != playConfig.enemyAmountMax)
							{
								gameStats.aliensAlive--;
							}
							break;
						}
						else if (enemyArray[i].x == bullet.x && enemyArray[i].y == bullet.y && enemyArray[i].alive)
						{
							enemyArray[i].movingRight = true;
							hit = true;
							HitAlien(bullet, playConfig, gameStats, scrnCoord, enemyArray[i]);
							if (i != playConfig.enemyAmountMax)
							{
								gameStats.aliensAlive--;
							}
							break;
						}
					}
					else
					{
						if (enemyArray[i].x == bullet.x && enemyArray[i].y == bullet.y && enemyArray[i].alive)
						{
							hit = true;
							HitAlien(bullet, playConfig, gameStats, scrnCoord, enemyArray[i]);
							if (i != playConfig.enemyAmountMax)
							{
								gameStats.aliensAlive--;
							}
							break;
						}
						else if (enemyArray[i].x == bullet.x && enemyArray[i].y == bullet.y + 1 && enemyArray[i].alive)
						{
							hit = true;
							HitAlien(bullet, playConfig, gameStats, scrnCoord, enemyArray[i]);
							if (i != playConfig.enemyAmountMax)
							{
								gameStats.aliensAlive--;
							}
							break;
						}
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
				gameStats.lifes--;
				hit = true;
				PlayerHitEffect(scrnCoord, playConfig, gameStats);
				StatsWriter(scrnCoord, gameStats);
			}
		}
		bool bulletCrashedWithCover = false;
		for (int i = 0; i < 4; i++)
		{
			if (bulletCrashedWithCover)
			{
				PlaySound(TEXT("invaderkilled.wav"), NULL, SND_ASYNC);
				break;
			}
			for (int j = 0; j < 4; j++)
			{					//BULLET WITH COVER
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
							SetConsoleTextAttribute(hConsole, 4);
							cout << (char)242;
							SetConsoleTextAttribute(hConsole, 7);
						}
						else
						{
							SetConsoleTextAttribute(hConsole, 4);
							cout << playerCovers[i].walls[j].halfLife;
							SetConsoleTextAttribute(hConsole, 7);
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
					gotoxy(bullet.x, bullet.y+1);
					SetConsoleTextAttribute(hConsole, 0);
					cout << ' ';
					SetConsoleTextAttribute(hConsole, 7);
				}

				if (origin.Y != bullet.y + 1 && bullet.goesUp)
				{
					gotoxy(bullet.x, bullet.y + 1);
					SetConsoleTextAttribute(hConsole, 0);
					cout << ' ';
					SetConsoleTextAttribute(hConsole, 7);
				}
				else if (origin.Y != bullet.y - 1 && !bullet.goesUp)
				{
					gotoxy(bullet.x, bullet.y - 1);
					SetConsoleTextAttribute(hConsole, 0);
					cout << ' ';
					SetConsoleTextAttribute(hConsole, 7);
				}
			}
			else
			{
				if (origin.Y != bullet.y + 1 && bullet.goesUp)
				{
					gotoxy(bullet.x, bullet.y + 1);
					SetConsoleTextAttribute(hConsole, 0);
					cout << ' ';
					SetConsoleTextAttribute(hConsole, 7);
				}
				else if (origin.Y != bullet.y - 1 && !bullet.goesUp)
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

void CheckParticles(PlayConfigs playConfig, Enemy enemyArray[])
{
	for (int i = 0; i <= playConfig.enemyAmountMax; i++)
	{
		if (enemyArray[i].exploded)
		{
			gotoxy(enemyArray[i].x, enemyArray[i].y);
			SetConsoleTextAttribute(hConsole, 0);
			cout << ' ';
			SetConsoleTextAttribute(hConsole, 7);
			enemyArray[i].exploded = false;
		}
	}
}

void BulletWithBulletColl(Bullet& bullet, Bullet& alienBullet)
{
	PlaySound(TEXT("invaderkilled.wav"), NULL, SND_ASYNC);
	SetConsoleTextAttribute(hConsole, 0);
	cout << ' ';
	SetConsoleTextAttribute(hConsole, 7);
	bullet.alive = false;
	alienBullet.alive = false;
}

//Player's bullet collided with an alien
void HitAlien(Bullet& bullet, PlayConfigs& playConfig, GameStats& gameStats, ScreenCoordinates scrnCoord, Enemy& enemyHit)
{
	bullet.alive = false;
	enemyHit.alive = false;

	gameStats.currIntervalChangeInMov++;

	if (gameStats.currIntervalChangeInMov >= playConfig.intervalChangeInMov)
	{
		playConfig.intervalMovement -= 4;
		gameStats.currIntervalChangeInMov = 0;
	}
	PlaySound(TEXT("invaderkilled.wav"), NULL, SND_ASYNC);
	gotoxy(enemyHit.x, enemyHit.y);
	SetConsoleTextAttribute(hConsole, 7);
	cout << (char)158;
	enemyHit.exploded = true;
	gameStats.playerScore += (int)enemyHit.alienType;
	StatsWriter(scrnCoord, gameStats);
}

//Move every alien alive in the screen
void MoveAliens(PlayConfigs playConfig, GameStats& gameStats, ScreenCoordinates scrnCoord, Enemy enemyArray[])
{
	int currentY;
	bool moveY = false;

	for (int i = 0; i < playConfig.enemyAmountMax; i++)
	{
		if (enemyArray[i].x >= scrnCoord.alienLimitR && enemyArray[i].alive)
		{
			moveY = true;
			gameStats.movingRight = false;
		}
		else if (enemyArray[i].x <= scrnCoord.alienLimitL && enemyArray[i].alive)
		{
			moveY = true;
			gameStats.movingRight = true;
		}
	}


	for (int i = 0; i < playConfig.enemyAmountMax; i++)
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


			if (gameStats.movingRight)
			{
				enemyArray[i].x++;
			}
			else
			{
				enemyArray[i].x--;
			}

		}

	}

	PrintAliens(playConfig, enemyArray);

}

bool trySpawnSpaceShip(GameStats& gameStats, PlayConfigs playConfig)
{
	if (gameStats.aliensAlive <= playConfig.enemyAmountMax / 2)
	{
		int randResult = rand() % 10;

		if (randResult == 5)
		{
			return true;
		}
	}
	return false;
}

void SpawnSpaceShip(PlayConfigs playConfig, GameStats& gameStats, Enemy enemyArray[])
{

	ScreenCoordinates scrnCoord;

	enemyArray[playConfig.enemyAmountMax].alive = true;
	enemyArray[playConfig.enemyAmountMax].y = 1;
	if (enemyArray[playConfig.enemyAmountMax].movingRight)
	{
		enemyArray[playConfig.enemyAmountMax].x = scrnCoord.alienLimitL;
	}
	else
	{
		enemyArray[playConfig.enemyAmountMax].x = scrnCoord.alienLimitR;
	}

	gotoxy(enemyArray[playConfig.enemyAmountMax].x, enemyArray[playConfig.enemyAmountMax].y);
	cout << enemyArray[playConfig.enemyAmountMax].icon;
}

void MoveSpaceShip(Enemy& spaceShip, ScreenCoordinates scrnCoord)
{
	if (spaceShip.alive)
	{
		if (spaceShip.movingRight)
		{
			gotoxy(spaceShip.x, spaceShip.y);
			SetConsoleTextAttribute(hConsole, 0);
			cout << ' ';
			SetConsoleTextAttribute(hConsole, 7);
			if (spaceShip.x == scrnCoord.alienLimitR)
			{
				spaceShip.movingRight = false;
				spaceShip.alive = false;
			}
			else
			{
				spaceShip.x++;
				cout << spaceShip.icon;
			}
		}
		else
		{
			gotoxy(spaceShip.x, spaceShip.y);
			SetConsoleTextAttribute(hConsole, 0);
			cout << ' ';
			SetConsoleTextAttribute(hConsole, 7);

			if (spaceShip.x == scrnCoord.alienLimitL)
			{
				spaceShip.movingRight = true;
				spaceShip.alive = false;
			}
			else
			{
				spaceShip.x--;
				gotoxy(spaceShip.x, spaceShip.y);
				cout << spaceShip.icon;
			}
		}
	}
}

//Make a designated alien shoot a bullet
Enemy AliensAttack(Bullet& alienBullet, PlayConfigs playConfig, Enemy enemyArray[])
{
	int aliensXpos[55];
	int emptySlot = 200;

	Enemy desiredAlien;

	for (int i = 0; i < playConfig.enemyAmountMax; i++)
	{
		aliensXpos[i] = emptySlot;
	}

	int counter = 0;

	for (int i = 0; i < playConfig.enemyAmountMax; i++)
	{
		if (enemyArray[i].alive)
		{
			bool isRepeated = false;

			for (int j = 0; j < playConfig.enemyAmountMax; j++)
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

		for (int i = 0; i < playConfig.enemyAmountMax; i++)
		{
			if (enemyArray[i].x == aliensXpos[randPos] && Ypos < enemyArray[i].y)
			{
				Ypos = enemyArray[i].y;
			}
		}

		for (int i = 0; i < playConfig.enemyAmountMax; i++)
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
bool TakeInput(Player& player, Bullet& bullet, int movementKeys[], char actionKeys[])
{

	if (_kbhit())
	{

		int ans = _getch();
		bool movementKey = false;
		bool actionKey = false;

		for (int i = 0; i < sizeof(movementKeys) / sizeof(movementKeys[0]); i++)
		{
			if (movementKeys[i] == ans)
			{
				movementKey = true;
			}
		}

		if (!movementKey)
		{
			for (int i = 0; i < sizeof(actionKeys) / sizeof(actionKeys[0]); i++)
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

				if (player.x < scrnCoord.screenSizeX - 1)
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
					PlaySound(TEXT("shoot.wav"), NULL, SND_ASYNC);
					bullet.x = player.x;
					bullet.y = player.y - 1;
					bullet.alive = true;
				}

				break;
			case ActionKeys::Quit:
			case ActionKeys::QuitAlt:
				return true;
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
	return false;
}

//Print bullet on the screen
void PrintBullet(Bullet bullet, COORD origin)
{
	gotoxy(bullet.x, bullet.y);
	if (!bullet.goesUp)
	{
		SetConsoleTextAttribute(hConsole, 6);
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 7);
	}
	cout << bullet.icon;
	SetConsoleTextAttribute(hConsole, 7);
	if (origin.Y - 1 != bullet.y + 1 && bullet.goesUp)
	{
		gotoxy(bullet.x, bullet.y + 1);
		SetConsoleTextAttribute(hConsole, 0);
		cout << ' ';
		SetConsoleTextAttribute(hConsole, 7);
	}
	else if ((origin.Y != bullet.y - 1 && !bullet.goesUp))
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
void PrintAliens(PlayConfigs playConfig, Enemy enemyArray[])
{
	for (int i = 0; i < playConfig.enemyAmountMax; i++)
	{
		if (enemyArray[i].alive)
		{
			switch (enemyArray[i].alienType)
			{
			case EnemyTypes::UpperAlien:
				SetConsoleTextAttribute(hConsole, 10);
				break;
			case EnemyTypes::MiddleAlien:
				SetConsoleTextAttribute(hConsole, 11);
				break;
			case EnemyTypes::LowerAlien:
				SetConsoleTextAttribute(hConsole, 13);
				break;
			default:
				break;
			}
			gotoxy(enemyArray[i].x, enemyArray[i].y);
			cout << enemyArray[i].icon;
		}
	}
	SetConsoleTextAttribute(hConsole, 7);
}

void PrintCovers(Cover playerCovers[], ScreenCoordinates scrnCoord)
{
	int currentX = scrnCoord.CoverStartX;
	SetConsoleTextAttribute(hConsole, 4);
	for (int j = 0; j < 4; j++)
	{
		int currentWall = 0;
		for (int i = currentX - 1; i <= currentX + 1; i++)
		{
			
			gotoxy(i, scrnCoord.CoverY);
			if (playerCovers[j].walls[currentWall].life == 2)
			{
				cout << (char)219;
			}
			else if (playerCovers[j].walls[i].life == 1)
			{
				cout << (char)177;
			}

			playerCovers[j].walls[currentWall].x = i;
			playerCovers[j].walls[currentWall].y = scrnCoord.CoverY;
			currentWall++;
		}
		gotoxy(currentX, scrnCoord.CoverY - 1);
		if (playerCovers[j].walls[currentWall].life == 2)
		{
			cout << (char)220;
		}
		else if (playerCovers[j].walls[currentWall].life == 1)
		{
			cout << (char)242;
		}
		playerCovers[j].walls[currentWall].x = currentX;
		playerCovers[j].walls[currentWall].y = scrnCoord.CoverY - 1;
		currentX += scrnCoord.DistanceBtwnCovers;
	}
	SetConsoleTextAttribute(hConsole, 7);
}

void PlayerHitEffect(ScreenCoordinates scrnCoord, PlayConfigs playConfig, GameStats gameStats)
{
	PlaySound(TEXT("explosion.wav"), NULL, SND_ASYNC);
	SetConsoleTextAttribute(hConsole, 12);
	HeadsUpDisplay(scrnCoord, gameStats);	//Print HUD
	Sleep(80);
	SetConsoleTextAttribute(hConsole, 7);
	HeadsUpDisplay(scrnCoord, gameStats);	//Print HUD

}

//Make console cursor go to designated x and y coordinates
void gotoxy(int x, int y)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { (short)x,(short)y });
}