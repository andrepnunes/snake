#include "snakeAPI.h"
#include "clientAPI.h"
#include "ret_type.h"
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char const *argv[])
{

	//variables wait for snake game
	char* gameName;
	int sizeX;
	int sizeY;
	int nbWalls;

	// boolean : vaut 1 si c'est mon tour, vaut 0 sinon
	int myTurn;
	
	t_return_code ret = NORMAL_MOVE;
	t_move myMove;
	t_move hisMove;
	
	// tableau murs
	int* walls = (int*) (malloc(sizeof(int)));

	char* messageFinal = "VICTOIRE";

	
	//	ME
	
	//		MY COORDS
	int myPOSx;
	int myPOSy;
	int myLastPosx;
	int myLastPosy;

	//	HIM
	//		HIS COORDS
	int hisPOSx;
	int hisPOSy;
	int hisLastPosx;
	int hisLastPosy;

	int* directionsInterdites = (int*) (malloc(4 * sizeof(int)));
	directionsInterdites[0] = 0;
	directionsInterdites[1] = 0;
	directionsInterdites[2] = 0;
	directionsInterdites[3] = 0;
	
	int* myBody = (int*) (malloc(2* 200 * sizeof(int)));
	int* hisBody = (int*) (malloc(2* 200 * sizeof(int)));

	// int : ma taille
	int mySize = 1;
	int hisSize = 1;

	//int : conteur de tours
	int ctr = 0;

	// boolean : vaut 0 si le jeu doit s'arreter
	int gameON = 1;
	connectToServer("polydev.cia-polytech-sorbonne.fr", 8080, "ovni");

	waitForSnakeGame("SUPER_PLAYER difficulty=2 timeout=1000 seed=135 start=0", gameName, &sizeX, &sizeY, &nbWalls);
	int obstacles[sizeX*sizeY];
	

//	myTurn != myTurn;	//IF START = 1
	
	// init pos joueur 0
	myPOSx = 2;
	myLastPosx = 2;
	myPOSy = sizeY / 2;
	myLastPosy = sizeY / 2;
	
	myBody[0] = myPOSx;	
	myBody[1] = myPOSy;

	// init pos joueur 1
	hisPOSx = sizeX - 3;
	hisLastPosx = sizeX - 3;
	hisPOSy = sizeY / 2;
	hisLastPosy = sizeY / 2;

	hisBody[0] = hisPOSx;	
	hisBody[1] = hisPOSy;

	for (int i = 2; i < 400; i++){myBody[i] = -1; hisBody[i] = -1;}

	
	// creation tableau murs
	walls = (int*) (realloc(walls, 4 * nbWalls * sizeof(int)));

	// = 1 si c'est mon tour, = 0 sinon
	myTurn = !getSnakeArena(walls); 

	myMove = 1;
	while(gameON){
		//printArena();

		if (myTurn)
		{
			printArena(); // so faz print na minha vez
/*			
			for (int i = 0; i < 4*nbWalls; i+=4)
			{
				printf("(%d\t%d)\t-\t(%d\t%d)\n", walls[i], walls[i+1], walls[i+2], walls[i+3]);
			}
/**/
			for (int i = 0; i < 4; ++i)
			{
				directionsInterdites[i] = 0;
			}
			
			for (int i = 0; i < sizeX*sizeY; ++i)
			{
				obstacles[i]=0;
			}

			//wall awareness
			for (int i = 0; i < 4*nbWalls; i+=4){
				if (walls[i] == walls[i+2] && walls[i] ==  myPOSx) // barreira horizontal
				{
					if (myPOSy == walls[i+1]) // se eu tou colado a uma parede
					{
						if (myPOSy < walls[i + 3]) // tou em CIMA
						{
							directionsInterdites[2]=1;
						}else{
							directionsInterdites[0]=1;
						}
					}
					if (myPOSy == walls[i+3])
					{
						if (myPOSy < walls[i + 1]) // tou em CIMA
						{
							directionsInterdites[2]=1;
						}else{
							directionsInterdites[0]=1;
						}
					}
				}
				if (walls[i+1] == walls[i+3] && walls[i+1] == myPOSy) // barreira vertical
				{
					if (myPOSx == walls[i]) // se eu tou colado a uma parede
					{
						if (myPOSx < walls[i + 2]) // tou a ESQUERDA
						{
							//obstacles[myPOSx + 1 + (myPOSy)*sizeX] = 1;
							directionsInterdites[1]=1;
						}else{
							//obstacles[myPOSx - 1 + (myPOSy)*sizeX] = 1;
							directionsInterdites[3]=1;
						}
					}
					if (myPOSx == walls[i+2])
					{
						if (myPOSx < walls[i]) // tou a ESQUERDA
						{
							//obstacles[myPOSx + 1 + (myPOSy)*sizeX] = 1;
							directionsInterdites[1]=1;
						}else{
							//obstacles[myPOSx - 1 + (myPOSy)*sizeX] = 1;
							directionsInterdites[3]=1;
						}
					}
				}
			}

			// SELF-AWARENESS
			for (int i = 0; i < mySize*2; i+=2)	{
				switch((myBody[i] - myPOSx) * (myBody[i+1] == myPOSy)){
					case -1:
						directionsInterdites[3] = 1;
						break;
					case 1:
						directionsInterdites[1] = 1;
						break;
					default:
						break;
				}
				switch((myBody[i+1] - myPOSy) * (myBody[i] == myPOSx)){
					case -1:
						directionsInterdites[0] = 1;
						break;
					case 1:
						directionsInterdites[2] = 1;
						break;
					default:
						break;
				}
				obstacles[myBody[i] + sizeX * myBody[i+1]] = 1;
			}

			// ENEMY-AWARENESS
			for (int i = 0; i < hisSize*2; i+=2){
				switch((hisBody[i] - myPOSx) * (hisBody[i+1] == myPOSy)){
					case -1:
					//case -2:
						directionsInterdites[3] = 1;
						break;
					case 1:
					//case 2:
						directionsInterdites[1] = 1;
						break;
					default:
						break;
				}
				switch((hisBody[i+1] - myPOSy) * (hisBody[i] == myPOSx)){
					case -1:
					//case -2:
						directionsInterdites[0] = 1;
						break;
					case 1:
					//case 2:
						directionsInterdites[2] = 1;
						break;
					default:
						break;
				}
				switch((hisPOSx - myPOSx) * (hisPOSy == myPOSy - 1 || hisPOSy == myPOSy + 1)){
					case -2:
						directionsInterdites[3] = 1;
						break;
					case 2:
						directionsInterdites[1] = 1;
						break;
					default:
						break;
				}
				switch((hisPOSy - myPOSy) * (hisPOSx == myPOSx - 1 || hisPOSx == myPOSx + 1)){
					case -2:
						directionsInterdites[0] = 1;
						break;
					case 2:
						directionsInterdites[2] = 1;
						break;
					default:
						break;
				}

				obstacles[hisBody[i] + sizeX * hisBody[i+1]] = 1;
			}


			obstacles[myPOSx + myPOSy * sizeX] = 0;// ma tete n'est pas un obstacle

			for (int jkl = 0; jkl < 5; ++jkl)
			{
				// wall traps - 5 blocs long
				for (int i = 0; i < sizeX*sizeY; i++)
				{
					int np = 0;
					//murs repetes
					int flag1 = 1;
					int flag2 = 1;
					int flag3 = 1;
					int flag4 = 1;
				
					if (i%sizeX == myPOSx && i/sizeX == myPOSy)
						i++;										// ne pas fermer tunel cree par ma tete
				
					for (int j = 0; j < 4*nbWalls; j+=4)
					{
						//N
						if (((i%sizeX == walls[j] && i/sizeX == walls[j+1] && i%sizeX == walls[j+2] && (i/sizeX - 1) == walls[j+3]) || (i%sizeX == walls[j] && (i/sizeX - 1) == walls[j+1] && i%sizeX == walls[j+2] && i/sizeX == walls[j+3]) || obstacles[i - sizeX] || i/sizeX == 0) && flag1)
						{
							np++;

							flag1 = 0;
						}
						//E
						if (((i%sizeX == walls[j] && i/sizeX == walls[j+1] && (i%sizeX + 1) == walls[j+2] && i/sizeX == walls[j+3]) || ((i%sizeX + 1) == walls[j] && i/sizeX == walls[j+1] && i%sizeX == walls[j+2] && i/sizeX == walls[j+3])  || obstacles[i + 1] || i%sizeX == sizeX-1) && flag2)
						{
							np++;

							flag2 = 0;
						}
						//S
						if (((i%sizeX == walls[j] && i/sizeX == walls[j+1] && i%sizeX == walls[j+2] && (i/sizeX + 1) == walls[j+3]) || (i%sizeX == walls[j] && (i/sizeX + 1) == walls[j+1] && i%sizeX == walls[j+2] && i/sizeX == walls[j+3]) || obstacles[i + sizeX] || i/sizeX == sizeY-1) && flag3)
						{
							np++;

							flag3 = 0;
						}
						//W
						if (((i%sizeX == walls[j] && i/sizeX == walls[j+1] && (i%sizeX - 1) == walls[j+2] && i/sizeX == walls[j+3]) || ((i%sizeX - 1) == walls[j] && i/sizeX == walls[j+1] && i%sizeX == walls[j+2] && i/sizeX == walls[j+3])  || obstacles[i - 1] || i%sizeX == 0) && flag4)
						{
							np++;

							flag4 = 0;
						}
					}
					if (np == 3)
					{
						obstacles[i] = 1;
					}
				}
			}

			obstacles[myPOSx + myPOSy * sizeX] = 0;// ma tete n'est pas un obstacle

/*
			if (!(ctr%20) || 1)
			{
				obstacles[myBody[2*(mySize - 1)] + myBody[2*mySize - 1] * sizeX] = 0;
				obstacles[hisBody[2*(hisSize - 1)] + hisBody[2*hisSize - 1] * sizeX] = 0;
			}
*/
			// DODGE WALLS
			if (myPOSy == 0        ){directionsInterdites[0] = 1;}
			if (myPOSx == sizeX - 1){directionsInterdites[1] = 1;}
			if (myPOSy == sizeY - 1){directionsInterdites[2] = 1;}
			if (myPOSx == 0        ){directionsInterdites[3] = 1;}

			// DODGE obstacles

			if (obstacles[myPOSx     + (sizeX * (myPOSy - 1))]){directionsInterdites[0] = 1;}
			if (obstacles[myPOSx + 1 + (sizeX *  myPOSy     )]){directionsInterdites[1] = 1;}
			if (obstacles[myPOSx     + (sizeX * (myPOSy + 1))]){directionsInterdites[2] = 1;}
			if (obstacles[myPOSx - 1 + (sizeX *  myPOSy     )]){directionsInterdites[3] = 1;}

/*
			// DETECT 1 SQUARE TRAPS nearby
			if ((obstacles[myPOSx - 1 + sizeX*(myPOSy - 1)] || myPOSx == 0) && (obstacles[myPOSx + sizeX*(myPOSy - 2)] || myPOSy == 1) && (obstacles[myPOSx + 1 + sizeX*(myPOSy - 1)] || myPOSx == sizeX - 1))
			{
				directionsInterdites[0] = 1;	//NORTH
			}
			if ((obstacles[myPOSx - 1 + sizeX*(myPOSy + 1)] || myPOSx == 0) && (obstacles[myPOSx + sizeX*(myPOSy + 2)] || myPOSy == sizeY - 2) && (obstacles[myPOSx + 1 + sizeX*(myPOSy + 1)] || myPOSx == sizeX - 1))
			{
				directionsInterdites[2] = 1;	//SOUTH
			}
			if ((obstacles[myPOSx - 1 + sizeX*(myPOSy + 1)] || myPOSy == sizeY - 1) && (obstacles[myPOSx - 2 + sizeX*myPOSy] || myPOSx == 1) && (obstacles[myPOSx - 1 + sizeX*(myPOSy - 1)] || myPOSy == 0))
			{
				directionsInterdites[3] = 1;	//EAST
			}
			if ((obstacles[myPOSx + 1 + sizeX*(myPOSy + 1)] || myPOSy == sizeY - 1) && (obstacles[myPOSx + 2 + sizeX*myPOSy] || myPOSx == sizeX - 2) && (obstacles[myPOSx + 1 + sizeX*(myPOSy - 1)] || myPOSy == 0))
			{
				directionsInterdites[2] = 1;	//WEST
			}
/**/


/*
			//affiche obstacles
			for (int i = 0; i < sizeY*sizeX; ++i)
			{
				printf("%d ", obstacles[i]);
				if (i%sizeX == sizeX - 1)
				{
					printf("\n");
				}
			}
/**/		

			while (directionsInterdites[myMove])
			{
				//myMove += 1;
				//myMove += aaaaaaaaaa;
				myMove+=3;
				if (myMove > 3)
				{
					myMove %= 4;
				}
				
				if (directionsInterdites[0] == 1 && directionsInterdites[1] == 1 && directionsInterdites[2] == 1 && directionsInterdites[3] == 1)
				{
					break;
				}
			}



/*
//manual
			scanf("%d", &move);
			switch(move){
				case 2: myMove = SOUTH; break;	
				case 4:	myMove = WEST; break;
				case 6:	myMove = EAST; break;
				case 8:	myMove = NORTH; break;
			}
*/
			switch(myMove){
				case NORTH: myLastPosy = myPOSy; myPOSy--; break;
				case WEST:	myLastPosx = myPOSx; myPOSx--; break;
				case EAST:	myLastPosx = myPOSx; myPOSx++; break;
				case SOUTH: myLastPosy = myPOSy; myPOSy++; break;
			}

			


			if (!(ctr % 20)){
				mySize++;
			}else{
				for (int i = 0; i < 2*(mySize - 1); i+=2){
					myBody[i] = myBody[i+2];
					myBody[i+1] = myBody[i+3];
				}
			}

			myBody[2*(mySize-1)] = myPOSx;
			myBody[2*(mySize-1) + 1] = myPOSy;

			//si le jeu termine a cause de mon move, j'ai perdu
			messageFinal = "DÉFAITE";

			gameON = (ret == sendMove(myMove));
		}else{
			gameON = (ret == getMove(&hisMove));
			
			switch(hisMove){
				case NORTH: hisLastPosy = hisPOSy; hisPOSy--; break;
				case WEST:	hisLastPosx = hisPOSx; hisPOSx--; break;
				case EAST:	hisLastPosx = hisPOSx; hisPOSx++; break;
				case SOUTH: hisLastPosy = hisPOSy; hisPOSy++; break;
			}

			if (ctr % 20 == 1){
				hisSize++;
			}else{
				for (int i = 0; i < 2*(hisSize - 1); i+=2){
					hisBody[i] = hisBody[i+2];
					hisBody[i+1] = hisBody[i+3];
				}
			}
			hisBody[2*(hisSize-1)] = hisPOSx;
			hisBody[2*(hisSize-1) + 1] = hisPOSy;			

			//si le jeu termine a cause de son move, j'ai gagne
			messageFinal = "VICTOIRE";
		}
		myTurn = !myTurn;
		ctr ++;

	}

	printf("%s\n", messageFinal);
/*
	for (int i = 0; i < 4; ++i)
	{
		printf("--%d", directionsInterdites[i]);
	}
/**/

/**/
	//why he lost
	switch(hisMove){
		case 0: printf("NORTH\n"); 	break;
		case 1: printf("EAST\n");	break;
		case 2: printf("SOUTH\n");	break;
		case 3: printf("WEST\n");	break;
	}
/**/	
	closeConnection();
	free(walls);
	return 0;
}