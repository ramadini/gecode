var string: sigmaStar_0;
var string: sigmaStar_1;
var string: sigmaStar_2;
var string: literal_10;
var string: x_13;
var string: sigmaStar_17;
var string: sigmaStar_24;
var string: literal_25;
var string: x_26;
var string: literal_27;
var string: x_28;
var string: x_18;
constraint (literal_10) = ("\x42\x79\x74\x65\x73");
constraint (x_13) = ((sigmaStar_1) ++ (literal_10));
constraint (x_18) = (str_replace(sigmaStar_2, "\x2c", "\x20"));
constraint
  (literal_25) =
    ("\x3c\x73\x70\x61\x6e\x20\x63\x6c\x61\x73\x73\x3d\x27\x73\x6d\x61\x6c\x6c\x32\x27\x3e");
constraint (x_26) = ((literal_25) ++ (sigmaStar_24));
constraint (literal_27) = ("\x3c\x2f\x73\x70\x61\x6e\x3e");
constraint (x_28) = ((x_26) ++ (literal_27));
constraint str_reg(x_28, "((.)*)(\x5c\x5c\x3c\x53\x43\x52\x49\x50\x54)((.)*)");
solve satisfy;
