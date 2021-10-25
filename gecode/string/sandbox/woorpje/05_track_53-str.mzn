var string: B;
var string: F;
var string: E;
constraint
  (("bfafd") ++ (B) ++ ("cedbcffb")) = (("bfafdceaed") ++ (F) ++ ("bcffb"));
constraint
  (("eefedbcbfec") ++ (B) ++ (E) ++ ("ada")) =
    ((F) ++ ("dacbfeabbdbdcdccefcada") ++ (""));
constraint
  (("cacaeecafabdaaeebaeffec") ++ (B) ++ ("fcdbdecbefeea")) =
    (("cacaeecafabdaaeebaeffec") ++ (B) ++ ("fcdbdecbefeea"));
constraint
  (("eebeccbfaadbbcefaefcbccddecfffac") ++ (B) ++ ("afac")) =
    (("eebeccbfaadbbcefaefcbccddecfffac") ++ (B) ++ ("afac"));
constraint ((str_len (B)) * (13)) >= (52);
constraint ((str_len (B)) * (17)) <= (306);
constraint ((str_len (F)) * (3)) >= (45);
constraint ((str_len (E)) * (11)) <= (1188);
solve satisfy;
