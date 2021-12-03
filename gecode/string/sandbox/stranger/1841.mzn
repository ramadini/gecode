var string: sigmaStar_0;
var string: sigmaStar_1;
var string: sigmaStar_4;
var string: sigmaStar_7;
var string: literal_8;
var string: x_9;
var string: literal_11;
var string: x_12;
var string: x_5;
constraint
  (x_5) =
    (str_replace(sigmaStar_1, "\x66\x6f\x72\x6d\x64\x61\x74\x61\x3d", ""));
constraint (literal_8) = ("\x2e\x67\x69\x66");
constraint (x_9) = ((sigmaStar_7) ++ (literal_8));
constraint (literal_11) = ("\x2f\x2f");
constraint (x_12) = ((literal_11) ++ (x_9));
constraint str_reg(x_12, "((.)*)(\x2f\x65\x76\x69\x6c)((.)*)");
solve satisfy;
