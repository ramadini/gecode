var string: var_0;
var string: var_1;
var string: var_2;
var string: var_3;
var string: var_4;
var string: var_5;
var string: var_6;
var string: var_7;
var string: var_8;
var string: var_9;
var string: var_10;
var string: var_11;
var string: var_12;
constraint
str_reg((var_5) ++ ("z") ++ (var_6), "(((z)|(((b)|(a))((z)|(b))))*)((b)|(a))");
constraint
str_reg((var_5) ++ ("z") ++ (var_6), "(((z)|(b)|((a)((z)|(b)|(a))))*)(a)");
constraint
str_reg((var_5) ++ ("z") ++ (var_6), "(((z)|(b)|((a)((z)|(a))))*)(a)");
constraint str_reg(var_6, "([a-u])*");
constraint str_reg(var_5, "([a-u])*");
constraint not (str_reg(("b") ++ (var_6), "([a-u])*"));
constraint (0) <= (str_len (var_6));
solve satisfy;
