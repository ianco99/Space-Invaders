#pragma once
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <cstdio>
#include <chrono>
#include <thread>
#include <time.h>

using namespace::std;


#pragma region Enums & Structs

enum class MovementKeys { Right = 77, Left = 75};
enum class ActionKeys { Shoot = ' ', Pause = 'p', PauseAlt = 'P' };
enum class GameStates { StillPlaying, Lost, Won };
enum class EnemyTypes { SpaceShip = 100, UpperAlien = 30, MiddleAlien = 20, LowerAlien = 10 };

struct Player
{
	int x{ 0 };
	int y{ 0 };
	char icon{ (char)254 };
};

struct Bullet
{
	int x;
	int y;
	bool goesUp{ true };
	char icon{ (char)179 };
	bool alive{ false };
};

struct Enemy
{
	int x{ 0 };
	int y{ 0 };
	char icon{ 'x' };
	int lifes{ 1 };
	bool alive{ true };
	EnemyTypes alienType{ EnemyTypes::LowerAlien };
	bool movingRight = true;

};

struct Wall
{
	int x;
	int y;
	int life{ 2 };
	char fullLife{ (char)219 };
	char halfLife{ (char)177 };
};

struct Cover
{
	Wall walls[4];
};



struct ScreenCoordinates
{
	int ScreenStartX{ 3 };	//Enemies start
	int screenSizeX{ 35 }; //70
	int screenSizeY{ 29 };

	int nameX{ 55 };
	int nameY{ 5 };

	int lifesX{ 55 };
	int lifesY{ 8 };

	int ScoreX{ 55 };	//75
	int ScoreY{ 10 };

	int CoverY{ 23 };
	int CoverStartX{ 7 };
	int DistanceBtwnCovers{ 7 };
};

struct PlayConfigs
{
	string playerName;

	const int enemyAmountMax = 55;	//Amount of starting enemies
	const int differenceBtwn = 2;	//Space between each enemy

	int alienLimitL{ 2 };	//Alien's limits in the screen on the left side
	int alienLimitR{ 34 };	//Alien's limits in the screen on the right side

	int intervalChangeInMov{ 5 };	//How often does the "intervalMovement" change

	int gameSpeed{ 20 };	//How fast does the game run (in milliseconds)
	int intervalMovement{ 60 };	//How often do the aliens move
	int intervalBulletSpeed{ 5 };	//How often does the bullets move
};

struct GameStats
{
	int currIntervalChangeInMov = 0;	//Current progress in the interval

	int spaceShipCount = 0;

	int playerScore{ 0 };
	int lifes{ 3 };
	int aliensAlive;	//How many aliens are alive

	bool movingRight = true;	//Are enemies moving to the right
};
#pragma endregion


#pragma region Functions
void StartPrint(Cover playerCovers[], ScreenCoordinates scrnCoord, PlayConfigs playConfig);
void GameLoop(PlayConfigs& playConfig, ScreenCoordinates scrnCoord, GameStats& gameStats);
void Startup(string playerName);
GameStates WinConditions(PlayConfigs& playConfig, GameStats& gameStats);
void WinScreen();
void LoseScreen();
void HeadsUpDisplay(PlayConfigs playConfig, ScreenCoordinates scrnCoord, GameStats gameStats);
void StatsWriter(PlayConfigs playConfig, ScreenCoordinates scrnCoord, GameStats gameStats);
void ScreenBorder(ScreenCoordinates scrnCoord);
void CreateAliens(ScreenCoordinates scrnCoord, PlayConfigs playConfig);
void BulletWork(Bullet& bullet, Bullet& alienBullet, Cover playerCovers[], COORD origin, ScreenCoordinates scrnCoord, PlayConfigs& playConfig, GameStats& gameStats);
void BulletWithBulletColl(Bullet& bullet, Bullet& alienBullet);
void HitAlien(Bullet& bullet, PlayConfigs& playConfig, GameStats& gameStats, ScreenCoordinates scrnCoord, int index, bool& hit);
void MoveAliens(PlayConfigs playConfig, GameStats& gameStats);
void trySpawnSpaceShip(PlayConfigs playConfig, GameStats& gameStats);
void SpawnSpaceShip(PlayConfigs playConfig, GameStats& gameStats);
Enemy AliensAttack(Bullet& alienBullet, PlayConfigs playConfig);
void TakeInput(Player& player, Bullet& bullet, int movementKeys[], char actionKeys[]);

void PrintBullet(Bullet bullet, COORD origin);
void PrintPlayer(Player player);
void PrintAliens(PlayConfigs playConfig);
void PrintCovers(Cover playerCovers[], ScreenCoordinates scrnCoord);
void PlayerHitEffect(ScreenCoordinates scrnCoord, Cover playerCovers[], PlayConfigs playConfig, GameStats gameStats);
void gotoxy(int x, int y);
#pragma endregion


