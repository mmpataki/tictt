//#define DEBUG

/*
 * This function reads the states from the given string.
 * the format of the string must be as follows.
 *
 * (x|o) '<' ([0-9]+)([ \t\n\r]) '>'
 *
 *  * the first character says the stater of the game.
 *  * the numbers in the angular brackets tell the further moves.
 *
 * some examples include.
 *     o <1 2 3 4 5>
 *     x <3 2 4 1 6 9>
 *
 * NOTE:
 *	The function doesn't care about the end of the game but raises an
 *	exception by returning -1 whenever an invalid move is mentioned. 
 *
 * returns zero on success and -1 on exception (refer above note.)
 *
 */

#include <stdio.h>

char *ttt_read_msgs[] = {
	"",
	"null pointer",
	"case mismatch",
	"invalid format",
	"invalid position sequence"
};

char *ttt_get_readerr(int code) {
	return ttt_read_msgs[-code];
}

int ttt_read_state(char *string, char board[][3]) {

	int p;
	int pos;
	char fp;
	char *s = string;
	char *is;
	char players[2] = {'x', 'o'};

	if(s == NULL)
		return -1;

	/* skip the whitespaces */
	while(*s && ( *s==' ' || *s=='\t' || *s=='\n' || *s=='\r')) s++;
	
	/* get the first player */
	fp = *s;

	if(fp != 'o' && fp != 'x')
		return -2;

	p = (fp == 'x');

	++s;

	/* skip the middle white space */
	while(*s && ( *s==' ' || *s=='\t' || *s=='\n' || *s=='\r')) s++;

	if(*s != '<')
		return -3;

	/* skip the left angular bracket */
	s++;

	while(*s) {

		while(*s && (*s==' ' || *s=='\t' || *s=='\n' || *s=='\r'))
			s++;

		if(!*s || *s == '>')
			break;

		pos = (*s - '0') - 1;

		if(board[pos / 3][pos % 3] == ' ')
			board[pos / 3][pos % 3] = players[(p = !p)];
		else
			return -4;
		++s;
	}

	return 0;
}

#ifdef DEBUG

int main() {

	int i;
	char *tst = "x <3 1 2 5 4>";
	char b[][3] = {{' ', ' ', ' '},{' ',' ',' '},{' ',' ',' '}};

	puts(ttt_get_readerr(ttt_read_state(tst, b)));

	for(i=0; i<9; i++) {
		if(i % 3 == 0) printf("\n");
		printf("%c ", b[i/3][i%3]);
	}
}
#endif
