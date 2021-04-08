var string: var_0;
var string: var_1;
var string: var_2;
var string: var_3;
var string: var_4;
var string: var_5;
var string: var_6;
var string(10000): var_7; %FIXME
var string(10000): var_8; %FIXME
var string: var_9;
var string: var_10;
var string: var_11;
var string: var_12;
constraint str_reg((var_7) ++ ("z") ++ (var_8), "(((b)|(a))((z)|(b)))*");
constraint str_reg(var_8, "([a-u])*");
constraint str_reg((var_8) ++ ("a"), "([a-u])*");
constraint str_reg(var_7, "([a-u])*");
constraint
not (str_reg((var_7) ++ ("z") ++ (var_8) ++ ("a"),
             "(((z)|(b)|((a)((z)|(b)|(a))))*)(a)"));
solve satisfy;
