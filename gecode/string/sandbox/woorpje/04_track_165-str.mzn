var string: B;
var string: E;
var string: H;
var string: F;
var string: G;
var string: A;
var string: C;
var string: D;
constraint
  ((E) ++ ("aaaa") ++ (C) ++ (C) ++ (E)) =
    ((H) ++ ("aaaaaa") ++ (D) ++ ("a"));
constraint
  (("aaaaaaaaaa") ++ (C) ++ (C) ++ ("aaaaaa")) =
    ((E) ++ (E) ++ (E) ++ (E) ++ (C) ++ ("aa"));
constraint
  ((C) ++ (F) ++ ("aaa") ++ (C) ++ ("aaaaaa")) =
    ((H) ++ ("aaaaaa") ++ (E) ++ ("a"));
constraint
  ((C) ++ (E) ++ ("aaaa") ++ (C) ++ ("aaaaaa")) =
    ((E) ++ (E) ++ (E) ++ ("aaaaaa") ++ (E) ++ ("aaaaaa"));
constraint
  ((D) ++ ("a") ++ (F) ++ (D) ++ (C) ++ (E)) =
    ((E) ++ ("aaaaaa") ++ (E) ++ (E) ++ (C) ++ ("aa"));
constraint
  ((G) ++ (C) ++ (F) ++ ("aa")) =
    (("aaaaaa") ++ (E) ++ (E) ++ ("aaaaaa") ++ (D) ++ (D));
constraint
  ((C) ++ (C) ++ (C) ++ (D)) =
    ((D) ++ (E) ++ (E) ++ (E) ++ (E) ++ ("aaaaaa"));
constraint
  ((G) ++ (G) ++ ("aa")) = ((E) ++ ("aaaaaa") ++ (E) ++ (E) ++ (E) ++ (E));
constraint ((G) ++ (G) ++ ("aa")) = ((H) ++ (E) ++ (E) ++ ("a"));
constraint
  ((C) ++ (D) ++ ("a") ++ (F) ++ (F) ++ ("aaaaa")) =
    ((H) ++ (D) ++ (D) ++ ("a"));
constraint ((G) ++ (G) ++ ("aa")) = ((H) ++ (D) ++ (E) ++ ("a"));
constraint
  ((C) ++ ("aaaaaaa") ++ (F) ++ (F) ++ ("aaaaa")) =
    ((H) ++ (E) ++ (E) ++ ("a"));
constraint
  ((D) ++ ("a") ++ (F) ++ ("aaa") ++ (G) ++ ("aa")) =
    ((H) ++ (D) ++ (E) ++ ("a"));
constraint ((G) ++ (E) ++ (D) ++ ("aaaaaaa")) = ((H) ++ (D) ++ (E) ++ ("a"));
constraint
  ((E) ++ ("aaaa") ++ (C) ++ (C) ++ (E)) = ((H) ++ (E) ++ (D) ++ ("a"));
constraint
  ((G) ++ (E) ++ ("a") ++ (F) ++ ("aaaaa")) = ((H) ++ (F) ++ ("aaaaaa"));
constraint ((G) ++ (F) ++ (D) ++ ("aaaaaa")) = ((H) ++ (E) ++ ("aaaaaaa"));
constraint ((G) ++ (G) ++ ("aa")) = ((E) ++ (E) ++ (E) ++ (B) ++ (D));
constraint
  ((G) ++ (G) ++ ("aa")) = (("aaaaaa") ++ (D) ++ (E) ++ (E) ++ (E) ++ (E));
constraint
  ((F) ++ ("aaaaaa") ++ (D) ++ ("a") ++ (C) ++ (E)) =
    ((E) ++ ("aaaaaa") ++ (D) ++ (E) ++ (E) ++ (E));
constraint
  ((G) ++ (G) ++ ("aa")) =
    (("aaaaaa") ++ (E) ++ ("aaaaaa") ++ (E) ++ (E) ++ (D));
constraint
  ((G) ++ (G) ++ ("aa")) =
    ((E) ++ (E) ++ ("aaaaaa") ++ (E) ++ (D) ++ ("aaaaaa"));
constraint ((G) ++ (G) ++ ("aa")) = ((H) ++ (D) ++ (E) ++ ("a"));
constraint ((G) ++ (G) ++ ("aa")) = ((A) ++ ("a") ++ (E));
constraint
  ((D) ++ (E) ++ (E) ++ ("aa") ++ (C) ++ ("aaaaaa")) =
    ((H) ++ (E) ++ ("aaaaaaa"));
constraint ((G) ++ (D) ++ ("aaaaaaaaaaaaa")) = ((H) ++ (D) ++ (E) ++ ("a"));
constraint ((G) ++ (C) ++ (F) ++ ("aa")) = ((H) ++ (E) ++ (D) ++ ("a"));
constraint
  ((E) ++ ("a") ++ (F) ++ ("aaa") ++ (G) ++ ("aa")) =
    ((H) ++ (E) ++ (E) ++ ("a"));
constraint
  ((E) ++ ("a") ++ (F) ++ (E) ++ (C) ++ ("aaaaaa")) =
    ((E) ++ (E) ++ (E) ++ (D) ++ (E) ++ (E));
constraint
  (("aaaaaa") ++ (E) ++ ("aaaaa") ++ (G) ++ ("aa")) = ((H) ++ (D) ++ (F));
constraint ((G) ++ (G) ++ ("aa")) = ((H) ++ ("aaaaaa") ++ (E) ++ ("a"));
constraint ((G) ++ (C) ++ (F) ++ ("aa")) = ((H) ++ (D) ++ (F));
constraint
  ((G) ++ (C) ++ (F) ++ ("aa")) = ((H) ++ ("aaaaaa") ++ (E) ++ ("a"));
constraint
  ((G) ++ (G) ++ ("aa")) =
    (("aaaaaa") ++ (E) ++ ("aaaaaa") ++ (D) ++ (E) ++ (E));
constraint
  ((C) ++ (F) ++ (G) ++ ("aa")) =
    ((C) ++ ("aa") ++ (D) ++ (E) ++ (E) ++ ("aaaaaa"));
constraint
  ((G) ++ (C) ++ ("aaaaaaaaa")) =
    ((D) ++ (D) ++ (E) ++ (D) ++ ("aaaaaa") ++ (E));
constraint ((G) ++ (F) ++ (B)) = ((H) ++ (D) ++ (D) ++ ("a"));
constraint ((G) ++ (G) ++ ("aa")) = ((A) ++ (F));
constraint
  ((C) ++ ("aaaaaaa") ++ (G) ++ ("aa")) =
    ((E) ++ (E) ++ ("aaaaaa") ++ (E) ++ (D) ++ (E));
constraint
  ((F) ++ (F) ++ ("aaa") ++ (G) ++ ("aa")) = ((H) ++ (D) ++ (D) ++ ("a"));
constraint
  (("aaaaaa") ++ (D) ++ (D) ++ ("aa") ++ (C) ++ (D)) =
    ((H) ++ (E) ++ (E) ++ ("a"));
constraint ((G) ++ (F) ++ (F) ++ ("aaaaa")) = ((H) ++ (E) ++ ("aaaaaaa"));
constraint ((G) ++ (G) ++ ("aa")) = ((H) ++ (E) ++ (E) ++ ("a"));
constraint
  ((F) ++ (E) ++ ("aaaa") ++ (G) ++ ("aa")) =
    (("aaaaaa") ++ (D) ++ ("aaaaaa") ++ (E) ++ (D) ++ (D));
constraint ((C) ++ (C) ++ (C) ++ (D)) = ((H) ++ ("aaaaaa") ++ (D) ++ ("a"));
constraint
  ((F) ++ (F) ++ ("aaa") ++ (G) ++ ("aa")) =
    (("aaaaaa") ++ (D) ++ ("aaaaaa") ++ (E) ++ (E) ++ (E));
constraint ((G) ++ (G) ++ ("aa")) = ((E) ++ (E) ++ (D) ++ (E) ++ (D) ++ (E));
constraint
  ((G) ++ (G) ++ ("aa")) = ((E) ++ ("aaaaaa") ++ (D) ++ (E) ++ (E) ++ (D));
constraint ((G) ++ (G) ++ ("aa")) = ((E) ++ (D) ++ (E) ++ (E) ++ (D) ++ (E));
constraint
  ((G) ++ (G) ++ ("aa")) = ((E) ++ (D) ++ (E) ++ (E) ++ (F) ++ ("aaaaa"));
constraint ((G) ++ (G) ++ ("aa")) = ((H) ++ (E) ++ (E) ++ ("a"));
constraint
  ((G) ++ (G) ++ ("aa")) = (("aaaaaa") ++ (E) ++ (E) ++ (D) ++ (D) ++ (D));
constraint
  ((C) ++ (F) ++ (F) ++ (F) ++ ("aaaaa")) =
    ((H) ++ ("aaaaaaaaaaaaa") ++ (""));
constraint
  ((G) ++ (C) ++ (E) ++ ("aaa")) =
    ((D) ++ (E) ++ (D) ++ (E) ++ ("aaaaaa") ++ (E));
constraint
  ((G) ++ (F) ++ (E) ++ ("aaaaaa")) =
    (("aaaaaa") ++ (E) ++ ("aaaaaa") ++ (E) ++ ("aaaaaa") ++ (E));
constraint
  ((G) ++ (G) ++ ("aa")) = ((E) ++ (E) ++ (D) ++ (E) ++ (E) ++ ("aaaaaa"));
constraint
  ((G) ++ (D) ++ ("a") ++ (F) ++ ("aaaaa")) =
    ((D) ++ (E) ++ (E) ++ ("aaaaaaaaaaaaaaaaaa"));
constraint
  ((C) ++ (E) ++ (E) ++ ("aa") ++ (F) ++ ("aaaaa")) =
    ((E) ++ (E) ++ (E) ++ ("aaaaaa") ++ (D) ++ (E));
constraint
  ((C) ++ (D) ++ ("a") ++ (F) ++ (E) ++ (E)) =
    ((H) ++ ("aaaaaa") ++ (E) ++ ("a"));
constraint
  ((C) ++ (E) ++ ("aaaaaaaa") ++ (F) ++ ("aaaaa")) =
    ((H) ++ (E) ++ (D) ++ ("a"));
constraint
  ((C) ++ (E) ++ ("a") ++ (G) ++ ("aa")) = ((H) ++ (E) ++ (D) ++ ("a"));
constraint ((G) ++ (G) ++ ("aa")) = ((E) ++ (E) ++ (D) ++ (D) ++ (D) ++ (E));
constraint
  ((E) ++ ("a") ++ (F) ++ ("aaa") ++ (G) ++ ("aa")) =
    ((D) ++ (E) ++ (C) ++ ("aa") ++ (E) ++ (E));
constraint
  ((F) ++ ("aaaaaaaaaa") ++ (G) ++ ("aa")) =
    ((E) ++ ("aaaaaa") ++ (D) ++ (D) ++ (E) ++ ("aaaaaa"));
constraint
  ((G) ++ (B) ++ (D) ++ ("a")) =
    ((E) ++ ("aaaaaa") ++ (E) ++ (D) ++ (E) ++ (D));
constraint
  ((D) ++ ("aaaa") ++ (C) ++ (F) ++ (F) ++ ("aa")) =
    ((H) ++ (E) ++ (D) ++ ("a"));
constraint
  ((E) ++ ("a") ++ (F) ++ ("aaa") ++ (G) ++ ("aa")) =
    ((H) ++ ("aaaaaa") ++ (E) ++ ("a"));
constraint
  ((C) ++ (E) ++ ("a") ++ (G) ++ ("aa")) = ((H) ++ (D) ++ (E) ++ ("a"));
constraint
  ((C) ++ (D) ++ ("a") ++ (G) ++ ("aa")) = ((H) ++ (E) ++ (E) ++ ("a"));
constraint
  ((D) ++ ("a") ++ (F) ++ ("aaa") ++ (G) ++ ("aa")) =
    ((D) ++ (E) ++ (E) ++ (B) ++ (E));
constraint
  ((G) ++ (C) ++ (F) ++ ("aa")) =
    ((D) ++ ("aaaaaa") ++ (D) ++ (E) ++ ("aaaaaa") ++ (D));
constraint
  ((G) ++ (G) ++ ("aa")) =
    ((D) ++ ("aaaaaa") ++ (D) ++ ("aaaaaa") ++ (D) ++ (E));
constraint
  ((D) ++ (D) ++ ("aaaaa") ++ (G) ++ ("aa")) = ((A) ++ (D) ++ ("a"));
constraint ((G) ++ (G) ++ ("aa")) = ((H) ++ ("aaaaaa") ++ (D) ++ ("a"));
constraint ((C) ++ (F) ++ (G) ++ ("aa")) = ((H) ++ (D) ++ ("aaaaaaa"));
constraint
  ((C) ++ (F) ++ ("aaaaaaa") ++ (F) ++ ("aaaaa")) =
    ((H) ++ (E) ++ (E) ++ ("a"));
constraint ((G) ++ (G) ++ ("aa")) = ((H) ++ (E) ++ (F));
constraint ((G) ++ (G) ++ ("aa")) = ((H) ++ (E) ++ (F));
constraint ((G) ++ (G) ++ ("aa")) = ((E) ++ (D) ++ (E) ++ (D) ++ (E) ++ (E));
constraint
  ((G) ++ (G) ++ ("aa")) = ((E) ++ ("aaaaaa") ++ (E) ++ (D) ++ (E) ++ (E));
constraint
  ((C) ++ (C) ++ (F) ++ (D) ++ ("aaa")) =
    ((E) ++ (D) ++ (E) ++ (D) ++ ("aaaaaa") ++ (E));
constraint
  ((C) ++ (F) ++ ("aaa") ++ (C) ++ (E)) =
    ((E) ++ (E) ++ ("aaaaaa") ++ (D) ++ (E) ++ (E));
constraint
  ((G) ++ (C) ++ (F) ++ ("aa")) =
    ((D) ++ (E) ++ ("aaaaaa") ++ (D) ++ (E) ++ (E));
constraint
  ((F) ++ (E) ++ ("aaaaaaaa") ++ (F) ++ (F) ++ ("a")) =
    ((H) ++ (D) ++ (E) ++ ("a"));
constraint
  ((F) ++ (D) ++ ("aaaa") ++ (G) ++ ("aa")) = ((H) ++ (E) ++ ("aaaaaaa"));
constraint
  ((G) ++ (F) ++ (F) ++ ("aaaaa")) =
    ((E) ++ (D) ++ (E) ++ ("aaaaaa") ++ (D) ++ (E));
constraint ((C) ++ (F) ++ (G) ++ ("aa")) = ((H) ++ (E) ++ (E) ++ ("a"));
constraint
  ((C) ++ (D) ++ ("a") ++ (G) ++ ("aa")) = ((H) ++ (D) ++ (E) ++ ("a"));
constraint
  ((C) ++ (C) ++ (C) ++ (D)) = ((E) ++ (E) ++ (E) ++ (D) ++ (E) ++ (E));
constraint ((G) ++ (F) ++ (F) ++ ("aaaaa")) = ((H) ++ (E) ++ (E) ++ ("a"));
constraint
  (("aaaaaaa") ++ (F) ++ ("aaa") ++ (G) ++ ("aa")) =
    ((E) ++ (E) ++ (E) ++ ("aaaaaaaaaaaa") ++ (E));
constraint ((G) ++ (G) ++ ("aa")) = ((H) ++ ("aaaaaa") ++ (E) ++ ("a"));
constraint
  ((G) ++ (F) ++ (F) ++ ("aaaaa")) =
    (("aaaaaa") ++ (E) ++ (E) ++ ("aaaaaa") ++ (E) ++ (E));
constraint
  ((C) ++ (E) ++ ("a") ++ (G) ++ ("aa")) =
    ((H) ++ ("aaaaaa") ++ (E) ++ ("a"));
solve satisfy;
