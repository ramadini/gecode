var string: B;
var string: E;
var string: J;
var string: H;
var string: F;
var string: G;
var string: A;
var string: D;
constraint
  (("dcaacfb") ++ (F) ++ ("afaeaeee")) =
    (("dcaacfb") ++ (E) ++ ("aedb") ++ (B) ++ ("caadafaeaeee"));
constraint
  (("cfe") ++ (A) ++ ("afcfb")) =
    (("c") ++ (D) ++ ("bcffdf") ++ (J) ++ ("dcaedffaeabafbafcfb"));
constraint
  (("caaafbbfebacaefadbfbbbdedefafeddbcffaeae") ++ ("")) =
    (("caaafbbfebacaefadbfbbbdedefaf") ++ (J) ++ ("faeae"));
constraint
  (("add") ++ (E) ++ ("ece") ++ (E) ++ ("cdb") ++ (G) ++ ("ecf") ++ (E) ++ ("b") ++ (F) ++ ("bbcba"))
    = (("a") ++ (H) ++ ("dbdafaaeddbbcba"));
constraint
  (("bafdbdcfbef") ++ (E) ++ ("ddb") ++ (E) ++ ("dddbde") ++ (E) ++ ("fda") ++ (E) ++ ("efcab") ++ (E) ++ ("d") ++ (E) ++ ("fffab"))
    =
    (("bafdbdcfb") ++ (E) ++ ("f") ++ (E) ++ ("ddbedddbdeefdaeefcabedefffab"));
constraint
  (("cdc") ++ (E) ++ ("cdadfcbcffedcbaaf") ++ (E) ++ ("ddbfccdfbfdaecfccc"))
    =
    (("cdcecdadfcbcff") ++ (E) ++ ("dcbaafeddbfccdfbfda") ++ (E) ++ ("cfccc"));
constraint ((str_len (J)) * (6)) <= (324);
constraint ((str_len (H)) * (19)) <= (874);
constraint ((str_len (E)) * (20)) <= (20);
constraint ((str_len (F)) * (20)) <= (2780);
constraint ((str_len (A)) * (19)) <= (684);
constraint ((str_len (D)) * (10)) >= (10);
constraint ((str_len (G)) * (4)) >= (4);
constraint ((str_len (G)) * (3)) <= (60);
solve satisfy;
