//John Conway's Game of Life by Marcelo Aragão https://github.com/Marcelofilho07
#define OLC_PGE_APPLICATION
#include <math.h>
#include "olcPixelGameEngine.h"
#include <vector>


class GameOfLife : public olc::PixelGameEngine
{
public:
	GameOfLife()
	{
		sAppName = "GameOfLife";
	}

private:
	bool** cells;
	bool** newCells;
	olc::vi2d mousePos;
	bool pause, nextGen;

public:

	//Check how many neighborcells are alive. Might have a better implemantion, but for now it's ok!
	int NeighborCells(bool** cells, int i, int j)
	{
		int nCells = 0;
		if (!((i == 0 && j == 0) || (j == 0) || (i == 0)))
			if (cells[i - 1][j - 1] == true)
				nCells++;

		if (!((i == 0 && j == 0) || (j == 0) || (i == ScreenWidth() - 1 && j == 0)))
			if (cells[i][j - 1] == true)
				nCells++;

		if (!((i == ScreenWidth() - 1) || (j == 0) || (i == ScreenWidth() - 1 && j == 0)))
			if (cells[i + 1][j - 1] == true)
				nCells++;

		if (!((i == 0 && j == 0) || (i == 0) || (i == 0 && j == ScreenHeight() - 1)))
			if (cells[i - 1][j] == true)
				nCells++;

		if (!((i == ScreenWidth() - 1 && j == 0) || (i == ScreenWidth() - 1) || (i == ScreenWidth() - 1 && j == ScreenHeight() - 1)))
			if (cells[i + 1][j] == true)
				nCells++;

		if (!((i == 0) || (i == 0 && j == ScreenHeight() - 1) || (j == ScreenHeight() - 1)))
			if (cells[i - 1][j + 1] == true)
				nCells++;

		if (!((i == 0 && j == ScreenHeight() - 1) || (j == ScreenHeight() - 1) || (i == ScreenWidth() - 1 && j == ScreenHeight() - 1)))
			if (cells[i][j + 1] == true)
				nCells++;

		if (!((j == ScreenHeight() - 1) || (i == ScreenWidth() - 1 && j == ScreenHeight() - 1) || (i == ScreenWidth() - 1)))
			if (cells[i + 1][j + 1] == true)
				nCells++;

		return nCells;
	}

	bool OnUserCreate() override
	{
		cells = new bool* [ScreenWidth()];
		newCells = new bool* [ScreenWidth()];
		for (int i = 0; i < ScreenWidth(); i++)
		{
			cells[i] = new bool[ScreenHeight()];
			newCells[i] = new bool[ScreenHeight()];
		}

		for (int i = 0; i < ScreenWidth(); i++)
			for (int j = 0; j < ScreenHeight(); j++)
			{
				newCells[i][j] = false;
				cells[i][j] = false;
			}
		pause = true;
		nextGen = false;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLACK);

		//Right click turns cell alive 
		if (GetMouse(0).bHeld)
		{
			int mouseX = GetMouseX();
			int mouseY = GetMouseY();
			cells[mouseX][mouseY] = true;
		}

		//Light click turns cell dead
		else if (GetMouse(1).bHeld)
		{
			int mouseX = GetMouseX();
			int mouseY = GetMouseY();
			cells[mouseX][mouseY] = false;
		}

		//P to pause/unpause
		if (GetKey(olc::Key::P).bPressed)
		{
			if (pause == false)
				pause = true;
			else
				pause = false;
		}
		//N to show next next generation
		if (GetKey(olc::Key::N).bPressed)
		{
			nextGen = true;
			pause = false;
		}

		//Game rules!
		if (!pause)
		{
			for (int i = 0; i < ScreenWidth(); i++)
				for (int j = 0; j < ScreenHeight(); j++)
				{
					int nCells;
					nCells = NeighborCells(cells, i, j);
					//checks if it lives or die
					if (cells[i][j] == true)
					{
						if (nCells == 2 || nCells == 3) //It lives if there is 2 or 3 neighbor cells
							newCells[i][j] = true;
						else 
							newCells[i][j] = false; //or dies otherwise...
					}
					//checks if a new one is born
					else
					{
						if (nCells == 3) //A new one is born if there is 3 neighbor cells
							newCells[i][j] = true;
					}
				}
		}
				

		//Draw Screen
		for(int i = 0; i < ScreenWidth(); i++)
			for (int j = 0; j < ScreenHeight(); j++)
			{
				if(!pause)
					cells[i][j] = newCells[i][j];
				if (cells[i][j] == true)
					FillRect(i, j, 1, 1, olc::RED);
			}

		if (nextGen)
		{
			nextGen = false;
			pause = true;
		}

		return true;
	}

};

int main()
{
	GameOfLife demo;
	if (demo.Construct(264,	200, 2, 2))
		demo.Start();

	return 0;
}

