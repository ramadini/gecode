var string: B;
var string: A;
var string: C;
constraint
  ((B) ++ ("aaaaaa") ++ (C) ++ ("aaaaaa") ++ (B) ++ (B) ++ ("a")) =
    (("aaa") ++ (B) ++ ("aaaaaa") ++ (B) ++ ("aaa") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("a"));
constraint
  ((C) ++ (B) ++ ("aaa") ++ (B) ++ (C) ++ (B) ++ ("a")) =
    (("aaa") ++ (B) ++ ("aaa") ++ (B) ++ ("aaaaaa") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("a"));
constraint
  (("aaa") ++ (B) ++ ("aaa") ++ (C) ++ (A)) =
    (("aaaaaa") ++ (B) ++ ("aaa") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("a"));
constraint
  ((C) ++ (C) ++ (C) ++ (B) ++ ("a")) =
    (("aaaaaa") ++ (B) ++ (C) ++ ("aaa") ++ (B) ++ (B) ++ ("aaaa"));
constraint
  ((A) ++ (A) ++ ("aaaaa")) =
    ((B) ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("aaaaaaaaa") ++ (B) ++ ("aaaa"));
constraint
  ((A) ++ (B) ++ ("aa") ++ (C) ++ (B) ++ ("a")) =
    ((B) ++ (B) ++ ("aaa") ++ (B) ++ ("aaa") ++ (B) ++ (C) ++ ("aaaa"));
constraint
  ((B) ++ (B) ++ ("aaa") ++ (C) ++ ("aaaaaaaaaaaaa")) =
    ((B) ++ ("aaaaaaaaa") ++ (B) ++ (B) ++ (B) ++ ("aaaaaaaaaa"));
constraint
  ((C) ++ (C) ++ (C) ++ ("aaaa")) =
    (("aaa") ++ (B) ++ ("aaa") ++ (B) ++ (A) ++ ("aaaaaa"));
constraint
  ((C) ++ (A) ++ (B) ++ (B) ++ ("aaa")) =
    ((B) ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ (B) ++ (C) ++ ("aaaa"));
constraint
  ((C) ++ ("aaa") ++ (B) ++ ("aaa") ++ (C) ++ (B) ++ ("a")) =
    (("aaa") ++ (B) ++ ("aaa") ++ (B) ++ ("aaaaaa") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("a"));
constraint
  (("aaaaaaaaa") ++ (C) ++ (C) ++ ("aaaa")) =
    ((B) ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("aaa") ++ (B) ++ ("a"));
constraint
  ((A) ++ (B) ++ ("aa") ++ (C) ++ ("aaaa")) =
    ((B) ++ (B) ++ ("aaaaaa") ++ (B) ++ (B) ++ ("aaaaaa") ++ (B) ++ ("aaaa"));
constraint
  ((C) ++ (C) ++ ("aaaaaa") ++ (B) ++ (B) ++ ("a")) =
    ((B) ++ (B) ++ (B) ++ (B) ++ (B) ++ ("aaaaaa") ++ (B) ++ (B) ++ (B) ++ ("a"));
constraint
  ((C) ++ (C) ++ (A)) =
    (("aaa") ++ (B) ++ (B) ++ ("aaaaaa") ++ (B) ++ ("aaaaaa") ++ (B) ++ ("aaaa"));
constraint
  ((C) ++ (A) ++ (B) ++ (B) ++ ("aaa")) =
    (("aaa") ++ (B) ++ (B) ++ ("aaaaaa") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ (B) ++ ("a"));
constraint
  ((B) ++ (B) ++ ("aaa") ++ (C) ++ (A)) =
    ((B) ++ (C) ++ (B) ++ (B) ++ (B) ++ ("aaaaaaaaaa"));
constraint
  (("aaa") ++ (B) ++ (B) ++ (C) ++ ("aaaaaaaaaaaaa")) =
    ((B) ++ (B) ++ ("aaaaaa") ++ (B) ++ ("aaaaaa") ++ (B) ++ (B) ++ ("aaaa"));
constraint
  ((C) ++ (C) ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("a")) =
    ((B) ++ (B) ++ (B) ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("a"));
constraint
  (("aaa") ++ (B) ++ (B) ++ (C) ++ (C) ++ (B) ++ ("a")) =
    (("aaa") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("aaa") ++ (B) ++ ("a"));
constraint
  ((C) ++ (C) ++ (C) ++ (B) ++ ("a")) =
    ((B) ++ (B) ++ ("aaaaaaaaa") ++ (B) ++ (B) ++ ("aaaaaa") ++ (B) ++ ("a"));
constraint
  (("aaa") ++ (B) ++ ("aaaaaa") ++ (B) ++ ("aaa") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("a"))
    = ((C) ++ (B) ++ (B) ++ (C) ++ ("aaaaaaa"));
constraint
  ((C) ++ (C) ++ (A)) =
    ((B) ++ ("aaa") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("aaa") ++ (B) ++ ("a"));
constraint
  ((B) ++ (B) ++ ("aaa") ++ (C) ++ (B) ++ (B) ++ (B) ++ ("aaaa")) =
    ((B) ++ (B) ++ ("aaa") ++ (B) ++ ("aaaaaaaaaaaaaaa") ++ (B) ++ ("a"));
constraint
  ((B) ++ ("aaa") ++ (B) ++ (C) ++ (C) ++ ("aaaa")) =
    ((C) ++ (B) ++ (B) ++ ("aaaaaa") ++ (B) ++ ("aaaaaaa"));
constraint
  ((B) ++ ("aaa") ++ (B) ++ ("aaaaaa") ++ (B) ++ (B) ++ (B) ++ (B) ++ (B) ++ ("a"))
    =
    ((B) ++ ("aaaaaa") ++ (B) ++ (B) ++ ("aaaaaa") ++ (B) ++ (B) ++ (B) ++ ("a"));
constraint
  ((B) ++ (B) ++ (B) ++ (C) ++ (A)) =
    (("aaaaaa") ++ (B) ++ (B) ++ (B) ++ (B) ++ (B) ++ ("aaaaaa") ++ (B) ++ ("a"));
constraint
  ((C) ++ (B) ++ ("aaaaaa") ++ (C) ++ ("aaaa")) =
    ((B) ++ (C) ++ ("aaa") ++ (B) ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("a"));
constraint
  ((C) ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("aaaaaa") ++ (B) ++ ("a")) =
    (("aaa") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("a"));
constraint
  (("aaaaaa") ++ (B) ++ (C) ++ (C) ++ (B) ++ ("a")) =
    ((B) ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ (B) ++ ("a"));
constraint
  ((C) ++ (B) ++ (B) ++ ("aaaaaa") ++ (B) ++ ("aaaaaaa")) =
    ((B) ++ ("aaa") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("aaaa"));
constraint
  ((C) ++ (A) ++ (B) ++ (B) ++ (B)) =
    ((B) ++ ("aaa") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("aaa") ++ (B) ++ ("a"));
constraint
  ((C) ++ (C) ++ (A)) =
    (("aaaaaa") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("aaaaaaa"));
constraint
  (("aaa") ++ (B) ++ (B) ++ (C) ++ (B) ++ ("aaa") ++ (B) ++ ("aaaa")) =
    ((B) ++ ("aaa") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("aaaaaaaaa") ++ (B) ++ ("a"));
constraint
  (("aaa") ++ (B) ++ ("aaa") ++ (C) ++ (C) ++ ("aaaa")) =
    ((B) ++ ("aaa") ++ (B) ++ (A) ++ ("aa") ++ (B) ++ ("aaaa"));
constraint
  (("aaa") ++ (B) ++ (B) ++ (C) ++ (A)) =
    ((B) ++ ("aaaaaaaaaaaa") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("aaaa"));
constraint
  (("aaaaaaaaa") ++ (C) ++ (A)) =
    ((B) ++ (B) ++ (B) ++ (B) ++ (B) ++ (B) ++ (C) ++ (B) ++ ("a"));
constraint
  ((B) ++ ("aaa") ++ (B) ++ (B) ++ (B) ++ (B) ++ (C) ++ (B) ++ ("a")) =
    (("aaaaaaaaa") ++ (C) ++ ("aaa") ++ (B) ++ ("aaa") ++ (B) ++ ("a"));
constraint
  ((B) ++ (B) ++ (B) ++ (C) ++ (B) ++ (B) ++ (B) ++ ("aaaa")) =
    ((C) ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ (C) ++ ("a"));
constraint
  ((C) ++ (B) ++ ("aaaaaa") ++ (C) ++ ("aaaa")) =
    ((B) ++ (B) ++ (B) ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("a"));
constraint
  ((B) ++ ("aaaaaa") ++ (C) ++ (C) ++ (B) ++ ("a")) =
    (("aaa") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("aaaaaaaaa") ++ (B) ++ ("aaaa"));
constraint
  (("aaaaaaaaa") ++ (C) ++ (C) ++ (B) ++ ("a")) =
    ((B) ++ (B) ++ (C) ++ (B) ++ (B) ++ ("aaaaaa") ++ (B) ++ ("a"));
constraint
  ((B) ++ ("aaaaaa") ++ (C) ++ (C) ++ (B) ++ ("a")) =
    (("aaaaaa") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("aaaaaa") ++ (B) ++ ("aaaa"));
constraint
  (("aaa") ++ (B) ++ (B) ++ (C) ++ (C) ++ (B) ++ ("a")) =
    ((B) ++ ("aaa") ++ (B) ++ (A) ++ ("aa") ++ (B) ++ ("aaaa"));
solve satisfy;
