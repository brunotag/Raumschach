BOOLEANVAL in_check(int s);
BOOLEANVAL attack(int sq, int side);
void take_back();
void trueTakeBack();
void generate();
void generateCaptures();
void genPush(int from, int to, int bits);
void genPushProms(int from, int to, int bits);
int getSimpleStrFromMove(moveBytes, char* result);
void getStringSquare(int number, char* result);
//void setHash();
//int hashRand();
void initBoard();
//void initHash();
BOOLEANVAL doMove(moveBytes m);
BOOLEANVAL trueDoMove(moveBytes m);
void parseFEN(char* FEN_string);
int parseMove(char *square);

int evaluate();
int eval_BLACK_king(int sq);
int eval_BLACK_pawn(int sq);
int eval_WHITE_king(int sq);
int eval_WHITE_pawn(int sq);

void sort(int from);
int search(int alpha, int beta, int depth);
void sort_pv();
void think(BOOLEANVAL print);
int quiesce(int alpha,int beta,int depth);
//int reps();
void checkup();

int get_ms();
void print_square(int number);
void print_move_str(moveBytes m);
void print_result();