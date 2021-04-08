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
constraint not (str_reg((var_2) ++ (var_3), "([a-u])*"));
constraint
(((((((str_len (var_3)) + (- 8)) <= (0)) /\ ((0) <= ((str_len (var_3)) + (- 8)))) /\ ((((((str_len (var_4)) + (str_len (var_2))) + ((- 1) * (str_len (var_0)))) + (8)) <= (0)) /\ ((0) <= ((((str_len (var_4)) + (str_len (var_2))) + ((- 1) * (str_len (var_0)))) + (8))))) /\ (((((str_len (var_1)) + ((- 1) * (str_len (var_2)))) + (- 8)) <= (0)) /\ ((0) <= (((str_len (var_1)) + ((- 1) * (str_len (var_2)))) + (- 8))))) /\ ((0) <= ((((- 1) * (str_len (var_2))) + (str_len (var_0))) + (- 8)))) /\ ((0) <= (str_len (var_2)));
solve satisfy;
