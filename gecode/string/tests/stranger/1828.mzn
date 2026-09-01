var string: sigmaStar_0;
var string: sigmaStar_1;
var string: sigmaStar_2;
var string: sigmaStar_3;
var string: sigmaStar_4;
var string: sigmaStar_5;
var string: sigmaStar_9;
var string: epsilon;
var string: sigmaStar_18;
var string: sigmaStar_20;
var string: x_10;
var string: sigmaStar_26;
var string: sigmaStar_35;
var string: x_21;
var string: literal_37;
var string: x_43;
var string: literal_42;
var string: x_36;
var string: x_50;
var string: literal_39;
var string: x_46;
var string: x_53;
var string: literal_45;
var string: x_52;
var string: x_58;
var string: literal_48;
var string: x_55;
var string: x_60;
var string: literal_49;
var string: x_56;
var string: x_61;
var string: literal_51;
var string: x_57;
var string: x_62;
var string: literal_59;
var string: x_64;
var string: sigmaStar_69;
var string: literal_63;
var string: x_71;
var string: sigmaStar_72;
var string: literal_54;
var string: literal_67;
var string: x_74;
var string: literal_68;
var string: x_75;
var string: literal_70;
var string: x_76;
var string: x_77;
var string: x_78;
var string: x_79;
var string: x_80;
var string: x_81;
var string: x_73;
var string: x_82;
var string: literal_84;
var string: x_89;
var string: literal_83;
var string: x_90;
var string: literal_91;
var string: x_93;
var string: literal_92;
var string: x_94;
var string: literal_95;
var string: x_96;
constraint (epsilon) = ("");
constraint (x_10) = (str_replace(epsilon, "\x26\x6c\x74\x3b", "\x3c"));
constraint (x_21) = (str_replace(x_10, "\x26\x67\x74\x3b", "\x3e"));
constraint
  (x_36) = (str_replace(x_21, "\x21\x5c\x5c\x72\x5c\x5c\x6e\x3f\x21", "\x20"));
constraint (literal_37) = ("\x2f\x2f");
constraint (x_43) = ((literal_37) ++ (sigmaStar_26));
constraint (literal_42) = ("\x5c\x5c\x4c\x61\x72\x67\x65\x20");
constraint (x_50) = ((literal_42) ++ (x_36));
constraint (literal_39) = ("\x20\x2b\x2b\x20\x2d\x65\x20\x20\x5c\x5c\x22");
constraint ((x_46) = (x_43)) \/ ((x_46) = (sigmaStar_3));
constraint (x_53) = ((literal_39) ++ (x_46));
constraint
  (literal_45) = ("\x2f\x2f\x6d\x69\x6d\x65\x74\x65\x78\x20\x2d\x65\x20");
constraint ((x_52) = (x_43)) \/ ((x_52) = (sigmaStar_4));
constraint (x_58) = ((literal_45) ++ (x_52));
constraint
  (literal_48) =
    ("\x5c\x5c\x22\x2f\x2f\x6d\x69\x6d\x65\x74\x65\x78\x2e\x66\x72\x65\x65\x62\x73\x64\x5c\x5c\x22\x20\x2d\x65\x20\x5c\x5c\x22");
constraint ((x_55) = (x_43)) \/ ((x_55) = (sigmaStar_0));
constraint (x_60) = ((literal_48) ++ (x_55));
constraint
  (literal_49) =
    ("\x5c\x5c\x22\x2f\x2f\x6d\x69\x6d\x65\x74\x65\x78\x2e\x64\x61\x72\x77\x69\x6e\x5c\x5c\x22\x20\x2d\x65\x20\x5c\x5c\x22");
constraint ((x_56) = (x_43)) \/ ((x_56) = (sigmaStar_5));
constraint (x_61) = ((literal_49) ++ (x_56));
constraint
  (literal_51) =
    ("\x5c\x5c\x22\x2f\x2f\x6d\x69\x6d\x65\x74\x65\x78\x2e\x6c\x69\x6e\x75\x78\x5c\x5c\x22\x20\x2d\x65\x20\x5c\x5c\x22");
constraint ((x_57) = (x_43)) \/ ((x_57) = (sigmaStar_2));
constraint (x_62) = ((literal_51) ++ (x_57));
constraint (literal_59) = ("\x5c\x5c\x22\x20\x2d\x2d\x20");
constraint (x_64) = ((x_53) ++ (literal_59));
constraint (literal_63) = ("\x20\x2d\x2d\x20");
constraint (x_71) = ((x_58) ++ (literal_63));
constraint
  (literal_54) = ("\x2f\x2f\x6d\x69\x6d\x65\x74\x65\x78\x2e\x65\x78\x65");
constraint (x_73) = (str_replace(literal_54, "\x20", "\x5e\x20"));
constraint (literal_67) = ("\x5c\x5c\x22\x20");
constraint (x_74) = ((x_60) ++ (literal_67));
constraint (literal_68) = ("\x5c\x5c\x22\x20\x2d\x2d\x20");
constraint (x_75) = ((x_61) ++ (literal_68));
constraint (literal_70) = ("\x5c\x5c\x22\x20\x2d\x2d\x20");
constraint (x_76) = ((x_62) ++ (literal_70));
constraint (x_77) = ((x_64) ++ (sigmaStar_69));
constraint (x_78) = ((x_71) ++ (sigmaStar_69));
constraint (x_79) = ((x_74) ++ (sigmaStar_69));
constraint (x_80) = ((x_75) ++ (sigmaStar_69));
constraint (x_81) = ((x_76) ++ (sigmaStar_69));
constraint (x_82) = ((x_73) ++ (x_77));
constraint
  (literal_84) =
    ("\x54\x68\x65\x20\x73\x68\x65\x6c\x6c\x20\x63\x6f\x6d\x6d\x61\x6e\x64\x3c\x62\x72\x20\x2f\x3e");
constraint (literal_83) = ("");
constraint
  ((x_89) = (x_82)) \/ ((x_89) = (literal_83)) \/ ((x_89) = (x_81)) \/ (
  (x_89) = (x_80)) \/ ((x_89) = (x_79)) \/ ((x_89) = (x_78));
constraint (x_90) = ((literal_84) ++ (x_89));
constraint
  (literal_91) =
    ("\x3c\x62\x72\x20\x2f\x3e\x72\x65\x74\x75\x72\x6e\x65\x64\x20\x73\x74\x61\x74\x75\x73\x20\x3d\x20");
constraint (x_93) = ((x_90) ++ (literal_91));
constraint (literal_92) = ("");
constraint (x_94) = ((x_93) ++ (literal_92));
constraint (literal_95) = ("\x3c\x62\x72\x20\x2f\x3e\x5c\x5c\x6e");
constraint (x_96) = ((x_94) ++ (literal_95));
constraint str_reg(x_96, "((.)*)(\x5c\x5c\x3c\x53\x43\x52\x49\x50\x54)((.)*)");
solve satisfy;
