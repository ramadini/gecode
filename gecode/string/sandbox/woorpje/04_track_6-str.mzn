var string: B;
var string: A;
var string: C;
constraint
  ((A) ++ ("aaaaaa") ++ (A) ++ (A) ++ ("a")) =
    ((B) ++ ("aaaaaaaaaaaaaaaa") ++ (""));
constraint
  (("aaaaaaaaaaaaaaaaaaaaaaaaa") ++ ("")) =
    (("aaaaaaaaa") ++ (B) ++ ("aaaaaaa"));
constraint
  ((A) ++ ("aaaaaa") ++ (A) ++ (A) ++ ("a")) = ((B) ++ (B) ++ (A) ++ ("a"));
constraint
  ((A) ++ (A) ++ (A) ++ (A) ++ ("a")) = (("aaaaaaaaa") ++ (B) ++ ("aaaaaaa"));
constraint
  (("aaaaaa") ++ (A) ++ ("aaaaaaaaaaaaa")) = ((B) ++ (A) ++ (A) ++ ("aaaa"));
constraint
  (("aaaaaa") ++ (A) ++ (A) ++ (A) ++ ("a")) =
    (("aaaaaaaaa") ++ (B) ++ ("aaaaaaa"));
constraint
  (("aaaaaa") ++ (A) ++ (A) ++ (A) ++ ("a")) =
    (("aaaaaaaaa") ++ (B) ++ (A) ++ ("a"));
constraint
  (("aaaaaa") ++ (A) ++ (A) ++ ("aaaaaaa")) = ((B) ++ (B) ++ ("aaaaaaa"));
constraint
  ((A) ++ (A) ++ (A) ++ (A) ++ ("a")) =
    ((A) ++ (A) ++ ("aaaaaa") ++ (A) ++ ("a"));
constraint
  (("aaaaaa") ++ (A) ++ (A) ++ (A) ++ ("a")) =
    ((A) ++ ("aaa") ++ (B) ++ ("aaaaaaa"));
constraint
  ((A) ++ (B) ++ ("aaa") ++ (A) ++ ("a")) = ((B) ++ (B) ++ (A) ++ ("a"));
constraint
  (("aaaaaa") ++ (A) ++ (B) ++ ("aaaa")) =
    ((A) ++ ("aaaaaa") ++ (A) ++ ("aaaaaaa"));
constraint ((A) ++ (A) ++ (A) ++ ("aaaaaaa")) = ((B) ++ (B) ++ (A) ++ ("a"));
constraint
  (("aaaaaa") ++ (A) ++ ("aaaaaa") ++ (A) ++ ("a")) =
    ((B) ++ (B) ++ ("aaaaaaa"));
constraint ((A) ++ (A) ++ ("aaaaaaaaaaaaa")) = ((B) ++ (B) ++ ("aaaaaaa"));
constraint
  ((B) ++ ("aaa") ++ (A) ++ (A) ++ ("a")) = ((B) ++ (A) ++ ("aaaaaaaaaa"));
constraint
  ((A) ++ ("aaaaaa") ++ (A) ++ (A) ++ ("a")) = ((B) ++ (B) ++ ("aaaaaaa"));
constraint
  (("aaaaaa") ++ (A) ++ ("aaaaaa") ++ (A) ++ ("a")) =
    ((B) ++ ("aaaaaaaaaaaaaaaa") ++ (""));
constraint
  (("aaaaaa") ++ (A) ++ (A) ++ (A) ++ ("a")) =
    (("aaaaaaaaa") ++ (B) ++ (A) ++ ("a"));
constraint
  ((A) ++ ("aaaaaa") ++ (A) ++ ("aaaaaaa")) = ((B) ++ (B) ++ ("aaaaaaa"));
constraint
  (("aaaaaa") ++ (A) ++ ("aaaaaaaaaaaaa")) =
    (("aaaaaaaaa") ++ (B) ++ (A) ++ ("a"));
constraint
  (("aaaaaa") ++ (A) ++ ("aaaaaaaaaaaaa")) =
    ((A) ++ ("aaaaaaaaaaaa") ++ (A) ++ ("a"));
constraint ((A) ++ (C) ++ ("aaaaa")) = ((B) ++ (B) ++ (A) ++ ("a"));
constraint
  ((A) ++ (A) ++ (B) ++ ("aaaa")) = ((B) ++ ("aaaaaa") ++ (A) ++ ("aaaa"));
constraint
  (("aaaaaa") ++ (A) ++ (A) ++ ("aaaaaaa")) = ((B) ++ (B) ++ ("aaaaaaa"));
constraint
  ((A) ++ ("aaaaaaaaaaaa") ++ (A) ++ ("a")) = ((B) ++ (B) ++ (A) ++ ("a"));
constraint
  (("aaaaaaaaaaaa") ++ (A) ++ ("aaaaaaa")) = ((B) ++ (B) ++ (A) ++ ("a"));
constraint
  ((A) ++ ("aaaaaaaaaaaa") ++ (A) ++ ("a")) = ((B) ++ (A) ++ ("aaaaaaaaaa"));
constraint
  ((A) ++ ("aaaaaaaaa") ++ (B) ++ ("a")) =
    (("aaaaaaaaa") ++ (B) ++ (A) ++ ("a"));
constraint
  ((A) ++ (A) ++ ("aaaaaa") ++ (A) ++ ("a")) = ((B) ++ (B) ++ (A) ++ ("a"));
constraint
  (("aaaaaa") ++ (A) ++ (A) ++ ("aaaaaaa")) =
    (("aaaaaaaaa") ++ (B) ++ (A) ++ ("a"));
constraint
  ((A) ++ ("aaaaaa") ++ (A) ++ ("aaaaaaa")) = ((B) ++ (B) ++ ("aaaaaaa"));
constraint
  (("aaaaaaaaaaaa") ++ (A) ++ (A) ++ ("a")) =
    ((A) ++ ("aaa") ++ (B) ++ (A) ++ ("a"));
constraint
  ((A) ++ (B) ++ ("aaa") ++ (A) ++ ("a")) = ((B) ++ (B) ++ (A) ++ ("a"));
solve satisfy;
