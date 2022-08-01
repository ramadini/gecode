bool: COMPACT;

int: N = 100;
% var string(N): y;  constraint str_reg(y, "(x|y)*"); constraint n=str_len(y);
var string(N): x1; constraint str_reg(x1, "[c-d,3,5][a-b,1,1]");
var string(N): x2; constraint str_reg(x2, "(a|d)*") /\ str_len(x2) = 1;
var string(N): x3; constraint str_reg(x3, "[a-b,0,4]");
var string(N): x;  constraint x = x1 ++ x2 ++ x3;
var 1..N: n = if COMPACT then str_match(x, "bb*(a|b)*aa*") 
                         else str_match(x, 0, N, "bb*(a|b)*aa*") endif;
  
solve
  ::string_search([x], blockmin, lllm, complete)
satisfy;
