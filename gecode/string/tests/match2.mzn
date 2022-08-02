bool: COMPACT;

int: L = 100;
var string(L): y;  constraint str_reg(y, "(x|y)*"); constraint i=str_len(y);
var string(L): x1; constraint str_reg(x1, "[c-d,3,5][a-b,1,1]");
var string(L): x2; constraint str_reg(x2, "(a|d)*") /\ str_len(x2) = 1;
var string(L): x3; constraint str_reg(x3, "[a-b,0,4]");
var string(L): x;  constraint x = x1 ++ x2 ++ x3;
var 1..L: i = if COMPACT then str_match(x, "dab(ab|ba)*") 
                         else str_match(x, 0, L, "dab(ab|ba)*") endif;
  
solve
  ::string_search([x,x1,x2,x3,y], blockmin, lllm, complete)
satisfy;
