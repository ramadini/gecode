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
constraint str_reg((var_9) ++ ("z") ++ (var_10), "(((z)|(a))((z)*)(a))*");
constraint str_reg(var_10, "([a-u])*");
constraint str_reg(var_9, "([a-u])*");
constraint
not (str_reg(("a") ++ (var_9) ++ ("z") ++ ("b") ++ (var_10),
             "(((z)|(a))((z)*)(a))*"));
solve satisfy;
