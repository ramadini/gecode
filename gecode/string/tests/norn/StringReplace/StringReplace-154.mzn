var string: var_0;
var string: var_1;
var string: var_2;
var string: var_3;
var string: var_4;
var string: var_5;
var string: var_6;
var string: var_7;
var string: var_8;
var string(10000): var_9; %FIXME
var string: var_10;
var string: var_11;
var string: var_12;
constraint
str_reg((var_7) ++ ("z") ++ (var_8) ++ (var_9),
        "(((z)|(a)|(c))*)(()|((b)(((b)|(a)|(c))*)))");
constraint str_reg((var_8) ++ (var_9), "([a-u])*");
constraint str_reg(var_7, "([a-u])*");
constraint not (str_reg(var_8, "b"));
constraint
not (str_reg((var_7) ++ (var_8) ++ ("z") ++ (var_9),
             "(((z)|(a)|(c))*)(()|((b)(((b)|(a)|(c))*)))"));
constraint
(((str_len (var_8)) + (- 1)) <= (0)) /\ ((0) <= ((str_len (var_8)) + (- 1)));
solve satisfy;
