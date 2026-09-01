var string: var_0;
var string: var_1;
var string: var_2;
var string: var_3;
var string(10000): var_4; %FIXME:
var string(10000): var_5; %FIXME:
var string: var_6;
var string: var_7;
var string: var_8;
var string: var_9;
var string: var_10;
var string: var_11;
var string: var_12;
constraint
str_reg((var_4) ++ ("z") ++ (var_5),
        "(((a)|((b)(((b)|(a))*)(z)))*)(b)(((b)|(a))*)");
constraint str_reg(var_5, "([a-u])*");
constraint str_reg(var_5, "(b)*");
constraint str_reg(var_4, "([a-u])*");
constraint str_reg(var_4, "(a)*");
constraint not (str_reg("", "[b-a]"));
constraint (0) <= (str_len (var_5));
solve satisfy;
