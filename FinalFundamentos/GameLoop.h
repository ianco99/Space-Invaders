#pragma once
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <chrono>
#include <time.h>
#pragma comment (lib, "winmm")

using namespace::std;


#pragma region Enums & Structs

enum class MovementKeys { Right = 77, Left = 75};
enum class ActionKeys { Shoot = ' ', Quit = 'q', QuitAlt = 'Q' };
enum class GameStates { StillPlaying, Lost, Won, Quit};
enum class EnemyTypes { SpaceShip = 203, UpperAlien = 30, MiddleAlien = 20, LowerAlien = 10 };
enum class TypeOfCollision{NoCollision,CollisionPlayer, CollisionBullet, CollisionEnemy, CollisionCover};

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
	bool exploded{ false };
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
	bool lastHit{ false };
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

	int alienLimitL{ 2 };	//Alien's limits in the screen on the left side
	int alienLimitR{ 34 };	//Alien's limits in the screen on the right side

	int lifesX{ 55 };
	int lifesY{ 8 };

	int ScoreX{ 55 };	//75
	int ScoreY{ 10 };

	int CoverY{ 23 };
	int CoverStartX{ 7 };
	int DistanceBtwnCovers{ 7 };

	int ControlsX{ 55 };
	int ControlsY{ 15 };
};

struct PlayConfigs
{
	const int enemyAmountMax = 55;	//Amount of starting enemies
	const int differenceBtwn = 2;	//Space between each enemy

	int intervalChangeInMov{ 8 };	//How often does the "intervalMovement" change
	int IntervalPlayerBulletSpeed = 2;

	int gameSpeed{ 60 };	//How fast does the game run (in milliseconds) 1/gameSpeed
	int intervalMovement{ 60 };	//How often do the aliens move
	int intervalBulletSpeed{ 5 };	//How often does the bullets move
};

struct GameStats
{
	string playerName{"IAN"};

	int currIntervalMovement = 0;	//Current count used in movement's interval
	int currIntervalBulletSpeed = 0;	//Current count used in bulletSpeed's interval
	int currIntervalPlayerBulletSpeed = 0;	//
	int currIntervalChangeInMov = 0;	//Current progress in the interval

	int playerScore{ 0 };
	int lastHighScore{ 1000 };
	int timesPlayed{ 0 };
	int lifes{ 3 };
	int aliensAlive;	//How many aliens are alive

	bool movingRight = true;	//Are enemies moving to the right
};
#pragma endregion


#pragma region Functions
void StartPrint(Cover playerCovers[], ScreenCoordinates scrnCoord, PlayConfigs playConfig, Player player, Enemy enemyArray[]);
void PrintControls(ScreenCoordinates scrnCoord);
void GameLoop(PlayConfigs& playConfig, ScreenCoordinates scrnCoord, GameStats& gameStats, Enemy enemyArray[]);
bool willQuit();
void Startup(GameStats& gameStats);
GameStates WinConditions(PlayConfigs& playConfig, GameStats& gameStats, Player player, Enemy enemyArray[]);
void WinScreen();
void LoseScreen();
void HeadsUpDisplay(ScreenCoordinates scrnCoord, GameStats gameStats);
void StatsWriter(ScreenCoordinates scrnCoord, GameStats gameStats);
void ScreenBorder(ScreenCoordinates scrnCoord);
void CreateAliens(ScreenCoordinates scrnCoord, PlayConfigs playConfig, Enemy enemyArray[]);
void CheckScore(GameStats& gameStats);

void CheckParticles(PlayConfigs playConfig, Enemy enemyArray[]);
void BulletWork(Bullet& bullet, Bullet& alienBullet, Cover playerCovers[], COORD origin, ScreenCoordinates scrnCoord, PlayConfigs& playConfig, GameStats& gameStats, Player& player, Enemy enemyArray[]);
void BulletWithBulletColl(Bullet& bullet, Bullet& alienBullet);
void HitAlien(Bullet& bullet, PlayConfigs& playConfig, GameStats& gameStats, ScreenCoordinates scrnCoord, Enemy& enemyHit);

void MoveAliens(PlayConfigs playConfig, GameStats& gameStats, ScreenCoordinates scrnCoord, Enemy enemyArray[]);
bool trySpawnSpaceShip(GameStats& gameStats, PlayConfigs playConfig);
void SpawnSpaceShip(PlayConfigs playConfig, GameStats& gameStats, Enemy enemyArray[]);
void MoveSpaceShip(Enemy& spaceShip, ScreenCoordinates scrnCoord);
Enemy AliensAttack(Bullet& alienBullet, PlayConfigs playConfig, Enemy enemyArray[]);
bool TakeInput(Player& player, Bullet& bullet, int movementKeys[], char actionKeys[]);

void PrintBullet(Bullet bullet, COORD origin);
void PrintPlayer(Player player);
void PrintAliens(PlayConfigs playConfig, Enemy enemyArray[]);
void PrintCovers(Cover playerCovers[], ScreenCoordinates scrnCoord);
void PlayerHitEffect(ScreenCoordinates scrnCoord, PlayConfigs playConfig, GameStats gameStats);
void gotoxy(int x, int y);
#pragma endregion


