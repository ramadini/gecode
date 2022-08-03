bool: COMPACT;

int: L = 100;
var string(L): x;
constraint str_reg(x, "[c-d,3,6](a|b)(a|d)[a-b,0,4]");
var 1..L: i;
%constraint i = if COMPACT then str_match(x, "dab(ab|ba)*") 
%                          else str_match(x, 0, L, "dab(ab|ba)*") endif;
%var string(L): y;  constraint str_reg(y, "x*"); constraint i=str_len(y);

%constraint str_reg(x, ".*dab.*");

solve
%  ::string_search([x], blockmin, lllm, complete)
%  ::string_search([x,y], blockmin, lllm, complete)
satisfy;
