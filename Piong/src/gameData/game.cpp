#include "game.h"
#include "sl.h"
#include <time.h>
#include <iostream>

using namespace std;

enum Screen
{
	MainMenu,
	SinglePalyer,
	MultiPlayer,
	Rules,
	Credits,
	Exit,
};

void SetScreenWidth(int screenWidth)
{
	screenWidth = 800;
}

void SetScreenHeight(int screenHeight)
{
	screenHeight = 450;
}

int GetScreenWidth(int screenWidth)
{
	return screenWidth;
}

int GetScreenHeight(int screenHeight)
{
	return screenHeight;
}

// inicializa todo
void InitAll(Ball& ball, Pad& pad1, Pad& pad2, Pad& ia, int screenWidth, int screenHeight)
{
	slWindow(screenWidth, screenHeight, "Pong", false);

	InitBall(ball);

	InitPads(pad1, GetScreenWidth(screenWidth) / 10, GetScreenHeight(screenHeight) / 2, SL_KEY_KEYPAD_5, SL_KEY_KEYPAD_2);
	InitPads(pad2, (GetScreenWidth(screenWidth) - 100), GetScreenHeight(screenHeight) / 2, SL_KEY_UP, SL_KEY_DOWN);
	InitIaPad(ia, (GetScreenWidth(screenWidth) - 100), GetScreenHeight(screenHeight) / 2);
}

void DrawFigures(Pad pad1, Pad pad2, Ball ball, int screenWidth, int screenHeight)
{
	//DrawText(TextFormat("%i", pad1.points), GetScreenWidth() / 2 - 50, 10, 20, WHITE);
	//DrawText(TextFormat("%i", pad2.points), GetScreenWidth() / 2 + 50, 10, 20, WHITE);

	slRectangleFill(GetScreenWidth(screenWidth) / 2, GetScreenHeight(screenHeight) / 2, 5, GetScreenHeight(screenHeight));

	slSetForeColor(4, 0, 4, 1);
	slRectangleFill(pad1.positionX, pad1.positionY, pad1.width, pad1.height);

	slSetForeColor(1, 0, 0, 1);
	slCircleFill(ball.positionX, ball.positionY, ball.radius, 2);

	slSetForeColor(0, 1, 1, 1);
	slRectangleFill(pad2.positionX, pad2.positionY, pad2.width, pad2.height);
}

// dibuja el men�
void DrawMenu(Screen& screen, int counter, int screenWidth, int screenHeight)
{
	/*DrawText("Pong", GetScreenWidth() / 2 - 100, GetScreenHeight() - 400, 100, WHITE);
	DrawText("Use arrow keys to navigate the menu", GetScreenWidth() - 600, GetScreenHeight() / 2 - 50, 20, WHITE);
	DrawText("SinglePalyer", GetScreenWidth() / 2 - 40, GetScreenHeight() / 2, 30, WHITE);
	DrawText("MultiPlayer", GetScreenWidth() / 2 - 40, GetScreenHeight() - 190, 30, WHITE);
	DrawText("Rules", GetScreenWidth() / 2 - 40, GetScreenHeight() - 160, 30, WHITE);
	DrawText("Credits", GetScreenWidth() / 2 - 40, GetScreenHeight() - 130, 30, WHITE);
	DrawText("Exit", GetScreenWidth() / 2 - 40, GetScreenHeight() - 100, 30, WHITE);*/

	switch (counter)
	{
	case SinglePalyer:
		//DrawText("SinglePalyer", GetScreenWidth() / 2 - 40, GetScreenHeight() / 2, 30, RED);
		if (slGetKey(SL_KEY_ENTER))
		{
			screen = SinglePalyer;
		}
		break;

	case MultiPlayer:
		//DrawText("MultiPlayer", GetScreenWidth() / 2 - 40, GetScreenHeight() - 190, 30, RED);
		if (slGetKey(SL_KEY_ENTER))
		{
			screen = MultiPlayer;
		}
		break;

	case Rules:
		//DrawText("Rules", GetScreenWidth() / 2 - 40, GetScreenHeight() - 160, 30, RED);
		if (slGetKey(SL_KEY_ENTER))
		{
			screen = Rules;
		}
		break;

	case Credits:
		//DrawText("Credits", GetScreenWidth() / 2 - 40, GetScreenHeight() - 130, 30, RED);
		if (slGetKey(SL_KEY_ENTER))
		{
			screen = Credits;
		}
		break;

	case Exit:
		//DrawText("Exit", GetScreenWidth() / 2 - 40, GetScreenHeight() - 100, 30, RED);
		if (slGetKey(SL_KEY_ENTER))
		{
			screen = Exit;
		}
		break;
	}
}

void PointsReset(Pad& pad)
{
	pad.points = pad.minPoints;
}

// colision de las palteas con los bordes
void PadColision(Pad& pad)
{
	int maxScreenHeight = GetScreenHeight();
	int minScreenHeight = (GetScreenHeight() - GetScreenHeight());

	if (pad.positionY <= minScreenHeight)
	{
		//std::cout << "Colison Up" << std::endl;		
		pad.positionY = pad.minPosition;
	}

	else if ((pad.positionY + pad.height) >= maxScreenHeight)
	{
		//std::cout << "Colison Down" << std::endl;		
		pad.positionY = pad.maxPosition;
	}
}

void PadMovement(Pad& pad)
{
	if (IsKeyDown(pad.keyUp))
	{
		pad.positionY -= pad.speed * GetFrameTime();
	}

	if (IsKeyDown(pad.keyDown))
	{
		pad.positionY += pad.speed * GetFrameTime();
	}

	PadColision(pad);
}

void IaPadMovement(Pad& pad, Ball ball)
{
	if (pad.positionY + pad.height / 2 > ball.position.y)
	{
		pad.positionY -= 180.0f * GetFrameTime();
	}
	else if (pad.positionY + pad.width / 2 <= ball.position.y)
	{
		pad.positionY += 180.0f * GetFrameTime();
	}
}

// chequea si la pelota colisiona con las paredes
void BallCollision(Ball& ball, Pad& pad1, Pad& pad2, bool& ballCollidingWithWalls, int& ballInicialSpeedX, int& ballInicialSpeedY)
{
	int minScreenPosX = 0;

	if (ball.position.x >= (GetScreenWidth() - ball.radius))
	{
		//std::cout << "Colison Right" << std::endl;
		pad1.points++;
		ball.speed.x = ball.normalSpeed.x;
		ball.speed.y = ball.normalSpeed.y;

		ballCollidingWithWalls = true;
	}

	else if (ball.position.x <= (minScreenPosX + ball.radius))
	{
		//std::cout << "Colison Left" << std::endl;
		pad2.points++;
		ball.speed.x = ball.normalSpeed.x;
		ball.speed.y = ball.normalSpeed.y;
		ballCollidingWithWalls = true;
	}
}

void PadBallCollision(Ball& ball, Pad pad)
{
	float ballWith = ball.radius * 2;
	float ballHeight = ball.radius * 2;

	if ((pad.positionX + pad.width >= ball.position.x) &&
		(pad.positionX <= (ball.position.x - ball.radius) + ballWith) &&
		(pad.positionY + pad.height >= ball.position.y) &&
		(pad.positionY <= (ball.position.y - ball.radius) + ballHeight))
	{

		if (ball.position.x > pad.positionX)
		{
			ball.position.x = (pad.positionX + pad.width) + ball.radius;
		}

		else
		{
			ball.position.x = pad.positionX - ball.radius;
		}

		ball.speed.x *= -1.1f;
	}
}

void BallMovement(Ball& ball, Pad& pad1, Pad& pad2, int& ballInicialSpeedX, int& ballInicialSpeedY, bool ballCollidingWithWalls, bool& isBallInicialSpeedRandomized)
{
	if (isBallInicialSpeedRandomized == false)
	{
		ballInicialSpeedX = rand() % 2;
		ballInicialSpeedY = rand() % 2;

		if (ballInicialSpeedX == 0)
		{
			ballInicialSpeedX = -1;
		}

		if (ballInicialSpeedY == 0)
		{
			ballInicialSpeedY = -1;
		}

		isBallInicialSpeedRandomized = true;
	}

	ball.position.x += ball.speed.x * ballInicialSpeedX * GetFrameTime();
	ball.position.y += ball.speed.y * ballInicialSpeedY * GetFrameTime();

	if ((ball.position.x >= (GetScreenWidth() - ball.radius)) || (ball.position.x <= ball.radius))
		ball.speed.x *= -1.0f;

	if ((ball.position.y >= (GetScreenHeight() - ball.radius)) || (ball.position.y <= ball.radius))
		ball.speed.y *= -1.0f;

	BallCollision(ball, pad1, pad2, ballCollidingWithWalls, ballInicialSpeedX, ballInicialSpeedY);

	if (ballCollidingWithWalls)
	{
		ball.position.x = ball.inicialPosX;
		ball.position.y = ball.inicialPosY;
		isBallInicialSpeedRandomized = false;
	}
}

//updatea el modo un jugador
void UpdateSinglePlayer(Ball& ball, Pad& pad1, Pad& ia, Screen& screen, int& ballInicialSpeedX, int& ballInicialSpeedY, bool ballCollidingWithWalls, bool& hasWon, bool& isBallInicialSpeedRandomized)
{
	if (!hasWon)
	{
		// mov. primer rectangulo restringido a la pantalla
		PadMovement(pad1);

		// mov. segundo rectangulo restringido a la pantalla
		IaPadMovement(ia, ball);

		// mov. pelota
		BallMovement(ball, pad1, ia, ballInicialSpeedX, ballInicialSpeedY, ballCollidingWithWalls, isBallInicialSpeedRandomized);

		PadBallCollision(ball, pad1);
		PadBallCollision(ball, ia);

		if (pad1.points >= pad1.maxPoints
			|| ia.points >= ia.maxPoints)
		{
			hasWon = true;
		}
	}

	if (pad1.points >= pad1.maxPoints)
	{
		DrawText("Player 1 wins!", 300, GetScreenHeight() - 300, 30, RED);
		if (IsKeyPressed(KEY_ENTER))
		{
			PointsReset(pad1);
			PointsReset(ia);
			hasWon = false;
			screen = Screen::MainMenu;
		}
	}

	else if (ia.points >= ia.maxPoints)
	{
		DrawText("The computer wins!", 300, GetScreenHeight() - 300, 30, RED);
		if (IsKeyPressed(KEY_ENTER))
		{
			PointsReset(pad1);
			PointsReset(ia);
			hasWon = false;
			screen = Screen::MainMenu;
		}
	}
}

//updatea el modo dos jugadores
void UpdateMultiplayer(Ball& ball, Pad& pad1, Pad& pad2, Screen& screen, int& ballInicialSpeedX, int& ballInicialSpeedY, bool ballCollidingWithWalls, bool& hasWon, bool& isBallInicialSpeedRandomized)
{
	if (!hasWon)
	{
		// mov. primer rectangulo restringido a la pantalla
		PadMovement(pad1);

		// mov. segundo rectangulo restringido a la pantalla
		PadMovement(pad2);

		// mov. pelota
		BallMovement(ball, pad1, pad2, ballInicialSpeedX, ballInicialSpeedY, ballCollidingWithWalls, isBallInicialSpeedRandomized);

		PadBallCollision(ball, pad1);
		PadBallCollision(ball, pad2);

		if (pad1.points >= pad1.maxPoints
			|| pad2.points >= pad2.maxPoints)
		{
			hasWon = true;
		}
	}

	if (pad1.points >= pad1.maxPoints)
	{
		DrawText("Player 1 wins1", 300, GetScreenHeight() - 300, 30, RED);
		if (IsKeyPressed(KEY_ENTER))
		{
			PointsReset(pad1);
			PointsReset(pad2);
			hasWon = false;
			screen = Screen::MainMenu;
		}
	}

	else if (pad2.points >= pad2.maxPoints)
	{
		DrawText("Player 2 wins!", 300, GetScreenHeight() - 300, 30, RED);
		if (IsKeyPressed(KEY_ENTER))
		{
			PointsReset(pad1);
			PointsReset(pad2);
			hasWon = false;
			screen = Screen::MainMenu;
		}
	}
}

void GameLoop()
{
	srand(time(NULL));

	Screen screen = MainMenu;

	Pad pad1;
	Pad pad2;
	Pad ia;

	Ball ball;

	int screenWidth = 0;
	int screenHeight = 0;

	int mainMenuChoice = 1;
	int minChoice = SinglePalyer;
	int maxChoice = Exit;

	int ballInicialSpeedX = 0;
	int ballInicialSpeedY = 0;

	bool isBallInicialSpeedRandomized = false;

	bool isCollidingWithScreen = false;

	bool endGame = false;

	bool hasWon = false;

	InitAll(ball, pad1, pad2, ia, screenWidth, screenHeight);

	while (!endGame && !WindowShouldClose())
	{
		SetExitKey(KEY_NULL);

		switch (screen)
		{
		case MainMenu:
			if (IsKeyPressed(KEY_DOWN))
			{
				mainMenuChoice++;
				if (mainMenuChoice >= maxChoice)
				{
					mainMenuChoice = maxChoice;
				}
			}

			else if (IsKeyPressed(KEY_UP))
			{
				mainMenuChoice--;
				if (mainMenuChoice <= minChoice)
				{
					mainMenuChoice = minChoice;
				}
			}

			DrawMenu(screen, mainMenuChoice);
			break;

		case SinglePalyer:
			UpdateSinglePlayer(ball, pad1, ia, screen, ballInicialSpeedX, ballInicialSpeedY, isCollidingWithScreen, hasWon, isBallInicialSpeedRandomized);
			break;

		case MultiPlayer:
			UpdateMultiplayer(ball, pad1, pad2, screen, ballInicialSpeedX, ballInicialSpeedY, isCollidingWithScreen, hasWon, isBallInicialSpeedRandomized);
			break;
		}

		BeginDrawing();

		switch (screen)
		{
		case MainMenu:

			break;

		case SinglePalyer:
			DrawFigures(pad1, ia, ball);
			break;

		case MultiPlayer:
			DrawFigures(pad1, pad2, ball);
			break;

		case Rules:
			DrawText("-Use \"w and s\" to move de left pad", GetScreenWidth() - 700, GetScreenHeight() / 2 - 100, 25, WHITE);
			DrawText("-And the up and down arrow keys for the right pad", GetScreenWidth() - 700, GetScreenHeight() / 2 - 50, 25, WHITE);
			DrawText("-The first one to score 7 points wins", GetScreenWidth() - 700, GetScreenHeight() / 2, 25, WHITE);
			DrawText("-Press \"backspace\" to return to the menu", GetScreenWidth() - 700, GetScreenHeight() - 180, 25, WHITE);
			if (IsKeyPressed(KEY_BACKSPACE))
			{
				screen = Screen::MainMenu;
			}
			break;

		case Credits:
			DrawText("Game made by Joaquin Herrero Lendner", GetScreenWidth() / 2 - 200, GetScreenHeight() / 2, 25, WHITE);
			if (IsKeyPressed(KEY_BACKSPACE))
			{
				screen = Screen::MainMenu;
			}
			break;

		case Exit:
			DrawText("Do yo want to close the game?", GetScreenWidth() - 700, GetScreenHeight() / 2, 25, WHITE);
			DrawText("Press \"esc\" to close the game or \"backspace\" to return to the menu", GetScreenWidth() - 700, GetScreenHeight() - 200, 15, WHITE);
			if (IsKeyPressed(KEY_ESCAPE))
			{
				endGame = true;
			}
			else if (IsKeyPressed(KEY_BACKSPACE))
			{
				screen = Screen::MainMenu;
			}

			break;
		}

		ClearBackground(BLACK);

		EndDrawing();
	}

	CloseWindow();
}