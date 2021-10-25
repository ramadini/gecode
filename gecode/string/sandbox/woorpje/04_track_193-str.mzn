var string: B;
var string: E;
var string: H;
var string: F;
var string: G;
var string: A;
var string: C;
var string: D;
constraint
  ((F) ++ (F) ++ ("aaa")) = ((A) ++ (A) ++ (E) ++ (E) ++ (H) ++ ("aaa"));
constraint
  (("aa") ++ (E) ++ (E) ++ (E) ++ (H) ++ (E) ++ (E) ++ (H) ++ (H) ++ (H) ++ (E) ++ ("a"))
    = ((D) ++ (H));
constraint
  ((F) ++ (F) ++ (E) ++ ("a")) = ((G) ++ (H) ++ ("aaaa") ++ (E) ++ ("a"));
constraint
  (("aa") ++ (E) ++ (E) ++ (E) ++ (E) ++ (E) ++ (E) ++ (H) ++ (E) ++ (H) ++ (E) ++ ("a"))
    = ((D) ++ (E));
constraint
  ((F) ++ (H) ++ (E) ++ (H) ++ (E) ++ ("aa") ++ (E) ++ ("a")) =
    ((C) ++ (E) ++ ("aa") ++ (E) ++ (A) ++ ("aa"));
constraint ((F) ++ (F) ++ (H) ++ ("a")) = ((G) ++ (A) ++ ("aa"));
constraint ((F) ++ (F) ++ (H) ++ ("a")) = ((D) ++ (E));
constraint
  ((F) ++ (E) ++ (E) ++ (H) ++ (E) ++ (H) ++ (E) ++ ("a")) = ((D) ++ (E));
constraint
  ((F) ++ (E) ++ (E) ++ (H) ++ (E) ++ (H) ++ ("aaa")) =
    ((C) ++ (H) ++ (H) ++ (H) ++ (H) ++ (H) ++ ("aaaaa"));
constraint ((F) ++ (F) ++ (H) ++ ("a")) = ((D) ++ (E));
constraint ((F) ++ (B) ++ ("a") ++ (E) ++ ("a")) = ((D) ++ (E));
constraint
  ((E) ++ (E) ++ ("aa") ++ (H) ++ ("aa") ++ (E) ++ ("aa") ++ (H) ++ ("aa") ++ (H) ++ (E) ++ ("a"))
    = ((E) ++ (E) ++ (E) ++ ("a") ++ (A) ++ (A) ++ (H));
constraint
  ((E) ++ (H) ++ ("aa") ++ (H) ++ ("aa") ++ (F) ++ (E) ++ ("a")) =
    ((D) ++ (H));
constraint
  ((H) ++ (E) ++ ("aa") ++ (E) ++ ("aa") ++ (F) ++ (E) ++ ("a")) =
    ((A) ++ ("aaaaaaa") ++ (A) ++ (H));
constraint
  ((F) ++ ("aa") ++ (E) ++ (E) ++ (E) ++ (E) ++ ("aaa")) =
    ((D) ++ ("aa") ++ (""));
constraint
  (("aa") ++ (E) ++ (E) ++ (H) ++ (E) ++ (E) ++ (E) ++ ("aa") ++ (E) ++ (E) ++ (E) ++ ("a"))
    = ((A) ++ (C) ++ (E) ++ (E) ++ ("aa") ++ (E));
constraint ((F) ++ (F) ++ (E) ++ ("a")) = ((D) ++ (E));
constraint ((F) ++ (F) ++ ("aaa")) = ((A) ++ (C) ++ (C));
constraint
  ((F) ++ (E) ++ (E) ++ (E) ++ (H) ++ (E) ++ (E) ++ ("a")) =
    ((A) ++ (A) ++ (A) ++ (H));
constraint
  ((E) ++ (E) ++ ("aa") ++ (H) ++ ("aa") ++ (E) ++ (H) ++ (E) ++ (H) ++ (E) ++ (E) ++ ("a"))
    = ((D) ++ (E));
constraint
  ((E) ++ (H) ++ (E) ++ (E) ++ (E) ++ (F) ++ (E) ++ ("a")) =
    ((D) ++ ("aa") ++ (""));
constraint
  ((F) ++ (F) ++ (E) ++ ("a")) =
    ((A) ++ (C) ++ (H) ++ ("aa") ++ (H) ++ ("aa"));
constraint
  ((E) ++ ("aa") ++ (E) ++ (E) ++ (E) ++ (H) ++ (H) ++ (H) ++ (E) ++ ("aa") ++ (H) ++ ("a"))
    = ((D) ++ (E));
constraint
  ((H) ++ (E) ++ (E) ++ ("aa") ++ (H) ++ (E) ++ (E) ++ (H) ++ (H) ++ (E) ++ ("aaa"))
    = ((A) ++ (E) ++ ("aaaaa") ++ (A) ++ (E));
constraint
  (("aa") ++ (E) ++ (H) ++ (E) ++ ("aa") ++ (F) ++ ("aaa")) =
    ((D) ++ ("aa") ++ (""));
constraint
  ((F) ++ (F) ++ (E) ++ ("a")) = ((E) ++ (E) ++ ("aaa") ++ (A) ++ (A) ++ (E));
constraint ((F) ++ (F) ++ (H) ++ ("a")) = ((D) ++ ("aa") ++ (""));
constraint
  (("aa") ++ (H) ++ (E) ++ (E) ++ (H) ++ (F) ++ ("aaa")) =
    ((A) ++ (A) ++ (A) ++ (H));
constraint
  ((E) ++ (E) ++ (E) ++ ("aa") ++ (H) ++ (F) ++ (E) ++ ("a")) = ((D) ++ (H));
constraint
  ((E) ++ (H) ++ ("aaaa") ++ (E) ++ (F) ++ (E) ++ ("a")) =
    ((H) ++ (H) ++ (E) ++ ("a") ++ (A) ++ (C) ++ ("a"));
constraint
  (("aaaaaa") ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ ("aa") ++ (E) ++ (H) ++ ("a"))
    = ((G) ++ (A) ++ (E));
constraint
  ((F) ++ (H) ++ (H) ++ ("aa") ++ (E) ++ (E) ++ (H) ++ ("a")) =
    ((H) ++ (E) ++ ("aaa") ++ (A) ++ ("aaaaaa") ++ (E) ++ ("a"));
constraint
  ((E) ++ ("aaaaaa") ++ (E) ++ (F) ++ ("aaa")) =
    ((A) ++ ("aaaa") ++ (H) ++ ("a") ++ (A) ++ (E));
constraint
  ((F) ++ ("aa") ++ (E) ++ (H) ++ (E) ++ (E) ++ (H) ++ ("a")) = ((D) ++ (E));
constraint
  ((E) ++ ("aa") ++ (E) ++ (E) ++ (H) ++ (E) ++ (H) ++ ("aa") ++ (E) ++ (E) ++ (H) ++ ("a"))
    = ((A) ++ (A) ++ (B));
constraint
  ((H) ++ (E) ++ (H) ++ ("aa") ++ (E) ++ (E) ++ ("aa") ++ (E) ++ (E) ++ (E) ++ (E) ++ ("a"))
    = ((H) ++ (E) ++ (E) ++ ("a") ++ (A) ++ (C) ++ ("a"));
constraint
  ((E) ++ (E) ++ (E) ++ ("aa") ++ (H) ++ (E) ++ (E) ++ (E) ++ (H) ++ (E) ++ (H) ++ ("a"))
    = ((D) ++ (E));
constraint ((F) ++ (F) ++ (H) ++ ("a")) = ((D) ++ (H));
constraint
  ((H) ++ (E) ++ (E) ++ ("aaaa") ++ (E) ++ (H) ++ (H) ++ (E) ++ (H) ++ (E) ++ ("a"))
    = ((D) ++ (H));
constraint
  ((F) ++ (H) ++ ("aaaa") ++ (E) ++ (H) ++ (E) ++ ("a")) = ((D) ++ (H));
constraint
  ((H) ++ (E) ++ ("aa") ++ (E) ++ (E) ++ (E) ++ (H) ++ (E) ++ ("aa") ++ (H) ++ (E) ++ ("a"))
    = ((D) ++ (E));
constraint
  ((H) ++ (E) ++ (E) ++ ("aa") ++ (H) ++ (H) ++ (H) ++ (E) ++ (E) ++ (H) ++ (E) ++ ("a"))
    = ((A) ++ (E) ++ (E) ++ (E) ++ ("a") ++ (A) ++ (E));
constraint ((F) ++ (F) ++ (H) ++ ("a")) = ((A) ++ (A) ++ (C) ++ ("a"));
constraint
  ((H) ++ ("aa") ++ (E) ++ (H) ++ (H) ++ (F) ++ ("aaa")) =
    ((C) ++ (H) ++ (E) ++ (E) ++ (A) ++ (H));
constraint
  ((E) ++ (H) ++ (E) ++ (B) ++ (H) ++ ("a") ++ (E) ++ ("aaa")) =
    ((C) ++ (E) ++ (E) ++ (E) ++ (A) ++ (E));
constraint
  ((F) ++ (E) ++ (E) ++ (E) ++ (E) ++ (H) ++ (E) ++ ("a")) =
    ((C) ++ (E) ++ (E) ++ (E) ++ (A) ++ ("aa"));
constraint
  (("aa") ++ (E) ++ ("aaaa") ++ (E) ++ (F) ++ (E) ++ ("a")) =
    ((A) ++ ("aaaa") ++ (H) ++ (H) ++ (C));
constraint
  ((H) ++ (H) ++ (E) ++ (E) ++ ("aa") ++ (E) ++ (H) ++ (E) ++ (E) ++ (H) ++ (E) ++ ("a"))
    = ((D) ++ (E));
constraint
  ((H) ++ (H) ++ ("aaaa") ++ (E) ++ (E) ++ ("aa") ++ (H) ++ (E) ++ ("aa") ++ (E) ++ ("a"))
    = ((D) ++ ("aa") ++ (""));
constraint
  ((E) ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ (H) ++ (E) ++ ("aa") ++ (E) ++ ("aaa"))
    = ((A) ++ (E) ++ ("aaaaa") ++ (A) ++ (H));
constraint
  ((E) ++ (E) ++ (H) ++ (H) ++ (E) ++ (F) ++ (E) ++ ("a")) =
    ((A) ++ (E) ++ (E) ++ (H) ++ ("a") ++ (A) ++ (H));
constraint
  ((E) ++ ("aa") ++ (E) ++ (E) ++ (H) ++ (F) ++ ("aaa")) =
    ((H) ++ (H) ++ (H) ++ ("a") ++ (A) ++ (A) ++ ("aa"));
constraint
  ((F) ++ (E) ++ (E) ++ ("aa") ++ (E) ++ (H) ++ ("aaa")) =
    ((D) ++ ("aa") ++ (""));
constraint ((F) ++ (F) ++ (H) ++ ("a")) = ((D) ++ (E));
constraint ((F) ++ (F) ++ ("aaa")) = ((D) ++ ("aa") ++ (""));
constraint
  ((F) ++ (F) ++ ("aaa")) =
    ((A) ++ (E) ++ (E) ++ (H) ++ ("a") ++ (A) ++ ("aa"));
constraint
  ((E) ++ (H) ++ (H) ++ (H) ++ (E) ++ (E) ++ (E) ++ (H) ++ (E) ++ ("aa") ++ (H) ++ ("a"))
    = ((E) ++ (H) ++ ("aaa") ++ (A) ++ (A) ++ (E));
constraint
  ((H) ++ ("aa") ++ (E) ++ (E) ++ (E) ++ ("aa") ++ (H) ++ (E) ++ ("aa") ++ (H) ++ ("aaa"))
    = ((D) ++ ("aa") ++ (""));
constraint
  ((E) ++ ("aa") ++ (E) ++ (E) ++ ("aa") ++ (H) ++ ("aaaa") ++ (E) ++ (E) ++ ("aaa"))
    = ((D) ++ ("aa") ++ (""));
constraint
  ((F) ++ ("aa") ++ (H) ++ (E) ++ (E) ++ (H) ++ ("aaa")) = ((D) ++ (E));
constraint
  ((F) ++ ("aa") ++ (H) ++ (E) ++ (E) ++ (E) ++ ("aaa")) =
    ((C) ++ (G) ++ ("a"));
constraint
  ((F) ++ (E) ++ (E) ++ (H) ++ (E) ++ (E) ++ ("aaa")) =
    ((D) ++ ("aa") ++ (""));
constraint
  ((F) ++ (E) ++ (H) ++ (H) ++ (H) ++ (E) ++ ("aaa")) = ((D) ++ (H));
constraint
  (("aaaa") ++ (H) ++ (H) ++ (E) ++ (H) ++ (H) ++ (E) ++ (E) ++ (E) ++ ("aaa"))
    = ((D) ++ ("aa") ++ (""));
constraint
  ((H) ++ (H) ++ (E) ++ (H) ++ (E) ++ (E) ++ (H) ++ (E) ++ (E) ++ (E) ++ (H) ++ ("a"))
    = ((A) ++ (C) ++ (C));
constraint
  ((E) ++ (H) ++ ("aa") ++ (E) ++ (E) ++ (F) ++ (H) ++ ("a")) = ((D) ++ (H));
constraint
  ((E) ++ (E) ++ (E) ++ ("aa") ++ (E) ++ (H) ++ ("aaaaaaaa") ++ (H) ++ ("a"))
    = ((A) ++ (A) ++ (A) ++ (H));
constraint ((F) ++ (F) ++ (E) ++ ("a")) = ((C) ++ (E) ++ (E) ++ (B) ++ (H));
constraint
  ((E) ++ ("aa") ++ (E) ++ (E) ++ ("aa") ++ (F) ++ ("aaa")) =
    ((A) ++ (C) ++ (E) ++ ("aaaa") ++ (H));
constraint ((F) ++ (F) ++ (E) ++ ("a")) = ((D) ++ (E));
constraint
  ((F) ++ (E) ++ (H) ++ ("aa") ++ (E) ++ ("aa") ++ (E) ++ ("a")) =
    ((D) ++ ("aa") ++ (""));
constraint
  ((H) ++ (E) ++ (H) ++ (H) ++ (E) ++ (F) ++ (E) ++ ("a")) = ((D) ++ (E));
constraint
  ((H) ++ ("aa") ++ (E) ++ (E) ++ (E) ++ (F) ++ (H) ++ ("a")) = ((D) ++ (E));
constraint
  ((F) ++ (F) ++ ("aaa")) =
    ((A) ++ (A) ++ ("aa") ++ (H) ++ ("aa") ++ (E) ++ ("a"));
constraint
  ((E) ++ ("aa") ++ (E) ++ (E) ++ (E) ++ (F) ++ (E) ++ ("a")) = ((D) ++ (H));
constraint
  ((F) ++ (F) ++ (H) ++ ("a")) = ((E) ++ (E) ++ ("aaa") ++ (A) ++ (A) ++ (E));
constraint
  ((E) ++ (E) ++ (E) ++ (H) ++ (H) ++ (E) ++ (E) ++ (E) ++ (E) ++ (E) ++ (E) ++ ("a"))
    = ((A) ++ (A) ++ (A) ++ ("aa"));
constraint
  ((E) ++ (E) ++ (H) ++ (E) ++ ("aa") ++ (F) ++ (H) ++ ("a")) =
    ((D) ++ ("aa") ++ (""));
constraint
  ((E) ++ (H) ++ ("aa") ++ (E) ++ (E) ++ (F) ++ (H) ++ ("a")) =
    ((C) ++ (H) ++ (H) ++ (E) ++ (A) ++ (H));
constraint
  ((E) ++ ("aaaa") ++ (H) ++ (H) ++ (F) ++ (H) ++ ("a")) =
    ((A) ++ (C) ++ (C));
constraint
  ((E) ++ (E) ++ (E) ++ (E) ++ (H) ++ (E) ++ (H) ++ (E) ++ (E) ++ (H) ++ (H) ++ ("a"))
    = ((D) ++ (E));
constraint
  (("aaaa") ++ (H) ++ (E) ++ (H) ++ (F) ++ (H) ++ ("a")) =
    ((A) ++ (C) ++ (C));
constraint ((F) ++ (F) ++ (H) ++ ("a")) = ((D) ++ ("aa") ++ (""));
constraint ((F) ++ (F) ++ ("aaa")) = ((D) ++ (E));
constraint
  ((F) ++ (F) ++ (E) ++ ("a")) =
    (("aa") ++ (E) ++ (E) ++ ("a") ++ (A) ++ (A) ++ (E));
constraint
  ((F) ++ (H) ++ ("aaaa") ++ (E) ++ (E) ++ (E) ++ ("a")) = ((D) ++ (E));
constraint
  ((E) ++ (E) ++ (E) ++ ("aa") ++ (E) ++ (E) ++ (E) ++ (E) ++ (H) ++ (H) ++ (H) ++ ("a"))
    = ((E) ++ (H) ++ (H) ++ ("aa") ++ (C) ++ ("aa") ++ (E) ++ (E) ++ ("a"));
constraint
  ((E) ++ ("aa") ++ (E) ++ ("aa") ++ (E) ++ (F) ++ (E) ++ ("a")) =
    ((E) ++ ("aaaaaa") ++ (H) ++ (F) ++ (H) ++ ("a"));
constraint ((F) ++ (F) ++ (H) ++ ("a")) = ((D) ++ (E));
constraint
  ((F) ++ (H) ++ ("aa") ++ (E) ++ (E) ++ ("aa") ++ (H) ++ ("a")) =
    ((A) ++ (E) ++ (E) ++ (H) ++ ("a") ++ (A) ++ (E));
constraint
  ((H) ++ ("aaaa") ++ (E) ++ (E) ++ ("aaaaaa") ++ (E) ++ (H) ++ ("aaa")) =
    ((D) ++ (E));
constraint
  ((E) ++ (E) ++ (E) ++ (H) ++ (H) ++ (F) ++ ("aaa")) = ((D) ++ (E));
constraint
  ((H) ++ (E) ++ (E) ++ ("aa") ++ (E) ++ ("aaaa") ++ (E) ++ (E) ++ (E) ++ (E) ++ ("a"))
    = ((C) ++ (C) ++ (H) ++ (E) ++ (E) ++ ("a"));
constraint
  ((H) ++ (E) ++ (H) ++ (E) ++ (H) ++ (F) ++ ("aaa")) = ((D) ++ (E));
constraint
  ((E) ++ (E) ++ ("aa") ++ (E) ++ (H) ++ (E) ++ (E) ++ (E) ++ ("aa") ++ (E) ++ (E) ++ ("a"))
    = ((E) ++ ("aaaaa") ++ (A) ++ (C) ++ ("a"));
constraint
  ((F) ++ ("aa") ++ (H) ++ (H) ++ (E) ++ (E) ++ (E) ++ ("a")) =
    ((A) ++ (C) ++ (H) ++ ("aa") ++ (E) ++ (E));
constraint ((F) ++ (F) ++ ("aaa")) = ((D) ++ (H));
constraint
  (("aa") ++ (E) ++ (E) ++ (E) ++ (E) ++ (F) ++ (H) ++ ("a")) = ((D) ++ (H));
constraint
  ((H) ++ (E) ++ ("aa") ++ (E) ++ (E) ++ (F) ++ (E) ++ ("a")) =
    ((D) ++ ("aa") ++ (""));
constraint
  ((A) ++ ("aaa") ++ (E) ++ (E) ++ (E) ++ ("aaaa") ++ (H) ++ ("a")) =
    ((A) ++ (C) ++ (C));
constraint
  ((F) ++ (H) ++ ("aa") ++ (E) ++ (E) ++ (E) ++ ("aaa")) =
    ((A) ++ ("aaaaaaa") ++ (A) ++ (H));
constraint
  ((H) ++ (E) ++ (E) ++ ("aaaa") ++ (F) ++ (E) ++ ("a")) =
    ((E) ++ (H) ++ (E) ++ ("a") ++ (A) ++ (A) ++ (H));
constraint
  ((F) ++ ("aa") ++ (E) ++ (H) ++ (E) ++ ("aa") ++ (E) ++ ("a")) =
    ((D) ++ (E));
constraint
  ((E) ++ ("aa") ++ (H) ++ ("aa") ++ (E) ++ (F) ++ (H) ++ ("a")) =
    ((A) ++ (E) ++ ("aa") ++ (H) ++ ("a") ++ (A) ++ (E));
constraint ((F) ++ (F) ++ (E) ++ ("a")) = ((D) ++ (H));
constraint
  ((F) ++ ("aa") ++ (E) ++ (E) ++ (E) ++ (E) ++ (E) ++ ("a")) = ((D) ++ (H));
constraint
  ((F) ++ (H) ++ (E) ++ (E) ++ ("aa") ++ (H) ++ (E) ++ ("a")) = ((D) ++ (E));
constraint
  ((F) ++ (E) ++ (E) ++ (E) ++ ("aa") ++ (E) ++ (E) ++ ("a")) =
    ((E) ++ (E) ++ (E) ++ ("a") ++ (A) ++ (C) ++ ("a"));
constraint
  ((E) ++ ("aa") ++ (E) ++ (H) ++ (H) ++ (E) ++ (E) ++ (H) ++ (E) ++ ("aa") ++ (E) ++ ("a"))
    = ((A) ++ (A) ++ (A) ++ ("aa"));
constraint
  ((E) ++ (H) ++ (E) ++ (E) ++ (H) ++ (F) ++ ("aaa")) =
    (("aaaa") ++ (E) ++ ("a") ++ (A) ++ (C) ++ ("a"));
constraint
  ((F) ++ (F) ++ (E) ++ ("a")) =
    ((E) ++ (E) ++ (E) ++ ("a") ++ (A) ++ (A) ++ (E));
constraint
  ((B) ++ ("a") ++ (F) ++ (E) ++ ("a")) =
    ((A) ++ (H) ++ (E) ++ (H) ++ ("a") ++ (A) ++ (E));
constraint
  ((F) ++ ("aa") ++ (E) ++ ("aa") ++ (H) ++ (E) ++ (E) ++ ("a")) =
    ((C) ++ (C) ++ (E) ++ (H) ++ (E) ++ ("a"));
constraint
  ((F) ++ (H) ++ (E) ++ (E) ++ (E) ++ (E) ++ (H) ++ ("a")) = ((D) ++ (H));
constraint
  ((F) ++ ("aa") ++ (E) ++ (H) ++ (E) ++ ("aa") ++ (E) ++ ("a")) =
    ((E) ++ (E) ++ (E) ++ ("a") ++ (A) ++ (A) ++ ("aa"));
constraint
  ((F) ++ (H) ++ (H) ++ (E) ++ (E) ++ (E) ++ (E) ++ ("a")) = ((D) ++ (H));
constraint
  ((E) ++ (E) ++ (H) ++ (E) ++ (E) ++ (H) ++ ("aa") ++ (E) ++ (E) ++ (E) ++ (E) ++ ("a"))
    = ((D) ++ (H));
constraint
  ((H) ++ (E) ++ ("aa") ++ (H) ++ ("aa") ++ (H) ++ (H) ++ (E) ++ ("aa") ++ (E) ++ (E) ++ ("a"))
    = ((G) ++ (E) ++ (E) ++ (E) ++ (H) ++ ("a"));
constraint
  ((F) ++ ("aa") ++ (E) ++ ("aa") ++ (H) ++ (E) ++ (E) ++ ("a")) =
    ((E) ++ ("aaaaaa") ++ (C) ++ (E) ++ (E) ++ (H) ++ ("a"));
constraint
  ((F) ++ (E) ++ (E) ++ ("aa") ++ (H) ++ ("aa") ++ (E) ++ ("a")) =
    ((D) ++ (E));
constraint ((F) ++ (E) ++ ("aaaaaa") ++ (E) ++ (H) ++ ("a")) = ((G) ++ (B));
constraint
  ((E) ++ (E) ++ ("aaaa") ++ (E) ++ (F) ++ ("aaa")) = ((D) ++ ("aa") ++ (""));
constraint
  ((H) ++ (H) ++ (H) ++ (E) ++ (H) ++ (H) ++ (H) ++ ("aaaa") ++ (E) ++ ("aaa"))
    = ((D) ++ (E));
constraint
  ((H) ++ (E) ++ ("aa") ++ (E) ++ (E) ++ (E) ++ (E) ++ ("aa") ++ (E) ++ (E) ++ ("aaa"))
    = ((A) ++ (A) ++ (H) ++ (E) ++ (E) ++ ("aaa"));
constraint ((F) ++ (F) ++ ("aaa")) = ((D) ++ (E));
constraint ((F) ++ (F) ++ (H) ++ ("a")) = ((D) ++ (E));
constraint
  ((B) ++ ("a") ++ (E) ++ (E) ++ ("aa") ++ (H) ++ ("aa") ++ (E) ++ ("a")) =
    ((D) ++ (H));
constraint
  ((F) ++ (E) ++ (H) ++ (E) ++ (E) ++ (H) ++ (E) ++ ("a")) = ((D) ++ (E));
constraint
  ((E) ++ (H) ++ ("aa") ++ (H) ++ (E) ++ (F) ++ (H) ++ ("a")) =
    ((H) ++ (E) ++ (E) ++ ("a") ++ (A) ++ (A) ++ (E));
constraint
  ((F) ++ (E) ++ ("aa") ++ (E) ++ (E) ++ (H) ++ (E) ++ ("a")) = ((D) ++ (E));
solve satisfy;
