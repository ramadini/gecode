var string: sigmaStar_0;
var string: sigmaStar_1;
var string: sigmaStar_3;
var string: sigmaStar_8;
var string: literal_11;
var string: x_10;
var string: x_12;
var string: sigmaStar_42;
var string: sigmaStar_46;
var string: literal_32;
var string: literal_39;
var string: x_49;
var string: literal_44;
var string: x_50;
var string: literal_45;
var string: x_51;
var string: literal_48;
var string: x_52;
var string: x_47;
var string: x_54;
var string: literal_55;
var string: x_60;
var string: epsilon;
var string: literal_53;
var string: x_61;
var string: literal_62;
var string: x_63;
var string: literal_64;
var string: x_65;
constraint (literal_11) = ("\x5c\x5c\x4c\x61\x72\x67\x65\x20");
constraint ((x_10) = (sigmaStar_8));
constraint (x_12) = ((literal_11) ++ (x_10));
constraint
  (literal_32) = ("\x2f\x2f\x6d\x69\x6d\x65\x74\x65\x78\x2e\x65\x78\x65");
constraint (x_47) = (str_replace_all(literal_32, "\x20", "\x5e\x20"));
constraint
  (literal_39) =
    ("\x20\x2b\x2b\x20\x2d\x65\x20\x20\x5c\x5c\x22\x2f\x2f\x2e\x67\x69\x66\x5c\x5c\x22\x20");
constraint (x_49) = ((literal_39) ++ (sigmaStar_42));
constraint
  (literal_44) =
    ("\x5c\x5c\x22\x2f\x2f\x6d\x69\x6d\x65\x74\x65\x78\x2e\x64\x61\x72\x77\x69\x6e\x5c\x5c\x22\x20\x2d\x65\x20\x5c\x5c\x22\x2f\x2f\x2e\x67\x69\x66\x5c\x5c\x22\x20");
constraint (x_50) = ((literal_44) ++ (sigmaStar_42));
constraint
  (literal_45) =
    ("\x5c\x5c\x22\x2f\x2f\x6d\x69\x6d\x65\x74\x65\x78\x2e\x6c\x69\x6e\x75\x78\x5c\x5c\x22\x20\x2d\x65\x20\x5c\x5c\x22\x2f\x2f\x2e\x67\x69\x66\x5c\x5c\x22\x20");
constraint (x_51) = ((literal_45) ++ (sigmaStar_42));
constraint
  (literal_48) =
    ("\x2f\x2f\x6d\x69\x6d\x65\x74\x65\x78\x20\x2d\x65\x20\x2f\x2f\x2e\x67\x69\x66\x20");
constraint (x_52) = ((literal_48) ++ (sigmaStar_42));
constraint (x_54) = ((x_47) ++ (x_49));
constraint
  (literal_55) =
    ("\x54\x68\x65\x20\x73\x68\x65\x6c\x6c\x20\x63\x6f\x6d\x6d\x61\x6e\x64\x3c\x62\x72\x20\x2f\x3e");
constraint (epsilon) = ("");
constraint (literal_53) = ("");
constraint
  ((x_60) = (epsilon)) \/ ((x_60) = (x_54)) \/ ((x_60) = (literal_53)) \/ (
  (x_60) = (x_52)) \/ ((x_60) = (x_51)) \/ ((x_60) = (x_50));
constraint (x_61) = ((literal_55) ++ (x_60));
constraint
  (literal_62) =
    ("\x3c\x62\x72\x20\x2f\x3e\x72\x65\x74\x75\x72\x6e\x65\x64\x20\x73\x74\x61\x74\x75\x73\x20\x3d\x20");
constraint (x_63) = ((x_61) ++ (literal_62));
constraint (literal_64) = ("\x3c\x62\x72\x20\x2f\x3e\x5c\x5c\x6e");
constraint (x_65) = ((x_63) ++ (literal_64));
constraint str_reg(x_65, "((.)*)(\x5c\x5c\x3c\x53\x43\x52\x49\x50\x54)((.)*)");
solve satisfy;
