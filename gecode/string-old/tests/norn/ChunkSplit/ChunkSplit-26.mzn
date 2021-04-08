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
str_reg((var_10) ++ (var_11) ++ (var_12) ++ ("z") ++ (var_10),
        "((z)|(a)|(b)|(c))*");
constraint str_reg(var_10, "([a-u])*");
constraint str_reg((var_10) ++ (var_11) ++ (var_12), "([a-u])*");
constraint
not (str_reg((var_10) ++ (var_11) ++ ("c") ++ (var_12) ++ ("z") ++ (var_10) ++ (var_11),
             "((z)|(a)|(b)|(c))*"));
constraint
(((((((str_len (var_11)) + (- 8)) <= (0)) /\ ((0) <= ((str_len (var_11)) + (- 8)))) /\ ((((((str_len (var_12)) + (str_len (var_10))) + ((- 1) * (str_len (var_5)))) + (8)) <= (0)) /\ ((0) <= ((((str_len (var_12)) + (str_len (var_10))) + ((- 1) * (str_len (var_5)))) + (8))))) /\ (((((str_len (var_8)) + ((- 1) * (str_len (var_10)))) + (- 8)) <= (0)) /\ ((0) <= (((str_len (var_8)) + ((- 1) * (str_len (var_10)))) + (- 8))))) /\ ((0) <= ((((- 1) * (str_len (var_10))) + (str_len (var_5))) + (- 8)))) /\ ((0) <= (str_len (var_10)));
solve satisfy;
