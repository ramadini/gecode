%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Simulating the execution of JS code: %
%   A = Object();                      %
%   B = Object;                        %
%   B[1] = true                        %
%   B["1"] = A;                        %
%   B[1]["0"] = "Hello";               %
%   x = A[0];                          %
%   y = new String                     %
%   if (x + y["length"])               %
%     /* ... */                        %
%   else                               %
%     /* UNREACHABLE */                %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%% PREAMBLE - Can be used in combination with a DZN file %%%%%

include "javascript.mzn";
include "alldifferent_except_0.mzn";

% Number of adresses needed.
int: N_ADDRS = 3;
% Number of properties implicitly defined.
int: N_IMPL_PROPS = 2;
% Number of properties explicitly defined.
int: N_EXPL_PROPS = 3;

% Array of the locations of the objects storing properties.
array[0..N_PROPS] of var 0..N_ADDRS: O_ADDRS;
% Array of the names of the properties.
array[0..N_PROPS] of var string: P_NAMES;
% Array of the types of the properties.
array[0..N_PROPS] of var JSType: P_TYPES;
% Array of the representations of the properties.
array[0..N_PROPS] of var string: P_SVALS;
% Array of the locations referenced by properties.
array[0..N_PROPS] of var 0..N_ADDRS: P_ADDRS;

constraint alldifferent_except_0([
  ADDR_A, ADDR_B, ADDR_y,
]);

%%%%% MAIN %%%%%

% Var. declaration: A = Object().
var JSType: TYPE_A = Object;
var string: SVAL_A = "[object Object]";
var 1..N_ADDRS: ADDR_A;
constraint js_var(TYPE_A, SVAL_A, ADDR_A);

% Var. declaration: B = Object.
var JSType: TYPE_B = Array;
var string: SVAL_B;
var 1..N_ADDRS: ADDR_B;
constraint js_var(TYPE_B, SVAL_B, ADDR_B);

% Writing properties on B.
constraint put_prop(TYPE_B, ADDR_B, "1", Bool, "true", 0, N_IMPL_PROPS + 1);
constraint put_prop(TYPE_B, ADDR_B, "1", TYPE_A, SVAL_A, ADDR_A, N_IMPL_PROPS + 2);

% Tmp. reading: TMP = B[1]
var int: IDX_TMP = get_prop(TYPE_B, ADDR_B, "1", N_IMPL_PROPS + 2);
var JSType: TYPE_TMP = P_TYPES[IDX_TMP];
var string: SVAL_TMP = P_SVALS[IDX_TMP];
var 0..N_ADDRS: ADDR_TMP = P_ADDRS[IDX_TMP];

% Tmp. writing: TMP["0"] = "hello". We are actually writing on object A.
constraint set_prop_at(ADDR_TMP, "0", Str, "Hello", 0, N_IMPL_PROPS + 3);

% Obj. reading: x = A[0].
var int: IDX_x = get_prop(TYPE_A, ADDR_A, "0", N_IMPL_PROPS + 3); 
var JSType: TYPE_x; constraint TYPE_x = P_TYPES[IDX_x];
var string: SVAL_x; constraint SVAL_x = P_SVALS[IDX_x];
var 0..N_ADDRS: ADDR_x; constraint ADDR_x = P_ADDRS[IDX_x];

% Assignment y = new String.
var JSType: TYPE_y = Str;
var string: SVAL_y = "";
var 1..N_ADDRS: ADDR_y;
constraint js_var(TYPE_y, SVAL_y, ADDR_y);

% Function call: TMP2 = y["length"]
constraint set_str_builtins(TYPE_y, SVAL_y, ADDR_y, 1);

var int: IDX_TMP2;
constraint IDX_TMP2 = get_prop(TYPE_y, ADDR_y, "length", N_IMPL_PROPS + 3);
var JSType: TYPE_TMP2;
constraint TYPE_TMP2 = P_TYPES[IDX_TMP2];
var string: SVAL_TMP2;
constraint SVAL_TMP2 = P_SVALS[IDX_TMP2];
var 0..N_ADDRS: ADDR_TMP2;
constraint ADDR_TMP2 = P_ADDRS[IDX_TMP2];

% Operation TMP_3 = x + TMP2
var JSType: TYPE_TMP3;
constraint TYPE_TMP3 = plus_TYPE(TYPE_x, TYPE_TMP2);
var string: SVAL_TMP3;
constraint SVAL_TMP3 = plus_SVAL(TYPE_x, SVAL_x, TYPE_TMP2, SVAL_TMP2);

% If condition: bool(TMP_3)
constraint to_bool(TYPE_TMP3, SVAL_TMP3) = "true";

solve satisfy;



