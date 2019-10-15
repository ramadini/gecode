var string: sigmaStar_0;
var string: sigmaStar_1;
var string: sigmaStar_4;
var string: literal_6;
var string: x_5;
var string: x_7;
var string: literal_8;
var string: x_9;
var string: x_10;
var string: literal_11;
var string: x_12;
constraint
  (x_5) =
    (str_replace_all(sigmaStar_0, "\x2f\x2e\x28\x5c\x5c\x64\x2b\x29\x2e\x2f",
       "\x5f\x24\x31\x2e"));
constraint (literal_6) = ("\x20\x20\x20\x20");
constraint (x_7) = ((literal_6) ++ (x_5));
constraint (literal_8) = ("\x20\x3d\x20\x27");
constraint (x_9) = ((x_7) ++ (literal_8));
constraint (x_10) = ((x_9) ++ (sigmaStar_0));
constraint (literal_11) = ("\x27\x3b\x5c\x5c\x6e");
constraint (x_12) = ((x_10) ++ (literal_11));
constraint str_reg(x_12, "((.)*)(\x5c\x5c\x3c\x53\x43\x52\x49\x50\x54)((.)*)");
solve satisfy;
