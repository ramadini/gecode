var string: B;
var string: E;
var string: G;
var string: F;
var string: A;
var string: C;
var string: D;
constraint
  ((C) ++ (B) ++ (D)) = ((E) ++ (E) ++ (B) ++ ("aaaaaaaaaaaaaaaaaaa"));
constraint
  ((A) ++ (D) ++ (D)) =
    ((G) ++ ("aaaaaaaaaaaaaaaaaaaaa") ++ (E) ++ ("aaaaa"));
constraint
  ((C) ++ ("aaaaaaaaa") ++ (A) ++ ("aaaaaaaa")) =
    ((G) ++ ("aaaaaaaaaaaaaaaaaaaaa") ++ (E) ++ ("aaaaa"));
constraint
  ((A) ++ (A) ++ ("aaaaaaaa")) =
    ((G) ++ (E) ++ ("aaaaaaaaaaaaaaaaaaaaaaaaaa"));
constraint
  ((E) ++ (E) ++ ("aa") ++ (A) ++ ("aaaaaaaa")) =
    (("aaaaaaaaaaaaaaaaaaaaa") ++ (E) ++ ("aaaaaaaaaaaaaaaaaaaaa") ++ (E) ++ ("aaaaaaaaaaaa"));
constraint
  ((B) ++ ("aaaaaaaaa") ++ (A) ++ ("aaaaaaaa")) =
    (("aaaaaaaaaaaaaaaaaaaaa") ++ (E) ++ (E) ++ (E) ++ ("aaaaaaaaaaaa"));
constraint
  (("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa") ++ (A) ++ ("aaaaaaaa")) =
    ((G) ++ (E) ++ (E) ++ ("aaaaa"));
constraint
  ((A) ++ (E) ++ ("aaaaa") ++ (D)) = ((G) ++ (E) ++ (E) ++ ("aaaaa"));
constraint
  ((A) ++ (A) ++ ("aaaaaaaa")) =
    (("aaaaaaaaaaaaaaaaaaaaa") ++ (E) ++ (A) ++ ("aaaaaaaaaa"));
constraint
  ((E) ++ (E) ++ ("aaaaaaaaaa") ++ (D) ++ ("aaaaaaaaaaaaaaaaaa")) =
    ((E) ++ (B) ++ ("aaaaaaa") ++ (E) ++ ("aaaaaaaaaaaa"));
constraint
  ((B) ++ ("aaaaaaaaa") ++ (A) ++ ("aaaaaaaa")) =
    ((E) ++ (C) ++ (B) ++ ("aaaaa"));
constraint
  ((C) ++ (E) ++ ("aaaaa") ++ (D) ++ ("aaaaaaaaa")) =
    ((G) ++ ("aaaaaaaaaaaaaaaaaaaaa") ++ (E) ++ ("aaaaa"));
constraint
  (("aaaaaaaaaaaaaaaaaaaaa") ++ (E) ++ ("aa") ++ (A) ++ ("aaaaaaaa")) =
    (("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa") ++ (E) ++ ("aaaaaaaaaaaa"));
constraint
  ((B) ++ ("aaaaaaaaa") ++ (A) ++ ("aaaaaaaa")) =
    ((G) ++ ("aaaaaaaaaaaaaaaaaaaaa") ++ (E) ++ ("aaaaa"));
constraint
  ((B) ++ (B) ++ (D)) =
    ((G) ++ ("aaaaaaaaaaaaaaaaaaaaa") ++ (E) ++ ("aaaaa"));
constraint ((A) ++ (A) ++ ("aaaaaaaa")) = ((G) ++ (C) ++ ("aaaaaaaaaaaa"));
constraint
  ((B) ++ (B) ++ ("aaaaaaaaaaaaaaaaaaaaaaaaaa")) =
    ((E) ++ ("aaaaaaaaaaaaaaaaaaaaa") ++ (E) ++ (E) ++ ("aaaaaaaaaaaa"));
constraint
  ((A) ++ (A) ++ ("aaaaaaaa")) =
    ((G) ++ ("aaaaaaaaaaaaaaaaaaaaa") ++ (E) ++ ("aaaaa"));
constraint
  ((B) ++ ("aaaaaaaaa") ++ (A) ++ ("aaaaaaaa")) =
    ((G) ++ ("aaaaaaaaaaaaaaaaaaaaa") ++ (E) ++ ("aaaaa"));
constraint
  ((D) ++ ("aaaaaaaaa") ++ (B) ++ (E) ++ ("aaaaa")) =
    ((G) ++ (E) ++ (E) ++ ("aaaaa"));
constraint
  ((A) ++ (D) ++ (D)) =
    ((A) ++ ("aaaaaaaaaaaaaaaaaaa") ++ (E) ++ ("aaaaaaaaaaaa"));
constraint
  ((A) ++ (A) ++ ("aaaaaaaa")) =
    ((B) ++ ("aaaaaaa") ++ (E) ++ (E) ++ ("aaaaaaaaaaaa"));
constraint
  ((A) ++ (A) ++ ("aaaaaaaa")) =
    (("aaaaaaaaaaaaaaaaaaaaa") ++ (E) ++ (B) ++ ("aaaaaaaaaaaaaaaaaaa"));
constraint
  ((A) ++ (B) ++ ("aaaaaaaaaaaaaaaaa")) =
    ((E) ++ (E) ++ ("aaaaaaaaaaaaaaaaaaaaa") ++ (E) ++ ("aaaaaaaaaaaa"));
constraint
  ((C) ++ ("aaaaaaaaa") ++ (A) ++ ("aaaaaaaa")) =
    ((F) ++ ("aaaaaaaaaaaaaaaaaaaaaa") ++ (""));
constraint
  ((A) ++ (B) ++ ("aaaaaaaaaaaaaaaaa")) =
    ((G) ++ (E) ++ ("aaaaaaaaaaaaaaaaaaaaaaaaaa"));
solve satisfy;
