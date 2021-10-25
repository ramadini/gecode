var string: G;
var string: A;
var string: C;
var string: D;
constraint
  (("fbf") ++ (C) ++ ("eb") ++ (D) ++ ("bfadcffbfebcc")) =
    (("fbfadceeacf") ++ (D) ++ ("c") ++ (D) ++ ("ebaeb") ++ (G) ++ ("bcc"));
constraint
  (("cebcfffefedf") ++ (D) ++ ("abbaae") ++ (D) ++ ("afcedbfdfcaec") ++ (D) ++ ("efadaa"))
    =
    (("cebcfffefedfdabb") ++ (A) ++ ("edafcedbfdfcaec") ++ (D) ++ ("efad") ++ (A));
constraint
  (("bbfbfeeeaad") ++ (A) ++ ("dfec") ++ (D) ++ ("cd") ++ (A) ++ ("eb") ++ (D) ++ ("eddcbbcefcacaca"))
    =
    (("bbfbfeeeaadaa") ++ (D) ++ ("fecdc") ++ (D) ++ (A) ++ ("ebdeddcbbcefcacaca"));
constraint ((str_len (G)) * (7)) >= (42);
constraint ((str_len (C)) * (20)) <= (700);
constraint ((str_len (D)) * (12)) >= (12);
constraint ((str_len (D)) * (6)) <= (54);
constraint ((str_len (A)) * (16)) >= (32);
constraint ((str_len (A)) * (18)) <= (594);
solve satisfy;
