% Auxiliary methods returning a temporary variable.
function var string: 
tmp_str_arr() ::promise_total = let { 
  var string of {"~", ","}: x;
  constraint str_reg(x, "(()|(~(,~)*))");
} in x;
function var int:
tmp_int() ::promise_total = let { var int: x } in x;

% Enumeration of JS types.
enum JSType = {Null, Undef, Bool, Num, Str, Object, Array, Function};
% FIXME: Just considering a subset of this atm:
%{Null, Undef, Bool, Num, Str, ActiveXObject, Array, ArrayBuffer, Boolean, DataView, Date, Debug, Enumerator, Error, Float32Array, Float64Array, Function, Global, Int8Array, Int16Array, Int32Array, Intl_Collator, Intl_DateTimeFormat, Intl_NumberFormat, JSON, Map, Math, Number, Object, Promise, Proxy, Reflect, RegExp, Expression, Set, String, Symbol, Uint8Array, Uint8ClampedArray, Uint16Array, Uint32Array, VBArray, WeakMap, WeakSet, WinRTError};

% Non-wrapper object.
predicate is_obj(var JSType: t) = t > Str;

% Force invariants for JS variables. Each JS variable x is represented by a 
% triple (Tx, Sx, Ax) where:
% - Tx is the TYPE of x (an element of JSType enumeration)
% - Sx is the STRING VALUE of x (a string variable)
% - Ax is the LOGICAL ADDRESS of x (a natural number identifying the memory 
%      location of x). We have that Ax = 0 iff x has a primitive type. If Tx in 
%      {Bool, Num, Str} and Ax > 0, we refer to the corresponding wrapper. This 
%      is necessary to read built-in properties of wrappers (e.g., O["length"]).
% FIXME: Do we really need reverse implications?
predicate
js_var(var JSType: Tx, var string: Sx, var 0..N_ADDRS: Ax) =
  (Tx = Undef -> (Sx = "undefined" /\ Ax = 0)) /\ 
%  (Sx = "undefined" -> Tx in {Undef, Str}) /\
  (Tx = Null -> (Sx = "null" /\ Ax = 0)) /\ 
%  (Sx = "null" -> Tx in {Null, Str}) /\
  (Tx = Bool -> let {var 1..2: b} in Sx = ["true", "false"][b]) /\
%  (let {var 1..2: b} in Sx = ["true", "false"][b] -> Tx in {Bool, Str}) /\ 
  (Tx = Num -> is_norm(Sx)) /\
%  (is_num(Sx) -> (Tx in {Num, Str} /\ (not(is_norm(Sx)) -> Tx = Str))) /\
  (Tx = Object -> Sx = "[object Object]") /\
%  (Sx = "[object Object]" -> Tx in {Str, Object}) /\
  (Tx = Array -> str_reg(Sx, "(()|(~(,~)*))")) /\ % FIXME: Workaround.
  (Tx = Function -> str_find("function ", Sx) = 1) /\  
  Ax <= N_ADDRS /\ Ax >= is_obj(Tx);

% For each property x of object O, there exists a *property index* i such that:
%  - O_ADDRS[i] is the memory address of object O
%  - P_NAMES[i] is the name of property x
%  - P_TYPES[i] is the type of value O[x]
%  - P_SVALS[i] is the string string value of O[x]
%  - P_ADDRS[i] is the address of O[x]
% with i in 1..N_PROPS = N_IMPL_PROPS + N_EXPL_PROPS, where:
%  - N_IMPL_PROPS: properties *implicitly* defined, e.g., built-in properties 
%                  or properties of input objects.
%  - N_EXPL_PROPS: properties *explicitly* defined by an assignment O[x] = y (or
%                  a deletion).
% Note that explicitly defined properties are always stored after implicit ones,
% to properly deal with property overwriting.
int: N_PROPS = N_IMPL_PROPS + N_EXPL_PROPS;

constraint 
% Cell 0 is reserved to undefined reads.
O_ADDRS[0] = 0 /\ P_TYPES[0] = Undef /\ P_SVALS[0] = "undefined" /\ 
P_ADDRS[0] = 0 /\ forall (i in 1..N_PROPS) (
  % Forcing property invariants. For simplicity, we avoid "circular" properties 
  % of the form O[x] = O.
  js_var(P_TYPES[i], P_SVALS[i], P_ADDRS[i])  /\ 
  ((O_ADDRS[i] = 0 \/ O_ADDRS[i] = P_ADDRS[i]) -> del_prop_at(i))
);

% Returns the property index i <= M for property x of object O having type t. 
% If there is no such a property, i = 0. The M parameter is a sequence number 
% meant to handle the possible overwriting of O[x].
function var 0..N_PROPS:
get_prop(
  var JSType: t, var 0..N_ADDRS: O, var string: x, 0..N_PROPS: M
) ::promise_total = 
let {
  var 0..N_PROPS: i;
  constraint t > Undef /\ (t in {Bool, Num, Str} -> i in 1..N_IMPL_PROPS) /\
  O = array1d(0..M, [O] ++ [O_ADDRS[j] | j in 1..M])[i] /\
  x = array1d(0..M, [x] ++ [P_NAMES[j] | j in 1..M])[i] /\
  forall (j in 1..M) ((O = O_ADDRS[j] /\ x = P_NAMES[j]) -> j <= i)
} in i;

% The equivalent of get_prop for an object wrapping a fixed string str. Note the
% property index returned is "implicit".
function var 0..N_PROPS:
get_str_prop(string: str, var string: x) ::promise_total = 
let {
  var 0..N_IMPL_PROPS: i;
  % Character selection.
  constraint if str_reg(x, "(0|[1-9][0-9]*)") then
    let {
      var string: chr = str_char_at(str, str2nat(x) + 1);
    } in 
      P_TYPES[i] = [Str, Undef][(chr = "") + 1] /\
      P_SVALS[i] = [chr, "undefined"][(chr = "") + 1] /\
      P_ADDRS[i] = 0 /\ P_NAMES[i] = x
  % String length.
  elseif x = "length" then
    P_TYPES[i] = Num /\ P_SVALS[i] = nat2str(str_len(str)) /\ P_ADDRS[i] = 0 /\
    P_NAMES[i] = x
  % Undefined.
  else
    i = 0
  endif;
} in i;  

% Setting explicit property {x: (Tx, Sx, Ax)} for object O having type t at 
% property index i (if possible).
predicate
put_prop(
  var JSType: t, var 0..N_ADDRS: O, var string: x, 
  var JSType: Tx, var string: Sx, var 0..N_ADDRS: Ax, N_IMPL_PROPS+1..N_PROPS: i
) = 
  t > Undef /\ if t = Object \/ (t = Array /\ is_nat(x)) then
    O_ADDRS[i] = O /\ P_NAMES[i] = x /\ P_TYPES[i] = Tx /\ P_SVALS[i] = Sx /\ 
    P_ADDRS[i] = Ax
  else
    del_prop_at(i)
  endif;
  
% Deleting entry x for object O of type t, i.e., setting an undefined entry.
% FIXME: This is not exactly the JS semantic, we should define a new type for 
% deleted properties...
predicate
del_prop(var JSType: t, var 0..N_ADDRS: O, var string: x, int: i) = 
  put_prop(t, O, x, Undef, "undefined", 0, i);

% Returns the property index of a property of O having value (Ty, Sy, Ay). If 
% there is no such a property, it returns 0.
function var 0..N_PROPS:
find_prop(
  var JSType: t, var 0..N_ADDRS: O, 
  var JSType: Ty, var string: Sy, var 0..N_ADDRS: Ay, 0..N_PROPS: M
) ::promise_total = 
let {
  var 0..M: i;
  constraint i > 0 <-> (
    is_obj(t) /\ O_ADDRS[i] = O /\
    P_TYPES[i] = Ty /\ P_SVALS[i] = Sy /\ P_ADDRS[i] = Ay
  );
} in i;

% Setting i-th property (no checks performed).
predicate
set_prop_at(
  var 0..N_ADDRS: O, var string: x, 
  var JSType: Tx, var string: Rx, var 0..N_ADDRS: Ax, int: i
) =
  O_ADDRS[i] = O /\ P_NAMES[i] = x /\ P_TYPES[i] = Tx /\ P_SVALS[i] = Rx /\ 
  P_ADDRS[i] = Ax;

% Setting i-th property to undefined (no checks performed).
predicate
del_prop_at(var 0..N_PROPS: i) ::promise_total =
  O_ADDRS[i] = 0 /\ P_NAMES[i] = "" /\ P_TYPES[i] = Undef /\ 
  P_SVALS[i] = "undefined" /\ P_ADDRS[i] = 0;

% True iff array at Ax includes element (Ty, Sy, Ay).
predicate
array_includes(
  var 0..N_ADDRS: Ax, var JSType: Ty, var string: Sy, var 0..N_ADDRS: Ay, 
  0..N_PROPS: M
) = 
let {
  var 1..N_PROPS: i = find_prop(Array, Ax, Ty, Sy, Ay, M);
} in is_nat(P_NAMES[i]);

% True iff property x is defined for object O having type t.
% FIXME: not dealing with prototype chains atm.
predicate
is_def(var string: x, var JSType: t, var 0..N_ADDRS: O) = 
  P_TYPES[get_prop(t, O, x, N_PROPS)] != Undef;

% JS typeof operator. FIXME: Dependent on the JSType ordering.
function var string:
typeof(var JSType: t) ::promise_total = 
  ["object", "undefined", "boolean", "number", "string", 
   "object", "object", "function"][t];

