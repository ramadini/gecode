var string(3): x;
var string(1): b;

constraint str_reg(b, "b*");
constraint x = "ac" ++ b;

var int: m; constraint m = str_match(x, "(a(a|c)*b|c)");

solve satisfy;
