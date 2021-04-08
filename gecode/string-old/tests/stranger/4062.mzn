var string: sigmaStar_0;
var string: literal_2;
var string: x_7;
var string: literal_3;
var string: x_6;
var string: literal_4;
var string: literal_5;
var string: x_12;
var string: sigmaStar_13;
var string: x_8;
var string: epsilon;
var string: literal_11;
var string: x_15;
var string: literal_16;
var string: x_17;
var string: x_18;
var string: x_14;
var string: x_20;
var string: literal_19;
var string: x_22;
var string: literal_21;
var string: x_23;
var string: literal_24;
var string: x_26;
var string: x_27;
var string: literal_25;
var string: x_28;
var string: x_32;
var string: literal_33;
var string: x_34;
var string: x_31;
var string: x_35;
var string: x_36;
constraint (literal_2) = ("");
constraint (x_7) = ((literal_2) ++ (sigmaStar_0));
constraint
  (literal_3) =
    ("\x3c\x74\x72\x3e\x0d\x0a\x3c\x74\x64\x20\x63\x6c\x61\x73\x73\x3d\x27");
constraint (literal_4) = ("\x74\x62\x6c\x32");
constraint (literal_5) = ("\x74\x62\x6c\x31");
var 1..2: b;
constraint x_6 = [literal_4, literal_5][b];
constraint (x_12) = ((literal_3) ++ (x_6));
constraint (epsilon) = ("");
constraint ((x_8) = (epsilon));
constraint (x_14) = (str_replace(x_8, "\x2e", "\x20"));
constraint (literal_11) = ("");
constraint (x_15) = ((x_7) ++ (literal_11));
constraint
  (literal_16) = ("\x3e\x3c\x73\x70\x61\x6e\x20\x74\x69\x74\x6c\x65\x3d");
constraint (x_17) = ((x_12) ++ (literal_16));
constraint ((x_18) = (x_15)) \/ ((x_18) = (x_14));
constraint (x_20) = ((x_17) ++ (x_18));
constraint
  (literal_19) =
    ("\x3c\x2f\x73\x70\x61\x6e\x3e\x3c\x2f\x74\x64\x3e\x0d\x0a\x3c\x74\x64\x20\x61\x6c\x69\x67\x6e\x3d\x27\x63\x65\x6e\x74\x65\x72\x27\x20\x77\x69\x64\x74\x68\x3d\x27\x31\x25\x27\x20\x63\x6c\x61\x73\x73\x3d\x27");
constraint (x_22) = ((literal_19) ++ (x_6));
constraint
  (literal_21) =
    ("\x27\x20\x73\x74\x79\x6c\x65\x3d\x27\x63\x75\x72\x73\x6f\x72\x3a\x68\x61\x6e\x64\x3b\x27\x3e");
constraint (x_23) = ((x_20) ++ (literal_21));
constraint
  (literal_24) =
    ("\x20\x73\x74\x79\x6c\x65\x3d\x77\x68\x69\x74\x65\x2d\x73\x70\x61\x63\x65\x3a\x6e\x6f\x77\x72\x61\x70\x27\x3e");
constraint (x_26) = ((x_22) ++ (literal_24));
constraint (x_27) = ((x_23) ++ (x_8));
constraint
  (literal_25) =
    ("\x3c\x2f\x74\x64\x3e\x0d\x0a\x3c\x74\x64\x20\x61\x6c\x69\x67\x6e\x3d\x27\x63\x65\x6e\x74\x65\x72\x27\x20\x77\x69\x64\x74\x68\x3d\x27\x31\x25\x27\x20\x63\x6c\x61\x73\x73\x3d\x27");
constraint (x_28) = ((literal_25) ++ (x_6));
constraint (x_32) = ((x_27) ++ (x_26));
constraint
  (literal_33) =
    ("\x20\x73\x74\x79\x6c\x65\x3d\x77\x68\x69\x74\x65\x2d\x73\x70\x61\x63\x65\x3a\x6e\x6f\x77\x72\x61\x70\x27\x3e\x5c\x5c\x6e");
constraint (x_34) = ((x_28) ++ (literal_33));
constraint ((x_31) = (epsilon));
constraint (x_35) = ((x_32) ++ (x_31));
constraint (x_36) = ((x_35) ++ (x_34));
constraint str_reg(x_36, "((.)*)(\x5c\x5c\x3c\x53\x43\x52\x49\x50\x54)((.)*)");
solve satisfy;
