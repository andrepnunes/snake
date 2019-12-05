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
	t_return_code ret = NORMAL_MOVE;
	int nbWalls;
	int myTurn;				// = 1 si c'est mon tour, = 0 sinon
	t_move myMove;
	int move;
	t_move hisMove;
	int* walls = (int*) (malloc(sizeof(int)));
	char* messageFinal = "VICTOIRE";

	connectToServer("polydev.cia-polytech-sorbonne.fr", 8080, "ovni");


	waitForSnakeGame("RANDOM_PLAYER difficulty=2 timeout=1000 seed=123 start=0", gameName, &sizeX, &sizeY, &nbWalls);
	

	printf("%d\n", nbWalls);
	walls = (int*) (realloc(walls, nbWalls * sizeof(int)));

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
			gameON = (ret == sendMove(myMove));
			messageFinal = "DÉFAITE";
		}else{
			// si gameON devient 0 (perdu) pendant le tour adv, je sors du while avec victoire
			gameON = (ret == getMove(&hisMove));
			messageFinal = "VICTOIRE";
		}
		myTurn = !myTurn;
	}
	printf("%s\n", messageFinal);
	closeConnection();
	free(walls);
	return 0;
}