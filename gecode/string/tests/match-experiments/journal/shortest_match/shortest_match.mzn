array[int] of string: PATTERNS;
int: N;
int: L;

array[1..N] of var 1..L: idx;
var string: str;
var 1..L: len;

constraint str_reg(str, "([a-z]|[A-Z]|[0-9])*");
constraint len = str_len(str);

constraint forall (i in 1..N) ( idx[i] = str_match(str, PATTERNS[i]) );

solve
  ::int_search(idx ++ [len], dom_w_deg, indomain_min, complete)
minimize len;
