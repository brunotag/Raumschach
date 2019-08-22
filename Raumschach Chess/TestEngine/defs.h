

#define BOOL			int
#define TRUE			1
#define FALSE			0

#define GEN_STACK		1120
#define MAX_PLY			12
#define HIST_STACK		800

#define LIGHT			0
#define DARK			1

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

#define LIGHT_PAWN_AHEAD_LEFT       -MAILBOX_WIDTH-1 
#define LIGHT_PAWN_AHEAD_RIGHT      -MAILBOX_WIDTH+1 
#define LIGHT_PAWN_UP_LEFT  -(MAILBOX_WIDTH * MAILBOX_WIDTH )-1 
#define LIGHT_PAWN_UP_RIGHT -(MAILBOX_WIDTH * MAILBOX_WIDTH )+1 

#define DARK_PAWN_BACK_LEFT         MAILBOX_WIDTH-1 
#define DARK_PAWN_BACK_RIGHT        MAILBOX_WIDTH+1 
#define DARK_PAWN_DOWN_LEFT  (MAILBOX_WIDTH * MAILBOX_WIDTH )-1 
#define DARK_PAWN_DOWN_RIGHT (MAILBOX_WIDTH * MAILBOX_WIDTH )+1 

#define LIGHT_PAWN_DOWN              -MAILBOX_WIDTH * MAILBOX_WIDTH
#define LIGHT_PAWN_AHEAD             -MAILBOX_WIDTH
#define DARK_PAWN_UP                MAILBOX_WIDTH * MAILBOX_WIDTH
#define DARK_PAWN_BACK              MAILBOX_WIDTH

#define SQUARE_NUMBER = BOARD_X_DIMENSION * BOARD_Y_DIMENSION * BOARD_Z_DIMENSION

//Useful squares

#define Ee1     120
#define Aa5     4



/* This is the basic description of a move. promote is what
   piece to promote the pawn to, if the move is a pawn
   promotion. bits is a bitfield that describes the move,
   with the following bits:

   1	capture
   2	pawn move
   4	promote

   It's union'ed with an integer so two moves can easily
   be compared with each other. */

typedef struct {
	char from;
	char to;
	char promote;
	char bits;
} move_bytes;

typedef union {
	move_bytes b;
	int u;
} move;

/* an element of the move stack. it's just a move with a
   score, so it can be sorted by the search functions. */
typedef struct {
	move m;
	int score;
} gen_t;

/* an element of the history stack, with the information
   necessary to take a move back. */
typedef struct {
	move m;
	int capture;
	int hash;
    int fifty;
} hist_t;


extern int quiesceCount;
extern int searchCount;

extern int fifty;
extern int nodes;
extern int sideToMove;  
extern int sideNotToMove;  
extern int mailbox[729];
extern int mailbox125[125];
extern int offsets[7];
extern int offset[7][26];
extern int color[125];
extern int piece[125];
extern int ply;  
extern int hply;  
extern int hash;
extern BOOL slide[7];
extern gen_t gen_dat[GEN_STACK];
extern int history[125][125];
extern hist_t hist_dat[HIST_STACK];
extern int first_move[MAX_PLY];
extern BOOL follow_pv;
extern move pv[MAX_PLY][MAX_PLY];
extern int pv_length[MAX_PLY];
extern int start_time;
extern int stop_time;
extern int max_time;
extern int max_depth;
extern int hash_piece[2][7][125];  
extern int hash_side;
extern int hash_ep[125];
extern char piece_initial[8];

