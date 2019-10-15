var string: sigmaStar_0;
var string: sigmaStar_3;
var string: x_4;
var string: literal_5;
var string: x_6;
constraint
  (x_4) = (str_replace_all(sigmaStar_0, "\x23\x5b\x5c\x5c\x7c\x2f\x5d\x23", ""));
constraint (literal_5) = ("\x2e\x70\x68\x70");
constraint (x_6) = ((x_4) ++ (literal_5));
constraint str_reg(x_6, "((.)*)(\x2f\x65\x76\x69\x6c)((.)*)");
solve satisfy;
