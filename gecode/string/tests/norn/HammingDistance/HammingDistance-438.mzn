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
constraint str_reg((var_9) ++ ("z") ++ (var_9), "(z)*");
constraint str_reg(var_9, "([a-u])*");
constraint
not (str_reg(("b") ++ (var_9) ++ ("z") ++ ("a") ++ (var_9), "(((z)|(a))*)(b)"));
constraint ((str_len (var_9)) <= (0)) /\ ((0) <= (str_len (var_9)));
solve satisfy;
