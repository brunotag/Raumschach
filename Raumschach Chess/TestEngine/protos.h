BOOL in_check(int s);
BOOL attack(int sq, int side);
void takeback();
void gen();
void gen_caps();
void gen_push(int from, int to, int bits);
void gen_promote(int from, int to, int bits);
void set_hash();
int hash_rand();
void init_board();
void init_hash();
BOOL makemove(move_bytes m);

int eval();
int eval_dark_king(int sq);
int eval_dark_pawn(int sq);
int eval_light_king(int sq);
int eval_light_pawn(int sq);

void sort(int from);
int search(int alpha, int beta, int depth);
void sort_pv();
void think();
int quiesce(int alpha,int beta,int depth);
int reps();
void checkup();

int get_ms();
void print_square(int number);
void print_move_str(move_bytes m);
void print_result();