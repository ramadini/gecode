var string: I;
var string: H;
var string: B;
var string: J;
constraint
  ((I) ++ ("ddffdadbe") ++ ("")) =
    (("eaceffaeefdcd") ++ (B) ++ ("fcffebbfddffdadbe"));
constraint
  (("cbeacfcdbeceafaaffebcaee") ++ (J) ++ ("cfedabafefdced")) =
    (("cbeacfcdbeceafaaffebc") ++ (J) ++ ("e") ++ (J) ++ ("cfedabafefdced"));
constraint
  (("dfadabdfbbeebfcecedfdcba") ++ (H) ++ ("afbbdfdadaaca")) =
    (("dfadabdfbbeebfcecedfdcbaacfafbbdfdadaaca") ++ (""));
constraint ((str_len (I)) * (15)) >= (195);
constraint ((str_len (J)) * (16)) >= (16);
constraint ((str_len (J)) * (10)) <= (180);
solve satisfy;
