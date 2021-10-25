var string: B;
var string: E;
var string: J;
var string: H;
var string: I;
var string: G;
var string: F;
var string: A;
constraint
  (("fcedcabfdfc") ++ (H) ++ ("cbfbedeeaea")) =
    (("fcedcabf") ++ (I) ++ ("bedeeaea"));
constraint
  (("ae") ++ (H) ++ ("cecbfbaece")) =
    (("a") ++ (I) ++ ("ebcd") ++ (G) ++ ("ce"));
constraint
  (("eacace") ++ (B) ++ ("eeaebacdaf")) =
    (("eacacedfe") ++ (I) ++ ("cddeeaebacdaf"));
constraint
  (("fbcacba") ++ (F) ++ ("ddfeebbebaeefabaeffc")) =
    (("fbcacba") ++ (A) ++ ("c"));
constraint
  (("debdf") ++ (E) ++ (E) ++ ("acbf") ++ (E) ++ ("cafdeefebbcffdbcc") ++ (E) ++ ("eaa") ++ (E) ++ ("bdbbbe"))
    =
    ((B) ++ (E) ++ (E) ++ ("acbf") ++ (E) ++ ("c") ++ (I) ++ (J) ++ ("cffdbccae") ++ (E) ++ ("aabdbbbe"));
constraint
  (("ccfcd") ++ (E) ++ ("dbcbdbadfdecacb") ++ (E) ++ ("addfdbdfcedbdbebcf"))
    =
    (("ccfcd") ++ (E) ++ ("dbcbdb") ++ (E) ++ ("dfdecacb") ++ (E) ++ ("addfdbdfcedbdbebcf"));
constraint
  (("fffadfa") ++ (E) ++ ("dfddbfbcaafcebccefdc") ++ (A) ++ ("edbcfddef") ++ (E))
    =
    (("ff") ++ (G) ++ ("df") ++ (E) ++ ("adfddbfbc") ++ (E) ++ (E) ++ ("fcebccefdcafedbcfdde") ++ (G));
constraint ((str_len (I)) * (11)) <= (484);
constraint ((str_len (A)) * (11)) <= (165);
constraint ((str_len (G)) * (14)) >= (14);
constraint ((str_len (F)) * (8)) <= (1384);
solve satisfy;
