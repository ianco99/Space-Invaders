#pragma once
#include <iostream>
#include <conio.h>
#include "GameLoop.h"
#include <Windows.h>
#include <time.h>

using namespace::std;

enum class MenuControls{LeftArrow = 77, RightArrow = 75, Spacebar = ' ' };

void Menu();
void hidecursor();
void PointsExplanations(ScreenCoordinates scrnCoord);
void SetFontSize();
void PrintPlay(int currentOption);
void PrintStats(int currentOption);
void PrintQuit(int currentOption);
void StatsMenu(GameStats gameStats);
bool MenuInput(int& currentOption);