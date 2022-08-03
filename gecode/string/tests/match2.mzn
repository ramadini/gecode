bool: COMPACT;

int: L = 100;
var string(L): x;

%constraint str_reg(x, "[c-d,3,5](a|b)(a|d)[a-b,0,4]");
var string(5): x1; constraint str_reg(x1, "(c|d)*") /\ str_len(x1) >= 3;
var string(1): x2; constraint str_reg(x2, "(a|b)");
var string(1): x3; constraint str_reg(x3, "(a|d)");
var string(4): x4; constraint str_reg(x4, "(a|b)*");
constraint x = x1 ++ x2 ++ x3 ++ x4;
var 1..L: i;
constraint i = if COMPACT then str_match(x, "dab(ab|ba)*") 
                          else str_match(x, 0, L, "dab(ab|ba)*") endif;
var string(L): y;  constraint str_reg(y, "x*"); constraint i=str_len(y);

%constraint str_reg(x, ".*dab.*");

solve
  ::string_search([x,y], blockmin, lllm, complete)
satisfy;
