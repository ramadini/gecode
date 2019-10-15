var string: sigmaStar_0;
var string: sigmaStar_1;
var string: sigmaStar_7;
var string: epsilon;
var string: literal_11;
var string: x_8;
var string: x_12;
var string: literal_13;
var string: x_14;
constraint (epsilon) = ("");
constraint
  (x_8) = (str_replace_all(epsilon, "\x2f\x5b\x2e\x7c\x20\x5d\x2f", "\x5f"));
constraint
  (literal_11) =
    ("\x3c\x63\x65\x6e\x74\x65\x72\x3e\x3c\x66\x6f\x6e\x74\x20\x63\x6f\x6c\x6f\x72\x3d\x22\x72\x65\x64\x22\x3e\x6e\x6f\x6e\x75\x6d\x65\x72\x69\x63\x77\x65\x69\x67\x68\x74\x3a\x20\x22");
constraint (x_12) = ((literal_11) ++ (x_8));
constraint
  (literal_13) =
    ("\x22\x3c\x2f\x66\x6f\x6e\x74\x3e\x3c\x2f\x63\x65\x6e\x74\x65\x72\x3e\x3c\x62\x72\x20\x2f\x3e");
constraint (x_14) = ((x_12) ++ (literal_13));
constraint str_reg(x_14, "((.)*)(\x5c\x5c\x3c\x53\x43\x52\x49\x50\x54)((.)*)");
solve satisfy;
