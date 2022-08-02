bool: COMPACT;

int: L = 100;
var string(L): x;
var string(L div 2 - 1): x1;
var string(L div 2 - 1): x2;
var 0..L: i = 
if COMPACT then 
  str_match(x, "bb*(a|b)*aa*") 
else
  str_match(x, 0, L, "bb*(a|b)*aa*")
endif
;
constraint str_reg(x1, "(ce|dd|ec)*aaaec");
constraint x = x1 ++ "ba" ++ x2; 
var string: y; constraint str_reg(y, "(x|y)*"); constraint i=str_len(y);
solve
  ::string_search([x,y,x1,x2], blockmin, lllm, complete)
satisfy;

%sudo make -j 4 install && mzn2fzn-gstrings gecode/string/tests/match1.mzn -D"COMPACT=true" -o x.fzn && fzn-gstrings -s -decomp-regex 0 x.fzn
%sudo make -j 4 install && mzn2fzn-gstrings gecode/string/tests/match2.mzn -D"COMPACT=true" -o x.fzn && fzn-gstrings -s -decomp-regex 1 x.fzn
%sudo make -j 4 install && mzn2fzn-gstrings gecode/string/tests/match1.mzn -D"COMPACT=false" -o x.fzn && fzn-gstrings -s -decomp-regex 0 x.fzn
%sudo make -j 4 install && mzn2fzn-gstrings gecode/string/tests/match1.mzn -D"COMPACT=false" -o x.fzn && fzn-gstrings -s -decomp-regex 1 x.fzn

