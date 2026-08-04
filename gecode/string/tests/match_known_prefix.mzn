var string(3): x;
var string(1): b;

constraint str_reg(b, "b*");
constraint x = "ac" ++ b;

var int: m1; constraint m1 = str_match(x, "(a(a|c)*b|c)");
var int: m2; constraint m2 = str_match_new(x, "(a(a|c)*b|c)");

solve satisfy;
