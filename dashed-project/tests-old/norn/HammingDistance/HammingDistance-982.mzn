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
str_reg((var_3) ++ ("z") ++ (var_4), "(((b)|(a))((a)*)((z)|(b)))*");
constraint
str_reg((var_3) ++ ("z") ++ (var_4),
        "(((z)|(a)|((b)((b)*)((z)|(a))))*)(b)((b)*)");
constraint
str_reg((var_3) ++ ("z") ++ (var_4), "(((a)(((b)|(a))*)(z))*)(a)(((b)|(a))*)");
constraint str_reg((var_3) ++ ("z") ++ (var_4), "(((z)|(a))*)(b)");
constraint str_reg(var_4, "([a-u])*");
constraint str_reg(var_4, "(b)*");
constraint str_reg(var_3, "([a-u])*");
constraint str_reg(var_3, "(a)*");
constraint (0) <= (str_len (var_4));
solve satisfy;
