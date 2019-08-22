#include "defs.h"
#include "protos.h"

/* get_ms() returns the milliseconds elapsed since midnight,
   January 1, 1970. */

#include <sys/timeb.h>
BOOLEANVAL ftime_ok = FALSE;  /* does ftime return milliseconds? */
#include <stdio.h>


int get_ms()
{
	struct timeb timebuffer;
	ftime(&timebuffer);
	if (timebuffer.millitm != 0)
		ftime_ok = TRUE;
	return (timebuffer.time * 1000) + timebuffer.millitm;
}

void print_square(int number)
{
    printf("%c",pieceInitial[pieces[number]]);
    number = 124-number;   
    printf("%c%c%d",
                    'A'+ (int)number/25,
                    'a'+ 4 - number % 5,
                    (int)(number % 25) / 5 + 1
                    );
}

/* print_move_str prints a string with moveUni m in coordinate notation */

void print_move_str(moveBytes m)
{
	static char str[6];

    char x = ' ';
    char p = ' ';

    //if capture
    if (m.bits & 1) x='x';
    
    //if promote
	if (m.bits & 4) p = pieceInitial[pieces[m.promote]];     
	
    print_square(m.from);
    printf("%c",x);
    print_square(m.to);
    printf("%c",p);

}



void print_result()
{
	int i;

	/* is there a legal move? */
	for (i = 0; i < firstMove[1]; ++i)
		if (doMove(generData[i].m.b)) {
			take_back();
			break;
		}
	if (i == firstMove[1]) {
		if (in_check(sideToMove)) {
			if (sideToMove == WHITE)
				printf("0-1 {Black mates}\n");
			else
				printf("1-0 {White mates}\n");
		}
		else
			printf("1/2-1/2 {Stalemate}\n");
	}
	//else if (reps() == 3)
	//	printf("1/2-1/2 {Draw by repetition}\n");
    else if (fiftMoves >= 100)
		printf("1/2-1/2 {Draw by fiftMoves move rule}\n");
}
//
//    
//int main(int argc, char* argv[])
//{       
//    //int i;
//    //int j;
//    //int k;
//    //int conta;
//    int computer_side;
//
//
//    //gener_t *g;
//
//    initHash();
//    initBoard();
//    generate();
//    
//    computer_side = WHITE;
//
///* //test doMove snippet
//    for(i=0;i<5;i++)
//    {
//        for(j=0;j<5;j++)
//        {
//            for(k=0;k<5;k++)
//            {
//                int numero = 25*i+5*j+k;
//                print_square(numero);
//            }
//            printf("\n");
//        }
//        printf("\n\n");
//    }
//    getchar();
//
//    g = (gener_t *) malloc( sizeof(gener_t) );
//    g->m.b.from =  92;
//	g->m.b.to =  67;
//	g->m.b.promote = 0;  //if we are here than it's not a promotion
//	g->m.b.bits =  0;
//
//    doMove(g->m.b);
//
//        for(i=0;i<5;i++)
//    {
//        for(j=0;j<5;j++)
//        {
//            for(k=0;k<5;k++)
//            {
//                int numero = 25*i+5*j+k;
//                print_square(numero);
//            }
//            printf("\n");
//        }
//        printf("\n\n");
//    }
//    getchar();
//    */
//
//    maxTime = 1 << 25;
//
//            //stampo la posizione e il punteggio relativo
//
//    /*
//    for(i=0;i<5;i++)
//    {
//        for(j=0;j<5;j++)
//        {
//            for(k=0;k<5;k++)
//            {
//                int numero = 25*i+5*j+k;
//                print_square(numero);
//            }
//            printf("\n");
//        }
//        printf("\n\n");
//    }
//    printf("Side to moveUni: %d\n", sideToMove);
//    printf("Score: %d\n", evaluate());
//    printf("moveUni done:");
//    print_move_str(generData[i].m.b);
//    getchar();
//    */
//    //fine stampo la posizione e il punteggio relativo
//
//    maxDepth = 2;
//
//
//	for (;;) {
//		if (sideToMove == computer_side) {  // computer's turn 
//			
//			// think about the moveUni and make it 
//			think(1);
//			if (!pv[0][0].u) {
//				printf("(no legal moves)\n");
//                printf("%d",pv[0][0].u);
//				computer_side = EMPTY;
//				continue;
//			}          
//            
//            printf("Computer's moveUni: ");
//            print_move_str(pv[0][0].b);
//			printf("\n");
//            
//            doMove(pv[0][0].b);
//			plyCurrent = 0;
//			generate();
//			print_result();
//
//            if (computer_side == WHITE){
//                computer_side = BLACK;
//                maxDepth = 2;
//            }
//            else 
//            {
//                computer_side = WHITE;
//                maxDepth = 2;
//            }
//            getchar();
//        }           
//        
//    }
//}
//
//
// /*
//            printf("quiesceCount: %d, searchCount: %d\n",quiesceCount,searchCount);
//            quiesceCount = 0;
//            searchCount = 0;
//
//
//			printf("Computer's moveUni: ");
//            print_move_str(pv[0][0].b);
//            printf("\n");
//
//			doMove(pv[0][0].b);
//			plyCurrent = 0;
//			generate();
//			print_result();
//
//            if (computer_side == WHITE)
//                computer_side = BLACK;
//            else computer_side = WHITE;
//            getchar();
//			continue;*/
//
//
///*for (j = 0; j<2*maxDepth; j++){
//                for (i = firstMove[j]; i < firstMove[j + 1]; ++i) {
//                    print_move_str(generData[i].m.b);
//                    printf(": %d",generData[i].score);
//                    printf("\n");
//                }
//            }*/