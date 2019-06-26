var string: var_0;
var string: var_1;
var string: var_2;
var string: var_3;
var string: var_4;
var string: var_5;
var string: var_6;
var string(10000): var_7; %FIXME:
var string(10000): var_8; %FIXME:
var string: var_9;
var string: var_10;
var string: var_11;
var string: var_12;
constraint
str_reg((var_7) ++ ("z") ++ (var_8), "(((z)|((a)((a)*)(z)))*)(a)((a)*)");
constraint
str_reg((var_7) ++ ("z") ++ (var_8), "(((a)(((b)|(a))*)(z))*)(a)(((b)|(a))*)");
constraint str_reg(var_8, "([a-u])*");
constraint str_reg(var_7, "([a-u])*");
constraint
not (str_reg((var_7) ++ ("z") ++ (var_8) ++ ("a"),
             "(((a)(((b)|(a))*)(z))*)(a)(((b)|(a))*)"));
solve satisfy;
