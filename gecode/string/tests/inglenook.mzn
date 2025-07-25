include "globals.mzn";

int: A_cap = 5;
int: B_cap = 3;
int: C_cap = 3;
int: X_cap = 3;
int: MAX_CAP = max([A_cap, B_cap, C_cap, X_cap]);

string: A_ini = "x4312";
string: B_ini = "xx5";
string: C_ini = "";
string: X_ini = "";

string: A_fin = "12345";
string: B_fin = "xxx";
string: C_fin = "";
string: X_fin = "";

int: N_MOVES = 20;
int: N_TRACKS = 3;
set of int: MOVES = 1..N_MOVES;
set of int: TRACKS = 1..N_TRACKS;

array[MOVES] of var -MAX_CAP..MAX_CAP: DiffA = [
  str_len(A[i]) - str_len(A[i-1]) | i in MOVES
];
array[MOVES] of var -MAX_CAP..MAX_CAP: DiffB = [
  str_len(B[i]) - str_len(B[i-1]) | i in MOVES
];
array[MOVES] of var -MAX_CAP..MAX_CAP: DiffC = [
  str_len(C[i]) - str_len(C[i-1]) | i in MOVES
];
array[MOVES] of var -MAX_CAP..MAX_CAP: DiffX = [
  str_len(X[i]) - str_len(X[i-1]) | i in MOVES
];

array[{0} union MOVES] of var string: A;
array[{0} union MOVES] of var string: B;
array[{0} union MOVES] of var string: C;
array[{0} union MOVES] of var string: X;

constraint A[0] = A_ini /\ str_len(A_ini) <= A_cap;
constraint B[0] = B_ini /\ str_len(B_ini) <= B_cap;
constraint C[0] = C_ini /\ str_len(C_ini) <= C_cap;
constraint X[0] = X_ini /\ str_len(C_ini) <= X_cap;

constraint forall (i in MOVES) (
  str_reg(A[i], "(1|2|3|4|5|x)*") /\
  str_reg(B[i], "(1|2|3|4|5|x)*") /\
  str_reg(C[i], "(1|2|3|4|5|x)*") /\
  str_reg(X[i], "(1|2|3|4|5|x)*") /\
  str_len(A[i]) <= A_cap /\ str_len(C[i]) <= C_cap /\
  str_len(B[i]) <= B_cap /\ str_len(X[i]) <= X_cap /\
  [% No move.
   forall (j in i..N_MOVES) (
     A[i-1] = A[j] /\ B[i-1] = B[j] /\ C[i-1] = C[j] /\ Moves[j] = 0 /\
     DiffA[j] = 0  /\ DiffB[j] = 0  /\ DiffC[j] = 0 /\ DiffX[j] = 0
   ),
   % A-move.
   A[i] ++ X[i] = A[i-1] ++ X[i-1] /\ DiffA[i] != 0 /\ DiffX[i] = -DiffA[i]
        /\ B[i] = B[i-1] /\ C[i] = C[i-1], 
   % B-move.
   B[i] ++ X[i] = B[i-1] ++ X[i-1] /\ DiffB[i] != 0 /\ DiffX[i] = -DiffB[i]
        /\ A[i] = A[i-1] /\ C[i] = C[i-1],
   % C-move.
   C[i] ++ X[i] = C[i-1] ++ X[i-1] /\ DiffC[i] != 0 /\ DiffX[i] = -DiffC[i]
        /\ A[i] = A[i-1] /\ B[i] = B[i-1]
  ][Moves[i] + 1] /\
  count ([DiffA[i], DiffB[i], DiffC[i], DiffX[i]], 0) in {2,4}
);

constraint A[N_MOVES] = A_fin /\ str_len(A_fin) <= A_cap;
constraint B[N_MOVES] = B_fin /\ str_len(B_fin) <= B_cap;
constraint C[N_MOVES] = C_fin /\ str_len(C_fin) <= C_cap;
constraint X[N_MOVES] = X_fin /\ str_len(X_fin) <= X_cap;

array[MOVES] of var {0} union TRACKS: Moves;
constraint TRACKS[1] > 0;
constraint sum([DiffA[i] + DiffB[i] + DiffC[i] + DiffX[i] | i in MOVES]) = 0;
constraint forall (i in 2..N_MOVES) (Moves[i] = 0 \/ Moves[i] != Moves[i-1]);

%constraint A[1] = "x4"     /\  X[1] = "312";
%constraint C[2] = "3"      /\  X[2] = "12";
%constraint A[3] = "x"      /\  X[3] = "412";
%constraint C[4] = "34"     /\  X[4] = "12";
%constraint B[5] = "xx"     /\  X[5] = "512";
%constraint C[6] = "345"    /\  X[6] = "12";
%constraint A[7] = ""       /\  X[7] = "x12";
%constraint B[8] = "xxx"    /\  X[8] = "12";
%constraint A[9] = "12"     /\  X[9] = "";
%constraint C[10] = ""      /\  X[10] = "345";
%constraint A[11] = "12345" /\  X[11] = "";
%constraint A[12] = "12345" /\  X[12] = "";

var MOVES: tot_moves = N_MOVES - count(Moves, 0);
solve minimize tot_moves;
