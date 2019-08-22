#include "defs.h"
#include "protos.h"

/* see the beginning of think() */
#include <setjmp.h>
jmp_buf env;
BOOL stop_search;

void sort(int from);
int search(int alpha, int beta, int depth);
void sort_pv();
void think(int output);
int quiesce(int alpha,int beta);
int reps();
void checkup();


/* checkup() is called once in a while during the search. */

void checkup()
{
	/* is the engine's time up? if so, longjmp back to the
	   beginning of think() */
	if (get_ms() >= stop_time) {
		stop_search = TRUE;
		longjmp(env, 0);
	}
}
// think() calls search() iteratively.

void think()
{
	int i, j, x;

	/* some code that lets us longjmp back here and return
	   from think() when our time is up */
	stop_search = FALSE;
	setjmp(env);
	if (stop_search) {
		
		/* make sure to take back the line we were searching */
		while (ply)
			takeback();
		return;
	}

	start_time = get_ms();
	stop_time = start_time + max_time;

	ply = 0;
	nodes = 0;

	memset(pv, 0, sizeof(pv));
	memset(history, 0, sizeof(history));
    printf("ply      nodes  score  pv\n");
	for (i = 1; i <= max_depth; ++i) {
		follow_pv = TRUE;
		x = search(-10000, 10000, i);
        printf("%3d  %9d  %5d ", i, nodes, x);
        for (j = 0; j < pv_length[0]; ++j){
		    print_move_str(pv[0][j].b);
            printf(" ");            
        }
       // getchar();
	    printf("\n");
//		fflush(stdout);
        if (x > 9000 || x < -9000)
			break;
	}
}


/* quiesce() is a recursive minimax search function with
   alpha-beta cutoffs. In other words, negamax. It basically
   only searches capture sequences and allows the evaluation
   function to cut the search off (and set alpha). The idea
   is to find a position where there isn't a lot going on
   so the static evaluation function will work. */

int quiesce(int alpha,int beta,int depth)
{
	int i, j, x;

    //printf("Starting quiesce alpha = %d, beta = %d, ply = %d, hply = %d\n",alpha, beta, ply, hply);

    quiesceCount++;

	++nodes;

    if (depth == 0)
		return eval();

    	/* do some housekeeping every 1024 nodes */
	if ((nodes & 1023) == 0)
		checkup();

	pv_length[ply] = ply;

	/* are we too deep? */
	if (ply >= MAX_PLY - 1)
		return eval();
	if (hply >= HIST_STACK - 1)
		return eval();

	/* check with the evaluation function */
	x = eval();
	if (x >= beta)
		return beta;
	if (x > alpha)
		alpha = x;

	gen_caps();
	if (follow_pv)  /* are we following the PV? */
		sort_pv();

	/* loop through the moves */
	for (i = first_move[ply]; i < first_move[ply + 1]; ++i) {
		sort(i);
		if (!makemove(gen_dat[i].m.b))
			continue;
/*

            //stampo la posizione e il punteggio relativo
    for(i=0;i<5;i++)
    {
        for(j=0;j<5;j++)
        {
            for(k=0;k<5;k++)
            {
                int numero = 25*i+5*j+k;
                print_square(numero);
            }
            printf("\n");
        }
        printf("\n\n");
    }
    printf("Side to move: %d\n", sideToMove);
    printf("Score: %d\n", eval());
    printf("Move done:");
    print_move_str(gen_dat[i].m.b);
    getchar();
*/

    //fine stampo la posizione e il punteggio relativo
		x = -quiesce(-beta, -alpha, depth-1);
		takeback();
		if (x > alpha) {
			if (x >= beta)
				return beta;
			alpha = x;

			/* update the PV */
			pv[ply][ply] = gen_dat[i].m;
			for (j = ply + 1; j < pv_length[ply + 1]; ++j)
				pv[ply][j] = pv[ply + 1][j];
			pv_length[ply] = pv_length[ply + 1];
		}
	}
	return alpha;
}



/* search() does just that, in negamax fashion */

int search(int alpha, int beta, int depth)
{

	int i, j, x;

	BOOL c, f;

       //printf("Starting search alpha = %d, beta = %d, depth = %d, ply = %d, hply = %d\n",alpha, beta, depth, ply, hply);

       searchCount++;

	/* we're as deep as we want to be; call quiesce() to get
	   a reasonable score and return it. */


	if (depth == 0)
		return quiesce(alpha,beta,2);
	++nodes;

	/* do some housekeeping every 1024 nodes */
	if ((nodes & 1023) == 0)
		checkup();

	pv_length[ply] = ply;

    	/* if this isn't the root of the search tree (where we have
	   to pick a move and can't simply return 0) then check to
	   see if the position is a repeat. if so, we can assume that
	   this line is a draw and return 0. */
	if (ply && reps())
		return 0;

	/* are we too deep? */
	if (ply >= MAX_PLY - 1)
		return eval();
	if (hply >= HIST_STACK - 1)
		return eval();

	/* are we in check? if so, we want to search deeper */
	c = in_check(sideToMove);
	if (c)
		++depth;
	gen();
	if (follow_pv)  /* are we following the PV? */
		sort_pv();
	f = FALSE;

	/* loop through the moves */

    /*printf("Going to search in %d moves",first_move[ply + 1]-first_move[ply]);
    getchar();*/

	for (i = first_move[ply]; i < first_move[ply + 1]; ++i) {
		sort(i);
		if (!makemove(gen_dat[i].m.b))
			continue;

        /*
                    //stampo la posizione e il punteggio relativo
    for(i=0;i<5;i++)
    {
        for(j=0;j<5;j++)
        {
            for(k=0;k<5;k++)
            {
                int numero = 25*i+5*j+k;
                print_square(numero);
            }
            printf("\n");
        }
        printf("\n\n");
    }
    printf("Side to move: %d\n", sideToMove);
    printf("Score: %d\n", eval());
    printf("Move done:");
    print_move_str(gen_dat[i].m.b);
    getchar();
    //fine stampo la posizione e il punteggio relativo

    */
		f = TRUE;
		x = -search(-beta, -alpha, depth - 1);
		takeback();
		if (x > alpha) {

			/* this move caused a cutoff, so increase the history
			   value so it gets ordered high next time we can
			   search it */
			history[(int)gen_dat[i].m.b.from][(int)gen_dat[i].m.b.to] += depth;
			if (x >= beta)
				return beta;
			alpha = x;

			/* update the PV */
			pv[ply][ply] = gen_dat[i].m;
			for (j = ply + 1; j < pv_length[ply + 1]; ++j)
				pv[ply][j] = pv[ply + 1][j];
			pv_length[ply] = pv_length[ply + 1];
		}
	}

	/* no legal moves? then we're in checkmate or stalemate */
	if (!f) {
		if (c)
			return -10000 + ply;
		else
			return 0;
	}


	/* fifty move draw rule */
	if (fifty >= 100)
		return 0;
	return alpha;
}

/* sort() searches the current ply's move list from 'from'
   to the end to find the move with the highest score. Then it
   swaps that move and the 'from' move so the move with the
   highest score gets searched next, and hopefully produces
   a cutoff. */

void sort(int from)
{
	int i;
	int bs;  /* best score */
	int bi;  /* best i */
	gen_t g;

	bs = -1;
	bi = from;
	for (i = from; i < first_move[ply + 1]; ++i)
		if (gen_dat[i].score > bs) {
			bs = gen_dat[i].score;
			bi = i;
		}
	g = gen_dat[from];
	gen_dat[from] = gen_dat[bi];
	gen_dat[bi] = g;
}

/* sort_pv() is called when the search function is following
   the PV (Principal Variation). It looks through the current
   ply's move list to see if the PV move is there. If so,
   it adds 10,000,000 to the move's score so it's played first
   by the search function. If not, follow_pv remains FALSE and
   search() stops calling sort_pv(). */

void sort_pv()
{
	int i;

	follow_pv = FALSE;
	for(i = first_move[ply]; i < first_move[ply + 1]; ++i)
		if (gen_dat[i].m.u == pv[0][ply].u) {
			follow_pv = TRUE;
			gen_dat[i].score += 10000000;
			return;
		}
}

/* reps() returns the number of times the current position
   has been repeated. It compares the current value of hash
   to previous values. */

int reps()
{
	int i;
	int r = 0;

	for (i = hply - fifty; i < hply; ++i)
		if (hist_dat[i].hash == hash)
			++r;
	return r;
}


