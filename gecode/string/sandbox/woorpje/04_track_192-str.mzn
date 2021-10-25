var string: B;
var string: E;
var string: H;
var string: F;
var string: G;
var string: A;
var string: C;
var string: D;
constraint
  ((F) ++ (E) ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ ("aaaa") ++ (E) ++ ("aaaaaaaa") ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ ("aaaa") ++ (E) ++ ("a"))
    = ((D) ++ (C) ++ (E) ++ ("aa"));
constraint ((G) ++ (C) ++ (E) ++ (E) ++ ("a")) = ((A) ++ (F) ++ ("aaaaaaa"));
constraint
  ((G) ++ (F) ++ ("aaaa") ++ (E) ++ ("aaaa") ++ (E)) =
    ((H) ++ ("aaaaaaaa") ++ (E) ++ (E));
constraint
  ((G) ++ (C) ++ ("aaaa") ++ (E) ++ ("a")) = ((A) ++ (B) ++ ("aaaa"));
constraint
  ((G) ++ (C) ++ (E) ++ ("aaaaa")) = ((H) ++ (E) ++ ("aaaaaaaa") ++ (E));
constraint
  ((C) ++ (E) ++ (E) ++ (E) ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ ("aaaaaaaaaaaaaaaa") ++ (E) ++ ("aaaaaa"))
    =
    ((D) ++ ("aaaaaaaaaaaaaaaa") ++ (E) ++ (E) ++ ("aaaa") ++ (E) ++ ("aaaaa"));
constraint
  ((G) ++ ("aaaa") ++ (E) ++ ("aaaaaaaaaaaa") ++ (E) ++ (E) ++ (E) ++ ("aaaaaaaa"))
    = ((A) ++ (F) ++ ("aaaaaaa"));
constraint
  ((G) ++ ("aaaa") ++ (E) ++ (E) ++ ("aaaa") ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ (E) ++ (E))
    =
    ((D) ++ ("aaaaaaaa") ++ (E) ++ ("aaaa") ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ ("aaaaa"));
constraint
  ((C) ++ (C) ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ ("aaaaaaa")) =
    ((D) ++ (B) ++ ("aaaa") ++ (E) ++ ("aa"));
constraint
  ((C) ++ (E) ++ (E) ++ ("aaaa") ++ (E) ++ ("aaaa") ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ ("aaaaaaaaaaaa") ++ (E) ++ ("aa"))
    = ((A) ++ (B) ++ ("aaaa"));
constraint
  ((G) ++ (C) ++ (E) ++ (E) ++ ("a")) = ((D) ++ (B) ++ ("aaaaaaaaaa"));
constraint
  ((F) ++ (F) ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ (E) ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ ("a"))
    =
    ((E) ++ ("aaaa") ++ (E) ++ (E) ++ (E) ++ ("aaaa") ++ (F) ++ ("aaaaaa") ++ (B) ++ ("aaaa"));
constraint
  ((G) ++ (C) ++ ("aaaa") ++ (E) ++ ("a")) =
    ((D) ++ (B) ++ (E) ++ ("aaaaaa"));
constraint
  ((C) ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ ("aaaa") ++ (E) ++ (F) ++ (E) ++ ("aa"))
    =
    (("aaaa") ++ (E) ++ ("aaaaaaaaaaaa") ++ (E) ++ ("aaa") ++ (B) ++ (B) ++ (E));
constraint
  ((G) ++ (F) ++ (E) ++ (E) ++ (E) ++ (E)) = ((A) ++ (B) ++ ("aaaa"));
constraint
  ((G) ++ (C) ++ ("aaaa") ++ (E) ++ ("a")) = ((A) ++ (B) ++ ("aaaa"));
constraint ((G) ++ (C) ++ (E) ++ (E) ++ ("a")) = ((A) ++ (C));
constraint
  ((F) ++ (F) ++ (F) ++ ("aaaa") ++ (E) ++ (E) ++ ("a")) =
    ((D) ++ (B) ++ ("aaaa") ++ (E) ++ ("aa"));
constraint
  ((G) ++ (F) ++ ("aaaa") ++ (E) ++ (E) ++ (E)) =
    ((C) ++ ("aaaa") ++ (E) ++ (E) ++ (E) ++ ("aaaaaaa") ++ (B) ++ (E));
constraint
  ((F) ++ ("aaaaaaaa") ++ (E) ++ (E) ++ (E) ++ (E) ++ (F) ++ ("aaaaaaaaaaaaa"))
    = ((D) ++ (B) ++ (E) ++ ("aaaaaa"));
constraint
  (("aaaaaaaa") ++ (E) ++ (E) ++ (E) ++ ("aaaa") ++ (E) ++ ("aaaa") ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ ("aaaa") ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ ("a"))
    = ((B) ++ (G) ++ ("aaaa") ++ (E) ++ ("aaaaa"));
constraint ((G) ++ (C) ++ (E) ++ ("aaaaa")) = ((A) ++ (B) ++ ("aaaa"));
constraint
  ((C) ++ (C) ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ (E) ++ ("aaa")) =
    ((D) ++ (F) ++ ("aaaaaaaa") ++ (E) ++ ("a"));
constraint
  ((F) ++ ("aaaa") ++ (E) ++ ("aaaaaaaaaaaa") ++ (E) ++ (F) ++ (E) ++ (E) ++ (E) ++ ("a"))
    = (("aaaaaaaa") ++ (E) ++ (E) ++ ("aaaaaaaaaaa") ++ (B) ++ (C));
constraint
  ((E) ++ (E) ++ (E) ++ (E) ++ (E) ++ (E) ++ ("aaaaaaa") ++ (C) ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ ("aaaaaaa"))
    = ((A) ++ (B) ++ (E));
constraint
  ((F) ++ (E) ++ ("aaa") ++ (C) ++ (E) ++ (E) ++ (E) ++ ("aaaaaaaaaaa")) =
    ((F) ++ ("aaa") ++ (B) ++ (B) ++ ("aaaa"));
constraint
  ((G) ++ (C) ++ (E) ++ (E) ++ ("a")) =
    ((D) ++ (F) ++ (E) ++ ("aaaa") ++ (E) ++ ("a"));
constraint
  ((G) ++ ("aaaaaaaa") ++ (E) ++ ("aaaaaaaaaaaaaaaaaaaaaaaaaaaa")) =
    ((B) ++ (G) ++ (E) ++ ("aaaaaaaaa"));
constraint ((G) ++ (C) ++ (E) ++ ("aaaaa")) = ((A) ++ (B) ++ (E));
constraint
  ((G) ++ (E) ++ ("aaaaaaaa") ++ (E) ++ ("aaaa") ++ (E) ++ ("aaaaaaaaaaaa") ++ (E))
    = ((A) ++ (C));
constraint
  ((G) ++ ("aaaa") ++ (E) ++ (E) ++ (E) ++ ("aaaa") ++ (E) ++ ("aaaa") ++ (E) ++ ("aaaa") ++ (E))
    = ((A) ++ (F) ++ (E) ++ ("aaa"));
constraint ((C) ++ (F) ++ (F) ++ (E) ++ ("aa")) = ((A) ++ (C));
constraint ((G) ++ (C) ++ (E) ++ ("aaaaa")) = ((A) ++ (F) ++ (E) ++ ("aaa"));
constraint
  ((G) ++ (F) ++ (E) ++ ("aaaaaaaaaaaa")) =
    ((G) ++ (E) ++ ("aaaaa") ++ (B) ++ (E));
constraint
  (("aaaaaaaa") ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ ("aaaaaaa") ++ (C) ++ ("aaaa") ++ (E) ++ ("aaaaaaaa") ++ (E) ++ ("aaa"))
    = ((D) ++ (B) ++ ("aaaaaaaaaa"));
constraint
  (("aaaa") ++ (E) ++ (E) ++ ("aaaaaaaa") ++ (E) ++ (F) ++ (F) ++ ("aaaaaaaaaaaaa"))
    = ((H) ++ ("aaaa") ++ (E) ++ ("aaaaaaaa"));
constraint
  ((G) ++ ("aaaa") ++ (E) ++ (E) ++ ("aaaa") ++ (E) ++ ("aaaaaaaaaaaa") ++ (E) ++ (E))
    = ((A) ++ (B) ++ ("aaaa"));
constraint ((G) ++ (C) ++ (E) ++ (E) ++ ("a")) = ((A) ++ (B) ++ ("aaaa"));
constraint ((G) ++ (F) ++ (E) ++ ("aaaaaaaa") ++ (E)) = ((A) ++ (C));
constraint ((G) ++ (C) ++ (E) ++ (E) ++ ("a")) = ((A) ++ (B) ++ ("aaaa"));
constraint
  ((G) ++ (F) ++ ("aaaa") ++ (E) ++ ("aaaaaaaa")) =
    ((F) ++ (E) ++ (E) ++ (E) ++ ("aaaaaaaa") ++ (E) ++ (E) ++ (E) ++ (E) ++ ("aaaaaaaaaaaaaaaa") ++ (E) ++ ("aaaaa"));
constraint
  (("aaaa") ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ ("aaaaaaaaaaa") ++ (C) ++ (E) ++ ("aaaa") ++ (E) ++ ("aaaa") ++ (E) ++ ("aaa"))
    = ((B) ++ (B) ++ (B) ++ (E));
constraint
  ((G) ++ (F) ++ (E) ++ (E) ++ (E) ++ ("aaaa")) = ((A) ++ (F) ++ ("aaaaaaa"));
constraint
  ((C) ++ (F) ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ (E) ++ (E) ++ ("aaaaaa")) =
    (("aaaa") ++ (E) ++ (E) ++ (E) ++ (E) ++ (E) ++ ("aaa") ++ (B) ++ ("aaaa") ++ (E) ++ (E) ++ (E) ++ ("aaaaaaaa") ++ (E) ++ ("aaa"));
constraint
  ((G) ++ ("aaaaaaaaaaaaaaaaaaaa") ++ (E) ++ ("aaaaaaaa") ++ (E) ++ (E)) =
    ((A) ++ (B) ++ (E));
constraint
  ((G) ++ (E) ++ (E) ++ (E) ++ (E) ++ (E) ++ (E) ++ (E) ++ (E) ++ (E) ++ (E))
    = ((H) ++ (E) ++ (E) ++ (E) ++ (E));
constraint
  ((G) ++ ("aaaaaaaa") ++ (E) ++ (E) ++ (E) ++ ("aaaaaaaaaaaa") ++ (E) ++ ("aaaa"))
    = ((H) ++ (E) ++ (E) ++ ("aaaa") ++ (E));
constraint
  ((G) ++ (E) ++ (E) ++ (E) ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ ("aaaaaaaaaaaa"))
    = ((A) ++ (B) ++ (E));
constraint
  ((G) ++ (C) ++ (E) ++ (E) ++ ("a")) =
    (("aaaaaaaa") ++ (E) ++ (E) ++ (E) ++ (E) ++ ("aaa") ++ (B) ++ (B) ++ ("aaaa"));
constraint
  ((C) ++ (C) ++ ("aaaa") ++ (E) ++ (E) ++ (E) ++ ("aaaaaaa")) =
    ((F) ++ ("aaa") ++ (B) ++ (C));
constraint
  ((G) ++ (C) ++ ("aaaaaaaaa")) =
    ((B) ++ (B) ++ (E) ++ ("aaaaaaaaaaaa") ++ (E) ++ (E) ++ (E) ++ ("aaa"));
solve satisfy;
