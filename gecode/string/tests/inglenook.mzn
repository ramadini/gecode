include "globals.mzn";

% Max no. of moves allowed.
int: N_MOVES = 20;
% No. of tracks (excluding the head-shunt).
int: N_TRACKS = 3;
set of int: MOVES = 1..N_MOVES;
set of int: TRACKS = 1..N_TRACKS;
string: DOMAIN = "(1|2|3|4|5|x)*";

% Capacities of each track.
int: A_cap = 5;
int: B_cap = 3;
int: C_cap = 3;
int: X_cap = 3;
int: MAX_CAP = max([A_cap, B_cap, C_cap, X_cap]);

% Initial configuration.
string: A_ini = "x4312";
string: B_ini = "xx5";
string: C_ini = "";
string: X_ini = "";

% Final configuration.
string: A_fin = "12345";
string: B_fin = "xxx";
string: C_fin = "";
string: X_fin = "";

% Array of string variables recording the transitions after each move.
array[{0} union MOVES] of var string: A;
array[{0} union MOVES] of var string: B;
array[{0} union MOVES] of var string: C;
array[{0} union MOVES] of var string: X;

% Length differences after each move.
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

% Each element is a move (0 = no move).
array[MOVES] of var {0} union TRACKS: Moves;

% Initial configuration.
constraint A[0] = A_ini /\ str_len(A_ini) <= A_cap;
constraint B[0] = B_ini /\ str_len(B_ini) <= B_cap;
constraint C[0] = C_ini /\ str_len(C_ini) <= C_cap;
constraint X[0] = X_ini /\ str_len(C_ini) <= X_cap;

% Intermediate moves.
constraint forall (i in MOVES) (
  % Domain and capacity constraints.
  str_reg(A[i], DOMAIN)  /\ str_reg(B[i], DOMAIN)  /\ 
  str_reg(C[i], DOMAIN)  /\ str_reg(X[i], DOMAIN)  /\
  str_len(A[i]) <= A_cap /\ str_len(B[i]) <= B_cap /\
  str_len(C[i]) <= C_cap /\ str_len(X[i]) <= X_cap /\
  [% No-move: once no-move, always no-move.
    forall (j in i..N_MOVES) (
      A[i-1] = A[j] /\ B[i-1] = B[j] /\ C[i-1] = C[j] /\ Moves[j] = 0 /\
      DiffA[j] = 0  /\ DiffB[j] = 0  /\ DiffC[j] = 0  /\ DiffX[j] = 0
    ),
    % A-move.
    A[i] ++ X[i] = A[i-1] ++ X[i-1] /\ B[i] = B[i-1] /\ C[i] = C[i-1] /\
    DiffA[i] != 0 /\ DiffA[i] = -DiffX[i] /\ DiffB[i] = 0 /\ DiffC[i] = 0 /\
    DiffX[i] != 0,
    % B-move.
    B[i] ++ X[i] = B[i-1] ++ X[i-1] /\ A[i] = A[i-1] /\ C[i] = C[i-1] /\
    DiffB[i] != 0 /\ DiffB[i] = -DiffX[i] /\ DiffA[i] = 0 /\ DiffC[i] = 0 /\
    DiffX[i] != 0,
    % C-move.
    C[i] ++ X[i] = C[i-1] ++ X[i-1] /\ A[i] = A[i-1] /\ B[i] = B[i-1] /\
    DiffC[i] != 0 /\ DiffC[i] = -DiffX[i] /\ DiffA[i] = 0 /\ DiffB[i] = 0 /\
    DiffX[i] != 0
  ][Moves[i] + 1] /\
  % Redundant constraints.
  let {
    array[int] of var int: Diffs = [DiffA[i], DiffB[i], DiffC[i], DiffX[i]]
  } in
  count (Diffs, 0) in {2,4} /\ min(Diffs) = -max(Diffs) /\ sum(Diffs) = 0
);
% Final configuration.
constraint A[N_MOVES] = A_fin /\ str_len(A_fin) <= A_cap;
constraint B[N_MOVES] = B_fin /\ str_len(B_fin) <= B_cap;
constraint C[N_MOVES] = C_fin /\ str_len(C_fin) <= C_cap;
constraint X[N_MOVES] = X_fin /\ str_len(X_fin) <= X_cap;

% Not using the same track twice in a row: it can be done in one move.
constraint forall (i in 2..N_MOVES) ( Moves[i-1] = Moves[i] -> Moves[i] = 0 );
% Redundant constraints.
constraint sum([DiffA[i] + DiffB[i] + DiffC[i] + DiffX[i] | i in MOVES]) = 0;
constraint str_len(X[1]) != str_len(X_ini) /\ forall (i in 2..N_MOVES)(
  str_len(X[i]) = 0 \/ str_len(X[i]) != str_len(X[i-1])
);

% Total number of moves, to be minimized (lower bound improvable...).
var MOVES: tot_moves;
constraint tot_moves = N_MOVES - count(Moves, 0);
solve 
%  ::int_search(Moves, first_fail, indomain_min, complete)
minimize tot_moves;

%var 1..N_MOVES: a1; constraint DiffA[a1] < 0 /\ DiffB[a1] + DiffC[a1] = 0 /\ DiffX[a1] > 0;
%var 2..N_MOVES: i1; constraint DiffA[i1] = 0 /\ DiffB[i1] + DiffC[i1] > 0 /\ DiffX[i1] < 0;
%var 3..N_MOVES: i2; constraint DiffA[i2] = 0 /\ DiffB[i2] + DiffC[i2] > 0 /\ DiffX[i2] < 0;
%var 4..N_MOVES: a2; constraint DiffA[a2] > 0 /\ DiffB[a2] + DiffC[a2] = 0 /\ DiffX[a2] < 0;
%var 5..N_MOVES: i3; constraint DiffA[i3] = 0 /\ DiffB[i3] + DiffC[i3] < 0 /\ DiffX[i3] > 0;
%var 6..N_MOVES: a3; constraint DiffA[a3] > 0 /\ DiffB[a3] + DiffC[a3] = 0 /\ DiffX[a3] < 0;
%constraint increasing([a1,i1,i2,a2,i3,a3]) /\ all_different([a1,i1,i2,a2,i3,a3]);
%constraint B[a1] ++ C[a1] = B[a1-1] ++ C[a1-1];
%constraint A[i1] = A[i1-1];
%constraint B[a2] ++ C[a2] = B[a2-1] ++ C[a2-1];
%constraint A[i2] = A[i2-1];
%constraint B[a3] ++ C[a3] = B[a3-1] ++ C[a3-1];
%constraint A[i3] = A[i3-1];

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
