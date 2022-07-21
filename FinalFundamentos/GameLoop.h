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
	int hitbox{ 3 };
	char icon{ (char)254 };
	int lifes{ 3 };
};

struct Bullet
{
	int x;
	int y;
	bool goesUp{ true };
	char icon{ (char)179 };
	bool alive{ false };
};

struct Enemy :Player
{
	bool alive{ true };
	EnemyTypes alienType{ EnemyTypes::LowerAlien };

	Enemy()
	{
		lifes = 1;
	}
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
};
#pragma endregion


#pragma region Functions
GameStates WinConditions();
void WinScreen();
void LoseScreen();
void HeadsUpDisplay(PlayConfigs playConfig, ScreenCoordinates scrnCoord);
void StatsWriter(PlayConfigs playConfig, ScreenCoordinates scrnCoord);
void ScreenBorder(ScreenCoordinates scrnCoord);
void CreateAliens(ScreenCoordinates scrnCoord);
void BulletWork(Bullet& bullet, Bullet& alienBullet, Cover playerCovers[], Player origin, ScreenCoordinates scrnCoord, PlayConfigs playConfig);
void HitAlien(Bullet& bullet, PlayConfigs playConfig, ScreenCoordinates scrnCoord, int index, bool& hit);
void MoveAliens();
void trySpawnSpaceShip();
void SpawnSpaceShip();
Enemy AliensAttack(Bullet& alienBullet);
void TakeInput(Player& player, Bullet& bullet);
void GameLoop(string playerName);
void PrintBullet(Bullet bullet, Player origin);
void PrintPlayer(Player player);
void PrintAliens();
void PrintCovers(Cover playerCovers[], ScreenCoordinates scrnCoord);
void PlayerHitEffect(ScreenCoordinates scrnCoord, Cover playerCovers[], PlayConfigs playConfig);
void gotoxy(int x, int y);
#pragma endregion


