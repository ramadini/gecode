var string: sigmaStar_0;
var string: sigmaStar_1;
var string: sigmaStar_2;
var string: sigmaStar_5;
var string: sigmaStar_8;
var string: x_4;
var string: epsilon;
var string: sigmaStar_10;
var string: sigmaStar_12;
var string: literal_13;
var string: x_14;
var string: literal_15;
var string: x_16;
var string: x_9;
constraint (epsilon) = ("");
constraint ((x_4) = (epsilon)) \/ ((x_4) = (sigmaStar_1));
constraint
  (x_9) =
    (str_replace(x_4, "\x2f\x2e\x28\x5c\x5c\x64\x2b\x29\x2e\x2f",
       "\x5f\x24\x31\x2e"));
constraint (literal_13) = ("\x20\x20\x20\x20");
constraint (x_14) = ((literal_13) ++ (sigmaStar_12));
constraint
  (literal_15) =
    ("\x2e\x73\x63\x6f\x72\x65\x2e\x6d\x69\x6e\x20\x3d\x20\x27\x27\x3b\x5c\x5c\x6e");
constraint (x_16) = ((x_14) ++ (literal_15));
constraint str_reg(x_16, "((.)*)(\x5c\x5c\x3c\x53\x43\x52\x49\x50\x54)((.)*)");
solve satisfy;
