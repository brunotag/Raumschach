#include "defs.h"

int quiesceCount = 0;
int searchCount = 0;

int sideToMove;  /* the side to move */
int sideNotToMove;  /* the side not to move */

int mail_box[729]={                      //2 + 5 + 2 Squares
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,

    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,

    -1, -1, -1, -1, -1, -1, -1, -1, -1,  
    -1, -1, -1, -1, -1, -1, -1, -1, -1,    
    -1, -1,  0,  1,  2,  3,  4, -1, -1,
    -1, -1,  5,  6,  7,  8,  9, -1, -1,
    -1, -1, 10, 11, 12, 13, 14, -1, -1,
    -1, -1, 15, 16, 17, 18, 19, -1, -1,
    -1, -1, 20, 21, 22, 23, 24, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,

    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,    
    -1, -1, 25, 26, 27, 28, 29, -1, -1,
    -1, -1, 30, 31, 32, 33, 34, -1, -1,
    -1, -1, 35, 36, 37, 38, 39, -1, -1,
    -1, -1, 40, 41, 42, 43, 44, -1, -1,
    -1, -1, 45, 46, 47, 48, 49, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,

    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,    
    -1, -1, 50, 51, 52, 53, 54, -1, -1,
    -1, -1, 55, 56, 57, 58, 59, -1, -1,
    -1, -1, 60, 61, 62, 63, 64, -1, -1,
    -1, -1, 65, 66, 67, 68, 69, -1, -1,
    -1, -1, 70, 71, 72, 73, 74, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,

    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,    
    -1, -1, 75, 76, 77, 78, 79, -1, -1,
    -1, -1, 80, 81, 82, 83, 84, -1, -1,
    -1, -1, 85, 86, 87, 88, 89, -1, -1,
    -1, -1, 90, 91, 92, 93, 94, -1, -1,
    -1, -1, 95, 96, 97, 98, 99, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,

    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,    
    -1, -1,100,101,102,103,104, -1, -1,
    -1, -1,105,106,107,108,109, -1, -1,
    -1, -1,110,111,112,113,114, -1, -1,
    -1, -1,115,116,117,118,119, -1, -1,
    -1, -1,120,121,122,123,124, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,

    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,

    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1
};


//prende soltanto le celle dove non c'è il -1 (cioè le celle che sono nei bounds della scacchiera)
int addresser[125] =
{
    182, 183, 184, 185, 186, 
    191, 192, 193, 194, 195, 
    200, 201, 202, 203, 204, 
    209, 210, 211, 212, 213, 
    218, 219, 220, 221, 222, 
    
    263, 264, 265, 266, 267, 
    272, 273, 274, 275, 276, 
    281, 282, 283, 284, 285, 
    290, 291, 292, 293, 294, 
    299, 300, 301, 302, 303, 
    
    344, 345, 346, 347, 348, 
    353, 354, 355, 356, 357, 
    362, 363, 364, 365, 366, 
    371, 372, 373, 374, 375, 
    380, 381, 382, 383, 384, 
    
    425, 426, 427, 428, 429, 
    434, 435, 436, 437, 438, 
    443, 444, 445, 446, 447, 
    452, 453, 454, 455, 456, 
    461, 462, 463, 464, 465, 
    
    506, 507, 508, 509, 510, 
    515, 516, 517, 518, 519, 
    524, 525, 526, 527, 528, 
    533, 534, 535, 536, 537, 
    542, 543, 544, 545, 546 
};


/* slide, off_sets, and off_set are basically the vectors that
   pieces can moveUni in. If slide for the pieces is FALSE, it can
   only moveUni one square in any one direction. off_sets is the
   number of directions it can moveUni in, and off_set is an array
   of the actual directions. */

//pawn, knight, unicorn, bishop, rook, queen, king
BOOLEANVAL slide[7] = {
	FALSE, FALSE, TRUE, TRUE, TRUE, TRUE, FALSE
};

char pieceInitial[8] = {
    'p', 'N', 'U', 'B', 'R', 'Q', 'K', ' '
};

int off_sets[7] = {
	0, 24, 8, 12, 6, 26, 26
};

int off_set[7][26] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0 },             //PAWN
	
    //KNIGHT
    {
        -171, -163, -161, -153,                     // 2 piani sopra
         -99,  -83,  -79,  -63,                     // piano sopra
        - 19,  -17,  -11,   -7,  7, 11, 17, 19,     // stesso piano                
          63,   79,   83,   99,                     // piano sotto
         153,  161,  163,  171       
    },   

    //UNICORN
    { 
         -91,  -89,  -73,  -71,     //piano sopra
          71,   73,   89,   91      //piano sotto 
    },   

    //BISHOP
	{ 
         -90,  -82,  -80,  -72,     //piano sopra
         -10,   -8,    8,   10,     //stesso piano
          72,   80,   82,   90      //piano sotto
    },      

    //ROOK
	{ -81, -9, -1, 1, 9, 81},         

    //QUEEN
	{         
        -91, -90, -89, -82, -81, -80, -73, -72, -71,  //piano sopra
        -10,  -9,  -8,  -1,   8,   1,   9,  10,       //stesso piano
         71,  72,  73,  80,  81,  82,  89,  90,  91   //piano sotto
    },  

    //KING
    {         
        -91,-90, -89,-82, -81, -80, -73, -72, -71,  //piano sopra
        -10, -9,  -8, -1,   8,   1,   9,  10,       //stesso piano
         71, 72,  73, 80,  81,  82,  89,  90, 91    //piano sotto
    }      
};

/* WHITE, BLACK, or EMPTY */
int colors[125] = { 
    BLACK, BLACK, BLACK, BLACK, BLACK,       //Ea5 Eb5 Ec5 Ed5 Ee5
    BLACK, BLACK, BLACK, BLACK, BLACK,       //Ea4 Eb4 Ec4 Ed4 Ee4
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,

    BLACK, BLACK, BLACK, BLACK, BLACK,       //Da5 Db5
    BLACK, BLACK, BLACK, BLACK, BLACK,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,

    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  //C
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,

    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  //B
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    WHITE, WHITE, WHITE, WHITE, WHITE, 
    WHITE, WHITE, WHITE, WHITE, WHITE, 

    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  //A
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    WHITE, WHITE, WHITE, WHITE, WHITE,  //Aa2 Ab2 Ac2 Ad2 Ae2
    WHITE, WHITE, WHITE, WHITE, WHITE   //Aa1 Ab1 Ac1 Ad1 Ae1
};

/* PAWN, KNIGHT, UNICORN, BISHOP, ROOK, QUEEN, KING, or EMPTY */
int pieces[125] = {
    ROOK   , KNIGHT , KING   , KNIGHT , ROOK   , 
    PAWN   , PAWN   , PAWN   , PAWN   , PAWN   , 
    EMPTY  , EMPTY  , EMPTY  , EMPTY  , EMPTY  , 
    EMPTY  , EMPTY  , EMPTY  , EMPTY  , EMPTY  , 
    EMPTY  , EMPTY  , EMPTY  , EMPTY  , EMPTY  , 

    BISHOP , UNICORN, QUEEN  , BISHOP , UNICORN,
    PAWN   , PAWN   , PAWN   , PAWN   , PAWN   , 
    EMPTY  , EMPTY  , EMPTY  , EMPTY  , EMPTY  ,
    EMPTY  , EMPTY  , EMPTY  , EMPTY  , EMPTY  ,
    EMPTY  , EMPTY  , EMPTY  , EMPTY  , EMPTY  ,

    EMPTY  , EMPTY  , EMPTY  , EMPTY  , EMPTY  ,
    EMPTY  , EMPTY  , EMPTY  , EMPTY  , EMPTY  ,
    EMPTY  , EMPTY  , EMPTY  , EMPTY  , EMPTY  ,
    EMPTY  , EMPTY  , EMPTY  , EMPTY  , EMPTY  ,
    EMPTY  , EMPTY  , EMPTY  , EMPTY  , EMPTY  ,
    
    EMPTY  , EMPTY  , EMPTY  , EMPTY  , EMPTY  ,
    EMPTY  , EMPTY  , EMPTY  , EMPTY  , EMPTY  ,
    EMPTY  , EMPTY  , EMPTY  , EMPTY  , EMPTY  ,
    PAWN   , PAWN   , PAWN   , PAWN   , PAWN   ,
    BISHOP , UNICORN, QUEEN  , BISHOP , UNICORN,

    EMPTY  , EMPTY  , EMPTY  , EMPTY  , EMPTY  ,
    EMPTY  , EMPTY  , EMPTY  , EMPTY  , EMPTY  ,
    EMPTY  , EMPTY  , EMPTY  , EMPTY  , EMPTY  ,
    PAWN   , PAWN   , PAWN   , PAWN   , PAWN   ,
    ROOK   , KNIGHT , KING   , KNIGHT , ROOK   
};

int plyCurrent;  /* the number of half-moves (plyCurrent) since the
             root of the search tree */
int histPlyCurrent;  /* h for history; the number of plyCurrent since the beginning
              of the game */
//int hash;  /* a (more or less) unique number that corresponds to the
//              position */

/* generData is some memory for move lists that are created by the move
   generators. The move list for plyCurrent n starts at firstMove[n] and ends
   at firstMove[n + 1]. */
gener_t generData[GENE_STACK];

int firstMove[MAXIMUM_PLY];

/* a "triangular" PV array; */
moveUni pv[MAXIMUM_PLY][MAXIMUM_PLY];
int pvLength[MAXIMUM_PLY];
BOOLEANVAL followPV;

/* the engine will search for maxTime milliseconds or until it finishes
   searching maxDepth plyCurrent. */
int maxTime[2];
int maxDepth[2];

/* the time when the engine starts searching, and when it should stop */
int startTime;
int stopTime;

int fiftMoves;  /* the number of moves since a capture or pawn moveUni, used
               to handle the fiftMoves-moveUni-draw rule */

int nodes;  /* the number of nodes we've searched */

/* the history heuristic array (used for move ordering) */
int history[125][125];

/* we need an array of history_t's so we can take back the
   moves we make */
history_t historyData[HISTORY_STACK];

/* random numbers used to compute hash; see setHash() in board.c */
//int hashPiece[2][7][125];  /* indexed by pieces [colors][type][square] */
//int hashSide;
//int hashEP[125];

long movesDone;
long trueMovesDone;

int score;

