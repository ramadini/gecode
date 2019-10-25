string: x = "supercalifragilisticexpialidocious";
int: n = string_length(x);
int: k = 2;
var string(n): q;
constraint str_find(q, x) > 0 /\ str_len(q) >= k;
var string(n): y = str_replace_all(x, q, "$");
var 1..n div k: obj;
constraint obj = str_gcc(y, ["$"])[1];
solve maximize obj;
