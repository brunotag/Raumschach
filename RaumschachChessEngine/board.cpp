#include "defs.h"
#include "protos.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>



BOOLEANVAL attack(int sq, int side)
{
    int i,j,n;

    for (i = 0; i < 125; i++)
    {
		if (colors[i] == side) 
        {
			if (pieces[i] == PAWN) 
            {
                if(side == WHITE)
                {
                    //the four capture moves of a pawn
                    if (mail_box[addresser[i] + WHITE_PAWN_AHEAD_LEFT] == sq)
                        return TRUE;
					if (mail_box[addresser[i] + WHITE_PAWN_AHEAD_RIGHT] == sq)
                        return TRUE;
					if (mail_box[addresser[i] + WHITE_PAWN_UP_LEFT] == sq)
                        return TRUE;
					if (mail_box[addresser[i] + WHITE_PAWN_UP_RIGHT] == sq)
                        return TRUE;
                }
                else //if colors is BLACK
                {
                    if (mail_box[addresser[i] + BLACK_PAWN_BACK_LEFT] == sq)
                        return TRUE;
					if (mail_box[addresser[i] + BLACK_PAWN_BACK_RIGHT] == sq)
                        return TRUE;
					if (mail_box[addresser[i] + BLACK_PAWN_DOWN_LEFT] == sq)
                        return TRUE;
					if (mail_box[addresser[i] + BLACK_PAWN_DOWN_RIGHT] == sq)
                        return TRUE;
			    }
            }
			else //if it's not a pawn
            {
				for (j = 0; j < off_sets[pieces[i]]; ++j)
                {
					for (n = i;;) 
                    {
						n = mail_box[addresser[n] + off_set[pieces[i]][j]];
						if (n == -1)
							break;
						if (n == sq)
							return TRUE;
						if (colors[n] != EMPTY)
							break;
						if (!slide[pieces[i]])
							break;
                    }
                }
            }
        }
    }
    return FALSE;
}

/* in_check() returns TRUE if side s is in check and FALSE
   otherwise. It just scans the board to find side s's king
   and calls attack() to see if it's being attacked. */

BOOLEANVAL in_check(int s)
{
    int j;
	for (j=0; j < 125; ++j)
		if (pieces[j] == KING && colors[j] == s)
			return attack(j, s ^ 1);
	return TRUE;  /* shouldn't get here */
}


void trueTakeBack()
{
    if (trueMovesDone>0){
        trueMovesDone --;
        take_back();
    }
}

/* take_back() is very similar to doMove(), only backwards :)  */

void take_back()
{
    if (movesDone>trueMovesDone){
    	moveBytes m;

    	sideToMove ^= 1;
    	sideNotToMove ^= 1;
    	--plyCurrent;
    	--histPlyCurrent;
        movesDone--;

    	m = historyData[histPlyCurrent].m.b;

        fiftMoves = historyData[histPlyCurrent].fiftMoves;
    	//hash = historyData[histPlyCurrent].hash;
    	
        colors[(int)m.from] = sideToMove;
	    if (m.bits & 4)
	    	pieces[(int)m.from] = PAWN;
    	else
		    pieces[(int)m.from] = pieces[(int)m.to];

	    if (historyData[histPlyCurrent].capture == EMPTY) {
		    colors[(int)m.to] = EMPTY;
    		pieces[(int)m.to] = EMPTY;
	    }
	    else {
		    colors[(int)m.to] = sideNotToMove;
		    pieces[(int)m.to] = historyData[histPlyCurrent].capture;
	    }

    }
}

//returns for example "Bc2Bc3" or a promotion string
int getSimpleStrFromMove(moveBytes m, char* result)
{
    char strTo[4];
    char strFrom[4];
    char c = ' ';

    getStringSquare(m.from, strFrom); 
    getStringSquare(m.to, strTo);   

    for (int i=0;i<3;i++){
        result[i] = strFrom[i];
    }
    for (int i=0;i<3;i++)
    {
        result[i+3] = strTo[i];
    }

    if (m.bits & 4)
    {
        switch (m.promote) {
			case KNIGHT:
				c = 'N';
				break;
            case UNICORN:
				c = 'U';
				break;
			case BISHOP:
				c = 'B';
				break;
			case ROOK:
				c = 'R';
				break;
			default:
				c = 'Q';
				break;
		}
    }   
    result[7] = c;
    result[8] = '\0';
    return 0;
}


/* parse the moveUni s (in coordinate notation) and return the moveUni's
   index in generData, or -1 if the moveUni is illegal */

int parseMove(char *square)
{
	int from, to, i;

	from = ('E'-square[0]) * 25 + square[1] - 'a' + ('5'-square[2]) * 5;
    to = ('E'-square[3]) * 25 + square[4] - 'a' + ('5'-square[5]) * 5;

	for (i = 0; i < firstMove[1]; ++i)
		if (generData[i].m.b.from == from && generData[i].m.b.to == to) {

			/* if the move is a promotion, handle the promotion pieces;
			   assume that the promotion moves occur consecutively in
			   generData. */
			if (generData[i].m.b.bits & 4)
				switch (square[6]) {
					case 'N':
						return i;
					case 'U':
						return i + 1;
					case 'B':
						return i + 2;
                    case 'R':
						return i + 3;
					default:  /* assume it's a queen */
						return i + 4;
				}
			return i;
		}

	/* didn't find the moveUni */
	return -1;
}


    

void getStringSquare(int number, char* result)
{
    number = 124-number;    
    result[0] = 'A'+ (int)number/25;
    result[1] = 'a'+ 4 - number % 5;
    result[2] = '0' + (int)(number % 25) / 5 + 1;
    result[3] = '\0';
}

void parseFEN(char* FEN_string)
{
    char* piece_placement;
    char* active_color;
    char* halfmove_clock;
    char* fullmove_number;
    char* current_plane_row;
    char current_square;
    char upper_current_square;
    char* current_plane;
    char* token;
    int i,j,k;
    int empty_counter;
    int square;

    char* old_plane;
    char* old_planes_rows;

    if(FEN_string != NULL )
    {
   
        piece_placement = strtok_s (FEN_string, " ", &token);      
        active_color = strtok_s (NULL, " ", &token);  

        if(strcmp(active_color, "w") == 0)
        {
            sideToMove = WHITE;
            sideNotToMove = BLACK;
        }else
        {
            sideToMove = BLACK;
            sideNotToMove = WHITE;
        }

        halfmove_clock = strtok_s (NULL, " ", &token); 
        fullmove_number = strtok_s (NULL, " ", &token);        

        current_plane = strtok_s (piece_placement, "-", &old_plane);
        old_planes_rows = current_plane;
        for (i=0;i<5;i++)
        {
            for (j=0;j<5;j++)
            {            
                current_plane_row = strtok_s (NULL, "/", &old_planes_rows);
    
                k=0;
                while (*current_plane_row != NULL)
                {
                    current_square = *current_plane_row;
                    if ((current_square>='1')&&(current_square<='5'))
                    {
                        empty_counter = atoi(&current_square);
                        for (; empty_counter>0 ; empty_counter--)
                        {
                            square = 25*i+5*j+k;
                            pieces[square] = EMPTY;
                            colors[square] = EMPTY;
                            k++;
                        }
                    }
                    else
                    {
                        square = 25*i+5*j+k;
                        upper_current_square = toupper(current_square);
                        if (current_square == upper_current_square)
                        {
                            colors[square] = WHITE;
                        }
                        else
                        {
                            colors[square] = BLACK;
                        }
                        switch(upper_current_square)
                        {
                            case 'P' : pieces[square] = PAWN; break;
                            case 'U' : pieces[square] = UNICORN; break;
                            case 'N' : pieces[square] = KNIGHT; break;
                            case 'B' : pieces[square] = BISHOP; break;    
                            case 'R' : pieces[square] = ROOK; break;
                            case 'Q' : pieces[square] = QUEEN; break;                               
                            case 'K' : pieces[square] = KING;                                
                        }
                        k++;
                    }
                    current_plane_row++;
                }
            }
            current_plane = strtok_s (NULL, "-", &old_plane);
            old_planes_rows = current_plane;
        }
    }
}

BOOLEANVAL trueDoMove(moveBytes m)
{
    BOOLEANVAL resp = doMove(m);
    if (resp)
        trueMovesDone++;
    return resp;
}

BOOLEANVAL doMove(moveBytes m)
{
	
	/* back up information so we can take the move back later. */
	historyData[histPlyCurrent].m.b = m;
	historyData[histPlyCurrent].capture = pieces[(int)m.to];
    historyData[histPlyCurrent].fiftMoves = fiftMoves;
	//historyData[histPlyCurrent].hash = hash;
	
    plyCurrent++;
	histPlyCurrent++;

    //mossa di un pedone o cattura
    if (m.bits & 3)
		fiftMoves = 0;
	else
		++fiftMoves;

	/* move the pieces */
	colors[(int)m.to] = sideToMove;
	if (m.bits & 4)
		pieces[(int)m.to] = m.promote;
	else
		pieces[(int)m.to] = pieces[(int)m.from];
	colors[(int)m.from] = EMPTY;
	pieces[(int)m.from] = EMPTY;

	/* switch sides and test for legality (if we can capture
	   the other guy's king, it's an illegal position and
	   we need to take the move back) */
	sideToMove ^= 1;
	sideNotToMove ^= 1;
    movesDone++;
	if (in_check(sideNotToMove)) {
		take_back();
		return FALSE;
	}
    //setHash();
	return TRUE;
}



/* genPush() puts a move on the move stack, unless it's a
   pawn promotion that needs to be handled by genPushProms().
   It also assigns a score to the move for alpha-beta move
   ordering. If the move is a capture, it uses MVV/LVA
   (Most Valuable Victim/Least Valuable Attacker). Otherwise,
   it uses the moveUni's history heuristic value. Note that
   1,000,000 is added to a capture move's score, so it
   always gets ordered above a "normal" move. */

void genPush(int from, int to, int bits)
{
	gener_t *g;
	
    //if it's a pawn move
	if (bits & 2) {
		if (sideToMove == WHITE) {
			if (to <= Ee5) {
				genPushProms(from, to, bits);
				return;
			}
		}
		else {
			if (to >= Aa1) {
				genPushProms(from, to, bits);
				return;
			}
		}
	}
	g = &generData[firstMove[plyCurrent + 1]++]; //firstMove[plyCurrent+1] is initially == 0. So, we get generData[0].
                                         //And then firstMove[plyCurrent+1]++ increments it, so it becomes 1 for the next.
	g->m.b.from =  from;
	g->m.b.to =  to;
	g->m.b.promote =  0;  //if we are here than it's not a promotion
	g->m.b.bits =  bits;
    //g->m.u += rand()+1;

    //if(g->m.b.to<0)
    //    g->score = 0;

	if (colors[to] != EMPTY)
		g->score = 1000000 + (pieces[to] * 10) - pieces[from];
	else
		g->score = history[from][to];
}

/* genPushProms() is just like genPush(), only it puts 5 moves
   on the move stack, one for each possible promotion pieces */

void genPushProms(int from, int to, int bits)
{
	int i;
	gener_t *g;
	
	for (i = KNIGHT; i <= QUEEN; ++i) 
    {
		g = &generData[firstMove[plyCurrent + 1]++];
		g->m.b.from =  from;
		g->m.b.to =  to;
		g->m.b.promote =  i;
		g->m.b.bits =  (bits | 4);
		g->score = 1000000 + (i * 10);
	}
}



///* hashRand() XORs some shifted random numbers together to make sure
//   we have good coverage of all 32 bits. (rand() returns 16-bit numbers
//   on some systems.) */
//
//int hashRand()
//{
//	int i;
//	int r = 0;
//
//	for (i = 0; i < 32; ++i)
//		r ^= rand() << i;
//	return r;
//}
//
///* initHash() initializes the random numbers used by setHash(). */
//
//void initHash()
//{
//	int i, j, k;
//
//	srand(0);
//	for (i = 0; i < 2; ++i)
//		for (j = 0; j < 7; ++j)
//			for (k = 0; k < 125; ++k)
//				hashPiece[i][j][k] = hashRand();
//	hashSide = hashRand();
//	for (i = 0; i < 125; ++i)
//		hashEP[i] = hashRand();
//}
//
//
///* setHash() uses the Zobrist method of generating a unique number (hash)
//   for the current chess position. Of course, there are many more chess
//   positions than there are 32 bit numbers, so the numbers generated are
//   not really unique, but they're unique enough for our purposes (to detect
//   repetitions of the position). 
//   The way it works is to XOR random numbers that correspond to features of
//   the position, e.g., if there's a black knight on B8, hash is XORed with
//   hashPiece[BLACK][KNIGHT][B8]. All of the pieces are XORed together,
//   hashSide is XORed if it's black's move. (A chess technicality is that 
//   one position can't
//   be a repetition of another if the en passant state is different.) */
//
//void setHash()
//{
//	int i;
//
//	hash = 0;	
//	for (i = 0; i < 125; ++i)
//		if (colors[i] != EMPTY)
//			hash ^= hashPiece[colors[i]][pieces[i]][i];
//	if (sideToMove == BLACK)
//		hash ^= hashSide;
//}

/* initBoard() sets the board to the initial game state. */

 
void initBoard()
{
	sideToMove = WHITE;
	sideNotToMove = BLACK;
	fiftMoves = 0;
	plyCurrent = 0;
	histPlyCurrent = 0;
	firstMove[0] = 0;
    movesDone = 0;
    trueMovesDone = 0;
}







/*void testMoves()
{
    gener_t *g;
    int i;



    for (i=firstMove[0];i<firstMove[1];i++)
    {
        char plain = 'E';
        char row = '5';
        char col = 'a';

        g = &generData[i];
        printf("moveUni %d:\n",i);

        plain -= g->m.b.from / 25;
        row -= (g->m.b.from % 25) / 5;
        col += g->m.b.from % 5;
        
        printf("from: %c%c%c\n",plain,col,row);

        plain = 'E';
        row = '5';
        col = 'a';

        plain -= g->m.b.to / 25;
        row -= (g->m.b.to % 25) / 5;
        col += g->m.b.to % 5;

        printf("to: %c%c%c\n",plain,col,row);
        printf("prom?:%d\n",g->m.b.promote);
        if(g->m.b.bits & 2) printf("It's a PAWN\n");
        printf("\n\n");
        getchar();
    }
}*/


/* generate() generates pseudo-legal moves for the current position.
   It scans the board to find friendly pieces and then determines
   what squares they attack. When it finds a pieces/square
   combination, it calls genPush to put the moveUni on the "moveUni
   stack." */

void generate()
{
	int i;
    int j;
    int n;

	/* so far, we have no moves for the current plyCurrent */
	firstMove[plyCurrent + 1] = firstMove[plyCurrent];

    for (i = 0; i < 125; i++)
    {
		if (colors[i] == sideToMove) 
        {
			if (pieces[i] == PAWN) 
            {
                if(colors[i] == WHITE)
                {
                    if ((mail_box[addresser[i] + WHITE_PAWN_AHEAD_LEFT] != -1)&&(colors[mail_box[addresser[i] + WHITE_PAWN_AHEAD_LEFT]]==BLACK))
						genPush(i, mail_box[addresser[i] + WHITE_PAWN_AHEAD_LEFT], 3); //3 means 11 in binary, so pawn + capture moveUni
					if ((mail_box[addresser[i] + WHITE_PAWN_AHEAD_RIGHT] != -1)&&(colors[mail_box[addresser[i] + WHITE_PAWN_AHEAD_RIGHT]]==BLACK))
						genPush(i, mail_box[addresser[i] + WHITE_PAWN_AHEAD_RIGHT], 3);
					if ((mail_box[addresser[i] + WHITE_PAWN_UP_LEFT] != -1)&&(colors[mail_box[addresser[i] + WHITE_PAWN_UP_LEFT]]==BLACK))
						genPush(i, mail_box[addresser[i] + WHITE_PAWN_UP_LEFT], 3);
					if ((mail_box[addresser[i] + WHITE_PAWN_UP_RIGHT] != -1)&&(colors[mail_box[addresser[i] + WHITE_PAWN_UP_RIGHT]]==BLACK))
						genPush(i, mail_box[addresser[i] + WHITE_PAWN_UP_RIGHT], 3);
                    if (colors[mail_box[addresser[i] + WHITE_PAWN_DOWN]] == EMPTY) 
						genPush(i,mail_box[addresser[i] + WHITE_PAWN_DOWN], 2);
                    if (colors[mail_box[addresser[i] + WHITE_PAWN_AHEAD]] == EMPTY) 
                        genPush(i,mail_box[addresser[i] + WHITE_PAWN_AHEAD], 2);
                }
                else //if colors is BLACK
                {
                    if ((mail_box[addresser[i] + BLACK_PAWN_BACK_LEFT] != -1)&&(colors[mail_box[addresser[i] + BLACK_PAWN_BACK_LEFT]]==WHITE))
						genPush(i, mail_box[addresser[i] + BLACK_PAWN_BACK_LEFT], 3);
					if ((mail_box[addresser[i] + BLACK_PAWN_BACK_RIGHT] != -1)&&(colors[mail_box[addresser[i] + BLACK_PAWN_BACK_RIGHT]]==WHITE))
						genPush(i, mail_box[addresser[i] + BLACK_PAWN_BACK_RIGHT], 3);
					if ((mail_box[addresser[i] + BLACK_PAWN_DOWN_LEFT] != -1)&&(colors[mail_box[addresser[i] + BLACK_PAWN_DOWN_LEFT]]==WHITE))
						genPush(i, mail_box[addresser[i] + BLACK_PAWN_DOWN_LEFT], 3);
					if ((mail_box[addresser[i] + BLACK_PAWN_DOWN_RIGHT] != -1)&&(colors[mail_box[addresser[i] + BLACK_PAWN_DOWN_RIGHT]]==WHITE))
						genPush(i, mail_box[addresser[i] + BLACK_PAWN_DOWN_RIGHT], 3);
                    if (colors[mail_box[addresser[i] + BLACK_PAWN_UP]] == EMPTY) 
						genPush(i,mail_box[addresser[i] + BLACK_PAWN_UP], 2);
                    if (colors[mail_box[addresser[i] + BLACK_PAWN_BACK]] == EMPTY) 
                        genPush(i,mail_box[addresser[i] + BLACK_PAWN_BACK], 2);
                }
            }
			else //if it's not a pawn
            {
				for (j = 0; j < off_sets[pieces[i]]; ++j)
                {
					for (n = i;;) 
                    {
						n = mail_box[addresser[n] + off_set[pieces[i]][j]];
						if (n == -1)
							break;
						if (colors[n] != EMPTY) 
                        {
							if (colors[n] == sideNotToMove) //it's a capture move!
								genPush(i, n, 1);
							break;
						}
						genPush(i, n, 0); //it's a normal moveUni
						if (!slide[pieces[i]])
							break;
                    }
                }
            }
        }
    }
}

void generateCaptures()
{
	int i;
    int j;
    int n;

	/* so far, we have no moves for the current plyCurrent */
	firstMove[plyCurrent + 1] = firstMove[plyCurrent];

    for (i = 0; i < 125; i++)
    {
		if (colors[i] == sideToMove) 
        {
			if (pieces[i] == PAWN) 
            {
                if(colors[i] == WHITE)
                {
                    if ((mail_box[addresser[i] + WHITE_PAWN_AHEAD_LEFT] != -1)&&(colors[mail_box[addresser[i] + WHITE_PAWN_AHEAD_LEFT]]==BLACK))
						genPush(i, mail_box[addresser[i] + WHITE_PAWN_AHEAD_LEFT], 3); //3 means 11 in binary, so pawn + capture moveUni
					if ((mail_box[addresser[i] + WHITE_PAWN_AHEAD_RIGHT] != -1)&&(colors[mail_box[addresser[i] + WHITE_PAWN_AHEAD_RIGHT]]==BLACK))
						genPush(i, mail_box[addresser[i] + WHITE_PAWN_AHEAD_RIGHT], 3);
					if ((mail_box[addresser[i] + WHITE_PAWN_UP_LEFT] != -1)&&(colors[mail_box[addresser[i] + WHITE_PAWN_UP_LEFT]]==BLACK))
						genPush(i, mail_box[addresser[i] + WHITE_PAWN_UP_LEFT], 3);
					if ((mail_box[addresser[i] + WHITE_PAWN_UP_RIGHT] != -1)&&(colors[mail_box[addresser[i] + WHITE_PAWN_UP_RIGHT]]==BLACK))
						genPush(i, mail_box[addresser[i] + WHITE_PAWN_UP_RIGHT], 3);
                }
                else //if colors is BLACK
                {
                    if ((mail_box[addresser[i] + BLACK_PAWN_BACK_LEFT] != -1)&&(colors[mail_box[addresser[i] + BLACK_PAWN_BACK_LEFT]]==WHITE))
						genPush(i, mail_box[addresser[i] + BLACK_PAWN_BACK_LEFT], 3);
					if ((mail_box[addresser[i] + BLACK_PAWN_BACK_RIGHT] != -1)&&(colors[mail_box[addresser[i] + BLACK_PAWN_BACK_RIGHT]]==WHITE))
						genPush(i, mail_box[addresser[i] + BLACK_PAWN_BACK_RIGHT], 3);
					if ((mail_box[addresser[i] + BLACK_PAWN_DOWN_LEFT] != -1)&&(colors[mail_box[addresser[i] + BLACK_PAWN_DOWN_LEFT]]==WHITE))
						genPush(i, mail_box[addresser[i] + BLACK_PAWN_DOWN_LEFT], 3);
					if ((mail_box[addresser[i] + BLACK_PAWN_DOWN_RIGHT] != -1)&&(colors[mail_box[addresser[i] + BLACK_PAWN_DOWN_RIGHT]]==WHITE))
						genPush(i, mail_box[addresser[i] + BLACK_PAWN_DOWN_RIGHT], 3);
			    }
            }
			else //if it's not a pawn
            {
				for (j = 0; j < off_sets[pieces[i]]; ++j)
                {
					for (n = i;;) 
                    {
						n = mail_box[addresser[n] + off_set[pieces[i]][j]];
						if (n == -1)
							break;
						if (colors[n] != EMPTY) 
                        {
							if (colors[n] == sideNotToMove) //it's a capture move!
								genPush(i, n, 1);
							break;
						}
						if (!slide[pieces[i]])
							break;
                    }
                }
            }
        }
    }
}


