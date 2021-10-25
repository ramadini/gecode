var string: B;
var string: E;
var string: J;
var string: H;
var string: I;
var string: F;
var string: G;
var string: A;
var string: C;
constraint ((J) ++ (E) ++ ("a") ++ (J)) = ((B) ++ (H) ++ (H) ++ ("a"));
constraint
  ((J) ++ (E) ++ ("a") ++ (J)) =
    ((F) ++ (H) ++ ("aaa") ++ (I) ++ (H) ++ (H) ++ (I));
constraint
  ((J) ++ (G) ++ (H) ++ (H)) =
    ((F) ++ ("aaa") ++ (H) ++ (H) ++ (H) ++ (H) ++ ("aaa"));
constraint ((J) ++ (G) ++ (I) ++ (H)) = ((B) ++ (H) ++ (I) ++ ("a"));
constraint ((G) ++ (H) ++ ("aaa") ++ (J)) = ((B) ++ (I) ++ (H) ++ ("a"));
constraint
  ((G) ++ (H) ++ (H) ++ (J)) =
    ((F) ++ ("aaa") ++ (I) ++ (I) ++ (H) ++ (I) ++ ("aaa"));
constraint
  ((J) ++ (I) ++ ("aaa") ++ (H) ++ ("aa") ++ (J)) =
    ((B) ++ (H) ++ (I) ++ ("a"));
constraint ((E) ++ ("a") ++ (J) ++ (J)) = ((B) ++ ("aaa") ++ (H) ++ ("a"));
constraint ((G) ++ (G) ++ ("a")) = ((B) ++ (H) ++ (H) ++ ("a"));
constraint ((G) ++ (G) ++ ("a")) = ((B) ++ ("aaaaaaa") ++ (""));
constraint ((J) ++ (J) ++ (J)) = ((B) ++ (H) ++ (I) ++ ("a"));
constraint ((G) ++ (G) ++ ("a")) = ((B) ++ (H) ++ ("aaaa"));
constraint ((E) ++ ("a") ++ (J) ++ (E) ++ ("a")) = ((B) ++ (H) ++ ("aaaa"));
constraint ((J) ++ (G) ++ ("aaa") ++ (H)) = ((B) ++ (C));
constraint
  ((E) ++ (H) ++ (I) ++ (H) ++ (H) ++ (J)) =
    ((C) ++ ("aa") ++ (H) ++ (H) ++ (H) ++ (C) ++ ("aaaaa") ++ (H));
constraint
  ((E) ++ ("a") ++ (J) ++ (I) ++ (H) ++ (H) ++ ("aa")) =
    ((F) ++ (I) ++ (H) ++ (I) ++ (I) ++ (I) ++ (H));
constraint
  ((J) ++ (H) ++ (C) ++ (E) ++ ("aa")) =
    ((F) ++ (H) ++ (H) ++ (I) ++ (H) ++ ("aaaaaa"));
constraint ((G) ++ (I) ++ (I) ++ (J)) = ((B) ++ (I) ++ (H) ++ ("a"));
constraint ((J) ++ (J) ++ (C) ++ ("aaaa")) = ((B) ++ (H) ++ ("aaaa"));
constraint
  ((H) ++ (I) ++ (H) ++ ("aa") ++ (J) ++ (E) ++ ("a")) =
    (("aaa") ++ (H) ++ (H) ++ (H) ++ ("aaa") ++ (H) ++ (H) ++ (H) ++ (H) ++ (H) ++ (H));
constraint ((J) ++ (G) ++ (H) ++ ("aaa")) = ((F) ++ (F) ++ (H));
constraint
  ((G) ++ (H) ++ (H) ++ (J)) =
    (("aaa") ++ (H) ++ (C) ++ ("aa") ++ (F) ++ (H));
constraint ((J) ++ (J) ++ (H) ++ (C) ++ ("a")) = ((B) ++ (C));
constraint
  ((J) ++ (J) ++ (J)) = ((H) ++ ("aaa") ++ (H) ++ ("aaaaaa") ++ (F) ++ (I));
constraint ((G) ++ ("aaa") ++ (H) ++ (J)) = ((B) ++ (H) ++ (H) ++ ("a"));
constraint
  ((G) ++ (H) ++ (I) ++ (J)) = ((F) ++ (C) ++ ("aaaaa") ++ (H) ++ (H));
constraint
  ((H) ++ ("aaa") ++ (H) ++ ("aa") ++ (J) ++ (J)) = ((B) ++ (H) ++ ("aaaa"));
constraint
  ((E) ++ ("a") ++ (J) ++ ("aaa") ++ (H) ++ (H) ++ ("aa")) =
    ((B) ++ (I) ++ (H) ++ ("a"));
constraint
  ((I) ++ (H) ++ ("aaaaa") ++ (J) ++ (E) ++ ("a")) =
    ((B) ++ (H) ++ (H) ++ ("a"));
constraint ((G) ++ (E) ++ (H) ++ ("aaaa")) = ((B) ++ (I) ++ ("aaaa"));
constraint ((J) ++ (G) ++ (H) ++ (H)) = ((B) ++ (C));
constraint
  ((A) ++ ("aaaa") ++ (G) ++ ("a")) = ((B) ++ ("aaa") ++ (H) ++ ("a"));
constraint
  ((J) ++ (E) ++ ("a") ++ (J)) =
    (("aaa") ++ (H) ++ (H) ++ (H) ++ (C) ++ ("aa") ++ (H) ++ (H) ++ ("aaa") ++ (H));
solve satisfy;
