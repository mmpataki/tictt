#include <stdio.h>
#include <stdlib.h>
#include "util.c"

#define N    3
#define POS  (N*N+1)

#define HVSH 1 //   human vs human
#define HVSC 2 //   human vs computer
#define CVSC 3 //computer vs computer

char problem[N][N];
int depth = 0;
int nmoves = 0;
char computer;

int usage();
int gethumanmove();
int pm(char a[][N]);
int load_problem(void);
int eval(char a[][N], char p);
int solve(char a[][N], char p);
int solvex(char a[][N], char p, int *solution);

int main(int argc, char **argv) {
	
	load_problem();

	if(argc <= 1) usage(argv[0]);
	if(argc == 3) puts(ttt_get_readerr(ttt_read_state(argv[1], problem)));

	int move, type = atoi(argv[1]), test = 0;

	do {
		system("clear");

		pm(problem);
		if(eval(problem, computer) % POS == 0) break;
		test = type == HVSH ? 0 : (type == HVSC ? !test : 1);

		computer = computer == 'o' ? 'x' : 'o';
		move = test ? solve(problem, computer) : gethumanmove();
		problem[move / N][move % N] = computer;

	} while(1);

}

int load_problem() {
	int i, j;
	for(i=0; i<N; i++)
		for(j=0; j<N; j++)
			problem[i][j] = ' ';
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
	} while(problem[move / N][move % N] != ' ');
	return move;
}

int pm(char a[][N]) {
	int i, j;
	for(i=0; i<N; i++, putchar('\n')) {
		for(j=0; j<N; j++)
			printf("%2c%c", a[i][j], (j+1==N)?' ':'|');
	}
	printf("\nlast evaluations : %dnodes\n", nmoves);
}

int solve(char a[][N], char p) {
	int move;

	nmoves = 0;
	solvex(a, p, &move);

	if(problem[move/N][move%N] == ' ')
		return move;

	/* search a empty location */
	for(move=0; move<N*N; move++)
		if(a[move/N][move%N] == ' ')
			return move;
}

int solvex(char a[][N], char p, int *solution) {

	depth++;

	int i, x, y;
	int tmp, mx;
	int state;
	char np = (p == 'x') ? 'o' : 'x';	//next player
	int states[N*N], valids[N*N];

	for(i=0; i<N*N; i++)
		states[i] = valids[i] = 0;

	mx = (p != computer) ? 999999 : -999999;

	for(i=0; i<N*N; i++) {
		
		y = i / N; x = i % N;
		
		if(a[y][x] != ' ') continue;

		nmoves++;
		a[y][x] = p;

		state = eval(a, p);

		if((state % POS) == 0) {
			*solution = i;
			a[y][x] = ' ';
			depth--;
			return (p != computer) ? -state : state;
		}

		a[y][x] = ' ';
	}

	for(i=0; i<N*N; i++) {

		y = i / N; x = i % N;

		if(a[y][x] != ' ') continue;

		a[y][x] = p;
		tmp = solvex(a, np, solution);

		if((p == computer && mx < tmp) || (p != computer && mx > tmp)) {
			mx = tmp;
		}

		valids[i] = 1;
		states[i] = tmp;
		a[y][x] = ' ';
	}

	//for(i=0; i<N*N; i++)printf("%d ", states[i]);puts("");


	/* find the maximum solution */
	tmp = -(POS+1);
	*solution = 0;
	for(i = 0; i < N*N; i++) {
		if(valids[i] && states[i] >= tmp && problem[i/N][i%N] == ' ') {
			*solution = i;
			tmp = states[i];
		}
	}

	depth--;

	return mx + (p != computer ? 1 : -1);
}


int eval(char b[][N], char p) {

	int i, j, cnt;

	// row matches.
	for(i = 0; i < N; i++) {
		for(cnt=0, j=0; j<N; j++)
			cnt += (b[i][j] == p);
		if(cnt == N) return POS;
	}

	// col matches.
	for(i = 0; i < N; i++) {
		for(cnt=0, j=0; j<N; j++)
			cnt += (b[j][i] == p);
		if(cnt == N) return POS;
	}

	//diag matches.
	for(cnt=0, i=0; i<N; i++)
		cnt += (b[i][i] == p);
	if(cnt == N) return POS;

	for(cnt=0, i=0; i<N; i++)
		cnt += (b[i][N-1-i] == p);
	if(cnt == N) return POS;
	
	//check if game is ended?
	for(i=0; i<N; i++) {
		for(j=0; j<N; j++) {
			if(b[i][j] == ' ')
				return -1;
		}
	}

	return 0;
}

