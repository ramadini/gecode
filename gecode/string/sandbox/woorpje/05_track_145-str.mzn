var string: H;
var string: B;
var string: A;
var string: J;
var string: C;
var string: D;
constraint (("edbedfaedca") ++ (H) ++ ("cffbf")) = (("e") ++ (A) ++ ("bf"));
constraint
  (("ceeaa") ++ (A) ++ (B) ++ ("fafccbeecefbfaa")) =
    (("ceeaa") ++ (A) ++ ("bcbebdcd") ++ (J) ++ ("faa"));
constraint
  (("ba") ++ (H) ++ ("aacdbbfaf")) =
    (("baed") ++ (C) ++ ("fa") ++ (D) ++ ("acaacfdbfaeaacdbbfaf"));
constraint
  (("bcceecc") ++ (A) ++ ("fbdbbbdfeadd") ++ (D) ++ ("bfdb") ++ (D) ++ (D) ++ ("bf") ++ (D) ++ ("cfdcf") ++ (D) ++ ("bcd"))
    =
    (("b") ++ (A) ++ (D) ++ ("eccccfbdbbbdf") ++ (D) ++ ("add") ++ (D) ++ ("bfdb") ++ (D) ++ (D) ++ ("bf") ++ (D) ++ ("cfdcf") ++ (D) ++ ("bcd"));
constraint
  (("edbacabeeddcafffcf") ++ (D) ++ ("bdfbdceccca") ++ (A) ++ ("bd") ++ (D) ++ ("feadc"))
    =
    ((D) ++ ("dbacab") ++ (D) ++ ("eddcafffcfebdfbdcecccaccbd") ++ (D) ++ ("f") ++ (D) ++ ("adc"));
constraint ((str_len (C)) * (1)) <= (64);
constraint ((str_len (J)) * (10)) >= (120);
constraint ((str_len (D)) * (4)) <= (40);
constraint ((str_len (H)) * (10)) <= (1620);
constraint ((str_len (B)) * (14)) >= (112);
solve satisfy;
