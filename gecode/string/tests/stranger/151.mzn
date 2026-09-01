var string: sigmaStar_0;
var string: sigmaStar_3;
var string: literal_7;
var string: x_6;
var string: x_4;
var string: literal_5;
var string: x_8;
var string: literal_9;
var string: x_10;
var string: sigmaStar_11;
constraint
  (x_4) =
    (str_replace(sigmaStar_0,
       "\x2f\x5b\x5e\x30\x2d\x39\x61\x2d\x7a\x5c\x5c\x2d\x5f\x2c\x5d\x2b\x2f\x69",
       ""));
constraint (literal_7) = ("\x6c\x61\x6e\x67\x73\x2f");
constraint (literal_5) = ("");
constraint ((x_6) = (x_4)) \/ ((x_6) = (literal_5));
constraint (x_8) = ((literal_7) ++ (x_6));
constraint (literal_9) = ("\x2e\x6a\x73");
constraint (x_10) = ((x_8) ++ (literal_9));
constraint str_reg(sigmaStar_11, "((.)*)(\x2f\x65\x76\x69\x6c)((.)*)");
solve satisfy;
