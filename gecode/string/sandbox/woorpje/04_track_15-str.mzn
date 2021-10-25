var string: E;
var string: G;
var string: F;
var string: A;
var string: C;
var string: D;
constraint
  (("aaa") ++ (F) ++ (F) ++ (G) ++ (F) ++ (F) ++ (F) ++ (G) ++ (F) ++ (F) ++ (F) ++ (F))
    = ((E) ++ (A) ++ ("aa") ++ (E) ++ (E) ++ (F) ++ ("a"));
constraint
  (("aaa") ++ (F) ++ (F) ++ (G) ++ (F) ++ ("aaa") ++ (F) ++ (G) ++ (G) ++ (F))
    = ((E) ++ (E) ++ (E) ++ (E) ++ ("aaaa"));
constraint
  ((C) ++ (F) ++ (F) ++ (F) ++ ("a")) =
    ((A) ++ ("aa") ++ (E) ++ (E) ++ (A) ++ ("aaa") ++ (F));
constraint
  ((C) ++ (F) ++ ("aaa") ++ (F) ++ ("a")) =
    ((E) ++ (E) ++ (E) ++ (A) ++ ("aaaaaa"));
constraint
  ((C) ++ (A) ++ ("a")) =
    ((F) ++ (F) ++ ("aaaaa") ++ (E) ++ (E) ++ (E) ++ ("aaaa"));
constraint
  (("aaaaaa") ++ (F) ++ ("aaaaaaaaa") ++ (F) ++ (F) ++ (F) ++ (G) ++ (G) ++ ("aaa"))
    = ((E) ++ (E) ++ (D) ++ ("aaa") ++ (F) ++ ("a"));
constraint
  ((C) ++ ("aaa") ++ (F) ++ ("aaaa")) =
    ((E) ++ (E) ++ (A) ++ ("aa") ++ (E) ++ (F) ++ ("a"));
constraint
  ((A) ++ (G) ++ (F) ++ ("aaa") ++ (F) ++ (G) ++ (F) ++ ("aaa") ++ (F) ++ (F))
    =
    ((A) ++ ("aaa") ++ (F) ++ ("aaaaaaa") ++ (E) ++ (F) ++ ("aaa") ++ (F) ++ ("aaaaaa"));
constraint
  (("aaa") ++ (F) ++ ("aaa") ++ (G) ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ (F) ++ ("aaa") ++ (G) ++ ("aaa"))
    = ((E) ++ (A) ++ ("aa") ++ (E) ++ (E) ++ ("aaaa"));
constraint
  ((C) ++ (G) ++ ("a")) =
    ((E) ++ (G) ++ ("aa") ++ (E) ++ (A) ++ ("aaa") ++ (F));
constraint
  ((C) ++ (F) ++ ("aaaaaaa")) = ((D) ++ (F) ++ (E) ++ (A) ++ (F) ++ (F));
constraint
  ((C) ++ (G) ++ ("a")) =
    ((F) ++ (F) ++ (F) ++ ("aa") ++ (E) ++ (A) ++ ("aa") ++ (E) ++ (F) ++ ("a"));
constraint
  (("aaaaaa") ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ (F) ++ ("aaa") ++ (F) ++ (G) ++ (F) ++ ("aaaaaa") ++ (F))
    = ((E) ++ (D) ++ (A) ++ (A));
constraint
  ((C) ++ ("aaa") ++ (F) ++ (F) ++ ("a")) =
    ((D) ++ (G) ++ (F) ++ ("aa") ++ (E) ++ ("aaaa"));
constraint
  ((C) ++ (F) ++ (F) ++ ("aaaa")) =
    ((E) ++ (A) ++ ("aa") ++ (E) ++ (A) ++ (F) ++ ("aaa"));
constraint
  ((G) ++ (G) ++ (G) ++ ("aaa") ++ (F) ++ (F) ++ (G) ++ ("aaa")) =
    ((F) ++ ("aaaaaaaaaaaaaaa") ++ (F) ++ ("a") ++ (E) ++ (F) ++ (F) ++ (F) ++ ("aaa") ++ (F));
constraint
  ((C) ++ ("aaaaaaaaaa") ++ ("")) =
    ((E) ++ (E) ++ (A) ++ ("aa") ++ (E) ++ (F) ++ ("a"));
constraint
  ((G) ++ (F) ++ (F) ++ ("aaa") ++ (G) ++ (F) ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ (F) ++ (F))
    = ((D) ++ (F) ++ (E) ++ (F) ++ ("aaaaaa") ++ (F) ++ (F));
constraint ((C) ++ (G) ++ ("a")) = ((E) ++ (E) ++ (E) ++ (A) ++ ("aaaaaa"));
constraint
  ((C) ++ (G) ++ ("a")) =
    ((A) ++ ("aa") ++ (E) ++ (D) ++ (F) ++ (F) ++ ("a"));
constraint
  (("aaaaaa") ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ (G) ++ (G) ++ (G) ++ (F)) =
    ((A) ++ (A) ++ (A) ++ (G) ++ (F) ++ ("aaaaaaaaa"));
constraint
  ((F) ++ ("aaa") ++ (F) ++ (F) ++ (F) ++ (F) ++ (F) ++ (F) ++ (A) ++ (F) ++ (F) ++ ("aaa") ++ (F) ++ (F))
    =
    ((E) ++ (F) ++ ("aaa") ++ (F) ++ ("aa") ++ (E) ++ (G) ++ (F) ++ ("aaa"));
constraint
  (("aaa") ++ (F) ++ ("aaaaaa") ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ (A) ++ ("aaa") ++ (G) ++ (F))
    = ((E) ++ (E) ++ (D) ++ (F) ++ ("aaaa"));
constraint
  ((G) ++ (F) ++ ("aaa") ++ (F) ++ (G) ++ (F) ++ (F) ++ ("aaa") ++ (G) ++ (F))
    = ((E) ++ (A) ++ ("aa") ++ (E) ++ (E) ++ (F) ++ ("a"));
constraint
  ((C) ++ ("aaaaaaaaaa") ++ ("")) =
    ((E) ++ (E) ++ (A) ++ ("aa") ++ (E) ++ (F) ++ ("a"));
constraint
  ((C) ++ (G) ++ ("a")) =
    ((A) ++ ("aa") ++ (E) ++ (A) ++ (G) ++ ("aaa") ++ (F) ++ ("aa"));
constraint
  ((F) ++ (F) ++ ("aaa") ++ (F) ++ ("aaa") ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ ("aaa") ++ (F) ++ ("aaa"))
    =
    ((E) ++ (A) ++ ("aaa") ++ (F) ++ ("aaa") ++ (F) ++ ("a") ++ (E) ++ ("aaaa"));
constraint
  ((C) ++ ("aaaaaa") ++ (F) ++ ("a")) =
    ((E) ++ ("aaaaaa") ++ (F) ++ ("aa") ++ (E) ++ (E) ++ (F) ++ ("a"));
constraint
  ((C) ++ (G) ++ ("a")) =
    ((G) ++ ("aa") ++ (E) ++ (D) ++ ("aaa") ++ (F) ++ ("a"));
constraint
  ((G) ++ (G) ++ (G) ++ ("aaaaaa") ++ (F) ++ (G) ++ (F)) =
    ((A) ++ ("aa") ++ (E) ++ (D) ++ ("aaa") ++ (F) ++ ("a"));
constraint
  ((G) ++ (G) ++ (G) ++ ("aaaaaa") ++ (F) ++ (G) ++ (F)) =
    ((E) ++ (G) ++ (A) ++ (A) ++ (A) ++ ("a"));
constraint
  (("aaa") ++ (F) ++ ("aaaaaaaaaaaa") ++ (G) ++ (F) ++ (F) ++ ("aaa") ++ (G) ++ ("aaa"))
    = ((F) ++ ("aaaaaaaa") ++ (E) ++ (D) ++ ("aaa") ++ (F) ++ ("a"));
constraint
  ((G) ++ (F) ++ (F) ++ ("aaa") ++ (G) ++ (G) ++ (G) ++ ("aaa")) =
    ((D) ++ (F) ++ (E) ++ (F) ++ ("aaa") ++ (F) ++ ("aaa") ++ (F));
constraint
  ((F) ++ ("aaaaaa") ++ (F) ++ ("aaaaaa") ++ (F) ++ ("aaa") ++ (F) ++ (G) ++ (F) ++ (F) ++ (F) ++ (F))
    = ((F) ++ ("aaaaaa") ++ (G) ++ (A) ++ ("aaaaaa") ++ (E) ++ (F) ++ ("a"));
constraint
  ((C) ++ (F) ++ ("aaa") ++ (F) ++ ("a")) =
    ((A) ++ (G) ++ (F) ++ ("a") ++ (E) ++ (E) ++ ("aaaa"));
constraint
  ((C) ++ ("aaa") ++ (F) ++ ("aaaa")) =
    ((E) ++ (E) ++ (A) ++ ("aa") ++ (E) ++ ("aaaa"));
constraint
  ((C) ++ (G) ++ ("a")) = ((E) ++ (A) ++ (G) ++ (A) ++ (A) ++ ("a"));
constraint
  ((C) ++ (G) ++ ("a")) =
    ((E) ++ (G) ++ ("aa") ++ (E) ++ (E) ++ (F) ++ ("a"));
constraint
  ((C) ++ (F) ++ (F) ++ ("aaaa")) =
    ((D) ++ ("aaa") ++ (E) ++ (A) ++ (F) ++ (F));
constraint
  (("aaa") ++ (F) ++ ("aaaaaa") ++ (F) ++ (F) ++ (F) ++ (F) ++ ("aaaaaa") ++ (F) ++ (F) ++ (F) ++ (F) ++ ("aaa") ++ (F))
    = ((E) ++ (G) ++ ("aa") ++ (E) ++ (E) ++ ("aaaa"));
constraint
  ((G) ++ ("aaa") ++ (F) ++ (F) ++ (G) ++ ("aaa") ++ (F) ++ (F) ++ ("aaaaaa") ++ (F) ++ ("aaa"))
    = ((E) ++ (E) ++ (A) ++ (G) ++ ("aaa") ++ (F) ++ ("aa"));
constraint
  ((G) ++ (G) ++ (G) ++ (F) ++ (F) ++ (F) ++ (G) ++ (F)) =
    ((A) ++ (A) ++ (F) ++ ("a") ++ (E) ++ (E) ++ (F) ++ ("a"));
constraint
  (("aaa") ++ (F) ++ (F) ++ (F) ++ ("aaa") ++ (F) ++ (G) ++ (A) ++ (G) ++ ("aaa"))
    = ((E) ++ (E) ++ (A) ++ ("aa") ++ (E) ++ ("aaaa"));
constraint
  ((C) ++ (F) ++ (F) ++ (F) ++ ("a")) =
    ((D) ++ ("aaa") ++ (E) ++ (E) ++ (F) ++ ("a"));
constraint
  ((C) ++ (G) ++ ("a")) =
    ((E) ++ (E) ++ ("aaaaaa") ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ ("aaa") ++ (F) ++ ("aa"));
constraint
  ((C) ++ (G) ++ ("a")) = ((E) ++ (D) ++ (F) ++ (E) ++ (F) ++ ("a"));
constraint
  ((G) ++ (G) ++ (F) ++ (F) ++ (E) ++ ("a") ++ (F) ++ (F) ++ (F) ++ (F)) =
    ((E) ++ (E) ++ (A) ++ (A) ++ ("aaa") ++ (F) ++ ("aa"));
constraint
  ((C) ++ ("aaa") ++ (F) ++ (F) ++ ("a")) =
    ((E) ++ (E) ++ (E) ++ (E) ++ (F) ++ ("a"));
constraint
  ((F) ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ (F) ++ (F) ++ ("aaaaaa") ++ (G) ++ (F) ++ (F) ++ (F) ++ ("aaa"))
    = ((E) ++ (E) ++ (E) ++ (E) ++ ("aaaa"));
constraint
  ((F) ++ ("aaa") ++ (F) ++ (G) ++ (F) ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ ("aaaaaa") ++ (F) ++ ("aaaaaa"))
    = ((A) ++ ("aa") ++ (E) ++ (A) ++ ("aa") ++ (E) ++ (F) ++ ("a"));
constraint
  (("aaaaaa") ++ (F) ++ (F) ++ ("aaa") ++ (F) ++ ("aaa") ++ (F) ++ ("aaa") ++ (G) ++ (G) ++ ("aaa"))
    = ((A) ++ (G) ++ (A) ++ ("aaa") ++ (F) ++ (E) ++ (F) ++ ("a"));
constraint ((C) ++ (G) ++ ("a")) = ((E) ++ (E) ++ (E) ++ (A) ++ (F) ++ (F));
constraint
  ((C) ++ (F) ++ (F) ++ (F) ++ ("a")) =
    ((F) ++ ("aaa") ++ (F) ++ ("aaaaaaaaa") ++ (G) ++ (F) ++ (F) ++ (E) ++ ("aaaa"));
constraint
  ((C) ++ (F) ++ ("aaaaaaa")) =
    ((E) ++ (E) ++ (D) ++ ("aaa") ++ (F) ++ ("a"));
constraint
  (("aaaaaa") ++ (F) ++ (G) ++ ("aaa") ++ (F) ++ ("aaa") ++ (G) ++ (G) ++ ("aaa"))
    = ((E) ++ (E) ++ (D) ++ (F) ++ ("aaaa"));
constraint
  ((G) ++ ("aaaaaa") ++ (F) ++ (G) ++ (F) ++ ("aaa") ++ (F) ++ (G) ++ ("aaa"))
    = ((D) ++ (F) ++ (E) ++ (F) ++ (F) ++ (F) ++ (F) ++ (F));
constraint
  ((C) ++ (G) ++ ("a")) = ((E) ++ (E) ++ (G) ++ ("aa") ++ (E) ++ ("aaaa"));
constraint
  ((G) ++ (G) ++ (G) ++ (G) ++ (F) ++ ("aaa") ++ (F) ++ ("aaa")) =
    ((E) ++ (E) ++ (E) ++ (G) ++ ("aaa") ++ (F));
constraint
  ((C) ++ ("aaaaaa") ++ (F) ++ ("a")) =
    ((D) ++ ("aaa") ++ (E) ++ (A) ++ (F) ++ (F));
constraint
  ((C) ++ (F) ++ ("aaaaaaa")) =
    (("aaaaaaaaa") ++ (A) ++ (A) ++ ("aaa") ++ (F) ++ (E) ++ ("aaaa"));
constraint
  ((F) ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ (F) ++ (G) ++ (G) ++ (G) ++ ("aaa"))
    = ((E) ++ (A) ++ (G) ++ (A) ++ (G) ++ ("a"));
constraint
  ((G) ++ (G) ++ (F) ++ ("aaa") ++ (F) ++ (G) ++ ("aaaaaa") ++ (F) ++ ("aaa"))
    = ((E) ++ (E) ++ (D) ++ (F) ++ (F) ++ ("a"));
constraint
  ((G) ++ ("aaa") ++ (F) ++ (E) ++ ("a") ++ (F) ++ (F) ++ (E) ++ ("aaaa")) =
    (("aaa") ++ (F) ++ ("aaaaa") ++ (E) ++ (A) ++ (G) ++ (F) ++ (F) ++ ("aa"));
constraint
  ((C) ++ ("aaa") ++ (F) ++ ("aaaa")) =
    ((E) ++ (E) ++ (D) ++ (F) ++ ("aaaa"));
constraint
  ((C) ++ (G) ++ ("a")) =
    ((E) ++ (F) ++ ("aaa") ++ (F) ++ ("aa") ++ (E) ++ (E) ++ ("aaaa"));
constraint ((C) ++ (G) ++ ("a")) = ((E) ++ (E) ++ (E) ++ (E) ++ ("aaaa"));
constraint
  (("aaaaaa") ++ (F) ++ (G) ++ (G) ++ (G) ++ (G) ++ ("aaa")) =
    ((E) ++ ("aaaaaaaaaaa") ++ (E) ++ (E) ++ (F) ++ ("a"));
constraint
  ((C) ++ (F) ++ ("aaa") ++ (F) ++ ("a")) =
    ((E) ++ (A) ++ (A) ++ ("aaaa") ++ (E) ++ (F) ++ ("a"));
constraint
  ((C) ++ (G) ++ ("a")) =
    ((A) ++ (A) ++ (A) ++ (F) ++ (F) ++ (E) ++ (F) ++ ("a"));
constraint
  ((F) ++ (F) ++ (F) ++ ("aaa") ++ (F) ++ ("aaa") ++ (G) ++ (G) ++ ("aaa") ++ (F) ++ ("aaa") ++ (F))
    = ((D) ++ ("aaa") ++ (E) ++ (E) ++ (F) ++ ("a"));
constraint
  ((C) ++ (G) ++ ("a")) =
    ((E) ++ (E) ++ (F) ++ (F) ++ (F) ++ ("aa") ++ (E) ++ (F) ++ ("a"));
constraint
  ((F) ++ ("aaa") ++ (F) ++ (G) ++ (G) ++ (G) ++ (G) ++ ("aaa")) =
    (("aaaaaa") ++ (F) ++ ("aa") ++ (E) ++ (E) ++ (E) ++ ("aaaa"));
constraint
  ((G) ++ (F) ++ (F) ++ (F) ++ (G) ++ (F) ++ (F) ++ (F) ++ ("aaa") ++ (F) ++ ("aaaaaa"))
    = ((G) ++ (G) ++ (A) ++ (F) ++ ("aaa") ++ (E) ++ ("aaaa"));
constraint
  ((C) ++ (G) ++ ("a")) =
    (("aaa") ++ (F) ++ ("aaa") ++ (G) ++ (A) ++ ("aaa") ++ (F) ++ (E) ++ (F) ++ ("a"));
constraint
  ((C) ++ ("aaaaaaaaaa") ++ ("")) =
    ((G) ++ ("aa") ++ (E) ++ (D) ++ ("aaa") ++ (F) ++ ("a"));
constraint
  ((A) ++ (F) ++ ("aaa") ++ (F) ++ (G) ++ (G) ++ (G) ++ (F)) =
    ((E) ++ (E) ++ (G) ++ ("aa") ++ (E) ++ (F) ++ ("a"));
constraint
  ((C) ++ (F) ++ (F) ++ ("aaaa")) =
    ((E) ++ (E) ++ (E) ++ (E) ++ (F) ++ ("a"));
constraint
  ((F) ++ ("aaaaaa") ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ ("aaaaaa") ++ (G) ++ (F))
    = ((A) ++ ("aa") ++ (E) ++ (G) ++ ("aa") ++ (E) ++ (F) ++ ("a"));
constraint
  (("aaa") ++ (F) ++ ("aaa") ++ (G) ++ (G) ++ ("aaaaaa") ++ (F) ++ (A) ++ (F))
    = ((A) ++ ("aa") ++ (E) ++ (E) ++ (A) ++ ("aaa") ++ (F));
constraint
  ((G) ++ (G) ++ (F) ++ (F) ++ ("aaa") ++ (G) ++ (G) ++ (F)) =
    ((A) ++ ("aa") ++ (E) ++ (E) ++ (E) ++ ("aaaa"));
constraint
  ((C) ++ (G) ++ ("a")) = ((D) ++ (D) ++ (F) ++ (F) ++ (F) ++ ("a"));
constraint
  ((C) ++ ("aaaaaaaaaa") ++ ("")) =
    (("aaa") ++ (F) ++ ("aaa") ++ (A) ++ (F) ++ ("a") ++ (E) ++ (E) ++ (F) ++ ("a"));
constraint
  ((G) ++ ("aaa") ++ (F) ++ (F) ++ (F) ++ (F) ++ (F) ++ (G) ++ (F) ++ ("aaaaaa") ++ (F))
    = ((E) ++ (G) ++ (G) ++ ("aaaa") ++ (E) ++ (F) ++ ("a"));
constraint
  ((C) ++ (A) ++ ("a")) = ((D) ++ (A) ++ ("aaaaaa") ++ (F) ++ (A) ++ ("aa"));
constraint
  ((C) ++ (F) ++ ("aaaaaaa")) =
    ((E) ++ (E) ++ (A) ++ ("aaa") ++ (F) ++ ("aaaaaa") ++ (F) ++ ("aa"));
constraint
  ((C) ++ (F) ++ ("aaaaaaa")) = ((E) ++ (D) ++ (F) ++ (E) ++ (F) ++ ("a"));
constraint
  ((C) ++ (G) ++ ("a")) = ((D) ++ (A) ++ ("aaaaa") ++ (E) ++ (F) ++ ("a"));
constraint
  ((C) ++ (G) ++ ("a")) = ((E) ++ (A) ++ ("aa") ++ (E) ++ (G) ++ ("aaaaaa"));
constraint
  (("aaaaaa") ++ (F) ++ (G) ++ (G) ++ (F) ++ ("aaaaaa") ++ (G) ++ ("aaa")) =
    ((E) ++ (A) ++ ("aa") ++ (E) ++ (F) ++ (F) ++ ("aaaaaaaaa"));
constraint
  ((C) ++ (G) ++ ("a")) =
    ((A) ++ ("aa") ++ (E) ++ (E) ++ (E) ++ (F) ++ ("a"));
constraint
  ((C) ++ ("aaaaaa") ++ (F) ++ ("a")) =
    ((E) ++ (E) ++ (E) ++ (E) ++ ("aaaa"));
constraint
  ((G) ++ (F) ++ (F) ++ ("aaa") ++ (G) ++ (G) ++ (G) ++ ("aaa")) =
    ((E) ++ (E) ++ (E) ++ (E) ++ (F) ++ ("a"));
constraint
  (("aaa") ++ (F) ++ ("aaaaaa") ++ (F) ++ (F) ++ (G) ++ (G) ++ (F) ++ (F) ++ ("aaaaaa"))
    =
    ((G) ++ ("aaa") ++ (F) ++ ("aaa") ++ (A) ++ ("aaa") ++ (F) ++ (E) ++ ("aaaa"));
constraint
  ((G) ++ (G) ++ (F) ++ ("aaaaaa") ++ (G) ++ (G) ++ ("aaa")) =
    (("aaaaaaaaaaa") ++ (E) ++ (E) ++ (E) ++ (F) ++ ("a"));
constraint
  ((F) ++ (F) ++ (F) ++ (F) ++ (F) ++ ("aaaaaa") ++ (F) ++ (F) ++ (G) ++ (G) ++ ("aaa"))
    = ((E) ++ (A) ++ ("aaaaaaaaaaaaa") ++ (E) ++ ("aaaa"));
constraint
  ((C) ++ (F) ++ ("aaaaaaa")) =
    ((F) ++ ("aaa") ++ (F) ++ (G) ++ (G) ++ (A) ++ (A) ++ (F));
constraint ((C) ++ (G) ++ ("a")) = ((G) ++ (A) ++ (A) ++ (G) ++ (A) ++ (F));
constraint
  ((F) ++ (F) ++ ("aaa") ++ (G) ++ (G) ++ (G) ++ (G) ++ ("aaa")) =
    ((A) ++ (A) ++ (F) ++ (F) ++ ("aaa") ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((C) ++ (G) ++ ("a")) =
    ((A) ++ (A) ++ ("aaaa") ++ (E) ++ (A) ++ ("aaaaaa"));
constraint
  ((F) ++ (F) ++ ("aaaaaa") ++ (F) ++ ("aaa") ++ (F) ++ ("aaaaaa") ++ (F) ++ (F) ++ ("aaa") ++ (G) ++ (F))
    = ((E) ++ ("aaaaaaaaaaa") ++ (E) ++ (E) ++ (F) ++ ("a"));
constraint
  ((C) ++ ("aaaaaaaaaa") ++ ("")) =
    ((E) ++ (A) ++ (A) ++ ("aaaa") ++ (E) ++ ("aaaa"));
constraint ((C) ++ (G) ++ ("a")) = ((D) ++ (A) ++ (G) ++ (A) ++ ("aa"));
constraint
  ((C) ++ (G) ++ ("a")) =
    ((G) ++ (A) ++ (G) ++ ("aaa") ++ (F) ++ (E) ++ ("aaaa"));
constraint
  ((C) ++ (G) ++ ("a")) = ((G) ++ (G) ++ ("a") ++ (D) ++ (G) ++ ("a"));
constraint
  (("aaaaaa") ++ (F) ++ (G) ++ (G) ++ (G) ++ ("aaaaaa") ++ (F) ++ ("aaa")) =
    ((E) ++ (E) ++ (G) ++ (G) ++ (F) ++ (F) ++ ("aa"));
constraint
  ((G) ++ (F) ++ ("aaaaaa") ++ (G) ++ (G) ++ (G) ++ (F)) =
    ((A) ++ ("aa") ++ (E) ++ (E) ++ (A) ++ ("aaaaaa"));
constraint ((C) ++ (G) ++ ("a")) = ((E) ++ (E) ++ (E) ++ (E) ++ ("aaaa"));
constraint
  ((F) ++ ("aaa") ++ (F) ++ (F) ++ (F) ++ (F) ++ (G) ++ ("aaaaaa") ++ (F) ++ (F) ++ (F) ++ (F) ++ ("aaa"))
    = ((A) ++ ("aa") ++ (E) ++ (G) ++ (A) ++ ("aaa") ++ (F) ++ ("aa"));
constraint ((C) ++ (G) ++ ("a")) = ((E) ++ (E) ++ (E) ++ (A) ++ (F) ++ (F));
constraint
  (("aaa") ++ (F) ++ ("aaa") ++ (G) ++ (F) ++ (F) ++ ("aaa") ++ (G) ++ (F) ++ ("aaa") ++ (F) ++ ("aaa"))
    = ((E) ++ (A) ++ (A) ++ (F) ++ ("a") ++ (E) ++ ("aaaa"));
constraint
  ((C) ++ (F) ++ ("aaa") ++ (F) ++ ("a")) =
    (("aaaaaa") ++ (F) ++ ("aa") ++ (E) ++ (E) ++ (E) ++ (F) ++ ("a"));
constraint
  ((G) ++ (G) ++ (G) ++ (F) ++ ("aaa") ++ (F) ++ (G) ++ ("aaa")) =
    ((G) ++ (A) ++ ("aaa") ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ (E) ++ (F) ++ ("a"));
constraint
  (("aaaaaa") ++ (F) ++ (G) ++ (F) ++ (F) ++ ("aaa") ++ (G) ++ (G) ++ ("aaa"))
    = ((A) ++ (A) ++ ("a") ++ (D) ++ (F) ++ (F) ++ (F) ++ ("a"));
constraint
  ((F) ++ (F) ++ ("aaa") ++ (F) ++ ("aaa") ++ (F) ++ (G) ++ (G) ++ (F) ++ (F) ++ (F) ++ (F))
    = ((G) ++ ("aa") ++ (E) ++ (E) ++ (A) ++ ("aaaaaa"));
constraint ((C) ++ (G) ++ ("a")) = ((E) ++ (E) ++ (E) ++ (G) ++ (F) ++ (F));
constraint
  ((A) ++ (G) ++ (G) ++ (G) ++ (G) ++ ("aaa")) =
    ((A) ++ ("aa") ++ (E) ++ (A) ++ ("aa") ++ (E) ++ (F) ++ ("a"));
constraint
  ((G) ++ (F) ++ (F) ++ (F) ++ (F) ++ (F) ++ ("aaa") ++ (G) ++ (F) ++ (F) ++ ("aaa") ++ (F))
    =
    ((A) ++ ("aaa") ++ (F) ++ (F) ++ (A) ++ ("aaa") ++ (F) ++ (E) ++ (F) ++ ("a"));
constraint
  ((C) ++ (G) ++ ("a")) = ((E) ++ (D) ++ (F) ++ (E) ++ (F) ++ ("a"));
constraint
  ((C) ++ (G) ++ ("a")) = ((A) ++ (A) ++ (A) ++ (G) ++ ("aaaaaaaaa") ++ (F));
constraint
  ((G) ++ (G) ++ ("aaa") ++ (F) ++ (F) ++ ("aaaaaa") ++ (F) ++ (G) ++ ("aaa"))
    =
    (("aaaaaa") ++ (F) ++ ("aa") ++ (E) ++ (A) ++ ("aa") ++ (E) ++ (F) ++ ("a"));
constraint
  ((F) ++ (F) ++ (F) ++ (G) ++ ("aaaaaa") ++ (F) ++ (G) ++ (G) ++ (F)) =
    ((F) ++ (F) ++ (F) ++ ("aa") ++ (E) ++ (G) ++ ("aa") ++ (E) ++ (F) ++ ("a"));
constraint ((C) ++ (G) ++ ("a")) = ((E) ++ (D) ++ (A) ++ (A));
constraint
  (("aaaaaa") ++ (F) ++ (G) ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ ("aaaaaa") ++ (G) ++ ("aaa"))
    = ((E) ++ (E) ++ (E) ++ (G) ++ ("aaa") ++ (F));
constraint
  ((C) ++ (G) ++ ("a")) =
    ((E) ++ ("aaa") ++ (F) ++ (F) ++ ("aa") ++ (E) ++ (G) ++ ("aaa") ++ (F));
constraint
  ((F) ++ (F) ++ ("aaa") ++ (G) ++ (G) ++ (G) ++ (F) ++ ("aaa") ++ (F) ++ ("aaa"))
    = ((E) ++ (G) ++ ("aa") ++ (E) ++ (A) ++ ("aaaaaa"));
constraint
  ((C) ++ (F) ++ ("aaaaaaa")) = ((D) ++ (A) ++ ("aaaaa") ++ (E) ++ ("aaaa"));
constraint ((C) ++ (G) ++ ("a")) = ((D) ++ (A) ++ (A) ++ (A) ++ ("aa"));
constraint
  ((G) ++ ("aaa") ++ (F) ++ (F) ++ (G) ++ ("aaa") ++ (F) ++ (A) ++ (F) ++ ("aaa"))
    = ((E) ++ (A) ++ (G) ++ (G) ++ (G) ++ ("a"));
constraint
  ((G) ++ (F) ++ (F) ++ ("aaa") ++ (G) ++ (G) ++ (F) ++ (F) ++ (F) ++ ("aaa"))
    = ((E) ++ (E) ++ (E) ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ ("aaa"));
constraint
  ((F) ++ (F) ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ ("aaa") ++ (F) ++ ("aaa") ++ (F) ++ ("aaaaaa") ++ (F) ++ (A))
    = ((A) ++ (A) ++ ("aaaaaa") ++ (F) ++ (A) ++ (G) ++ ("aaa"));
constraint
  ((G) ++ (G) ++ ("aaa") ++ (F) ++ (F) ++ (G) ++ ("aaaaaa") ++ (F) ++ (F)) =
    ((E) ++ (G) ++ (F) ++ ("aaaaaa") ++ (A) ++ (G) ++ ("a"));
constraint
  ((F) ++ ("aaaaaaaaa") ++ (F) ++ (F) ++ ("aaaaaa") ++ (F) ++ (G) ++ (G) ++ (F))
    = ((E) ++ (E) ++ (E) ++ (G) ++ (F) ++ (F));
constraint
  ((C) ++ (F) ++ ("aaa") ++ (F) ++ ("a")) =
    ((E) ++ (G) ++ ("aa") ++ (E) ++ (F) ++ (F) ++ (F) ++ (F) ++ (F));
constraint
  ((F) ++ (F) ++ (F) ++ ("aaaaaa") ++ (F) ++ (G) ++ (G) ++ (G) ++ ("aaa")) =
    ((G) ++ (A) ++ (F) ++ ("a") ++ (E) ++ (E) ++ (F) ++ ("a"));
constraint
  ((F) ++ ("aaa") ++ (F) ++ (F) ++ (F) ++ ("aaa") ++ (G) ++ (G) ++ ("aaa") ++ (F) ++ (F) ++ ("aaa"))
    = ((E) ++ (E) ++ (E) ++ ("aaa") ++ (F) ++ ("aaaaaa") ++ (F));
constraint
  ((C) ++ (F) ++ (F) ++ ("aaaa")) =
    ((A) ++ ("aa") ++ (E) ++ ("aaaaaaaaa") ++ (A) ++ ("aaaaaaaa"));
constraint
  ((C) ++ ("aaa") ++ (F) ++ (F) ++ ("a")) =
    ((E) ++ (E) ++ (A) ++ ("aa") ++ (E) ++ (F) ++ ("a"));
constraint
  ((G) ++ (F) ++ ("aaaaaa") ++ (G) ++ (F) ++ (F) ++ (F) ++ (F) ++ (F) ++ (F) ++ ("aaa"))
    = ((E) ++ (E) ++ (E) ++ (E) ++ ("aaaa"));
constraint
  ((C) ++ ("aaaaaa") ++ (F) ++ ("a")) =
    ((E) ++ (D) ++ (F) ++ (E) ++ ("aaaa"));
constraint
  ((C) ++ (A) ++ ("a")) = ((E) ++ (E) ++ (E) ++ (A) ++ ("aaa") ++ (F));
constraint
  ((C) ++ (G) ++ ("a")) =
    ((A) ++ ("aa") ++ (E) ++ ("aaaaaaaaa") ++ (A) ++ ("aaaaaaaa"));
constraint
  ((F) ++ (F) ++ (F) ++ (F) ++ (F) ++ (A) ++ (F) ++ (G) ++ (G) ++ ("aaa")) =
    ((A) ++ (G) ++ ("a") ++ (D) ++ (A) ++ ("a"));
constraint
  ((G) ++ (G) ++ (G) ++ (F) ++ (F) ++ (F) ++ (F) ++ ("aaaaaa") ++ (F)) =
    ((E) ++ (D) ++ (F) ++ (E) ++ (F) ++ ("a"));
constraint
  ((C) ++ (A) ++ ("a")) = ((D) ++ ("aaa") ++ (E) ++ (E) ++ (F) ++ ("a"));
constraint
  ((F) ++ (F) ++ (F) ++ (G) ++ ("aaaaaaaaa") ++ (G) ++ (F) ++ (F) ++ (F) ++ (F))
    = ((A) ++ (A) ++ ("a") ++ (D) ++ (G) ++ ("a"));
constraint
  ((C) ++ ("aaaaaa") ++ (F) ++ ("a")) =
    ((F) ++ (F) ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ (F) ++ ("aaaaaa") ++ (F) ++ ("aaa") ++ (E) ++ (F) ++ ("a"));
constraint
  ((C) ++ (F) ++ (F) ++ (F) ++ ("a")) =
    ((E) ++ (F) ++ (F) ++ (F) ++ (A) ++ (A) ++ (F) ++ (F) ++ ("aaaa"));
constraint
  (("aaaaaaaaa") ++ (G) ++ (G) ++ ("aaa") ++ (F) ++ (F) ++ (G) ++ ("aaa")) =
    ((A) ++ ("aa") ++ (E) ++ (A) ++ ("aa") ++ (E) ++ (F) ++ ("a"));
constraint
  (("aaa") ++ (F) ++ (F) ++ (F) ++ (F) ++ (F) ++ (G) ++ ("aaa") ++ (F) ++ (F) ++ (F) ++ (A))
    = ((A) ++ (A) ++ ("aaaaaa") ++ (F) ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((C) ++ ("aaa") ++ (F) ++ ("aaaa")) =
    ((A) ++ ("aa") ++ (E) ++ (E) ++ (E) ++ (F) ++ ("a"));
solve satisfy;
