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
str_reg((var_5) ++ (var_6) ++ (var_7) ++ ("z") ++ (var_5),
        "((z)|(a)|(b)|(c))*");
constraint str_reg(var_5, "([a-u])*");
constraint str_reg((var_5) ++ (var_6) ++ (var_7), "([a-u])*");
constraint
(((((((str_len (var_6)) + (- 8)) <= (0)) /\ ((0) <= ((str_len (var_6)) + (- 8)))) /\ ((((((str_len (var_7)) + (str_len (var_5))) + ((- 1) * (str_len (var_2)))) + (8)) <= (0)) /\ ((0) <= ((((str_len (var_7)) + (str_len (var_5))) + ((- 1) * (str_len (var_2)))) + (8))))) /\ (((((str_len (var_3)) + ((- 1) * (str_len (var_5)))) + (- 8)) <= (0)) /\ ((0) <= (((str_len (var_3)) + ((- 1) * (str_len (var_5)))) + (- 8))))) /\ ((0) <= ((((- 1) * (str_len (var_5))) + (str_len (var_2))) + (- 8)))) /\ ((0) <= (str_len (var_5)));
solve satisfy;
