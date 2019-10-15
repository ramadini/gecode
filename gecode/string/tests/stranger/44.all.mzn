var string: sigmaStar_0;
var string: sigmaStar_6;
var string: literal_9;
var string: x_7;
var string: x_10;
var string: literal_11;
var string: x_12;
constraint
  (x_7) =
    (str_replace_all(sigmaStar_0,
       "\x2f\x5b\x5e\x41\x53\x43\x52\x20\x2e\x5c\x5c\x2d\x40\x3a\x2f\x5d\x2f", ""));
constraint (literal_9) = ("\x3c\x74\x64\x3e\x55\x52\x4c\x3a\x20");
constraint (x_10) = ((literal_9) ++ (x_7));
constraint (literal_11) = ("\x3c\x2f\x74\x64\x3e");
constraint (x_12) = ((x_10) ++ (literal_11));
constraint str_reg(x_12, "((.)*)(\x5c\x5c\x3c\x53\x43\x52\x49\x50\x54)((.)*)");
solve satisfy;
