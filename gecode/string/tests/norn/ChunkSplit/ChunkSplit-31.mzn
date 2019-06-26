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
str_reg((var_4) ++ (var_5) ++ ("z") ++ (var_4), "((z)|(a)|(b)|(c))*");
constraint str_reg(var_4, "([a-u])*");
constraint str_reg((var_4) ++ (var_5), "([a-u])*");
constraint
(((((((str_len (var_5)) + (str_len (var_4))) + ((- 1) * (str_len (var_2)))) <= (0)) /\ ((0) <= (((str_len (var_5)) + (str_len (var_4))) + ((- 1) * (str_len (var_2)))))) /\ ((0) <= (((- 1) * (str_len (var_4))) + (str_len (var_2))))) /\ ((0) <= (((str_len (var_4)) + ((- 1) * (str_len (var_2)))) + (7)))) /\ ((0) <= (str_len (var_4)));
solve satisfy;
