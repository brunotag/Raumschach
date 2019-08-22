// RaumschachChessEngine.cpp : Defines the exported functions for the DLL application.
//

#include "defs.h"
#include "protos.h"
#include "stdafx.h"
#include "RaumschachChessEngine.h"
#include <stdlib.h>
#include <malloc.h>
    
//last moveUni made
char result[8];

//last chessboard translation
int chessboardResult[125];
BOOLEANVAL computerIsThinking = FALSE;

extern "C" RAUMSCHACHCHESSENGINE_API void Init(int mTime, int mDepth)
{
    //initHash();
    initBoard();

    maxTime[0] = maxTime[1] = mTime;
    maxDepth[0] = maxDepth[1] = mDepth;
    score = 0;
}

int dirty = TRUE;

extern "C" RAUMSCHACHCHESSENGINE_API int CanMove(char* moveUni)
{
    int retVal = 0;

    //sideToMove = sToMove;
    //sideNotToMove = sideToMove ^ 1;
    plyCurrent = 0;

    //if (dirty)
    //{
        generate();
    //    dirty = FALSE;
    //}

    int move_index = parseMove(moveUni);
    if(move_index >= 0){
        retVal = doMove(generData[move_index].m.b);
        if (retVal) take_back();
    }
    
    return retVal;
}

extern "C" RAUMSCHACHCHESSENGINE_API int MakeMove(char* moveUni)
{
    //sideToMove = sToMove;
    //sideNotToMove = sideToMove ^ 1;
    plyCurrent = 0;

    //if (dirty)
    //{
        generate();
    //}

    int move_index = parseMove(moveUni);

    if(move_index >= 0){
        return trueDoMove(generData[move_index].m.b);        
        //dirty = TRUE;
    }
    else
        return 0;
}

extern "C" RAUMSCHACHCHESSENGINE_API char* GetAndMakeNextComputerMove()
{
    plyCurrent = 0;
    generate();
    think(1);
    getSimpleStrFromMove(pv[0][0].b,result);
    trueDoMove(pv[0][0].b);
    return result;
}

extern "C" RAUMSCHACHCHESSENGINE_API char* GetNextComputerMove()
{
    plyCurrent = 0;
    generate();
    think(1);
    getSimpleStrFromMove(pv[0][0].b,result);
    return result;
}

extern "C" RAUMSCHACHCHESSENGINE_API char* MakeMoveAndGetNext(char *moveUni)
{
    if (MakeMove(moveUni)){

        generate();

        think(1);

        getSimpleStrFromMove(pv[0][0].b,result);

        return result;
    }
    else
        return "ERROR";
}

extern "C" RAUMSCHACHCHESSENGINE_API void Takeback()
{
    trueTakeBack();
}

extern "C" RAUMSCHACHCHESSENGINE_API char* GetMoveFromFEN(int sToMove, int maxTime, char* FENRepresentation)
{

    sideToMove = sToMove;
    sideNotToMove = sToMove ^ 1;

    parseFEN(FENRepresentation);
    
    generate();
        
    think(1);    

    getSimpleStrFromMove(pv[0][0].b, result);
    
    return result ;
}

extern "C" RAUMSCHACHCHESSENGINE_API void SetChessboardToFEN(char* FENRepr)
{
    parseFEN(FENRepr);
}

//this method is bound with interface
extern "C" RAUMSCHACHCHESSENGINE_API void GetChessboard(int* retVal)
{
    int tempval;

    for (int i=0;i<125;i++)
    {
        tempval = pieces[i] +1;
        if (colors[i] == BLACK)
            tempval *= -1;
        retVal[i] = tempval;
    }

}

//this method is bound with interface
extern "C" RAUMSCHACHCHESSENGINE_API int GetPieceBySquare(int square)
{
    int tempval;
        tempval = pieces[square]+1;
    if (colors[square] == BLACK)
        tempval *= -1;
    return tempval;
}

// START Status Getter Setter

extern "C" RAUMSCHACHCHESSENGINE_API int GetSideToMove()
{
    return sideToMove;
}

extern "C" RAUMSCHACHCHESSENGINE_API void SetSideToMove(int sTM)
{
    sideToMove = sTM;
    sideNotToMove = sideToMove ^ 1;
}

extern "C" RAUMSCHACHCHESSENGINE_API int GetFifty()
{
    return fiftMoves;
}

extern "C" RAUMSCHACHCHESSENGINE_API void SetFifty(int fifty)
{
    fiftMoves = fifty;
}

extern "C" RAUMSCHACHCHESSENGINE_API int GetMaxTimeWhite()
{
    return maxTime[WHITE];
}

extern "C" RAUMSCHACHCHESSENGINE_API void SetMaxTimeWhite(int mTime)
{
    maxTime[WHITE] = mTime;
}

extern "C" RAUMSCHACHCHESSENGINE_API int GetMaxTimeBlack()
{
    return maxTime[BLACK];
}

extern "C" RAUMSCHACHCHESSENGINE_API void SetMaxTimeBlack(int mTime)
{
    maxTime[BLACK] = mTime;
}

extern "C" RAUMSCHACHCHESSENGINE_API int GetMaxDepthWhite()
{
    return maxDepth[WHITE];
}

extern "C" RAUMSCHACHCHESSENGINE_API void SetMaxDepthWhite(int mTime)
{
    maxDepth[WHITE] = mTime;
}

extern "C" RAUMSCHACHCHESSENGINE_API int GetMaxDepthBlack()
{
    return maxDepth[BLACK];
}

extern "C" RAUMSCHACHCHESSENGINE_API void SetMaxDepthBlack(int mTime)
{
    maxDepth[BLACK] = mTime;
}

extern "C" RAUMSCHACHCHESSENGINE_API int GetScore()
{
    return score;
}

extern "C" RAUMSCHACHCHESSENGINE_API BOOLEANVAL IsComputerThinking()
{
    return computerIsThinking;
}

extern "C" RAUMSCHACHCHESSENGINE_API BOOLEANVAL IsPromo(char* move)
{
    plyCurrent = 0;
    generate();
    int move_index = parseMove(move);
    
    return (generData[move_index].m.b.bits & 4);
}

extern "C" RAUMSCHACHCHESSENGINE_API int GetResult()
{
    plyCurrent = 0;
    generate();
    int i;

    for (i = 0; i < firstMove[1]; ++i)
		if (doMove(generData[i].m.b)) {
			take_back();
			break;
		}

	if (i == firstMove[1]) {
		if (in_check(sideToMove)) {
			if (sideToMove == WHITE)
				return 0; //black wins
			else
				return 1; //white wins
		}
		else
			return 2; //draw by stalemate
	}
	//else if (reps() == 3)
	//	printf("1/2-1/2 {Draw by repetition}\n");
    else if (fiftMoves >= 100)
		return 3; //draw by fiftmoves

    //else
    return 4; //still playing
}


// END Status Getter Setter




