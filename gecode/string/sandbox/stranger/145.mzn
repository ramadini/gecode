var string: sigmaStar_0;
var string: sigmaStar_3;
var string: x_8;
var string: literal_6;
var string: x_4;
var string: x_7;
var string: literal_5;
var string: x_11;
var string: x_10;
var string: literal_9;
var string: x_15;
var string: x_14;
var string: literal_13;
var string: x_17;
var string: x_16;
var string: epsilon;
var string: x_19;
var string: sigmaStar_20;
var string: literal_21;
var string: x_22;
var string: literal_23;
var string: x_24;
var string: literal_25;
var string: x_26;
var string: sigmaStar_29;
constraint
  (x_4) =
    (str_replace(sigmaStar_0,
       "\x2f\x5b\x5e\x30\x2d\x39\x61\x2d\x7a\x5c\x5c\x2d\x5f\x2c\x5d\x2b\x2f\x69",
       ""));
constraint (literal_6) = ("");
constraint ((x_8) = (literal_6)) \/ ((x_8) = (x_4));
constraint (literal_5) = ("");
constraint ((x_7) = (x_4)) \/ ((x_7) = (literal_5));
constraint (x_11) = ((x_8) ++ (x_7));
constraint (literal_9) = ("");
constraint ((x_10) = (literal_9)) \/ ((x_10) = (x_4));
constraint (x_15) = ((x_11) ++ (x_10));
constraint (literal_13) = ("\x65\x6e");
constraint ((x_14) = (x_4)) \/ ((x_14) = (literal_13));
constraint (x_17) = ((x_15) ++ (x_14));
constraint (epsilon) = ("");
constraint ((x_16) = (epsilon)) \/ ((x_16) = (x_4));
constraint (x_19) = ((x_17) ++ (x_16));
constraint (literal_21) = ("\x2f");
constraint (x_22) = ((sigmaStar_20) ++ (literal_21));
constraint (literal_23) = ("\x74\x69\x6e\x79\x6d\x63\x65\x5f");
constraint (x_24) = ((x_22) ++ (literal_23));
constraint (literal_25) = ("\x2e\x67\x7a");
constraint (x_26) = ((x_24) ++ (literal_25));
constraint
  str_reg(sigmaStar_29, "((.)*)(\x5c\x5c\x3c\x53\x43\x52\x49\x50\x54)((.)*)");
solve satisfy;
