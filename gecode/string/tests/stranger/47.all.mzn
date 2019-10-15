var string: sigmaStar_0;
var string: sigmaStar_3;
var string: sigmaStar_7;
var string: x_4;
var string: sigmaStar_11;
var string: x_8;
var string: literal_13;
var string: x_12;
var string: x_14;
var string: literal_15;
var string: x_16;
constraint
  (x_4) =
    (str_replace_all(sigmaStar_0,
       "\x2f\x5b\x4a\x6a\x5d\x5b\x41\x61\x5d\x5b\x56\x76\x5d\x5b\x41\x61\x5d\x5b\x53\x73\x5d\x5b\x43\x63\x5d\x5b\x52\x72\x5d\x5b\x49\x69\x5d\x5b\x50\x70\x5d\x5b\x54\x74\x5d\x2f",
       "\x6a\x61\x76\x61\x20\x73\x63\x72\x69\x70\x74"));
constraint
  (x_8) = (str_replace_all(x_4, "\x2f\x5c\x5c\x3c\x2f", "\x26\x6c\x74\x3b"));
constraint
  (x_12) = (str_replace_all(x_8, "\x2f\x5c\x5c\x3e\x2f", "\x26\x67\x74\x3b"));
constraint
  (literal_13) =
    ("\x3c\x70\x3e\x54\x69\x74\x6c\x65\x3a\x3c\x62\x72\x20\x2f\x3e");
constraint (x_14) = ((literal_13) ++ (x_12));
constraint (literal_15) = ("\x3c\x2f\x70\x3e");
constraint (x_16) = ((x_14) ++ (literal_15));
constraint str_reg(x_16, "((.)*)(\x5c\x5c\x3c\x53\x43\x52\x49\x50\x54)((.)*)");
solve satisfy;
