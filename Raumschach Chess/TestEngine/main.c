#include "defs.h"
#include "protos.h"

/* get_ms() returns the milliseconds elapsed since midnight,
   January 1, 1970. */

#include <sys/timeb.h>
BOOL ftime_ok = FALSE;  /* does ftime return milliseconds? */



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
    printf("%c",piece_initial[piece[number]]);
    number = 124-number;   
    printf("%c%c%d",
                    'A'+ (int)number/25,
                    'a'+ 4 - number % 5,
                    (int)(number % 25) / 5 + 1
                    );
}



/* print_move_str prints a string with move m in coordinate notation */

void print_move_str(move_bytes m)
{
	static char str[6];

    char x = ' ';
    char p = ' ';

    //if capture
    if (m.bits & 1) x='x';
    
    //if promote
	if (m.bits & 4) p = piece_initial[piece[m.promote]];     
	
    print_square(m.from);
    printf("%c",x);
    print_square(m.to);
    printf("%c",p);

}





void print_result()
{
	int i;

	/* is there a legal move? */
	for (i = 0; i < first_move[1]; ++i)
		if (makemove(gen_dat[i].m.b)) {
			takeback();
			break;
		}
	if (i == first_move[1]) {
		if (in_check(sideToMove)) {
			if (sideToMove == LIGHT)
				printf("0-1 {Black mates}\n");
			else
				printf("1-0 {White mates}\n");
		}
		else
			printf("1/2-1/2 {Stalemate}\n");
	}
	else if (reps() == 3)
		printf("1/2-1/2 {Draw by repetition}\n");
    else if (fifty >= 100)
		printf("1/2-1/2 {Draw by fifty move rule}\n");
}

    
int main(int argc, char* argv[])
{       
    int i;
    int j;
    int k;
    int conta;
    int computer_side;


    gen_t *g;

    init_hash();
    init_board();
    gen();
    
    computer_side = LIGHT;

/* //test makemove snippet
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
    getchar();

    g = (gen_t *) malloc( sizeof(gen_t) );
    g->m.b.from = (char)92;
	g->m.b.to = (char)67;
	g->m.b.promote = 0;  //if we are here than it's not a promotion
	g->m.b.bits = (char)0;

    makemove(g->m.b);

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
    getchar();
    */

    max_time = 1 << 25;

            //stampo la posizione e il punteggio relativo

    /*
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

    max_depth = 2;


	for (;;) {
		if (sideToMove == computer_side) {  // computer's turn 
			
			// think about the move and make it 
			think(1);
			if (!pv[0][0].u) {
				printf("(no legal moves)\n");
                printf("%d",pv[0][0].u);
				computer_side = EMPTY;
				continue;
			}          
            
            printf("Computer's move: ");
            print_move_str(pv[0][0].b);
			        printf("\n");
            
            makemove(pv[0][0].b);
			ply = 0;
			gen();
			print_result();

            if (computer_side == LIGHT){
                computer_side = DARK;
                max_depth = 2;
            }
            else 
            {
                computer_side = LIGHT;
                max_depth = 2;
            }
            getchar();
        }           
        
    }
}


 /*
            printf("quiesceCount: %d, searchCount: %d\n",quiesceCount,searchCount);
            quiesceCount = 0;
            searchCount = 0;


			printf("Computer's move: ");
            print_move_str(pv[0][0].b);
            printf("\n");

			makemove(pv[0][0].b);
			ply = 0;
			gen();
			print_result();

            if (computer_side == LIGHT)
                computer_side = DARK;
            else computer_side = LIGHT;
            getchar();
			continue;*/


/*for (j = 0; j<2*max_depth; j++){
                for (i = first_move[j]; i < first_move[j + 1]; ++i) {
                    print_move_str(gen_dat[i].m.b);
                    printf(": %d",gen_dat[i].score);
                    printf("\n");
                }
            }*/