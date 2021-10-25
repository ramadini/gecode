var string: B;
var string: E;
var string: H;
var string: F;
var string: G;
var string: A;
var string: C;
var string: D;
constraint
  ((A) ++ (D) ++ ("aaaa") ++ (F) ++ ("aaa")) =
    ((G) ++ (E) ++ (E) ++ ("aaaaaaaaa") ++ (E) ++ (F) ++ ("aaa"));
constraint
  ((A) ++ (D) ++ (E) ++ ("aa")) =
    ((E) ++ (E) ++ (F) ++ (F) ++ ("a") ++ (E) ++ (F) ++ ("aaa") ++ (G));
constraint
  ((H) ++ (H) ++ ("aaaa") ++ (F)) =
    ((G) ++ ("aaaa") ++ (F) ++ ("a") ++ (E) ++ (E) ++ ("aaaaaaaa") ++ (F) ++ (F));
constraint
  ((H) ++ (D) ++ (D) ++ (F) ++ ("a")) =
    ((F) ++ (F) ++ ("a") ++ (E) ++ (E) ++ ("aaaa") ++ (F) ++ (F) ++ (F) ++ (G));
constraint
  ((H) ++ (F) ++ (F) ++ (F) ++ (F) ++ ("aaaaa") ++ (D) ++ ("aaaaa")) =
    (("aaaa") ++ (F) ++ ("aaaaaaaaaaaaaaaa") ++ (F) ++ ("aaaaaaaa") ++ (F) ++ ("aaaaaaaa") ++ (F) ++ ("aaaa") ++ (F) ++ (F) ++ (F) ++ (F) ++ (E) ++ ("aaaaa"));
constraint
  ((A) ++ (D) ++ ("aaaaaaaaaaa")) =
    ((E) ++ ("aaaa") ++ (F) ++ (F) ++ ("aaaaaa") ++ (E) ++ (E) ++ (E) ++ (E) ++ (F) ++ ("aaaaa") ++ (E) ++ ("aaaaa"));
constraint
  ((A) ++ (F) ++ ("aaaaa") ++ (E) ++ (E) ++ ("aaaaa")) =
    (("aaaa") ++ (F) ++ ("a") ++ (E) ++ (F) ++ ("aaaaa") ++ (E) ++ (F) ++ ("aaa") ++ (G));
constraint
  ((A) ++ (D) ++ (E) ++ ("aa")) =
    (("aaaa") ++ (F) ++ (F) ++ ("aaaa") ++ (F) ++ (F) ++ ("aaa") ++ (E) ++ ("aaaaaaaaaaaaaaaaaa") ++ (E) ++ ("aaaa") ++ (F) ++ ("a") ++ (E) ++ (F) ++ ("a"));
constraint
  ((B) ++ (D) ++ (E) ++ ("aa")) =
    ((F) ++ (F) ++ ("a") ++ (E) ++ ("aaaa") ++ (F) ++ ("a") ++ (E) ++ ("aaaa") ++ (F) ++ ("a") ++ (E) ++ (E) ++ ("aaaaaaaaa") ++ (E) ++ (F) ++ ("a"));
constraint
  ((A) ++ (D) ++ (E) ++ ("aa")) =
    ((E) ++ (E) ++ ("aaaa") ++ (F) ++ ("aaaaaaaaaaaa") ++ (F) ++ ("a") ++ (G));
constraint
  ((B) ++ (D) ++ ("aaaaaaaaaaa")) =
    ((G) ++ (F) ++ ("aaaaa") ++ (E) ++ (F) ++ ("aaaa") ++ (F) ++ ("aaaaaaaa") ++ (F) ++ ("a"));
constraint
  ((A) ++ (F) ++ (F) ++ (F) ++ ("aaaaaaaa") ++ (F) ++ ("aaaaaaaa")) =
    ((E) ++ (E) ++ (F) ++ (F) ++ ("a") ++ (E) ++ (F) ++ ("aaa") ++ (G));
constraint
  ((A) ++ (E) ++ ("aaaa") ++ (F) ++ ("a") ++ (E) ++ (F) ++ ("a")) =
    ((G) ++ (G));
constraint
  ((A) ++ (D) ++ (E) ++ ("aa")) =
    ((G) ++ ("aaaaaaaaaaaa") ++ (F) ++ ("aa") ++ (E) ++ (E) ++ (F) ++ ("aaa"));
constraint
  ((B) ++ ("aaaa") ++ (F) ++ ("aaaa") ++ (F) ++ ("aa") ++ (E) ++ (F) ++ ("a"))
    = ((G) ++ (G));
constraint
  ((A) ++ (D) ++ (F) ++ (F) ++ ("aaa")) =
    ((E) ++ (F) ++ ("aaaaaaaa") ++ (F) ++ ("aa") ++ (E) ++ ("aaaaaaa") ++ (G));
constraint
  ((A) ++ (F) ++ (F) ++ (F) ++ (F) ++ ("aaaaaaaaaaaa") ++ (F)) =
    ((G) ++ (F) ++ (F) ++ (F) ++ (F) ++ ("aa") ++ (E) ++ ("aaaaaaaaaaaa") ++ (F));
constraint
  ((D) ++ ("aaaa") ++ (F) ++ ("a") ++ (E) ++ ("aaa") ++ (D) ++ (D) ++ ("aa"))
    =
    ((G) ++ (F) ++ (F) ++ (F) ++ ("aaaaaa") ++ (E) ++ ("aaaa") ++ (F) ++ (F) ++ ("aaaa"));
constraint
  ((A) ++ (E) ++ ("aaaa") ++ (F) ++ ("a") ++ (E) ++ ("aaaaa")) =
    ((F) ++ (F) ++ ("a") ++ (E) ++ ("aaaaaaaaa") ++ (E) ++ ("aaaa") ++ (F) ++ ("aaaaaaaaaaaaaaaa") ++ (F) ++ (F) ++ (F) ++ (E) ++ ("aaaaa"));
constraint
  ((A) ++ (E) ++ (E) ++ (F) ++ (F) ++ ("aaaaaa")) =
    ((G) ++ (E) ++ (E) ++ (E) ++ (F) ++ (F) ++ ("aaaaaaaa"));
constraint
  ((B) ++ (D) ++ ("aaaaaaaaaaa")) =
    ((E) ++ (E) ++ (F) ++ (F) ++ ("a") ++ (E) ++ (E) ++ (E) ++ ("aaaa") ++ (F) ++ ("aaaa") ++ (F) ++ (F) ++ ("aaaa") ++ (F) ++ (F));
constraint
  ((A) ++ (F) ++ ("aaaaa") ++ (E) ++ (E) ++ (F) ++ ("a")) = ((G) ++ (G));
constraint
  ((A) ++ (E) ++ ("aaaa") ++ (F) ++ ("aaaaaaaa") ++ (F) ++ ("aaa")) =
    ((G) ++ (E) ++ (E) ++ (E) ++ (E) ++ ("aaaaaaa"));
constraint
  ((A) ++ (E) ++ (F) ++ (F) ++ ("a") ++ (E) ++ (F) ++ ("a")) =
    ((F) ++ (F) ++ ("a") ++ (E) ++ (E) ++ (E) ++ ("aaaa") ++ (F) ++ (F) ++ ("aaaaaa") ++ (E) ++ (E) ++ (E) ++ ("aaaaa"));
constraint
  ((A) ++ (E) ++ ("aaaaaaaa") ++ (F) ++ ("aaaa") ++ (F) ++ ("aaa")) =
    (("aaaa") ++ (F) ++ ("a") ++ (E) ++ (E) ++ (E) ++ (E) ++ (F) ++ (F) ++ (F) ++ ("aaaa") ++ (F) ++ ("aaaaaaa") ++ (E) ++ ("aaaaa"));
constraint
  ((H) ++ (D) ++ (D) ++ ("aaaaa")) =
    ((F) ++ (F) ++ ("aaaa") ++ (F) ++ (F) ++ (F) ++ ("aaa") ++ (E) ++ (F) ++ ("aaaaa") ++ (E) ++ ("aaaaaaaaaaaa") ++ (F) ++ (F) ++ ("aaaa") ++ (F) ++ (F));
constraint
  ((A) ++ (F) ++ ("aaaaa") ++ (E) ++ (E) ++ ("aaaaa")) = ((G) ++ (G));
constraint
  ((A) ++ (F) ++ (F) ++ ("a") ++ (E) ++ (E) ++ (F) ++ ("a")) = ((G) ++ (G));
constraint
  ((H) ++ (G) ++ (F)) =
    ((D) ++ ("aaa") ++ (F) ++ ("aaaa") ++ (F) ++ ("aaaaaaa") ++ (G));
constraint
  ((H) ++ (H) ++ (F) ++ ("aaaa")) =
    ((E) ++ (E) ++ (E) ++ (F) ++ ("aaaaaaaaaaaa") ++ (G));
constraint ((D) ++ (E) ++ (E) ++ (H) ++ (F) ++ (F)) = ((G) ++ (G));
constraint ((A) ++ (D) ++ (E) ++ ("aa")) = ((G) ++ (G));
constraint
  ((B) ++ ("aaaaaaaaa") ++ (E) ++ ("aaaaaaaaaaaaaa")) = ((G) ++ (G));
constraint ((B) ++ (D) ++ ("aaaaaaaaaaa")) = ((G) ++ (G));
constraint
  ((A) ++ (D) ++ ("aaaa") ++ (F) ++ ("aaa")) =
    (("aaaaaaaa") ++ (F) ++ (F) ++ ("aaaaaaaaaaa") ++ (E) ++ ("aaaa") ++ (F) ++ (F) ++ ("aaaaaa") ++ (E) ++ (E) ++ ("aaaa") ++ (F) ++ ("aaaaaa"));
constraint
  ((A) ++ (E) ++ (F) ++ ("aaaaa") ++ (E) ++ ("aaaaa")) =
    ((G) ++ (F) ++ (F) ++ ("aaaaaaaaaaaa") ++ (F) ++ ("aaaa") ++ (F) ++ ("aaaaaaaaaaa"));
constraint
  ((H) ++ (D) ++ (E) ++ (E) ++ ("aaaaaaaa")) =
    ((E) ++ (F) ++ ("aaaaa") ++ (E) ++ (E) ++ ("aaaa") ++ (F) ++ ("a") ++ (E) ++ ("aaaaaaaaa") ++ (E) ++ ("aaaa") ++ (F) ++ (F) ++ ("aa"));
constraint
  ((A) ++ ("aaaaaaaaaaaaaaaa") ++ (F) ++ ("aaaa") ++ (F) ++ (F)) =
    ((E) ++ (F) ++ ("aaaa") ++ (F) ++ ("aaaa") ++ (F) ++ (F) ++ ("aaaa") ++ (F) ++ ("aa") ++ (G));
constraint
  ((A) ++ (E) ++ (F) ++ (F) ++ (F) ++ (F) ++ (F) ++ ("aaa")) = ((G) ++ (G));
constraint
  ((A) ++ (F) ++ (F) ++ ("aaaaaaaaaa") ++ (E) ++ ("aaaaa")) =
    ((G) ++ (E) ++ (F) ++ ("aaaaa") ++ (E) ++ (E) ++ (F) ++ ("aaa"));
constraint
  ((A) ++ (E) ++ (E) ++ (F) ++ ("aaaaaaaaaa")) =
    ((G) ++ (E) ++ ("aaaaaaaaaaaa") ++ (F) ++ (F) ++ (F) ++ ("aaaaaaaaaa"));
constraint
  ((B) ++ (E) ++ (F) ++ (F) ++ ("a") ++ (E) ++ (F) ++ ("a")) =
    ((E) ++ (E) ++ (E) ++ (E) ++ (E) ++ (E) ++ (F) ++ ("aaaaaaaaaaaaaa") ++ (E) ++ ("aaaaa"));
constraint
  ((A) ++ ("aaaa") ++ (F) ++ (F) ++ ("aaaaaa") ++ (E) ++ (F) ++ ("a")) =
    ((F) ++ ("aaaaa") ++ (E) ++ ("aaaa") ++ (F) ++ (F) ++ (F) ++ (F) ++ ("aaaaaaa") ++ (E) ++ (F) ++ ("aaaa") ++ (F) ++ (F) ++ ("aaaaaaaaaaaaaaaa"));
constraint
  ((B) ++ (D) ++ (E) ++ ("aa")) =
    ((G) ++ (F) ++ ("aaaa") ++ (F) ++ ("aaaaaa") ++ (E) ++ ("aaaaaaaaaaaaaaaa"));
constraint
  ((B) ++ (E) ++ (F) ++ (F) ++ ("a") ++ (E) ++ (F) ++ ("a")) =
    ((E) ++ ("aaaaaaaaa") ++ (E) ++ (F) ++ ("aaaa") ++ (F) ++ ("aaaa") ++ (G));
constraint
  ((A) ++ (F) ++ (F) ++ (F) ++ (F) ++ (F) ++ ("aaaaaaaaaaaa")) =
    ((G) ++ ("aaaaaaaaa") ++ (E) ++ (E) ++ (E) ++ (F) ++ ("aaa"));
constraint
  ((A) ++ (D) ++ (E) ++ ("aa")) =
    ((G) ++ (E) ++ (F) ++ ("aaaaa") ++ (E) ++ (F) ++ ("aaaa") ++ (F) ++ ("aaaa"));
constraint
  ((A) ++ (D) ++ (F) ++ (F) ++ ("aaa")) =
    ((E) ++ (E) ++ ("aaaa") ++ (F) ++ ("a") ++ (E) ++ ("aaaaaaa") ++ (G));
constraint
  ((H) ++ (G) ++ (F)) =
    ((E) ++ ("aaaa") ++ (F) ++ ("a") ++ (E) ++ (F) ++ ("aaaaaaaa") ++ (F) ++ ("aa") ++ (E) ++ (F) ++ ("aaaaaaaa") ++ (F) ++ (F) ++ (F) ++ ("aaaa") ++ (F));
constraint
  ((B) ++ (F) ++ ("aaaaa") ++ (E) ++ (E) ++ (F) ++ ("a")) = ((G) ++ (G));
constraint
  ((B) ++ (D) ++ (F) ++ (F) ++ ("aaa")) =
    ((E) ++ (E) ++ (E) ++ (F) ++ ("aaaa") ++ (F) ++ (F) ++ (G));
constraint
  ((H) ++ (H) ++ (F) ++ ("aaaa")) =
    ((E) ++ (E) ++ (F) ++ (F) ++ ("a") ++ (E) ++ ("aaaaaaa") ++ (G));
constraint
  ((H) ++ (E) ++ (F) ++ (F) ++ ("a") ++ (E) ++ (E) ++ (E) ++ ("aa")) =
    ((G) ++ (G));
constraint
  ((A) ++ (D) ++ ("aaaa") ++ (F) ++ ("aaa")) =
    ((G) ++ (F) ++ (F) ++ ("a") ++ (E) ++ ("aaaa") ++ (F) ++ ("a") ++ (E) ++ (F) ++ ("aaa"));
constraint
  ((H) ++ (E) ++ (F) ++ ("aaaa") ++ (F) ++ (D) ++ (F) ++ ("a")) =
    ((F) ++ (F) ++ ("a") ++ (E) ++ ("aaaa") ++ (F) ++ ("aaaa") ++ (F) ++ ("aa") ++ (E) ++ (F) ++ ("aaaaa") ++ (E) ++ ("aaaaaaaaa") ++ (E) ++ ("aaaaa"));
constraint
  ((H) ++ (E) ++ (E) ++ ("aaa") ++ (D) ++ (F) ++ ("a")) =
    ((G) ++ (E) ++ ("aaaaaaaa") ++ (F) ++ (F) ++ ("aaaa") ++ (F) ++ ("aaaaaaaaaa"));
constraint
  ((H) ++ (D) ++ (D) ++ (F) ++ ("a")) =
    ((G) ++ (E) ++ (E) ++ (E) ++ (E) ++ ("aaaaaaa"));
constraint ((A) ++ (D) ++ (F) ++ (F) ++ ("aaa")) = ((G) ++ (G));
constraint
  ((F) ++ (F) ++ ("a") ++ (E) ++ (E) ++ (E) ++ ("aaa") ++ (H) ++ ("aaaa") ++ (F))
    = ((G) ++ (G));
constraint
  ((H) ++ (H) ++ (F) ++ (F)) =
    ((F) ++ (F) ++ ("a") ++ (E) ++ (E) ++ ("aaaa") ++ (F) ++ ("aaaa") ++ (F) ++ (G));
constraint ((A) ++ (D) ++ (F) ++ ("aaaaaaa")) = ((G) ++ (G));
constraint
  ((A) ++ (D) ++ ("aaaaaaaaaaa")) =
    ((E) ++ (F) ++ ("aaaaaaaaaaaaaa") ++ (E) ++ (E) ++ (E) ++ (E) ++ (F) ++ (F) ++ ("a") ++ (E) ++ ("aaaaa"));
constraint
  ((A) ++ (F) ++ ("aaaaa") ++ (E) ++ (F) ++ (F) ++ (F) ++ ("aa")) =
    ((G) ++ (E) ++ (F) ++ ("aaaaaaaa") ++ (F) ++ ("aa") ++ (E) ++ ("aaaaaaa"));
constraint ((A) ++ (E) ++ (E) ++ (E) ++ ("aaaaa")) = ((G) ++ (G));
constraint
  ((A) ++ (D) ++ ("aaaaaaaaaaa")) =
    (("aaaaaaaaa") ++ (E) ++ (E) ++ (E) ++ ("aaaaaaa") ++ (G));
constraint
  ((A) ++ ("aaaa") ++ (F) ++ ("a") ++ (E) ++ (E) ++ ("aaaaa")) =
    ((G) ++ ("aaaa") ++ (F) ++ ("aaaaaaaa") ++ (F) ++ (F) ++ ("aaaaaaaaaaaaaaaaaaa"));
constraint
  ((D) ++ (G) ++ (F) ++ ("aaaaa") ++ (E) ++ ("aaaa")) = ((G) ++ (G));
constraint ((B) ++ (E) ++ ("aaaaaaaaa") ++ (E) ++ ("aaaaa")) = ((G) ++ (G));
constraint
  ((A) ++ (F) ++ ("aaaaa") ++ (E) ++ (F) ++ (F) ++ (F) ++ ("aa")) =
    ((G) ++ (E) ++ (E) ++ ("aaaa") ++ (F) ++ ("a") ++ (E) ++ (F) ++ ("aaa"));
constraint
  ((D) ++ ("aaaa") ++ (F) ++ (F) ++ (F) ++ ("aa") ++ (E) ++ (E) ++ (E) ++ (E) ++ (E) ++ ("aa"))
    =
    ((G) ++ (F) ++ (F) ++ ("a") ++ (E) ++ ("aaaaaaaaa") ++ (E) ++ (F) ++ ("aaa"));
constraint
  ((A) ++ (F) ++ (F) ++ ("aaaaaaaaaaaa") ++ (F) ++ (F) ++ (F)) =
    ((E) ++ (E) ++ (F) ++ ("aaaaaaaaaaaa") ++ (F) ++ ("aaaaa") ++ (G));
constraint
  ((E) ++ ("aaaa") ++ (F) ++ ("aaaaaaaaaa") ++ (E) ++ ("aaa") ++ (H) ++ (F) ++ (F))
    =
    ((E) ++ (F) ++ (F) ++ ("a") ++ (E) ++ ("aaaaaaaa") ++ (F) ++ (F) ++ (G));
constraint
  ((B) ++ (D) ++ (E) ++ ("aa")) =
    (("aaaa") ++ (F) ++ ("a") ++ (E) ++ ("aaaaaaaaa") ++ (E) ++ (E) ++ ("aaaa") ++ (F) ++ ("a") ++ (E) ++ ("aaaa") ++ (F) ++ (F) ++ ("aaaa") ++ (F) ++ ("aaa"));
constraint
  ((D) ++ (E) ++ (F) ++ (F) ++ ("a") ++ (H) ++ ("aaaaaaaa")) =
    ((E) ++ (E) ++ (E) ++ (F) ++ (F) ++ ("a") ++ (E) ++ ("aaaaaaaa") ++ (F) ++ (F) ++ ("aaaaaaaaaaaaaaaa") ++ (F) ++ (F) ++ ("a"));
constraint
  ((B) ++ (E) ++ (F) ++ ("aaaa") ++ (F) ++ ("aaaaaaaaaaa")) =
    ((G) ++ (F) ++ ("aaaaa") ++ (E) ++ ("aaaa") ++ (F) ++ ("a") ++ (E) ++ (F) ++ ("aaa"));
constraint
  ((H) ++ (H) ++ (F) ++ ("aaaa")) =
    ((E) ++ (F) ++ (F) ++ ("aaaaaaaaaaaa") ++ (F) ++ (F) ++ (F) ++ (F) ++ (E) ++ (E) ++ (F) ++ (F) ++ ("a") ++ (E) ++ ("aaaaa"));
constraint
  ((A) ++ (E) ++ (E) ++ (E) ++ ("aaaaa")) =
    ((E) ++ (E) ++ ("aaaa") ++ (F) ++ (F) ++ (F) ++ ("aaaa") ++ (F) ++ ("a") ++ (G));
constraint
  ((B) ++ (D) ++ ("aaaaaaaaaaa")) =
    ((E) ++ (C) ++ (F) ++ ("aaaaa") ++ (E) ++ (E) ++ ("aaaaa"));
constraint
  ((B) ++ (D) ++ (E) ++ ("aa")) =
    ((G) ++ (E) ++ (E) ++ (E) ++ ("aaaa") ++ (F) ++ ("aaaaaaaa"));
constraint
  ((A) ++ (E) ++ (E) ++ ("aaaa") ++ (F) ++ ("aaaaaa")) =
    ((F) ++ ("aaaa") ++ (F) ++ (F) ++ ("aa") ++ (E) ++ (E) ++ (C) ++ ("aaaaa"));
constraint
  ((A) ++ (D) ++ ("aaaaaaaaaaa")) =
    ((G) ++ ("aaaa") ++ (F) ++ ("aaaa") ++ (F) ++ ("aa") ++ (E) ++ ("aaaa") ++ (F) ++ ("aaaaaaaa"));
constraint
  ((D) ++ ("aaaa") ++ (F) ++ ("aaaaaaaaaa") ++ (E) ++ ("aaaa") ++ (F) ++ ("aaaaaaa") ++ (D) ++ ("aa"))
    =
    ((E) ++ (E) ++ (E) ++ (F) ++ (F) ++ ("a") ++ (E) ++ (E) ++ (E) ++ (F) ++ ("aaaaaaaa") ++ (F) ++ (F) ++ ("aaa"));
constraint
  ((D) ++ (E) ++ (F) ++ ("aaaaa") ++ (E) ++ (F) ++ (F) ++ ("aaaa") ++ (F) ++ (F) ++ (F) ++ ("aaa") ++ (E) ++ ("aa"))
    = ((G) ++ (E) ++ (E) ++ (E) ++ ("aaaaaaaa") ++ (F) ++ ("aaaa"));
constraint
  ((H) ++ (H) ++ ("aaaa") ++ (F)) =
    ((G) ++ (E) ++ (E) ++ (F) ++ (F) ++ ("a") ++ (E) ++ ("aaaaaaa"));
constraint
  ((A) ++ (E) ++ (E) ++ (F) ++ (F) ++ ("aaaaaa")) =
    ((G) ++ (E) ++ (E) ++ (E) ++ (E) ++ (F) ++ ("aaa"));
constraint
  ((B) ++ (D) ++ (E) ++ ("aa")) =
    (("aaaa") ++ (F) ++ ("a") ++ (E) ++ (F) ++ (F) ++ ("aaaaaaaaaaaaaaaaa") ++ (G));
constraint
  ((A) ++ (D) ++ (E) ++ ("aa")) =
    (("aaaaaaaaa") ++ (E) ++ ("aaaaaaaaa") ++ (E) ++ (E) ++ ("aaaaaaaa") ++ (F) ++ (F) ++ (F) ++ (F) ++ ("aaaa") ++ (F) ++ ("aaaa") ++ (F) ++ ("a"));
constraint
  ((A) ++ (D) ++ (E) ++ ("aa")) =
    (("aaaaaaaaa") ++ (E) ++ (E) ++ (E) ++ (E) ++ ("aaaaaaaa") ++ (F) ++ ("aaaa") ++ (F) ++ ("aaaaaaa") ++ (E) ++ ("aaaaa"));
constraint
  ((A) ++ (E) ++ ("aaaaaaaa") ++ (F) ++ ("aaaa") ++ (F) ++ ("aaa")) =
    ((F) ++ (F) ++ ("a") ++ (E) ++ (E) ++ (E) ++ ("aaaaaaa") ++ (G));
constraint
  ((A) ++ (E) ++ ("aaaa") ++ (F) ++ (F) ++ ("aaaa") ++ (F) ++ ("aaa")) =
    ((F) ++ (F) ++ ("a") ++ (E) ++ (E) ++ (F) ++ (F) ++ ("a") ++ (E) ++ (F) ++ ("aaaa") ++ (F) ++ ("aaaaaa") ++ (E) ++ (F) ++ (F) ++ (F) ++ ("aa"));
constraint
  ((B) ++ (D) ++ (E) ++ ("aa")) =
    ((E) ++ ("aaaa") ++ (F) ++ (F) ++ ("aaaaaa") ++ (E) ++ (E) ++ (F) ++ ("aaaaaaaa") ++ (F) ++ ("aaaa") ++ (F) ++ ("aaaa") ++ (F) ++ (F) ++ (F) ++ ("a"));
constraint
  ((D) ++ (E) ++ (F) ++ ("aaaaa") ++ (H) ++ (F) ++ (F)) =
    ((F) ++ (F) ++ (F) ++ ("aaaa") ++ (F) ++ (F) ++ ("aaa") ++ (E) ++ ("aaaaaaa") ++ (G));
constraint
  ((B) ++ ("aaaaaaaaaaaa") ++ (F) ++ ("aa") ++ (E) ++ (F) ++ ("a")) =
    ((G) ++ (E) ++ ("aaaa") ++ (F) ++ ("a") ++ (E) ++ (F) ++ ("aaaa") ++ (F) ++ (F));
constraint
  ((B) ++ (D) ++ ("aaaa") ++ (F) ++ ("aaa")) =
    ((G) ++ (E) ++ (E) ++ (F) ++ (F) ++ ("a") ++ (E) ++ ("aaaaaaa"));
constraint ((A) ++ (D) ++ (F) ++ ("aaaaaaa")) = ((G) ++ (G));
constraint
  ((B) ++ ("aaaa") ++ (F) ++ ("aaaa") ++ (F) ++ ("aa") ++ (E) ++ (F) ++ ("a"))
    =
    ((E) ++ (E) ++ ("aaaa") ++ (F) ++ ("a") ++ (E) ++ (E) ++ (E) ++ ("aaaa") ++ (F) ++ (F) ++ ("aaaaaa") ++ (E) ++ (F) ++ ("a"));
constraint
  ((H) ++ (H) ++ ("aaaa") ++ (F)) =
    (("aaaaaaaaaaaaaaaa") ++ (F) ++ (F) ++ ("aaa") ++ (E) ++ (E) ++ (E) ++ (E) ++ (F) ++ (F) ++ (F) ++ ("aaaaaaaaaaa"));
constraint
  ((B) ++ (E) ++ ("aaaaaaaaa") ++ (E) ++ (F) ++ ("a")) =
    ((G) ++ ("aaaa") ++ (F) ++ ("a") ++ (E) ++ (E) ++ (E) ++ ("aaaaaaa"));
constraint
  ((A) ++ (E) ++ (E) ++ ("aaaa") ++ (F) ++ (F) ++ ("aa")) =
    ((E) ++ (F) ++ (F) ++ ("aaaa") ++ (F) ++ ("aaaa") ++ (F) ++ ("aaaa") ++ (F) ++ ("aa") ++ (G));
constraint
  ((A) ++ (E) ++ ("aaaaaaaaaaaaaaaa") ++ (F) ++ ("aaa")) = ((G) ++ (G));
constraint
  ((A) ++ (F) ++ (F) ++ ("a") ++ (E) ++ (F) ++ ("aaaaaaaaaa")) =
    ((E) ++ (F) ++ ("aaaaa") ++ (E) ++ ("aaaa") ++ (F) ++ ("aaaaaaaa") ++ (G));
constraint ((B) ++ (D) ++ (F) ++ (F) ++ ("aaa")) = ((G) ++ (G));
constraint
  ((E) ++ (E) ++ ("aaaa") ++ (F) ++ ("a") ++ (E) ++ ("aaa") ++ (H) ++ ("aaaa") ++ (F))
    = ((G) ++ ("aaaa") ++ (F) ++ ("aaaaaaaaaa") ++ (E) ++ (E) ++ ("aaaaaaa"));
solve satisfy;
