array[int] of string: alpha = ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
                               "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"];
int: N;

array[1..N] of var string: row_str;
array[1..N] of var string: col_str;
array[1..N,1..N] of var 1..N: m_row;
array[1..N,1..N] of var 1..N: m_col;

constraint forall (k in 1..N) (
  str_len(row_str[k]) = N /\ str_reg(row_str[k], "[A-" ++ alpha[N] ++ "]*") /\
  str_len(col_str[k]) = N /\ str_reg(col_str[k], "[A-" ++ alpha[N] ++ "]*") /\
  forall (i in 1..N, j in 1..N) ( m_row[i,k] = j <-> m_col[j,k] = i ) /\
  forall (i in 1..N) ( m_row[i,k] = str_match(row_str[i], alpha[k]) ) /\
  forall (j in 1..N) ( m_col[j,k] = str_match(col_str[j], alpha[k]) )
);

solve 
  %::int_search([m_row[i,j] | i,j in 1..N], dom_w_deg, indomain_min, complete)
satisfy;

%include "match.mzn";
