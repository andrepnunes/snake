#include "snakeAPI.h"
#include "clientAPI.h"
#include "ret_type.h"
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char const *argv[])
{
	char* gameName;
	int gameON = 1;
	int sizeX;
	int sizeY;
	t_return_code ret;
	int nbWalls;
	int myTurn;				// = 1 si c'est mon tour, = 0 sinon
	t_move myMove;
	int move;
	t_move hisMove;

	connectToServer("pc4131.polytech.upmc.fr", 8000, "ovni");

	waitForSnakeGame("RANDOM_PLAYER difficulty=0 timeout=100 seed=123 start=0", gameName, &sizeX, &sizeY, &nbWalls);

	int* walls = (int*)(malloc( nbWalls*sizeof(int)));
	
	myTurn = !getSnakeArena(walls); // = 1 si c'est mon tour, = 0 sinon


	while(gameON){
		printArena();
		if (myTurn)
		{
			scanf("%d", &move);
			switch(move){
				case 2: myMove = SOUTH; break;	
				case 4:	myMove = WEST; break;
				case 6:	myMove = EAST; break;
				case 8:	myMove = NORTH; break;
			}
			gameON = !(ret == sendMove(myMove));
		}else{
			gameON = !(ret == getMove(&hisMove));
		}
		myTurn = !myTurn;
	}


	closeConnection();
	free(walls);
	return 0;
}