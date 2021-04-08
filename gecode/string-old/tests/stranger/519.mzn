var string: sigmaStar_0;
var string: sigmaStar_1;
var string: sigmaStar_2;
var string: sigmaStar_3;
var string: sigmaStar_6;
var string: sigmaStar_8;
var string: sigmaStar_10;
var string: literal_11;
var string: x_12;
var string: literal_13;
var string: x_14;
var string: x_7;
constraint
  (x_7) =
    (str_replace(sigmaStar_0, "\x2f\x2e\x28\x5c\x5c\x64\x2b\x29\x2e\x2f",
       "\x5f\x24\x31\x2e"));
constraint (literal_11) = ("\x20\x20\x20\x20");
constraint (x_12) = ((literal_11) ++ (sigmaStar_10));
constraint
  (literal_13) =
    ("\x2e\x73\x63\x6f\x72\x65\x20\x3d\x20\x6e\x65\x77\x20\x4f\x62\x6a\x65\x63\x74\x28\x29\x3b\x5c\x5c\x6e");
constraint (x_14) = ((x_12) ++ (literal_13));
constraint str_reg(x_14, "((.)*)(\x5c\x5c\x3c\x53\x43\x52\x49\x50\x54)((.)*)");
solve satisfy;
