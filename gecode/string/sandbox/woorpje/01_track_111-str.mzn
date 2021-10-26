var string: I;
var string: D;
var string: A;
constraint
  (("dbebgddbecfcbb") ++ (A) ++ ("adeeaec") ++ (A) ++ ("gebegeecafegebdbagddaadbddcaeeebfabfefabfacdg") ++ (A) ++ ("gaabgegagf"))
    =
    (("dbebgddbe") ++ (A) ++ ("fcbb") ++ (A) ++ ("a") ++ (I) ++ ("egee") ++ (A) ++ ("a") ++ (D) ++ ("egagf"));
solve satisfy;

%FIXME: Remove these constraints:
constraint str_len(I) <= 100 /\ str_len(D) <= 100 /\ str_len(A) <= 100;
constraint str_range(I, "a", "g") /\ str_range(D, "a", "g")  /\ 
           str_range(A, "a", "g");
