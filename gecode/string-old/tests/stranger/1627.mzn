var string: sigmaStar_0;
var string: sigmaStar_3;
var string: sigmaStar_6;
var string: x_2;
var string: epsilon;
var string: literal_8;
var string: x_7;
var string: x_9;
var string: literal_10;
var string: x_11;
var string: x_12;
var string: literal_13;
var string: x_14;
constraint (epsilon) = ("");
constraint ((x_2) = (epsilon)) \/ ((x_2) = (sigmaStar_0));
constraint
  (x_7) =
    (str_replace(x_2, "\x2f\x2e\x28\x5c\x5c\x64\x2b\x29\x2e\x2f",
       "\x5f\x24\x31\x2e"));
constraint (literal_8) = ("\x20\x20\x20\x20");
constraint (x_9) = ((literal_8) ++ (x_7));
constraint (literal_10) = ("\x20\x3d\x20\x27");
constraint (x_11) = ((x_9) ++ (literal_10));
constraint (x_12) = ((x_11) ++ (x_2));
constraint (literal_13) = ("\x27\x3b\x5c\x5c\x6e");
constraint (x_14) = ((x_12) ++ (literal_13));
constraint str_reg(x_14, "((.)*)(\x5c\x5c\x3c\x53\x43\x52\x49\x50\x54)((.)*)");
solve satisfy;
