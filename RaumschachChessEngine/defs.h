#define BOOLEANVAL	int
#define TRUE			1
#define FALSE			0

#define GENE_STACK		10000
#define MAXIMUM_PLY			16
#define HISTORY_STACK		10000

#define WHITE			0
#define BLACK			1

#define PAWN			0
#define KNIGHT			1
#define UNICORN         2 
#define BISHOP			3
#define ROOK			4
#define QUEEN			5
#define KING			6

#define EMPTY           7

#define BOARD_X_DIMENSION 5 //rows
#define BOARD_Y_DIMENSION 5 //planes
#define BOARD_Z_DIMENSION 5 //columns

#define MAILBOX_WIDTH 9

#define WHITE_PAWN_AHEAD_LEFT       -MAILBOX_WIDTH-1 
#define WHITE_PAWN_AHEAD_RIGHT      -MAILBOX_WIDTH+1 
#define WHITE_PAWN_UP_LEFT  -(MAILBOX_WIDTH * MAILBOX_WIDTH )-1 
#define WHITE_PAWN_UP_RIGHT -(MAILBOX_WIDTH * MAILBOX_WIDTH )+1 

#define BLACK_PAWN_BACK_LEFT         MAILBOX_WIDTH-1 
#define BLACK_PAWN_BACK_RIGHT        MAILBOX_WIDTH+1 
#define BLACK_PAWN_DOWN_LEFT  (MAILBOX_WIDTH * MAILBOX_WIDTH )-1 
#define BLACK_PAWN_DOWN_RIGHT (MAILBOX_WIDTH * MAILBOX_WIDTH )+1 

#define WHITE_PAWN_DOWN              -MAILBOX_WIDTH * MAILBOX_WIDTH
#define WHITE_PAWN_AHEAD             -MAILBOX_WIDTH
#define BLACK_PAWN_UP                MAILBOX_WIDTH * MAILBOX_WIDTH
#define BLACK_PAWN_BACK              MAILBOX_WIDTH

#define SQUARE_NUMBER = BOARD_X_DIMENSION * BOARD_Y_DIMENSION * BOARD_Z_DIMENSION

//Useful squares

#define Ee5     4
#define Aa1     120


/* This is the basic description of a moveUni. promote is what
   pieces to promote the pawn to, if the moveUni is a pawn
   promotion. bits is a bitfield that describes the moveUni,
   with the following bits:

   1	capture
   2	pawn move
   4	promote

   It's union'ed with an integer so two moves can easily
   be compared with each other. */

typedef struct {
	int from;
	int to;
	int promote;
	char bits;
} moveBytes;

typedef union {
	moveBytes b;
	int u;
} moveUni;

/* an element of the moveUni stack. it's just a moveUni with a
   score, so it can be sorted by the search functions. */
typedef struct {
	moveUni m;
	int score;
} gener_t;

/* an element of the history stack, with the information
   necessary to take a moveUni back. */
typedef struct {
	moveUni m;
	int capture;
	//int hash;
    int fiftMoves;
} history_t;


extern int quiesceCount;
extern int searchCount;

extern int fiftMoves;
extern int nodes;
extern int sideToMove;  
extern int sideNotToMove;  
extern int mail_box[729];
extern int addresser[125];
extern int off_sets[7];
extern int off_set[7][26];
extern int colors[125];
extern int pieces[125];
extern int plyCurrent;  
extern int histPlyCurrent;  
//extern int hash;
extern BOOLEANVAL slide[7];
extern gener_t generData[GENE_STACK];
extern int history[125][125];
extern history_t historyData[HISTORY_STACK];
extern int firstMove[MAXIMUM_PLY];
extern BOOLEANVAL followPV;
extern moveUni pv[MAXIMUM_PLY][MAXIMUM_PLY];
extern int pvLength[MAXIMUM_PLY];
extern int startTime;
extern int stopTime;
extern int maxTime[2];
extern int maxDepth[2];
//extern int hashPiece[2][7][125];  
//extern int hashSide;
//extern int hashEP[125];
extern char pieceInitial[8];
extern long movesDone;
extern long trueMovesDone;
extern int score;
extern BOOLEANVAL computerIsThinking;
