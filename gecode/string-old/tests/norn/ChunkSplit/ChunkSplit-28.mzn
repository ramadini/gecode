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
str_reg((var_8) ++ (var_9) ++ (var_10) ++ ("z") ++ (var_8),
        "((z)|(a)|(b)|(c))*");
constraint str_reg((var_8) ++ (var_9), "([a-u])*");
constraint str_reg(var_8, "([a-u])*");
constraint str_reg((var_8) ++ (var_9) ++ (var_10), "([a-u])*");
constraint
not (str_reg((var_8) ++ (var_9) ++ ("c") ++ (var_10), "([a-u])*"));
constraint
(((((((str_len (var_9)) + (- 8)) <= (0)) /\ ((0) <= ((str_len (var_9)) + (- 8)))) /\ ((((((str_len (var_10)) + (str_len (var_8))) + ((- 1) * (str_len (var_4)))) + (8)) <= (0)) /\ ((0) <= ((((str_len (var_10)) + (str_len (var_8))) + ((- 1) * (str_len (var_4)))) + (8))))) /\ (((((str_len (var_6)) + ((- 1) * (str_len (var_8)))) + (- 8)) <= (0)) /\ ((0) <= (((str_len (var_6)) + ((- 1) * (str_len (var_8)))) + (- 8))))) /\ ((0) <= ((((- 1) * (str_len (var_8))) + (str_len (var_4))) + (- 8)))) /\ ((0) <= (str_len (var_8)));
solve satisfy;
