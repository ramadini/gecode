bool: COMPACT;

int: N = 100;
var string(N): x;
var string(N): x1;
var string(N): x2;
var 0..N: n = if COMPACT then str_match(x, "bb*(a|b)*aa*") 
                        else str_match(x, 0, N, "bb*(a|b)*aa*") endif;
constraint str_reg(x1, "(ce|dd|ec)*aaaec");
constraint x = x1 ++ "ba" ++ x2;
constraint n = str_match(x, "bb*(a|b)*aa*");
%var string: y; constraint str_reg(y, "(x|y)*"); constraint n=str_len(y);
solve
  ::string_search([x], blockmin, lllm, complete)
satisfy;

% var string(N): y;  constraint str_reg(y, "(x|y)*"); constraint n=str_len(y);
% var string(N): x1; constraint str_reg(x1, "[c-d,3,5][a-b,1,1]");
% var string(N): x2; constraint str_reg(x2, "(a|d)*") /\ str_len(x2) = 1;
% var string(N): x3; constraint str_reg(x3, "[a-b,0,4]");
% var string(N): x;  constraint x = x1 ++ x2 ++ x3;
% var 1..N: n = 
% %   str_match(x, 0, N, "dab(ab|ba)*");
%   str_match(x, "dab(ab|ba)*");
  
% solve
%   ::string_search([x,y], blockmin, lllm, complete)
% satisfy;
