var string: B;
var string: E;
var string: F;
var string: A;
var string: C;
var string: D;
constraint
  ((C) ++ (D) ++ (D) ++ (D) ++ ("aaaa")) =
    ((A) ++ (A) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaaaaaaa"));
constraint
  (("aaaaaaaaaaaaaaaaaaaaaaaa") ++ (F) ++ (F) ++ (F) ++ (F) ++ ("aaaaaaaaa"))
    = ((A) ++ (D) ++ (D) ++ ("aaaaaaaaaa") ++ (A) ++ ("aaa"));
constraint
  ((A) ++ (A) ++ (D) ++ (F) ++ ("aaaaaaaaa")) =
    ((A) ++ (A) ++ (A) ++ ("aaa"));
constraint ((E) ++ (A) ++ ("aa")) = ((A) ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (F) ++ ("aaaaaaaaaaaaaaaa")) =
    (("aaaaaaaa") ++ (D) ++ (D) ++ ("aa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((C) ++ (D) ++ ("aaaa") ++ (F) ++ ("aaaa")) =
    ((D) ++ (D) ++ ("aaaaaaaaaa") ++ (A) ++ (D) ++ (D) ++ (D) ++ ("aaaaa"));
constraint
  ((C) ++ (F) ++ (D) ++ ("aaaaaaaa")) =
    ((D) ++ ("aaaaaaaaaaaaaaaaaa") ++ (A) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ ("aaaaa"));
constraint
  ((E) ++ ("aaaaaaaaaaaa") ++ (F) ++ ("aaaa")) =
    ((D) ++ (D) ++ (D) ++ ("aa") ++ (A) ++ (D) ++ (F) ++ ("aaaaaaaaa"));
constraint
  ((C) ++ (A) ++ ("aa")) =
    ((F) ++ (F) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ ("aaaa") ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (A) ++ ("aa")) =
    ((D) ++ (D) ++ ("aaaaaaaaaaaa") ++ (F) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ ("aaaaaaaaa"));
constraint
  ((C) ++ (D) ++ ("aaaa") ++ (F) ++ ("aaaa")) =
    ((A) ++ (D) ++ ("aaaaaaaaaaaaaaaaaa") ++ (A) ++ ("aaa"));
constraint ((E) ++ (A) ++ ("aa")) = ((A) ++ (A) ++ (A) ++ ("aaa"));
constraint
  (("aaaaaaaaaaaa") ++ (F) ++ ("aaaaaaaaaaaa") ++ (F) ++ (F) ++ (D) ++ ("aaaaaaaaaaaaa"))
    = ((A) ++ (A) ++ (D) ++ (D) ++ (D) ++ ("aaaaa"));
constraint ((C) ++ (F) ++ (F) ++ ("aaaa")) = ((A) ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((F) ++ ("aaaaaaaaaaaa") ++ (F) ++ (F) ++ (F) ++ (F) ++ (D) ++ ("a")) =
    ((D) ++ ("aaaaaaaa") ++ (D) ++ ("aa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (F) ++ (D) ++ (D)) = ((A) ++ (A) ++ (D) ++ (D) ++ (D) ++ ("aaaaa"));
constraint
  ((E) ++ ("aaaaaaaaaaaa") ++ (F) ++ ("aaaa")) =
    ((D) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ ("aaaa") ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (F) ++ (F) ++ ("aaaa")) =
    ((A) ++ (D) ++ (D) ++ (D) ++ (D) ++ ("aaaaaaaaaaaaaaaaaaaaaaa"));
constraint
  ((C) ++ (D) ++ ("aaaa") ++ (F) ++ ("aaaa")) =
    ((A) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ ("aa") ++ (A) ++ ("aaa"));
constraint
  ((A) ++ (D) ++ ("aa") ++ (F) ++ (D) ++ ("aaaa") ++ (F) ++ ("aaaaaaaaa")) =
    ((F) ++ ("aaaaaaaaaaaa") ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aaaa") ++ (A) ++ ("aaa"));
constraint
  ((F) ++ ("aaaaaaaaaaaa") ++ (F) ++ (A) ++ ("aaaaaaaa") ++ (D) ++ ("aaa")) =
    (("aaaaaaaaaaaa") ++ (F) ++ (D) ++ (D) ++ (D) ++ ("aaaa") ++ (A) ++ ("aaa"));
constraint
  ((D) ++ ("aaaa") ++ (F) ++ (F) ++ (F) ++ (F) ++ (F) ++ ("aaaaaaaaa")) =
    ((A) ++ (D) ++ (D) ++ (D) ++ ("aa") ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (F) ++ (F) ++ ("aaaa")) =
    ((A) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaa") ++ (D) ++ (F) ++ ("aaa"));
constraint
  ((E) ++ (A) ++ ("aa")) =
    ((A) ++ (D) ++ (F) ++ (F) ++ (D) ++ (F) ++ ("aaa"));
constraint
  (("aaaaaaaaaaaa") ++ (F) ++ (D) ++ ("aaaa") ++ (F) ++ (F) ++ ("aaaaaaaaaaaaaaaaaaaaa"))
    = ((F) ++ (F) ++ ("aa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (A) ++ ("aa")) =
    ((A) ++ (A) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ ("aaaaa"));
constraint
  (("aaaaaaaaaaaa") ++ (F) ++ ("aaaaaaaaaaaa") ++ (F) ++ (F) ++ (F) ++ ("aaaaaaaaa"))
    = (("aaaaaaaa") ++ (D) ++ (D) ++ ("aa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  (("aaaaaaaa") ++ (D) ++ (D) ++ ("aaaaaaaaaaaa") ++ (F) ++ ("aaaaaaaaaaaa") ++ (F) ++ (D) ++ ("a"))
    = ((D) ++ (F) ++ ("aaaaaa") ++ (A) ++ (D) ++ (D) ++ (D) ++ ("aaaaa"));
constraint
  ((D) ++ ("aaaa") ++ (F) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ (F) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaa"))
    =
    ((D) ++ (D) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ ("aaaaaaaaaaaa") ++ (A) ++ ("aaa"));
constraint
  ((C) ++ (F) ++ ("aaaaaaaa") ++ (D)) =
    ((A) ++ (D) ++ (D) ++ (D) ++ ("aa") ++ (A) ++ ("aaa"));
constraint
  ((C) ++ (A) ++ ("aa")) = ((A) ++ (A) ++ (D) ++ ("aaaaaaaaaaaaaaaaaaaaa"));
constraint
  ((E) ++ (F) ++ ("aaaaaaaaaaaaaaaa")) =
    ((A) ++ ("aaaaaaaaaaaaaaaaaaaaaaaaaa") ++ (A) ++ ("aaa"));
constraint ((C) ++ (F) ++ (D) ++ (D)) = ((A) ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (F) ++ (F) ++ ("aaaa")) =
    ((D) ++ (D) ++ (D) ++ ("aa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (A) ++ ("aa")) = ((A) ++ (B) ++ ("aaaaa") ++ (D) ++ (F) ++ ("aaa"));
constraint ((E) ++ (A) ++ ("aa")) = ((A) ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (D) ++ ("aaaa") ++ (F) ++ ("aaaa")) =
    ((D) ++ ("aaaaaaaaaaaaaaaaaa") ++ (A) ++ (D) ++ (D) ++ ("aaaaaaaaaaaaa"));
constraint
  ((C) ++ (D) ++ ("aaaa") ++ (F) ++ ("aaaa")) =
    ((A) ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (F) ++ (F) ++ ("aaaa")) =
    ((D) ++ (D) ++ (D) ++ ("aa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((F) ++ (F) ++ (F) ++ (F) ++ (F) ++ (F) ++ (D) ++ ("a")) =
    ((D) ++ (D) ++ ("aaaaaaaaaa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (F) ++ (F) ++ ("aaaa")) =
    ((D) ++ ("aaaaaaaa") ++ (D) ++ ("aa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((C) ++ (D) ++ ("aaaa") ++ (F) ++ ("aaaa")) =
    ((A) ++ (A) ++ (D) ++ (D) ++ (D) ++ ("aaaaa"));
constraint
  ((C) ++ (F) ++ (F) ++ ("aaaa")) =
    ((A) ++ (D) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaaaaaaaaaaaaaaaaa"));
constraint
  ((E) ++ (F) ++ ("aaaaaaaaaaaaaaaa")) =
    ((A) ++ (A) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ ("aaaaa"));
constraint
  ((E) ++ (A) ++ ("aa")) =
    (("aaaaaaaa") ++ (D) ++ ("aaaaaaaaaa") ++ (A) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ ("aaaaa"));
constraint
  ((E) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaaaaaa")) =
    ((B) ++ ("aaaaa") ++ (D) ++ (D) ++ ("aaaa") ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (D) ++ ("aaaa") ++ (F) ++ ("aaaa")) =
    ((F) ++ (F) ++ ("aaaaaaaa") ++ (D) ++ (F) ++ (A) ++ ("aaa"));
constraint
  ((F) ++ (F) ++ (A) ++ (A) ++ ("aaaaa")) =
    ((A) ++ (F) ++ ("aaaa") ++ (D) ++ ("aa") ++ (A) ++ ("aaa"));
constraint
  ((F) ++ (F) ++ (F) ++ ("aaaaaaaaaaaa") ++ (F) ++ (D) ++ (D) ++ ("aaaaa")) =
    (("aaaaaaaa") ++ (D) ++ (D) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ (D) ++ ("a"));
constraint
  ((C) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aaaa")) =
    ((D) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aaaa") ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (A) ++ ("aa")) =
    ((A) ++ ("aaaaaaaaaaaa") ++ (F) ++ ("aa") ++ (A) ++ ("aaa"));
constraint
  ((C) ++ (F) ++ (D) ++ ("aaaaaaaa")) =
    ((D) ++ ("aaaaaaaa") ++ (D) ++ ("aa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (A) ++ ("aa")) =
    (("aaaaaaaa") ++ (D) ++ ("aaaaaaaaaa") ++ (A) ++ (D) ++ ("aaaaaaaaaaaaaaaaaaaaa"));
constraint
  ((C) ++ (F) ++ ("aaaaaaaa") ++ (D)) =
    ((A) ++ (A) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaa"));
constraint
  (("aaaaaaaaaaaaaaaaaaaaaaaa") ++ (F) ++ (F) ++ (D) ++ ("aaaa") ++ (F) ++ ("aaaaaaaaa"))
    = ((D) ++ ("aaaaaaaa") ++ (D) ++ ("aa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((C) ++ ("aaaaaaaaaaaa") ++ (F) ++ ("aaaa")) =
    (("aaaaaaaaaaaaaaaa") ++ (D) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("a"));
constraint
  ((C) ++ (A) ++ ("aa")) = ((A) ++ (A) ++ (D) ++ (D) ++ (D) ++ ("aaaaa"));
constraint ((E) ++ (A) ++ ("aa")) = ((A) ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (A) ++ ("aa")) =
    (("aaaaaaaa") ++ (D) ++ (D) ++ (D) ++ ("aaaaaaaaaaaaaaaaaaaa") ++ (A) ++ ("aaa"));
constraint
  ((C) ++ (F) ++ ("aaaaaaaaaaaaaaaa")) =
    ((F) ++ ("aaaaaaaaaaaaaa") ++ (A) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaaaaaaa"));
constraint
  ((C) ++ (F) ++ (F) ++ ("aaaa")) =
    (("aaaaaaaaaaaaaaaa") ++ (D) ++ ("aa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((F) ++ (A) ++ ("aaaaaaaaaa") ++ (F) ++ (D) ++ ("aaaaaaaaaaaaa")) =
    ((A) ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (F) ++ ("aaaaaaaaaaaaaaaa")) =
    ((D) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ (F) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ ("aaaaaaaaaaaaaaaaaaaaaaaaaaaa") ++ ("")) =
    ((A) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ ("aa") ++ (A) ++ ("aaa"));
constraint ((C) ++ (F) ++ (F) ++ ("aaaa")) = ((A) ++ (B) ++ (B) ++ ("a"));
constraint
  (("aaaaaaaa") ++ (D) ++ ("aaaaaaaa") ++ (F) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ (F) ++ ("aaaaaaaaa"))
    =
    ((A) ++ (D) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaa"));
constraint
  ((C) ++ (F) ++ (F) ++ ("aaaa")) =
    ((A) ++ (A) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaa"));
constraint
  ((E) ++ (F) ++ ("aaaaaaaa") ++ (D)) =
    ((A) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ ("aa") ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (F) ++ ("aaaaaaaa") ++ (D)) =
    (("aaaaaaaaaaaaaaaa") ++ (D) ++ ("aa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (D) ++ ("aaaa") ++ (F) ++ ("aaaa")) =
    (("aaaaaaaa") ++ (D) ++ (D) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aaaa") ++ (A) ++ ("aaa"));
constraint
  ((E) ++ ("aaaaaaaaaaaa") ++ (F) ++ ("aaaa")) =
    ((A) ++ (A) ++ ("aaaaaaaa") ++ (D) ++ (F) ++ ("a"));
constraint
  ((E) ++ (A) ++ ("aa")) =
    ((D) ++ (F) ++ ("aaaaaaaaaaaaaaaaaaaa") ++ (D) ++ ("aaaa") ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (F) ++ ("aaaaaaaaaaaaaaaa")) =
    ((D) ++ (F) ++ (F) ++ ("aaaaaaaa") ++ (D) ++ ("aaaa") ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (F) ++ ("aaaaaaaaaaaaaaaa")) =
    ((A) ++ (A) ++ (D) ++ (F) ++ ("aaaaaaaaa"));
constraint
  ((F) ++ (D) ++ ("aaaaaaaaaaaaaaaa") ++ (F) ++ (F) ++ (F) ++ (D) ++ ("a")) =
    ((A) ++ (D) ++ (D) ++ ("aaaaaaaaaaaaaaaaaaaaaaaa") ++ (D) ++ ("aaaaaaa"));
constraint
  ((E) ++ (F) ++ ("aaaaaaaa") ++ (D)) =
    (("aaaaaaaa") ++ (D) ++ (D) ++ ("aa") ++ (A) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaa"));
constraint
  ((E) ++ ("aaaaaaaaaaaa") ++ (F) ++ ("aaaa")) =
    ((A) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaaaaaaaaaaaaaaaaaa") ++ (D) ++ ("aaaaaaa"));
constraint
  ((F) ++ (F) ++ (F) ++ (A) ++ ("aaaaaaaa") ++ (D) ++ ("aaa")) =
    ((D) ++ (D) ++ (D) ++ ("aa") ++ (A) ++ (F) ++ (F) ++ ("aaaaa"));
constraint ((E) ++ (F) ++ (F) ++ ("aaaa")) = ((A) ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((C) ++ (F) ++ ("aaaaaaaa") ++ (D)) =
    ((A) ++ (A) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ ("aaaaa"));
constraint
  ((C) ++ (F) ++ ("aaaaaaaaaaaaaaaa")) =
    ((A) ++ (D) ++ (D) ++ ("aaaaaaaaaa") ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (F) ++ (F) ++ ("aaaa")) =
    (("aaaaaaaa") ++ (D) ++ ("aaaaaaaaaa") ++ (A) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ ("aaaaa"));
constraint
  ((E) ++ (F) ++ ("aaaaaaaa") ++ (D)) =
    ((D) ++ (D) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ (D) ++ (D) ++ ("a"));
constraint
  ((F) ++ (D) ++ ("aaaa") ++ (F) ++ (F) ++ ("aaaaaaaaaaaa") ++ (F) ++ (D) ++ ("a"))
    = (("aaaaaaaaaaaa") ++ (F) ++ ("aa") ++ (A) ++ (B) ++ ("aa"));
constraint
  ((F) ++ ("aaaaaaaaaaaa") ++ (F) ++ ("aaaaaaaaaaaaaaaaaaaaaaaa") ++ (F) ++ (D) ++ ("a"))
    = (("aaaaaaaa") ++ (D) ++ ("aaaaaaaaaa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (F) ++ (D) ++ ("aaaaaaaa")) =
    ((A) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ ("aa") ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (F) ++ ("aaaaaaaaaaaaaaaa")) =
    ((D) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ (D) ++ (D) ++ (F) ++ ("aaaa") ++ (D) ++ ("aaaaaaaaa"));
constraint
  ((E) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ ("aaaa")) =
    ((A) ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (F) ++ (F) ++ ("aaaa")) =
    ((F) ++ ("aaaa") ++ (D) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ (D) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ ("a"));
constraint
  ((E) ++ (F) ++ (F) ++ ("aaaa")) =
    ((A) ++ (A) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaaaaaaa"));
constraint
  (("aaaaaaaa") ++ (D) ++ ("aaaaaaaa") ++ (F) ++ (F) ++ (F) ++ (F) ++ ("aaaaaaaaa"))
    = ((A) ++ (A) ++ (D) ++ (D) ++ (F) ++ ("a"));
constraint
  ((E) ++ (F) ++ (F) ++ ("aaaa")) =
    ((A) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ ("aa") ++ (A) ++ ("aaa"));
constraint
  (("aaaaaaaa") ++ (D) ++ ("aaaaaaaa") ++ (F) ++ (D) ++ ("aaaa") ++ (F) ++ (F) ++ (D) ++ ("a"))
    = (("aaaaaaaa") ++ (D) ++ (E) ++ ("aaa") ++ (D) ++ ("a"));
constraint
  ((E) ++ (F) ++ (F) ++ ("aaaa")) =
    (("aaaaaaaa") ++ (D) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaaaaaa") ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (F) ++ ("aaaaaaaa") ++ (D)) =
    ((D) ++ (D) ++ (D) ++ ("aa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (F) ++ (F) ++ ("aaaa")) =
    ((A) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aa") ++ (A) ++ ("aaa"));
constraint
  ((C) ++ (F) ++ (D) ++ (D)) =
    ((A) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ ("aaaaaaaaaaaaaaa"));
constraint
  ((C) ++ (D) ++ ("aaaaaaaaaaaaaaaaaaaa")) =
    ((A) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ ("aa") ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (A) ++ ("aa")) =
    (("aaaaaaaa") ++ (D) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaaaaaa") ++ (A) ++ ("aaa"));
constraint
  ((F) ++ ("aaaaaaaaaaaa") ++ (F) ++ ("aaaaaaaaaaaa") ++ (F) ++ (F) ++ (D) ++ ("a"))
    = ((D) ++ (D) ++ (D) ++ ("aa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((F) ++ (F) ++ (F) ++ ("aaaaaaaaaaaa") ++ (F) ++ (F) ++ ("aaaaaaaaa")) =
    ((D) ++ (D) ++ ("aaaaaaaaaa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((C) ++ (A) ++ ("aa")) = ((A) ++ (A) ++ (D) ++ (D) ++ (F) ++ ("a"));
constraint
  ((C) ++ ("aaaaaaaaaaaa") ++ (F) ++ ("aaaa")) =
    ((A) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aa") ++ (A) ++ ("aaa"));
constraint
  ((E) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaaaaaa")) =
    ((A) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aa") ++ (A) ++ ("aaa"));
constraint
  ((E) ++ ("aaaaaaaaaaaa") ++ (F) ++ ("aaaa")) =
    ((D) ++ ("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa") ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("a"));
constraint
  ((C) ++ ("aaaaaaaaaaaa") ++ (F) ++ ("aaaa")) =
    (("aaaaaaaaaaaaaaaa") ++ (D) ++ ("aa") ++ (A) ++ (D) ++ (D) ++ (D) ++ ("aaaaa"));
constraint
  ((E) ++ (D) ++ (D) ++ ("aaaaaaaaaaaa")) =
    ((D) ++ (D) ++ ("aaaaaaaaaa") ++ (A) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ ("aaaaa"));
constraint
  ((C) ++ ("aaaaaaaaaaaa") ++ (F) ++ ("aaaa")) =
    ((D) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ (F) ++ (A) ++ ("aaa"));
constraint
  (("aaaaaaaaaaaaaaaaaaaaaaaa") ++ (F) ++ (F) ++ (F) ++ (F) ++ (D) ++ ("a"))
    =
    ((A) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ (D) ++ ("aaaaaaa"));
constraint
  ((C) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ ("aaaa")) =
    (("aaaaaaaa") ++ (D) ++ ("aaaaaaaaaa") ++ (A) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaaaaaaa"));
constraint
  ((D) ++ (D) ++ ("aaaaaaaaaa") ++ (A) ++ (A) ++ ("aaa")) =
    ((D) ++ ("aaaaaaaa") ++ (D) ++ ("aa") ++ (A) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ ("aaaaa"));
constraint
  ((E) ++ (F) ++ (F) ++ ("aaaa")) =
    ((D) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ (D) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ (D) ++ ("aaaaaaaaa"));
constraint
  ((C) ++ (A) ++ ("aa")) =
    ((A) ++ (D) ++ (D) ++ ("aaaaaaaaaa") ++ (A) ++ ("aaa"));
constraint
  (("aaaaaaaaaaaaaaaa") ++ (D) ++ (F) ++ (D) ++ ("aaaa") ++ (F) ++ (F) ++ ("aaaaaaaaa"))
    = ((A) ++ (A) ++ (D) ++ (D) ++ (D) ++ ("aaaaa"));
constraint
  ((E) ++ (A) ++ ("aa")) =
    ((A) ++ ("aaaaaaaaaaaa") ++ (F) ++ (F) ++ ("aaaa") ++ (D) ++ ("aaaaaaa"));
constraint
  ((C) ++ (D) ++ ("aaaa") ++ (F) ++ ("aaaa")) =
    ((D) ++ (D) ++ ("aaaaaaaaaa") ++ (A) ++ (D) ++ (D) ++ (D) ++ ("aaaaa"));
constraint
  ((C) ++ (F) ++ ("aaaaaaaaaaaaaaaa")) =
    ((D) ++ ("aaaaaaaa") ++ (D) ++ ("aa") ++ (A) ++ (B) ++ ("aa"));
constraint ((C) ++ (F) ++ (F) ++ ("aaaa")) = ((A) ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (F) ++ (D) ++ ("aaaaaaaa")) =
    (("aaaaaaaaaaaaaaaa") ++ (D) ++ ("aa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((C) ++ (F) ++ ("aaaaaaaaaaaaaaaa")) =
    ((D) ++ (D) ++ (D) ++ ("aa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((C) ++ (D) ++ ("aaaaaaaaaaaaaaaaaaaa")) =
    ((A) ++ (A) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ ("aaaaa"));
constraint
  ((E) ++ (F) ++ ("aaaaaaaa") ++ (D)) =
    ((D) ++ (D) ++ ("aaaaaaaaaa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (D) ++ ("aaaa") ++ (F) ++ ("aaaa")) =
    ((A) ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (F) ++ (F) ++ ("aaaa")) =
    (("aaaaaaaa") ++ (D) ++ ("aaaaaaaaaa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (F) ++ ("aaaaaaaaaaaaaaaa")) =
    ((A) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaaaa") ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (A) ++ ("aa")) =
    ((A) ++ (F) ++ ("aaaa") ++ (D) ++ (D) ++ (D) ++ ("aaaaaaaaaaaaaaa"));
constraint
  ((C) ++ (A) ++ ("aa")) =
    (("aaaaaaaa") ++ (D) ++ (D) ++ (D) ++ (D) ++ (D) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ ("a"));
constraint
  ((E) ++ (F) ++ (F) ++ ("aaaa")) =
    ((A) ++ (A) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ ("aaaaa"));
constraint
  ((E) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ ("aaaa")) =
    ((A) ++ (B) ++ ("aaaaa") ++ (D) ++ ("aaaaaaaaaaaaaaa"));
constraint
  ((E) ++ (A) ++ ("aa")) =
    ((F) ++ ("aaaa") ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("a"));
constraint
  ((D) ++ ("aaaa") ++ (F) ++ (D) ++ ("aaaaaaaaaaaaaaaa") ++ (F) ++ (F) ++ ("aaaaaaaaa"))
    =
    (("aaaaaaaa") ++ (D) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ (D) ++ ("aaaa") ++ (A) ++ ("aaa"));
constraint
  ((C) ++ (F) ++ ("aaaaaaaaaaaaaaaa")) =
    ((A) ++ (A) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaa"));
constraint
  ((F) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ (F) ++ (F) ++ (F) ++ ("aaaaaaaaa"))
    =
    ((D) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ (D) ++ ("aaaaaaaaaaaaaaaaa"));
constraint
  ((E) ++ (A) ++ ("aa")) =
    ((A) ++ (D) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ (D) ++ ("aaaaaaaaaaaaaaa"));
constraint ((E) ++ (F) ++ (F) ++ ("aaaa")) = ((A) ++ (A) ++ (B) ++ ("aa"));
constraint
  ((E) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ ("aaaa")) =
    ((A) ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ ("aaaaaaaaaaaa") ++ (F) ++ ("aaaa")) =
    ((D) ++ (D) ++ ("aaaaaaaaaa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((C) ++ ("aaaaaaaaaaaa") ++ (F) ++ ("aaaa")) =
    (("aaaaaaaaaaaaaaaa") ++ (D) ++ (D) ++ (D) ++ (D) ++ ("aaaa") ++ (A) ++ ("aaa"));
constraint ((E) ++ (A) ++ ("aa")) = ((A) ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((E) ++ (A) ++ ("aa")) =
    ((D) ++ ("aaaaaaaaaaaaaaaa") ++ (D) ++ (D) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ ("aaaaaaaaa"));
constraint
  ((E) ++ (F) ++ ("aaaaaaaaaaaaaaaa")) = ((A) ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((C) ++ ("aaaaaaaaaaaaaaaaaaaaaaaaaaaa") ++ ("")) =
    (("aaaaaaaa") ++ (D) ++ ("aaaaaaaaaa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((C) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ ("aaaa")) =
    ((F) ++ ("aaaa") ++ (D) ++ (D) ++ ("aaaaaaaa") ++ (D) ++ (D) ++ (D) ++ (D) ++ (D) ++ ("a"));
constraint
  ((E) ++ (F) ++ (D) ++ ("aaaaaaaa")) =
    ((A) ++ (A) ++ (D) ++ (D) ++ (F) ++ ("a"));
constraint
  ((E) ++ (F) ++ ("aaaaaaaa") ++ (D)) =
    ((D) ++ ("aaaaaaaa") ++ (D) ++ ("aa") ++ (A) ++ (A) ++ ("aaa"));
solve satisfy;
