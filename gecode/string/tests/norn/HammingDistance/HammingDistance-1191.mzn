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
constraint str_reg(("a") ++ ("b") ++ ("a") ++ ("b"), "(b)*");
constraint str_reg(("b") ++ ("a"), "(a)*");
constraint
(((((((((str_len (var_0)) + (- 1)) <= (0)) /\ ((0) <= ((str_len (var_0)) + (- 1)))) /\ ((((str_len (var_1)) + (- 1)) <= (0)) /\ ((0) <= ((str_len (var_1)) + (- 1))))) /\ (((str_len (var_8)) <= (0)) /\ ((0) <= (str_len (var_8))))) /\ ((((str_len (var_2)) + (- 1)) <= (0)) /\ ((0) <= ((str_len (var_2)) + (- 1))))) /\ ((((str_len (var_6)) + (- 2)) <= (0)) /\ ((0) <= ((str_len (var_6)) + (- 2))))) /\ ((((str_len (var_5)) + (- 3)) <= (0)) /\ ((0) <= ((str_len (var_5)) + (- 3))))) /\ ((((str_len (var_7)) + (- 4)) <= (0)) /\ ((0) <= ((str_len (var_7)) + (- 4))));
solve satisfy;
