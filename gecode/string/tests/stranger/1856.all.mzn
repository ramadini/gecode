var string: sigmaStar_0;
var string: sigmaStar_1;
var string: sigmaStar_2;
var string: sigmaStar_3;
var string: sigmaStar_4;
var string: sigmaStar_8;
var string: epsilon;
var string: sigmaStar_16;
var string: sigmaStar_18;
var string: x_9;
var string: sigmaStar_23;
var string: sigmaStar_31;
var string: x_19;
var string: literal_33;
var string: x_38;
var string: literal_37;
var string: x_32;
var string: x_44;
var string: literal_36;
var string: x_42;
var string: x_48;
var string: literal_40;
var string: x_46;
var string: x_51;
var string: literal_41;
var string: x_47;
var string: x_52;
var string: literal_45;
var string: x_50;
var string: x_54;
var string: literal_53;
var string: x_57;
var string: sigmaStar_60;
var string: literal_55;
var string: x_62;
var string: literal_56;
var string: x_63;
var string: sigmaStar_64;
var string: literal_49;
var string: literal_61;
var string: x_66;
var string: x_67;
var string: x_68;
var string: x_69;
var string: x_70;
var string: x_65;
var string: x_72;
var string: literal_73;
var string: x_77;
var string: literal_71;
var string: x_78;
var string: literal_79;
var string: x_81;
var string: literal_80;
var string: x_82;
var string: literal_83;
var string: x_84;
constraint (epsilon) = ("");
constraint (x_9) = (str_replace_all(epsilon, "\x26\x6c\x74\x3b", "\x3c"));
constraint (x_19) = (str_replace_all(x_9, "\x26\x67\x74\x3b", "\x3e"));
constraint
  (x_32) = (str_replace_all(x_19, "\x21\x5c\x5c\x72\x5c\x5c\x6e\x3f\x21", "\x20"));
constraint (literal_33) = ("\x2f\x2f");
constraint (x_38) = ((literal_33) ++ (sigmaStar_23));
constraint (literal_37) = ("\x5c\x5c\x4c\x61\x72\x67\x65\x20");
constraint (x_44) = ((literal_37) ++ (x_32));
constraint (literal_36) = ("\x20\x2b\x2b\x20\x2d\x65\x20\x20\x5c\x5c\x22");
constraint ((x_42) = (x_38)) \/ ((x_42) = (sigmaStar_0));
constraint (x_48) = ((literal_36) ++ (x_42));
constraint
  (literal_40) =
    ("\x5c\x5c\x22\x2f\x2f\x6d\x69\x6d\x65\x74\x65\x78\x2e\x64\x61\x72\x77\x69\x6e\x5c\x5c\x22\x20\x2d\x65\x20\x5c\x5c\x22");
constraint ((x_46) = (x_38)) \/ ((x_46) = (sigmaStar_4));
constraint (x_51) = ((literal_40) ++ (x_46));
constraint
  (literal_41) =
    ("\x5c\x5c\x22\x2f\x2f\x6d\x69\x6d\x65\x74\x65\x78\x2e\x6c\x69\x6e\x75\x78\x5c\x5c\x22\x20\x2d\x65\x20\x5c\x5c\x22");
constraint ((x_47) = (sigmaStar_1)) \/ ((x_47) = (x_38));
constraint (x_52) = ((literal_41) ++ (x_47));
constraint
  (literal_45) = ("\x2f\x2f\x6d\x69\x6d\x65\x74\x65\x78\x20\x2d\x65\x20");
constraint ((x_50) = (sigmaStar_2)) \/ ((x_50) = (x_38));
constraint (x_54) = ((literal_45) ++ (x_50));
constraint (literal_53) = ("\x5c\x5c\x22\x20\x2d\x2d\x20");
constraint (x_57) = ((x_48) ++ (literal_53));
constraint (literal_55) = ("\x5c\x5c\x22\x20\x2d\x2d\x20");
constraint (x_62) = ((x_51) ++ (literal_55));
constraint (literal_56) = ("\x5c\x5c\x22\x20\x2d\x2d\x20");
constraint (x_63) = ((x_52) ++ (literal_56));
constraint
  (literal_49) = ("\x2f\x2f\x6d\x69\x6d\x65\x74\x65\x78\x2e\x65\x78\x65");
constraint (x_65) = (str_replace_all(literal_49, "\x20", "\x5e\x20"));
constraint (literal_61) = ("\x20\x2d\x2d\x20");
constraint (x_66) = ((x_54) ++ (literal_61));
constraint (x_67) = ((x_57) ++ (sigmaStar_60));
constraint (x_68) = ((x_62) ++ (sigmaStar_60));
constraint (x_69) = ((x_63) ++ (sigmaStar_60));
constraint (x_70) = ((x_66) ++ (sigmaStar_60));
constraint (x_72) = ((x_65) ++ (x_67));
constraint
  (literal_73) =
    ("\x54\x68\x65\x20\x73\x68\x65\x6c\x6c\x20\x63\x6f\x6d\x6d\x61\x6e\x64\x3c\x62\x72\x20\x2f\x3e");
constraint (literal_71) = ("");
constraint
  ((x_77) = (x_72)) \/ ((x_77) = (literal_71)) \/ ((x_77) = (x_70)) \/ (
  (x_77) = (x_69)) \/ ((x_77) = (x_68));
constraint (x_78) = ((literal_73) ++ (x_77));
constraint
  (literal_79) =
    ("\x3c\x62\x72\x20\x2f\x3e\x72\x65\x74\x75\x72\x6e\x65\x64\x20\x73\x74\x61\x74\x75\x73\x20\x3d\x20");
constraint (x_81) = ((x_78) ++ (literal_79));
constraint (literal_80) = ("");
constraint (x_82) = ((x_81) ++ (literal_80));
constraint (literal_83) = ("\x3c\x62\x72\x20\x2f\x3e\x5c\x5c\x6e");
constraint (x_84) = ((x_82) ++ (literal_83));
constraint str_reg(x_84, "((.)*)(\x5c\x5c\x3c\x53\x43\x52\x49\x50\x54)((.)*)");
solve satisfy;
