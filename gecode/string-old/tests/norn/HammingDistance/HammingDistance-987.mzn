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
constraint str_reg((var_7) ++ ("z") ++ (var_8), "(((z)|(a))*)(b)");
constraint
str_reg((var_7) ++ ("z") ++ (var_8),
        "(((z)|(a)|((b)((b)*)((z)|(a))))*)(b)((b)*)");
constraint
str_reg((var_7) ++ ("z") ++ (var_8), "(((z)|(((b)|(a))((z)|(b))))*)((b)|(a))");
constraint str_reg(var_8, "([a-u])*");
constraint str_reg(var_7, "([a-u])*");
constraint
not (str_reg((var_7) ++ ("z") ++ (var_8) ++ ("b"),
             "(((z)|(b)|((a)((z)|(a))))*)(a)"));
constraint (0) <= (str_len (var_8));
solve satisfy;
