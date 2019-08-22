
#include "defs.h"
#include "protos.h"


BOOL attack(int sq, int side)
{
    int i,j,n;

    for (i = 0; i < 125; i++)
    {
		if (color[i] == side) 
        {
			if (piece[i] == PAWN) 
            {
                if(side == LIGHT)
                {
                    //the four capture moves of a pawn
                    if (mailbox[mailbox125[i] + LIGHT_PAWN_AHEAD_LEFT] == sq)
                        return TRUE;
					if (mailbox[mailbox125[i] + LIGHT_PAWN_AHEAD_RIGHT] == sq)
                        return TRUE;
					if (mailbox[mailbox125[i] + LIGHT_PAWN_UP_LEFT] == sq)
                        return TRUE;
					if (mailbox[mailbox125[i] + LIGHT_PAWN_UP_RIGHT] == sq)
                        return TRUE;
                }
                else //if color is DARK
                {
                    if (mailbox[mailbox125[i] + DARK_PAWN_BACK_LEFT] == sq)
                        return TRUE;
					if (mailbox[mailbox125[i] + DARK_PAWN_BACK_RIGHT] == sq)
                        return TRUE;
					if (mailbox[mailbox125[i] + DARK_PAWN_DOWN_LEFT] == sq)
                        return TRUE;
					if (mailbox[mailbox125[i] + DARK_PAWN_DOWN_RIGHT] == sq)
                        return TRUE;
			    }
            }
			else //if it's not a pawn
            {
				for (j = 0; j < offsets[piece[i]]; ++j)
                {
					for (n = i;;) 
                    {
						n = mailbox[mailbox125[n] + offset[piece[i]][j]];
						if (n == -1)
							break;
						if (n == sq)
							return TRUE;
						if (color[n] != EMPTY)
							break;
						if (!slide[piece[i]])
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

BOOL in_check(int s)
{
    int j;
	for (j=0; j < 125; ++j)
		if (piece[j] == KING && color[j] == s)
			return attack(j, s ^ 1);
	return TRUE;  /* shouldn't get here */
}

/* takeback() is very similar to makemove(), only backwards :)  */

void takeback()
{
	move_bytes m;

	sideToMove ^= 1;
	sideNotToMove ^= 1;
	--ply;
	--hply;

	m = hist_dat[hply].m.b;
    fifty = hist_dat[hply].fifty;
	hash = hist_dat[hply].hash;
    
	
    color[(int)m.from] = sideToMove;
	if (m.bits & 4)
		piece[(int)m.from] = PAWN;
	else
		piece[(int)m.from] = piece[(int)m.to];
	if (hist_dat[hply].capture == EMPTY) {
		color[(int)m.to] = EMPTY;
		piece[(int)m.to] = EMPTY;
	}
	else {
		color[(int)m.to] = sideNotToMove;
		piece[(int)m.to] = hist_dat[hply].capture;
	}
}

BOOL makemove(move_bytes m)
{
	
	/* back up information so we can take the move back later. */
	hist_dat[hply].m.b = m;
	hist_dat[hply].capture = piece[(int)m.to];
    hist_dat[hply].fifty = fifty;
	hist_dat[hply].hash = hash;
	
    ply++;
	hply++;

    //mossa di un pedone o cattura
    if (m.bits & 3)
		fifty = 0;
	else
		++fifty;

	/* move the piece */
	color[(int)m.to] = sideToMove;
	if (m.bits & 4)
		piece[(int)m.to] = m.promote;
	else
		piece[(int)m.to] = piece[(int)m.from];
	color[(int)m.from] = EMPTY;
	piece[(int)m.from] = EMPTY;

	/* switch sides and test for legality (if we can capture
	   the other guy's king, it's an illegal position and
	   we need to take the move back) */
	sideToMove ^= 1;
	sideNotToMove ^= 1;
	if (in_check(sideNotToMove)) {
		takeback();
		return FALSE;
	}
    set_hash();
	return TRUE;
}



/* gen_push() puts a move on the move stack, unless it's a
   pawn promotion that needs to be handled by gen_promote().
   It also assigns a score to the move for alpha-beta move
   ordering. If the move is a capture, it uses MVV/LVA
   (Most Valuable Victim/Least Valuable Attacker). Otherwise,
   it uses the move's history heuristic value. Note that
   1,000,000 is added to a capture move's score, so it
   always gets ordered above a "normal" move. */

void gen_push(int from, int to, int bits)
{
	gen_t *g;
	
    //if it's a pawn promotion move
	if (bits & 2) {
		if (sideToMove == LIGHT) {
			if (to >= Ee1) {
				gen_promote(from, to, bits);
				return;
			}
		}
		else {
			if (to <= Aa5) {
				gen_promote(from, to, bits);
				return;
			}
		}
	}
	g = &gen_dat[first_move[ply + 1]++]; //first_move[ply+1] is initially == 0. So, we get gen_dat[0].
                                         //And then first_move[ply+1]++ increments it, so it becomes 1 for the next.
	g->m.b.from = (char)from;
	g->m.b.to = (char)to;
	g->m.b.promote = 0;  //if we are here than it's not a promotion
	g->m.b.bits = (char)bits;
    //g->m.u += rand()+1;


	if (color[to] != EMPTY)
		g->score = 1000000 + (piece[to] * 10) - piece[from];
	else
		g->score = history[from][to];
}

/* gen_promote() is just like gen_push(), only it puts 5 moves
   on the move stack, one for each possible promotion piece */

void gen_promote(int from, int to, int bits)
{
	int i;
	gen_t *g;
	
	for (i = KNIGHT; i <= QUEEN; ++i) 
    {
		g = &gen_dat[first_move[ply + 1]++];
		g->m.b.from = (char)from;
		g->m.b.to = (char)to;
		g->m.b.promote = (char)i;
		g->m.b.bits = (char)(bits | 32);
		g->score = 1000000 + (i * 10);
	}
}



/* hash_rand() XORs some shifted random numbers together to make sure
   we have good coverage of all 32 bits. (rand() returns 16-bit numbers
   on some systems.) */

int hash_rand()
{
	int i;
	int r = 0;

	for (i = 0; i < 32; ++i)
		r ^= rand() << i;
	return r;
}

/* init_hash() initializes the random numbers used by set_hash(). */

void init_hash()
{
	int i, j, k;

	srand(0);
	for (i = 0; i < 2; ++i)
		for (j = 0; j < 7; ++j)
			for (k = 0; k < 125; ++k)
				hash_piece[i][j][k] = hash_rand();
	hash_side = hash_rand();
	for (i = 0; i < 125; ++i)
		hash_ep[i] = hash_rand();
}



/* set_hash() uses the Zobrist method of generating a unique number (hash)
   for the current chess position. Of course, there are many more chess
   positions than there are 32 bit numbers, so the numbers generated are
   not really unique, but they're unique enough for our purposes (to detect
   repetitions of the position). 
   The way it works is to XOR random numbers that correspond to features of
   the position, e.g., if there's a black knight on B8, hash is XORed with
   hash_piece[BLACK][KNIGHT][B8]. All of the pieces are XORed together,
   hash_side is XORed if it's black's move, and the en passant square is
   XORed if there is one. (A chess technicality is that one position can't
   be a repetition of another if the en passant state is different.) */

void set_hash()
{
	int i;

	hash = 0;	
	for (i = 0; i < 125; ++i)
		if (color[i] != EMPTY)
			hash ^= hash_piece[color[i]][piece[i]][i];
	if (sideToMove == DARK)
		hash ^= hash_side;
}

/* init_board() sets the board to the initial game state. */

 
void init_board()
{
	sideToMove = LIGHT;
	sideNotToMove = DARK;
	fifty = 0;
	ply = 0;
	hply = 0;
	set_hash();  // init_hash() must be called before this function 
	first_move[0] = 0;
}







/*void testMoves()
{
    gen_t *g;
    int i;



    for (i=first_move[0];i<first_move[1];i++)
    {
        char plain = 'E';
        char row = '5';
        char col = 'a';

        g = &gen_dat[i];
        printf("Move %d:\n",i);

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

/* gen() generates pseudo-legal moves for the current position.
   It scans the board to find friendly pieces and then determines
   what squares they attack. When it finds a piece/square
   combination, it calls gen_push to put the move on the "move
   stack." */

void gen()
{
	int i;
    int j;
    int n;

	/* so far, we have no moves for the current ply */
	first_move[ply + 1] = first_move[ply];

    for (i = 0; i < 125; i++)
    {
		if (color[i] == sideToMove) 
        {
			if (piece[i] == PAWN) 
            {
                if(color[i] == LIGHT)
                {
                    if ((mailbox[mailbox125[i] + LIGHT_PAWN_AHEAD_LEFT] != -1)&&(color[mailbox[mailbox125[i] + LIGHT_PAWN_AHEAD_LEFT]]==DARK))
						gen_push(i, mailbox[mailbox125[i] + LIGHT_PAWN_AHEAD_LEFT], 3); //3 means 11 in bynary, so pawn + capture move
					if ((mailbox[mailbox125[i] + LIGHT_PAWN_AHEAD_RIGHT] != -1)&&(color[mailbox[mailbox125[i] + LIGHT_PAWN_AHEAD_RIGHT]]==DARK))
						gen_push(i, mailbox[mailbox125[i] + LIGHT_PAWN_AHEAD_RIGHT], 3);
					if ((mailbox[mailbox125[i] + LIGHT_PAWN_UP_LEFT] != -1)&&(color[mailbox[mailbox125[i] + LIGHT_PAWN_UP_LEFT]]==DARK))
						gen_push(i, mailbox[mailbox125[i] + LIGHT_PAWN_UP_LEFT], 3);
					if ((mailbox[mailbox125[i] + LIGHT_PAWN_UP_RIGHT] != -1)&&(color[mailbox[mailbox125[i] + LIGHT_PAWN_UP_RIGHT]]==DARK))
						gen_push(i, mailbox[mailbox125[i] + LIGHT_PAWN_UP_RIGHT], 3);
                    if (color[mailbox[mailbox125[i] + LIGHT_PAWN_DOWN]] == EMPTY) 
						gen_push(i,mailbox[mailbox125[i] + LIGHT_PAWN_DOWN], 2);
                    if (color[mailbox[mailbox125[i] + LIGHT_PAWN_AHEAD]] == EMPTY) 
                        gen_push(i,mailbox[mailbox125[i] + LIGHT_PAWN_AHEAD], 2);
                }
                else //if color is DARK
                {
                    if ((mailbox[mailbox125[i] + DARK_PAWN_BACK_LEFT] != -1)&&(color[mailbox[mailbox125[i] + DARK_PAWN_BACK_LEFT]]==LIGHT))
						gen_push(i, mailbox[mailbox125[i] + DARK_PAWN_BACK_LEFT], 3);
					if ((mailbox[mailbox125[i] + DARK_PAWN_BACK_RIGHT] != -1)&&(color[mailbox[mailbox125[i] + DARK_PAWN_BACK_RIGHT]]==LIGHT))
						gen_push(i, mailbox[mailbox125[i] + DARK_PAWN_BACK_RIGHT], 3);
					if ((mailbox[mailbox125[i] + DARK_PAWN_DOWN_LEFT] != -1)&&(color[mailbox[mailbox125[i] + DARK_PAWN_DOWN_LEFT]]==LIGHT))
						gen_push(i, mailbox[mailbox125[i] + DARK_PAWN_DOWN_LEFT], 3);
					if ((mailbox[mailbox125[i] + DARK_PAWN_DOWN_RIGHT] != -1)&&(color[mailbox[mailbox125[i] + DARK_PAWN_DOWN_RIGHT]]==LIGHT))
						gen_push(i, mailbox[mailbox125[i] + DARK_PAWN_DOWN_RIGHT], 3);
                    if (color[mailbox[mailbox125[i] + DARK_PAWN_UP]] == EMPTY) 
						gen_push(i,mailbox[mailbox125[i] + DARK_PAWN_UP], 2);
                    if (color[mailbox[mailbox125[i] + DARK_PAWN_BACK]] == EMPTY) 
                        gen_push(i,mailbox[mailbox125[i] + DARK_PAWN_BACK], 2);
                }
            }
			else //if it's not a pawn
            {
				for (j = 0; j < offsets[piece[i]]; ++j)
                {
					for (n = i;;) 
                    {
						n = mailbox[mailbox125[n] + offset[piece[i]][j]];
						if (n == -1)
							break;
						if (color[n] != EMPTY) 
                        {
							if (color[n] == sideNotToMove) //it's a capture move!
								gen_push(i, n, 1);
							break;
						}
						gen_push(i, n, 0); //it's a normal move
						if (!slide[piece[i]])
							break;
                    }
                }
            }
        }
    }
}

void gen_caps()
{
	int i;
    int j;
    int n;

	/* so far, we have no moves for the current ply */
	first_move[ply + 1] = first_move[ply];

    for (i = 0; i < 125; i++)
    {
		if (color[i] == sideToMove) 
        {
			if (piece[i] == PAWN) 
            {
                if(color[i] == LIGHT)
                {
                    if ((mailbox[mailbox125[i] + LIGHT_PAWN_AHEAD_LEFT] != -1)&&(color[mailbox[mailbox125[i] + LIGHT_PAWN_AHEAD_LEFT]]==DARK))
						gen_push(i, mailbox[mailbox125[i] + LIGHT_PAWN_AHEAD_LEFT], 3); //3 means 11 in bynary, so pawn + capture move
					if ((mailbox[mailbox125[i] + LIGHT_PAWN_AHEAD_RIGHT] != -1)&&(color[mailbox[mailbox125[i] + LIGHT_PAWN_AHEAD_RIGHT]]==DARK))
						gen_push(i, mailbox[mailbox125[i] + LIGHT_PAWN_AHEAD_RIGHT], 3);
					if ((mailbox[mailbox125[i] + LIGHT_PAWN_UP_LEFT] != -1)&&(color[mailbox[mailbox125[i] + LIGHT_PAWN_UP_LEFT]]==DARK))
						gen_push(i, mailbox[mailbox125[i] + LIGHT_PAWN_UP_LEFT], 3);
					if ((mailbox[mailbox125[i] + LIGHT_PAWN_UP_RIGHT] != -1)&&(color[mailbox[mailbox125[i] + LIGHT_PAWN_UP_RIGHT]]==DARK))
						gen_push(i, mailbox[mailbox125[i] + LIGHT_PAWN_UP_RIGHT], 3);
                }
                else //if color is DARK
                {
                     if ((mailbox[mailbox125[i] + DARK_PAWN_BACK_LEFT] != -1)&&(color[mailbox[mailbox125[i] + DARK_PAWN_BACK_LEFT]]==LIGHT))
						gen_push(i, i + DARK_PAWN_BACK_LEFT, 3);
					if ((mailbox[mailbox125[i] + DARK_PAWN_BACK_RIGHT] != -1)&&(color[mailbox[mailbox125[i] + DARK_PAWN_BACK_RIGHT]]==LIGHT))
						gen_push(i, i + DARK_PAWN_BACK_RIGHT, 3);
					if ((mailbox[mailbox125[i] + DARK_PAWN_DOWN_LEFT] != -1)&&(color[mailbox[mailbox125[i] + DARK_PAWN_DOWN_LEFT]]==LIGHT))
						gen_push(i, i + DARK_PAWN_DOWN_LEFT, 3);
					if ((mailbox[mailbox125[i] + DARK_PAWN_DOWN_RIGHT] != -1)&&(color[mailbox[mailbox125[i] + DARK_PAWN_DOWN_RIGHT]]==LIGHT))
						gen_push(i, i + DARK_PAWN_DOWN_RIGHT, 3);
			    }
            }
			else //if it's not a pawn
            {
				for (j = 0; j < offsets[piece[i]]; ++j)
                {
					for (n = i;;) 
                    {
						n = mailbox[mailbox125[n] + offset[piece[i]][j]];
						if (n == -1)
							break;
						if (color[n] != EMPTY) 
                        {
							if (color[n] == sideNotToMove) //it's a capture move!
								gen_push(i, n, 1);
							break;
						}
						if (!slide[piece[i]])
							break;
                    }
                }
            }
        }
    }
}


