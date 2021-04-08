var string: sigmaStar_0;
var string: sigmaStar_3;
var string: literal_7;
var string: x_6;
var string: x_4;
var string: literal_5;
var string: x_8;
var string: literal_10;
var string: x_12;
var string: x_11;
var string: literal_9;
var string: x_13;
var string: literal_14;
var string: x_15;
var string: sigmaStar_16;
constraint
  (x_4) =
    (str_replace(sigmaStar_0,
       "\x2f\x5b\x5e\x30\x2d\x39\x61\x2d\x7a\x5c\x5c\x2d\x5f\x2c\x5d\x2b\x2f\x69",
       ""));
constraint (literal_7) = ("\x74\x68\x65\x6d\x65\x73\x2f");
constraint (literal_5) = ("");
constraint ((x_6) = (x_4)) \/ ((x_6) = (literal_5));
constraint (x_8) = ((literal_7) ++ (x_6));
constraint (literal_10) = ("\x2f\x6c\x61\x6e\x67\x73\x2f");
constraint (x_12) = ((x_8) ++ (literal_10));
constraint (literal_9) = ("\x65\x6e");
constraint ((x_11) = (literal_9)) \/ ((x_11) = (x_4));
constraint (x_13) = ((x_12) ++ (x_11));
constraint (literal_14) = ("\x2e\x6a\x73");
constraint (x_15) = ((x_13) ++ (literal_14));
constraint str_reg(sigmaStar_16, "((.)*)(\x2f\x65\x76\x69\x6c)((.)*)");
solve satisfy;
