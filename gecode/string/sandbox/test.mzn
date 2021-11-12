%include "replace.mzn";
var string: x;
var string(5): y;
var string of {"a", "b", "e"}: z;
var string(5) of {"d", "e", "g"}: t;
var string of {"a", "b", "c"}: u;
var string of {"a", "b"}: v;
var string: x1;
var string: x2;
var string: r;
var string: s;
%var string: d;
%var string: z1;
%var string: z2;
%var string: z3;
%var string: z4;
%var string: z5;
%var string: z6;
%var string: r1;
var string: r2;
var string: r3;
%var string: r4;
var string: r5;

var int: n = 5;
var 0..1: b1;
var int: a;
%var int: b;
%var int: c;
var int: i;
%var int: j;

constraint x = y;
constraint x = "abb";
constraint y != z;
constraint "" != t;
constraint b1 = (x = t);
constraint b1 = (x = y) -> (y = z);
constraint z = x ++ y;
constraint u = str_pow(z, n);
constraint u = str_rev(v);
constraint str_len(t) <= n;
constraint str_chars(x1, {"0", "1"});
constraint str_len(x1) = 1;
constraint str_range(x2, "1", "5");
constraint str_len(x2) > 3;
constraint str_char_at(x2, 2) = "2";
constraint str_sub(t, 3, 4) = "gg";

%constraint str_gcc(z, ["a", "b", "c"], [a, b, c]);
constraint [x, y, z][i] = "abb";
constraint i >= a;
constraint str_len(r) > 2;
constraint str_contains(r, "]");
%constraint str_reg(r, "(\(|\[)(\**)(\\|\])");
%array[1..4, 1..3] of int: delta = [|
%  2, 0, 3 | 0, 4, 0 | 2, 0, 3 | 2, 0, 3
%|];
%constraint str_dfa(s, 4, {"a", "b", "c"}, delta, 1, {1, 3, 4});
constraint str_len(s) > 1;
%constraint if z > y then
%  v > u
%else
%  x1 = x2
%endif;
%constraint str_reg(z, "[a-z,5,8]&[a-z,3]");
%constraint z1 = str_ucase(z ++ x2 ++ t);
%constraint str_lcase(z1) = z ++ x2 ++ t;
%constraint z2 != str_lcase(str_ucase(z2));
%constraint z3 = str_code2char(str_len(u ++ v));
%constraint j = str_code_at(z, b);
%constraint str_reg(r1, "\\\n");
  %FIXME constraint r2 = "\\"; 
%constraint str_len(r2) = 1 /\ str_reg(r2, "\\");
  %constraint r3 = "***" /\ str_len(r3) > 2;
%constraint str_reg(r3, "\**");
%constraint str_reg(r4, "(\"\"|\")") /\ r4 = "\"";
%constraint str_reg(r5, "(\(|\[)(\*)(\**)(\\|\])\"");
  %FIXME constraint str_find("\\", r5) > 0; %FIXME: reverse arguments!
%constraint z4 = str_replace_all("ABBABB1211DDGGD", d ++ d, d) /\ z1 != z4;
%constraint z5 = str_replace(z4, "BB", z6) /\ z4 < z5;
%constraint str_replace_last(z1, "G", z6) > z1;

solve maximize str_len(t);
