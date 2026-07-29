var string: sigmaStar_0;
var string: sigmaStar_1;
var string: sigmaStar_2;
var string: sigmaStar_3;
var string: sigmaStar_6;
var string: sigmaStar_8;
var string: literal_9;
var string: x_10;
var string: literal_11;
var string: x_12;
var string: x_7;
constraint (x_7) = (str_replace(sigmaStar_1, "\x2c", "\x20"));
constraint
  (literal_9) =
    ("\x3c\x73\x70\x61\x6e\x20\x63\x6c\x61\x73\x73\x3d\x27\x73\x6d\x61\x6c\x6c\x32\x27\x3e");
constraint (x_10) = ((literal_9) ++ (sigmaStar_8));
constraint (literal_11) = ("\x3c\x2f\x73\x70\x61\x6e\x3e");
constraint (x_12) = ((x_10) ++ (literal_11));
constraint str_reg(x_12, "((.)*)(\x5c\x5c\x3c\x53\x43\x52\x49\x50\x54)((.)*)");
solve satisfy;
