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
str_reg((var_9) ++ ("z") ++ ("b") ++ (var_10),
        "((((z)|(a)|(c))(((a)|(c))*)((z)|(b)))*)(()|(((z)|(a)|(c))(((a)|(c))*)))");
constraint str_reg(("b") ++ (var_10), "([a-u])*");
constraint str_reg(var_9, "([a-u])*");
constraint str_reg(var_10, "([a-u])*");
constraint str_reg((var_9) ++ ("a"), "([a-u])*");
constraint
not (str_reg((var_9) ++ ("a") ++ ("z") ++ (var_10),
             "(((z)|(((a)|(c))((z)*)((a)|(c))))*)((a)|(c))((z)*)"));
solve satisfy;
