var string: sigmaStar_0;
var string: sigmaStar_3;
var string: sigmaStar_8;
var string: sigmaStar_9;
var string: literal_10;
var string: x_11;
var string: literal_13;
var string: x_15;
var string: x_14;
var string: x_4;
var string: literal_12;
var string: x_16;
var string: literal_17;
var string: x_18;
var string: sigmaStar_19;
constraint
  (x_4) =
    (str_replace(sigmaStar_0,
       "\x2f\x5b\x5e\x30\x2d\x39\x61\x2d\x7a\x5c\x5c\x2d\x5f\x2c\x5d\x2b\x2f\x69",
       ""));
constraint (literal_10) = ("\x70\x6c\x75\x67\x69\x6e\x73\x2f");
constraint (x_11) = ((literal_10) ++ (sigmaStar_8));
constraint (literal_13) = ("\x2f\x6c\x61\x6e\x67\x73\x2f");
constraint (x_15) = ((x_11) ++ (literal_13));
constraint (literal_12) = ("\x65\x6e");
constraint ((x_14) = (x_4)) \/ ((x_14) = (literal_12));
constraint (x_16) = ((x_15) ++ (x_14));
constraint (literal_17) = ("\x2e\x6a\x73");
constraint (x_18) = ((x_16) ++ (literal_17));
constraint str_reg(sigmaStar_19, "((.)*)(\x2f\x65\x76\x69\x6c)((.)*)");
solve satisfy;
