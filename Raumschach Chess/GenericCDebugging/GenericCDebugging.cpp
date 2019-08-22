// GenericCDebugging.cpp : Defines the entry point for the console application.
//








#include "stdafx.h"
#include "defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

int mailbox[729]={                      //2 + 5 + 2 Squares
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
    -1, -1, -1, -1, -1, -1, -1, -1, -1,
};


/* LIGHT, DARK, or EMPTY */
int color[125] = { 
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,

    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, DARK, DARK, DARK, EMPTY,
    EMPTY, DARK, EMPTY, DARK, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,

    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, DARK, DARK, DARK, EMPTY,
    EMPTY, EMPTY, LIGHT, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,

    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,

    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY 
};

/* PAWN, KNIGHT, UNICORN, BISHOP, ROOK, QUEEN, KING, or EMPTY */
int piece[125] = {
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 

    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, PAWN, EMPTY, EMPTY,
    EMPTY, PAWN, EMPTY, PAWN, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 

    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, PAWN, PAWN, PAWN, EMPTY,
    EMPTY, EMPTY, PAWN, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 
    
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 

    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

int mailbox125[125];

char movementMask[125];

int sideToMove = LIGHT;
int xside = DARK;

//pawn, knight, unicorn, bishop, rook, queen, king
BOOL slide[7] = {
	FALSE, FALSE, TRUE, TRUE, TRUE, TRUE, FALSE
};

int offsets[7] = {
	0, 24, 8, 12, 6, 26, 26
};

int offset[7][26] = {
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
        -10,  -9,  -8,  -1,   1,   8,   9,  10,       //stesso piano
         71,  72,  73,  80,  81,  82,  89,  90,  91   //piano sotto
    },  

    //KING
    {         
        -91,-90, -89,-82, -81, -80, -73, -72, -71,  //piano sopra
        -10, -9,  -8, -1,   8,   1,   9,  10,       //stesso piano
         71, 72,  73, 80,  81,  82,  89,  90, 91    //piano sotto
    }      
};


int _tmain(int argc, _TCHAR* argv[])
{
    char* piece_placement;
    char* active_color;
    char* halfmove_clock;
    char* fullmove_number;
    long file_length;
    FILE *fp;
    char* current_plane_row;
    char current_square;
    char upper_current_square;
    char* current_plane;
    int i,j,k;
    char* FEN_string;
    int empty_counter;
    char* token;

    char* old_plane;
    char* old_planes_rows;

    fp = fopen("c:\\filetto.txt", "r");
    
    fseek(fp, 0L, SEEK_END);  /* Position to end of file */
    file_length = ftell(fp);     /* Get file length */
    rewind(fp);               /* Back to start of file */

    FEN_string = (char*)calloc(file_length + 1, sizeof(char));

    if(FEN_string == NULL )
    {
        printf("\nInsufficient memory to read file.\n");
        return 0;
    }

    fread(FEN_string, file_length, 1, fp); /* Read the entire file into cFile */
    
    piece_placement = strtok_s (FEN_string, " ", &token);      
    active_color = strtok_s (NULL, " ", &token);   
    halfmove_clock = strtok_s (NULL, " ", &token); 
    fullmove_number = strtok_s (NULL, " ", &token);
        
    current_plane = strtok_s (piece_placement, "-", &old_plane);
    old_planes_rows = current_plane;
    for (i=0;i<5;i++)
    {
        for (j=0;j<5;j++)
        {            
            current_plane_row = strtok_s (NULL, "/", &old_planes_rows);

            k=0;
            while (*current_plane_row != NULL)
            {
                current_square = *current_plane_row;
                if ((current_square>='1')&&(current_square<='5'))
                {
                    empty_counter = atoi(&current_square);
                    for (; empty_counter>0 ; empty_counter--)
                    {
                        //piece[i][j][k] = EMPTY
                        //color[i][j][k] = EMPTY
                        k++;
                    }
                }
                else
                {
                    upper_current_square = toupper(current_square);
                    if (current_square == upper_current_square)
                    {
                        //color[i][j][k] = LIGHT
                    }
                    else
                    {
                        //color[i][j][k] = DARK
                    }
                    switch(upper_current_square)
                    {
                        case 'P' : /*piece[i][j][k] = PAWN*/;
                        case 'U' : /*piece[i][j][k] = UNICORN*/;
                        case 'N' : /*piece[i][j][k] = KNIGHT*/;
                        case 'B' : /*piece[i][j][k] = BISHOP*/;     
                        case 'R' : /*piece[i][j][k] = ROOK*/;                                
                        case 'Q' : /*piece[i][j][k] = QUEEN*/;                                
                        case 'K' : /*piece[i][j][k] = KING*/;                                
                    }
                    k++;
                }
                current_plane_row++;
            }
        }
        current_plane = strtok_s (NULL, "-", &old_plane);
        old_planes_rows = current_plane;
    }
    
    free(FEN_string);
    getchar();
    /*
    int contaMoves[125];

    int next=0;
    for(int j=0;j<729;j++){
        if (mailbox[j]>-1){
            mailbox125[next] = j;
            next++;
        }
    }

    for(int i=0;i<125;i++)
        movementMask[i]='O';


       int n;
    for(int i=0;i<125;i++){  

        //contaMoves[i]=0;
        //piece[i]=BISHOP;
        //color[i]=LIGHT;

               if(color[i] == LIGHT)
            {    
        if(piece[i] == PAWN){
 
                    if ((mailbox[mailbox125[i + LIGHT_PAWN_AHEAD_LEFT]] != -1)&&(color[i + LIGHT_PAWN_AHEAD_LEFT]==DARK))
						movementMask[i + LIGHT_PAWN_AHEAD_LEFT] = 'X';
					if ((mailbox[mailbox125[i + LIGHT_PAWN_AHEAD_RIGHT]] != -1)&&(color[i + LIGHT_PAWN_AHEAD_RIGHT]==DARK))
						movementMask[i + LIGHT_PAWN_AHEAD_RIGHT] = 'X';
					if ((mailbox[mailbox125[i + LIGHT_PAWN_UP_LEFT]] != -1)&&(color[i + LIGHT_PAWN_UP_LEFT]==DARK))
						movementMask[i + LIGHT_PAWN_UP_LEFT] = 'X';
					if ((mailbox[mailbox125[i + LIGHT_PAWN_UP_RIGHT]] != -1)&&(color[i + LIGHT_PAWN_UP_RIGHT]==DARK))
						movementMask[i + LIGHT_PAWN_UP_RIGHT] = 'X';
                    if (color[i - BOARD_X_DIMENSION] == EMPTY) 
						movementMask[i - BOARD_X_DIMENSION] = 'X';
                    if (color[i + LIGHT_PAWN_DOWN])
                        movementMask[i + LIGHT_PAWN_DOWN] = 'X';
            
        }
        else //if it's not a pawn
				for (int j = 0; j < offsets[piece[i]]; ++j)
					for (n = i;;) {
						n = mailbox[mailbox125[n] + offset[piece[i]][j]];
						if (n == -1)
							break;
						if (color[n] != EMPTY) {
							if (color[n] == xside)
								movementMask[n] = 'X';
							break;
						}
                        contaMoves[i]++;
						movementMask[n] = 'X';
						if (!slide[piece[i]])
							break;
                    }
            }
        //piece[i]=EMPTY;
        //color[i]=EMPTY;
    }


    for(int i=0;i<=4;i++)
    {
        for(int j=0;j<=4;j++){

            for(int k=0;k<=4;k++)
            {
                int numero = 25*i+5*j+k;
                printf("%c", movementMask[numero]);
   //             printf("%*d ",3, contaMoves[numero]*2 - 24);
            }
            printf("\n");
        }
        printf("\n\n");
    }
    getchar();

    */
	return 0;
}

