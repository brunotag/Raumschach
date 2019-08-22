#include "defs.h"
#include "protos.h"
#include <stdio.h>
#include <cstring>


/* see the beginning of think() */
#include <setjmp.h>
jmp_buf env;
BOOLEANVAL stop_search;

void sort(int from);
int search(int alpha, int beta, int depth);
void sort_pv();
void think(int output);
int quiesce(int alpha,int beta);
//int reps();
void checkup();


/* checkup() is called once in a while during the search. */

void checkup()
{
	/* is the engine's time up? if so, longjmp back to the
	   beginning of think() */
	if (get_ms() >= stopTime) {
		stop_search = TRUE;
		longjmp(env, 0);
	}
}
// think() calls search() iteratively.

void think(BOOLEANVAL print)
{
	int i, j, x;

    computerIsThinking = TRUE;

	/* some code that lets us longjmp back here and return
	   from think() when our time is up */
	stop_search = FALSE;
	setjmp(env);
	if (stop_search) {
		
		/* make sure to take back the line we were searching */
		while (plyCurrent)
			take_back();
        computerIsThinking = FALSE;
		return;
	}

	startTime = get_ms();
    stopTime = startTime + maxTime[sideToMove];

	plyCurrent = 0;
	nodes = 0;

	memset(pv, 0, sizeof(pv));
	memset(history, 0, sizeof(history));
    if(print) printf("plyCurrent      nodes  score  pv\n");
    for (i = 1; i <= maxDepth[sideToMove]; ++i) {
		followPV = TRUE;
		x = search(-10000, 10000, i);
        if(print){
            printf("%3d  %9d  %5d ", i, nodes, x);
            for (j = 0; j < pvLength[0]; ++j){
		        print_move_str(pv[0][j].b);
                printf(" ");            
            }
            printf("\n");
        }
       // getchar();	    
//		fflush(stdout);
        if (x > 9000 || x < -9000)
			break;
	}

    score = x;
    computerIsThinking = FALSE;
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

    quiesceCount++;

	++nodes;

    if (depth == 0)
		return evaluate();

    	/* do some housekeeping every 1024 nodes */
	if ((nodes & 1023) == 0)
		checkup();

	pvLength[plyCurrent] = plyCurrent;

	/* are we too deep? */
	if (plyCurrent >= MAXIMUM_PLY - 1)
		return evaluate();
	if (histPlyCurrent >= HISTORY_STACK - 1)
		return evaluate();

	/* check with the evaluation function */
	x = evaluate();
	if (x >= beta)
		return beta;
	if (x > alpha)
		alpha = x;

	generateCaptures();
	if (followPV)  /* are we following the PV? */
		sort_pv();

	/* loop through the moves */
	for (i = firstMove[plyCurrent]; i < firstMove[plyCurrent + 1]; ++i) {

		sort(i);
		if (!doMove(generData[i].m.b))
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
    printf("Side to moveUni: %d\n", sideToMove);
    printf("Score: %d\n", evaluate());
    printf("moveUni done:");
    print_move_str(generData[i].m.b);
    getchar();
*/

    //fine stampo la posizione e il punteggio relativo
		x = -quiesce(-beta, -alpha, depth-1);
		take_back();
		if (x > alpha) {
			if (x >= beta)
				return beta;
			alpha = x;

			/* update the PV */
			pv[plyCurrent][plyCurrent] = generData[i].m;
			for (j = plyCurrent + 1; j < pvLength[plyCurrent + 1]; ++j)
				pv[plyCurrent][j] = pv[plyCurrent + 1][j];
			pvLength[plyCurrent] = pvLength[plyCurrent + 1];
		}
	}
	return alpha;
}



/* search() does just that, in negamax fashion */

int search(int alpha, int beta, int depth)
{
	int i, j, x;
	BOOLEANVAL c, f;

       //printf("Starting search alpha = %d, beta = %d, depth = %d, plyCurrent = %d, histPlyCurrent = %d\n",alpha, beta, depth, plyCurrent, histPlyCurrent);
     searchCount++;

	/* we're as deep as we want to be; call quiesce() to get
	   a reasonable score and return it. */
	if (depth == 0)
		//return quiesce(alpha,beta,2);
        return evaluate();
	++nodes;

	/* do some housekeeping every 1024 nodes */
	if ((nodes & 1023) == 0)
		checkup();

	pvLength[plyCurrent] = plyCurrent;

    	/* if this isn't the root of the search tree (where we have
	   to pick a moveUni and can't simply return 0) then check to
	   see if the position is a repeat. if so, we can assume that
	   this line is a draw and return 0. */
	//if (plyCurrent && reps())
	//	return 0;

	/* are we too deep? */
	if (plyCurrent >= MAXIMUM_PLY - 1)
		return evaluate();
	if (histPlyCurrent >= HISTORY_STACK - 1)
		return evaluate();

	/* are we in check? if so, we want to search deeper */
	c = in_check(sideToMove);
	if (c)
		++depth;
	generate();
	if (followPV)  /* are we following the PV? */
		sort_pv();
	f = FALSE;

	/* loop through the moves */

    /*printf("Going to search in %d moves",firstMove[plyCurrent + 1]-firstMove[plyCurrent]);
    getchar();*/

	for (i = firstMove[plyCurrent]; i < firstMove[plyCurrent + 1]; ++i) {
		sort(i);
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
    printf("Side to moveUni: %d\n", sideToMove);
    printf("Score: %d\n", evaluate());
    printf("moveUni done:");
    print_move_str(generData[i].m.b);
    getchar();
    //fine stampo la posizione e il punteggio relativo

    */
        if (doMove(generData[i].m.b)){
		    f = TRUE;
		    x = -search(-beta, -alpha, depth - 1);
		    take_back();
		    if (x > alpha) {

			/* this move caused a cutoff, so increase the history
			   value so it gets ordered high next time we can
			   search it */
			    history[(int)generData[i].m.b.from][(int)generData[i].m.b.to] += depth;
			    if (x >= beta)
    				return beta;
			    alpha = x;

			/* update the PV */
			    pv[plyCurrent][plyCurrent] = generData[i].m;
			    for (j = plyCurrent + 1; j < pvLength[plyCurrent + 1]; ++j)
				    pv[plyCurrent][j] = pv[plyCurrent + 1][j];
			    pvLength[plyCurrent] = pvLength[plyCurrent + 1];
		    }
	    }
    }

	/* no legal moves? then we're in checkmate or stalemate */
	if (!f) {
		if (c)
			return -10000 + plyCurrent;
		else
			return 0;
	}


	/* fiftMoves moveUni draw rule */
	if (fiftMoves >= 100)
		return 0;
	return alpha;
}

/* sort() searches the current plyCurrent's move list from 'from'
   to the end to find the move with the highest score. Then it
   swaps that move and the 'from' move so the move with the
   highest score gets searched next, and hopefully produces
   a cutoff. */

void sort(int from)
{
	int i;
	int bs;  /* best score */
	int bi;  /* best i */
	gener_t g;

	bs = -1;
	bi = from;

	for (i = from; i < firstMove[plyCurrent + 1]; ++i)
		if (generData[i].score > bs) {
			bs = generData[i].score;
			bi = i;
		}
	g = generData[from];
	generData[from] = generData[bi];
	generData[bi] = g;
}

/* sort_pv() is called when the search function is following
   the PV (Principal Variation). It looks through the 
   plyCurrent's move list to see if the PV move is there. If so,
   it adds 10,000,000 to the moveUni's score so it's played first
   by the search function. If not, followPV remains FALSE and
   search() stops calling sort_pv(). */

void sort_pv()
{
	int i;

	followPV = FALSE;
	for(i = firstMove[plyCurrent]; i < firstMove[plyCurrent + 1]; ++i)
		if (generData[i].m.u == pv[0][plyCurrent].u) {
			followPV = TRUE;
			generData[i].score += 10000000;
			return;
		}
}

///* reps() returns the number of times the current position
//   has been repeated. It compares the current value of hash
//   to previous values. */
//
//int reps()
//{
//	int i;
//	int r = 0;
//
//	for (i = histPlyCurrent - fiftMoves; i < histPlyCurrent; ++i)
//		if (historyData[i].hash == hash)
//			++r;
//	return r;
//}


