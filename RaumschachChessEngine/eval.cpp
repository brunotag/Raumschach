#include <string.h>
#include "defs.h"
#include "protos.h"

//#define DOUBLED_PAWN_PENALTY		10
//#define ISOLATED_PAWN_PENALTY		20
//#define BACKWARDS_PAWN_PENALTY		8
//#define PASSED_PAWN_BONUS			20
//#define ROOK_SEMI_OPEN_FILE_BONUS	10
//#define ROOK_OPEN_FILE_BONUS		15
//#define ROOK_ON_SEVENTH_BONUS		20


/* the values of the pieces */
int piece_value[7] = {
	100, 220, 780, 510, 380, 1100, 0
};

/* The "pcsq" arrays are pieces/square tables. They're values
   added to the material value of the pieces based on the
   location of the pieces. */

int pawn_pcsq[125] = {
	  //0,   0,   0,   0,   0,   0,   0,   0,
	  //5,  10,  15,  20,  20,  15,  10,   5,
	  //4,   8,  12,  16,  16,  12,   8,   4,
	  //3,   6,   9,  12,  12,   9,   6,   3,
	  //2,   4,   6,   8,   8,   6,   4,   2,
	  //1,   2,   3, -10, -10,   3,   2,   1,
	  //0,   0,   0, -40, -40,   0,   0,   0,
	  //0,   0,   0,   0,   0,   0,   0,   0

      0,   0,   0,   0,   0,
      6,  12,  18,  12,   6,
      5,  10,  15,  10,   5,
      4,   8,  12,   8,   4,
      3,   6,   9,   6,   3,

      6,  12,  18,  12,   6,
      5,  10,  15,  10,   5,
      4,   8,  12,   8,   4,
      3,   6,   9,   6,   3,
      2,   4,   6,   4,   2,

      5,  10,  15,  10,   5,
      4,   8,  12,   8,   4,
      3,   6,   9,   6,   3,
      2,   4,   6,   4,   2,
      1,   2,   3,   2,   1,

      4,   8,  12,   8,   4,
      3,   6,   9,   6,   3,
      2,   4,   6,   4,   2,
      1,   2,   3,   2,   1,
      0,   0,   0,   0,   0,

      3,   6,   9,   6,   3,
      2,   4,   6,   4,   2,
      1,   2,   3,   2,   1,
      0,   0,   0,   0,   0,
      0,   0,   0,   0,   0
};

int knight_pcsq[125] = {
	//-10, -10, -10, -10, -10, -10, -10, -10,
	//-10,   0,   0,   0,   0,   0,   0, -10,
	//-10,   0,   5,   5,   5,   5,   0, -10,
	//-10,   0,   5,  10,  10,   5,   0, -10,
	//-10,   0,   5,  10,  10,   5,   0, -10,
	//-10,   0,   5,   5,   5,   5,   0, -10,
	//-10,   0,   0,   0,   0,   0,   0, -10,
	//-10, -30, -10, -10, -10, -10, -30, -10
    
    //n° mosse * 2 - 24

  -12,  -8,  -4,  -8, -12,
   -8,  -4,   2,  -4,  -8,
   -4,   2,   8,   2,  -4,
   -8,  -4,   2,  -4,  -8,
  -12,  -8,  -4,  -8, -12,

   -8,  -4,   2,  -4,  -8,
   -4,   0,   8,   0,  -4,
    2,   8,  16,   8,   2,
   -4,   0,   8,   0,  -4,
   -8,  -4,   2,  -4,  -8,

   -4,   2,   8,   2,  -4,
    2,   8,  16,   8,   2,
    8,  16,  24,  16,   8,
    2,   8,  16,   8,   2,
   -4,   2,   8,   2,  -4,

   -8,  -4,   2,  -4,  -8,
   -4,   0,   8,   0,  -4,
    2,   8,  16,   8,   2,
   -4,   0,   8,   0,  -4,
   -8,  -4,   2,  -4,  -8,

  -12,  -8,  -4,  -8, -12,
   -8,  -4,   2,  -4,  -8,
   -4,   2,   8,   2,  -4,
   -8,  -4,   2,  -4,  -8,
  -12,  -8,  -4,  -8, -12
};

int bishop_pcsq[125] = {
	//-10, -10, -10, -10, -10, -10, -10, -10,
	//-10,   0,   0,   0,   0,   0,   0, -10,
	//-10,   0,   5,   5,   5,   5,   0, -10,
	//-10,   0,   5,  10,  10,   5,   0, -10,
	//-10,   0,   5,  10,  10,   5,   0, -10,
	//-10,   0,   5,   5,   5,   5,   0, -10,
	//-10,   0,   0,   0,   0,   0,   0, -10,
	//-10, -10, -20, -10, -10, -20, -10, -10

    //uguale a quella del cavallo

  -12,  -8,  -4,  -8, -12,
   -8,  -4,   2,  -4,  -8,
   -4,   2,   8,   2,  -4,
   -8,  -4,   2,  -4,  -8,
  -12,  -8,  -4,  -8, -12,

   -8,  -4,   2,  -4,  -8,
   -4,   0,   8,   0,  -4,
    2,   8,  16,   8,   2,
   -4,   0,   8,   0,  -4,
   -8,  -4,   2,  -4,  -8,

   -4,   2,   8,   2,  -4,
    2,   8,  16,   8,   2,
    8,  16,  24,  16,   8,
    2,   8,  16,   8,   2,
   -4,   2,   8,   2,  -4,

   -8,  -4,   2,  -4,  -8,
   -4,   0,   8,   0,  -4,
    2,   8,  16,   8,   2,
   -4,   0,   8,   0,  -4,
   -8,  -4,   2,  -4,  -8,

  -12,  -8,  -4,  -8, -12,
   -8,  -4,   2,  -4,  -8,
   -4,   2,   8,   2,  -4,
   -8,  -4,   2,  -4,  -8,
  -12,  -8,  -4,  -8, -12
};



int king_pcsq[125] = {
	//-40, -40, -40, -40, -40, -40, -40, -40,
	//-40, -40, -40, -40, -40, -40, -40, -40,
	//-40, -40, -40, -40, -40, -40, -40, -40,
	//-40, -40, -40, -40, -40, -40, -40, -40,
	//-40, -40, -40, -40, -40, -40, -40, -40,
	//-40, -40, -40, -40, -40, -40, -40, -40,
	//-20, -20, -20, -20, -20, -20, -20, -20,
	//  0,  20,  40, -20,   0, -20,  40,  20

    -40, -40, -40, -40, -40,
    -40, -40, -40, -40, -40,
    -40, -40, -40, -40, -40,
    -40, -40, -40, -40, -40,
    -40, -40, -40, -40, -40,

    -40, -40, -40, -40, -40,
    -40, -40, -40, -40, -40,
    -40, -40, -40, -40, -40,
    -40, -40, -40, -40, -40,
    -40, -40, -40, -40, -40,

    -40, -40, -40, -40, -40,
    -40, -40, -40, -40, -40,
    -40, -40, -40, -40, -40,
    -40, -40, -40, -40, -40,
    -40, -40, -40, -40, -40,

    -40, -40, -40, -40, -40,
    -40, -40, -40, -40, -40,
    -40, -40, -40, -40, -40,
    -40, -40, -40, -40, -40,
    -40, -10, -10, -10, -40,

    -40, -40, -40, -40, -40,
    -40, -40, -40, -40, -40,
    -40, -40, -40, -40, -40,
    -40, -40, -10, -40, -40,
    -10, -10,   0, -10, -10,
};

int king_endgame_pcsq[125] = {
	 // 0,  10,  20,  30,  30,  20,  10,   0,
	 //10,  20,  30,  40,  40,  30,  20,  10,
	 //20,  30,  40,  50,  50,  40,  30,  20,
	 //30,  40,  50,  60,  60,  50,  40,  30,
	 //30,  40,  50,  60,  60,  50,  40,  30,
	 //20,  30,  40,  50,  50,  40,  30,  20,
	 //10,  20,  30,  40,  40,  30,  20,  10,
	 // 0,  10,  20,  30,  30,  20,  10,   0

      0,  10,  20,  10,   0,
     10,  20,  30,  20,  10,
     20,  30,  40,  30,  20,
     10,  20,  30,  20,  10,
      0,  10,  20,  10,   0,

     10,  20,  30,  20,  10,
     20,  30,  40,  30,  20,
     30,  40,  50,  40,  30,
     20,  30,  40,  30,  20,
     10,  20,  30,  20,  10,

     20,  30,  40,  30,  20,
     30,  40,  50,  40,  30,
     40,  50,  60,  50,  40,
     30,  40,  50,  40,  30,
     20,  30,  40,  30,  20,

     10,  20,  30,  20,  10,
     20,  30,  40,  30,  20,
     30,  40,  50,  40,  30,
     20,  30,  40,  30,  20,
     10,  20,  30,  20,  10,

      0,  10,  20,  10,   0,
     10,  20,  30,  20,  10,
     20,  30,  40,  30,  20,
     10,  20,  30,  20,  10,
      0,  10,  20,  10,   0,
};

/* The flip array is used to calculate the pieces/square
   values for BLACK pieces. The pieces/square value of a
   WHITE pawn is pawn_pcsq[sq] and the value of a BLACK
   pawn is pawn_pcsq[flip[sq]] */
int flip[125] = {

    120, 121, 122, 123, 124,
    115, 116, 117, 118, 119,
    110, 111, 112, 113, 114,
    105, 106, 107, 108, 109,
    100, 101, 102, 103, 104,

     95,  96,  97,  98,  99,
     90,  91,  92,  93,  94,
     85,  86,  87,  88,  89,
     80,  81,  82,  83,  84,
     75,  76,  77,  78,  79,

     70,  71,  72,  73,  74,
     65,  66,  67,  68,  69,
     60,  61,  62,  63,  64,
     55,  56,  57,  58,  59,
     50,  51,  52,  53,  54,

     45,  46,  47,  48,  49,
     40,  41,  42,  43,  44,
     35,  36,  37,  38,  39,
     30,  31,  32,  33,  34,
     25,  26,  27,  28,  29,

     20,  21,  22,  23,  24,
     15,  16,  17,  18,  19,
     10,  11,  12,  13,  14,
      5,   6,   7,   8,   9,
      0,   1,   2,   3,   4,
};

///* pawn_rank[x][y] is the rank of the least advanced pawn of colors x on file
//   y - 1. There are "buffer files" on the left and right to avoid special-case
//   logic later. If there's no pawn on a rank, we pretend the pawn is
//   impossibly far advanced (0 for WHITE and 7 for BLACK). This makes it easy to
//   test for pawns on a rank and it simplifies some pawn evaluation code. */

//int pawn_rank[2][10];

int piece_mat[2];  /* the value of a side's pieces */
int pawn_mat[2];  /* the value of a side's pawns */

int evaluate()
{
	int i;

	//int f;  /* file */
	int score[2];  /* each side's score */

	/* this is the first pass: set up pawn_rank, piece_mat, and pawn_mat. */
	//for (i = 0; i < 10; ++i) {
	//	pawn_rank[WHITE][i] = 0;
	//	pawn_rank[BLACK][i] = 7;
	//}
	piece_mat[WHITE] = 0;
	piece_mat[BLACK] = 0;
	pawn_mat[WHITE] = 0;
	pawn_mat[BLACK] = 0;
	for (i = 0; i < 125; ++i) {
		if (colors[i] == EMPTY)
			continue;
		if (pieces[i] == PAWN) {
			pawn_mat[colors[i]] += piece_value[PAWN]; // == valore base di tutti i pedoni sulla scacchiera
			//f = COL(i) + 1;  /* add 1 because of the extra file in the array */
			//if (colors[i] == WHITE) {
			//	if (pawn_rank[WHITE][f] < ROW(i))
			//		pawn_rank[WHITE][f] = ROW(i);
			//}
			//else {
			//	if (pawn_rank[BLACK][f] > ROW(i))
			//		pawn_rank[BLACK][f] = ROW(i);
			//}
		}
		else
			piece_mat[colors[i]] += piece_value[pieces[i]];  // == a valore base di tutti i pezzi sulla scacchiera
	}

	
	score[WHITE] = piece_mat[WHITE] + pawn_mat[WHITE];
	score[BLACK] = piece_mat[BLACK] + pawn_mat[BLACK];

    /* this is the second pass: evaluate each pieces */
	for (i = 0; i < 125; ++i) {
		if (colors[i] == EMPTY)
			continue;
		if (colors[i] == WHITE) {
			switch (pieces[i]) {
				case PAWN:
					score[WHITE] += eval_WHITE_pawn(i);
					break;
				case KNIGHT:
					score[WHITE] += knight_pcsq[i];
					break;
				case BISHOP:
					score[WHITE] += bishop_pcsq[i]; //a seconda dei pezzi aggiungo il valore a seconda della posiz.
					break;
				case ROOK:
					//if (pawn_rank[WHITE][COL(i) + 1] == 0) {  //colonna aperta o semiaperta
					//	if (pawn_rank[BLACK][COL(i) + 1] == 7)
					//		score[WHITE] += ROOK_OPEN_FILE_BONUS;
					//	else
					//		score[WHITE] += ROOK_SEMI_OPEN_FILE_BONUS;
					//}
					//if (ROW(i) == 1)
					//	score[WHITE] += ROOK_ON_SEVENTH_BONUS; //bonus per torre sulla settima riga
					break;
				case KING:
					if (piece_mat[BLACK] <= 1200)
						score[WHITE] += king_endgame_pcsq[i];  //evaluate particolare x re in endgame
					else
						score[WHITE] += eval_WHITE_king(i);  //evaluate normale x il cavallo bianco
					break;
			}
		}
		else {
			switch (pieces[i]) { //IDEM ma x il nero
				case PAWN:
					score[BLACK] += eval_BLACK_pawn(i);
					break;
				case KNIGHT:
					score[BLACK] += knight_pcsq[flip[i]];
					break;
				case BISHOP:
					score[BLACK] += bishop_pcsq[flip[i]];
					break;
				case ROOK:
					//if (pawn_rank[BLACK][COL(i) + 1] == 7) {
					//	if (pawn_rank[WHITE][COL(i) + 1] == 0)
					//		score[BLACK] += ROOK_OPEN_FILE_BONUS;
					//	else
					//		score[BLACK] += ROOK_SEMI_OPEN_FILE_BONUS;
					//}
					//if (ROW(i) == 6)
					//	score[BLACK] += ROOK_ON_SEVENTH_BONUS;
					break;
				case KING:
					if (piece_mat[WHITE] <= 1200)
						score[BLACK] += king_endgame_pcsq[flip[i]];
					else
						score[BLACK] += eval_BLACK_king(i);
					break;
			}
		}
	}

	/* the score[] array is set, now return the score relative
	   to the side to moveUni */
	if (sideToMove == WHITE)
		return score[WHITE] - score[BLACK];

    //else

	return score[BLACK] - score[WHITE];
}

int eval_WHITE_pawn(int sq)
{

	
    int r;  /* the value to return */
	r = 0;
	r += pawn_pcsq[sq];

 //   int f;  /* the pawn's file */
	//f = COL(sq) + 1;

	///* if there's a pawn behind this one, it's doubled */
	//if (pawn_rank[WHITE][f] > ROW(sq))
	//	r -= DOUBLED_PAWN_PENALTY;

	///* if there aren't any friendly pawns on either side of
	//   this one, it's isolated */
	//if ((pawn_rank[WHITE][f - 1] == 0) &&
	//		(pawn_rank[WHITE][f + 1] == 0))
	//	r -= ISOLATED_PAWN_PENALTY;

	///* if it's not isolated, it might be backwards */
	//else if ((pawn_rank[WHITE][f - 1] < ROW(sq)) &&
	//		(pawn_rank[WHITE][f + 1] < ROW(sq)))
	//	r -= BACKWARDS_PAWN_PENALTY;

	///* add a bonus if the pawn is passed */
	//if ((pawn_rank[BLACK][f - 1] >= ROW(sq)) &&
	//		(pawn_rank[BLACK][f] >= ROW(sq)) &&
	//		(pawn_rank[BLACK][f + 1] >= ROW(sq)))
	//	r += (7 - ROW(sq)) * PASSED_PAWN_BONUS;

	return r;
}

int eval_BLACK_pawn(int sq)
{
    int r;  /* the value to return */
    r = 0;
	r += pawn_pcsq[flip[sq]];

 //   int f;  /* the pawn's file */	
	//f = COL(sq) + 1;

	///* if there's a pawn behind this one, it's doubled */
	//if (pawn_rank[BLACK][f] < ROW(sq))
	//	r -= DOUBLED_PAWN_PENALTY;

	///* if there aren't any friendly pawns on either side of
	//   this one, it's isolated */
	//if ((pawn_rank[BLACK][f - 1] == 7) &&
	//		(pawn_rank[BLACK][f + 1] == 7))
	//	r -= ISOLATED_PAWN_PENALTY;

	///* if it's not isolated, it might be backwards */
	//else if ((pawn_rank[BLACK][f - 1] > ROW(sq)) &&
	//		(pawn_rank[BLACK][f + 1] > ROW(sq)))
	//	r -= BACKWARDS_PAWN_PENALTY;

	///* add a bonus if the pawn is passed */
	//if ((pawn_rank[WHITE][f - 1] <= ROW(sq)) &&
	//		(pawn_rank[WHITE][f] <= ROW(sq)) &&
	//		(pawn_rank[WHITE][f + 1] <= ROW(sq)))
	//	r += ROW(sq) * PASSED_PAWN_BONUS;

	return r;
}



/* eval_lkp(f) evaluates the WHITE King Pawn on file f */

//int eval_lkp(int f)
//{
//	int r = 0;
//
//	if (pawn_rank[WHITE][f] == 6);  /* pawn hasn't moved */
//	else if (pawn_rank[WHITE][f] == 5)
//		r -= 10;  /* pawn moved one square */
//	else if (pawn_rank[WHITE][f] != 0)
//		r -= 20;  /* pawn moved more than one square */
//	else
//		r -= 25;  /* no pawn on this file */
//
//	if (pawn_rank[BLACK][f] == 7)
//		r -= 15;  /* no enemy pawn */
//	else if (pawn_rank[BLACK][f] == 5)
//		r -= 10;  /* enemy pawn on the 3rd rank */
//	else if (pawn_rank[BLACK][f] == 4)
//		r -= 5;   /* enemy pawn on the 4th rank */
//
//	return r;
//}
//
//int eval_dkp(int f)
//{
//	int r = 0;
//
//	if (pawn_rank[BLACK][f] == 1);
//	else if (pawn_rank[BLACK][f] == 2)
//		r -= 10;
//	else if (pawn_rank[BLACK][f] != 7)
//		r -= 20;
//	else
//		r -= 25;
//
//	if (pawn_rank[WHITE][f] == 0)
//		r -= 15;
//	else if (pawn_rank[WHITE][f] == 2)
//		r -= 10;
//	else if (pawn_rank[WHITE][f] == 3)
//		r -= 5;
//
//	return r;
//}

int eval_WHITE_king(int sq)
{
	int r;  /* the value to return */
//	int i;

	r = king_pcsq[sq];

	/* if the king is castled, use a special function to evaluate the
	   pawns on the appropriate side */

	//if (COL(sq) < 3) {
	//	r += eval_lkp(1);
	//	r += eval_lkp(2);
	//	r += eval_lkp(3) / 2;  /* problems with pawns on the c & f files
	//							  are not as severe */
	//}
	//else if (COL(sq) > 4) {
	//	r += eval_lkp(8);
	//	r += eval_lkp(7);
	//	r += eval_lkp(6) / 2;
	//}

	/* otherwise, just assess a penalty if there are open files near
	   the king */
	//else {
	//	for (i = COL(sq); i <= COL(sq) + 2; ++i)
	//		if ((pawn_rank[WHITE][i] == 0) &&
	//				(pawn_rank[BLACK][i] == 7))
	//			r -= 10;
	//}
    {}
	/* scale the king safety value according to the opponent's material;
	   the premise is that your king safety can only be bad if the
	   opponent has enough pieces to attack you */
	r *= piece_mat[BLACK];
	r /= 3100;

	return r;
}

int eval_BLACK_king(int sq)
{
	int r;
//	int i;

	r = king_pcsq[flip[sq]];
	//if (COL(sq) < 3) {
	//	r += eval_dkp(1);
	//	r += eval_dkp(2);
	//	r += eval_dkp(3) / 2;
	//}
	//else if (COL(sq) > 4) {
	//	r += eval_dkp(8);
	//	r += eval_dkp(7);
	//	r += eval_dkp(6) / 2;
	//}
	//else {
	//	for (i = COL(sq); i <= COL(sq) + 2; ++i)
	//		if ((pawn_rank[WHITE][i] == 0) &&
	//				(pawn_rank[BLACK][i] == 7))
	//			r -= 10;
	//}
	r *= piece_mat[WHITE];
	r /= 3100;
	return r;
}


