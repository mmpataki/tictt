#include <stdio.h>
#include <stdlib.h>
#include "util.c"

#define HVSH 1 //   human vs human
#define HVSC 2 //   human vs computer
#define CVSC 3 //computer vs computer

char problem[][3] = {
	{' ', ' ', ' '},
	{' ', ' ', ' '},
	{' ', ' ', ' '}
};
int depth = 0;
char computer;

int usage();
int gethumanmove();
int pm(char a[][3]);
int eval(char a[][3], char p);
int solve(char a[][3], char p);
int solvex(char a[][3], char p, int *solution);

int main(int argc, char **argv) {
	
	if(argc <= 1) usage(argv[0]);
	if(argc == 3) puts(ttt_get_readerr(ttt_read_state(argv[1], problem)));

	int move, type = atoi(argv[1]), test = 1;

	do {
		system("clear");

		pm(problem);
		if(eval(problem, computer) % 10 == 0) break;
		test = type == HVSH ? 0 : (type == HVSC ? !test : 1);

		computer = computer == 'o' ? 'x' : 'o';
		move = test ? solve(problem, computer) : gethumanmove();
		problem[move / 3][move % 3] = computer;

	} while(1);

}

int usage(char *arg0) {
	printf(
		"usage : %s <game_type_code> [previous_game]\n"
		"game type codes:\n"
		"	 1 : Human vs Human\n"
		"	 2 : Human vs Computer\n"	
		"	 3 : Computer vs Computer\n"
		"previous_game: (optional)\n"
		"	You can load the previous game by\n"
		"	providing the game-string whose format\n"
		"	is as follows. \n"
		"		(x|o) '<' ([0-9][ \\t\\n])* '>'\n"
		"	eg. x <1,2,3,4,5,6,7>\n",
		arg0
	);
	exit(0);
}

int gethumanmove() {
	int move;
	do {
		printf("\nEnter a move : "); scanf("%d", &move); 
		--move;
	} while(problem[move / 3][move % 3] != ' ');
	return move;
}

int pm(char a[][3]) {
	printf("-------------\n");
	printf("| %c | %c | %c |\n", a[0][0], a[0][1], a[0][2]);
	printf("| %c | %c | %c |\n", a[1][0], a[1][1], a[1][2]);
	printf("| %c | %c | %c |\n", a[2][0], a[2][1], a[2][2]);
	printf("-------------\n\n");
}


int solve(char a[][3], char p) {
	int move;
	solvex(a, p, &move);

	if(problem[move/3][move%3] == ' ')
		return move;

	/* search a empty location */
	for(move=0; move<9; move++)
		if(a[move/3][move%3] == ' ')
			return move;
}

int solvex(char a[][3], char p, int *solution) {

	depth++;

	int i, x, y;
	int tmp, mx;
	int state;
	char np = (p == 'x') ? 'o' : 'x';	//next player
	int states[] = {0,0,0, 0,0,0, 0,0,0};
	int valids[] = {0,0,0, 0,0,0, 0,0,0};	//simhack for neglecting invalids.

	mx = (p != computer) ? 100 : -100;

	for(i=0; i<9; i++) {

		y = i / 3; x = i % 3;

		if(a[y][x] != ' ') continue;
		
		a[y][x] = p;

		state = eval(a, p);

		/* check whether it's end state */
		if((state % 10) == 0) {
			*solution = i;
			a[y][x] = ' ';
			depth--;
			return (p != computer) ? -state : state;
		}
		
		tmp = solvex(a, np, solution);

		if((p == computer && mx < tmp) || (p != computer && mx > tmp)) {
			mx = tmp;
		}

		valids[i] = 1;
		states[i] = tmp;

		a[y][x] = ' ';
	}

	/* find the maximum solution */
	tmp = -100;
	*solution = 0;
	for(i = 0; i < 9; i++) {
		if(valids[i] && states[i] >= tmp && problem[i/3][i%3] == ' ') {
			*solution = i;
			tmp = states[i];
		}
	}

	depth--;

	return mx + (p != computer ? 1 : -1);
}


int eval(char b[][3], char p) {

	int i, j;

	// row matches.
	for(i = 0; i < 3; i++) {
		if(b[i][0]==p && b[i][1]==p && b[i][2]==p)
			return 10;
	}
	// col matches.
	for(i = 0; i < 3; i++) {
		if(b[0][i]==p && b[1][i]==p && b[2][i]==p)
			return 10;
	}
	//diag matches.
	if ((b[0][0]==p && b[1][1]==p && b[2][2]==p) ||
	    (b[0][2]==p && b[1][1]==p && b[2][0]==p))
		return 10;
	
	//check if game is ended?
	for(i=0; i<3; i++) {
		for(j=0; j<3; j++) {
			if(b[i][j] == ' ')
				return -1;
		}
	}

	return 0;
}

