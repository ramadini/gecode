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
constraint str_reg(("a") ++ ("b"), "(b)*");
constraint str_reg(("a") ++ ("a"), "(a)*");
constraint
(((((((str_len (var_0)) + (- 1)) <= (0)) /\ ((0) <= ((str_len (var_0)) + (- 1)))) /\ ((((str_len (var_1)) + (- 1)) <= (0)) /\ ((0) <= ((str_len (var_1)) + (- 1))))) /\ (((str_len (var_6)) <= (0)) /\ ((0) <= (str_len (var_6))))) /\ ((((str_len (var_2)) + (- 1)) <= (0)) /\ ((0) <= ((str_len (var_2)) + (- 1))))) /\ ((((str_len (var_4)) + (- 2)) <= (0)) /\ ((0) <= ((str_len (var_4)) + (- 2))));
solve satisfy;
