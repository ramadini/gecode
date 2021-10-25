var string: I;
var string: B;
var string: J;
constraint
  (("fdbe") ++ (B) ++ ("bec")) = ((I) ++ ("dacdfffeadaabec") ++ (""));
constraint
  (("edcbfc") ++ (J) ++ ("dbcadccbfeedbfedaafbfeccfdeafc")) =
    (("edcbfc") ++ (J) ++ ("dbcadccbfeedbfedaafbfeccfdeafc"));
constraint
  (("aadeefbeeedbecfaafafeebb") ++ (J) ++ ("fceceedcaebd")) =
    (("aadeefbeeedbecfaafafeebbfbbefceceedcaebd") ++ (""));
constraint ((str_len (J)) * (4)) >= (16);
constraint ((str_len (J)) * (11)) <= (440);
constraint ((str_len (I)) * (11)) >= (253);
constraint ((str_len (I)) * (16)) <= (3520);
solve satisfy;
