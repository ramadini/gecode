var string: B;
var string: E;
var string: F;
var string: A;
var string: C;
var string: D;
constraint
  ((E) ++ (F) ++ ("a") ++ (C) ++ ("aa") ++ (C) ++ (B) ++ (C)) = ((D) ++ (D));
constraint
  ((E) ++ (E) ++ (F) ++ (B) ++ ("a")) =
    ((D) ++ (F) ++ ("aa") ++ (C) ++ (C) ++ ("aa") ++ (C));
constraint
  ((B) ++ (C) ++ ("aaaaa") ++ (C) ++ (C) ++ ("aa") ++ (E)) =
    ((F) ++ (F) ++ (C) ++ (C) ++ (C) ++ ("aa") ++ (C) ++ ("aa") ++ (C) ++ (C) ++ ("aaaa"));
constraint
  ((A) ++ (C) ++ (C) ++ (C) ++ ("a")) =
    ((D) ++ (C) ++ (C) ++ (C) ++ (C) ++ (F) ++ (C) ++ (C) ++ (C));
constraint ((B) ++ (B) ++ (B) ++ (C)) = ((D) ++ (D));
constraint ((B) ++ (B) ++ (E) ++ (C) ++ (C) ++ ("a")) = ((D) ++ (D));
constraint
  ((F) ++ ("aaa") ++ (B) ++ (B) ++ (C)) =
    (("aaa") ++ (C) ++ (F) ++ (C) ++ (C) ++ (C) ++ (D));
constraint ((B) ++ (B) ++ (B) ++ ("a")) = ((D) ++ (D));
constraint ((E) ++ (C) ++ ("a") ++ (B) ++ (B) ++ (C)) = ((D) ++ (D));
constraint
  ((B) ++ ("aa") ++ (C) ++ (C) ++ (F) ++ (C) ++ (C) ++ (E)) =
    ((D) ++ (C) ++ ("a") ++ (C) ++ (C) ++ (C) ++ (C) ++ ("aa") ++ (C) ++ (C) ++ (C));
constraint
  ((B) ++ ("aa") ++ (C) ++ (C) ++ (C) ++ ("aa") ++ (B) ++ (C)) = ((D) ++ (D));
constraint
  ((F) ++ ("a") ++ (C) ++ (C) ++ (B) ++ (E) ++ ("a") ++ (C) ++ (C)) =
    ((D) ++ (F) ++ (F) ++ (C) ++ ("aa"));
constraint ((B) ++ (F) ++ ("a") ++ (C) ++ (C) ++ (B) ++ (C)) = ((D) ++ (D));
constraint
  ((B) ++ (B) ++ (B) ++ (C)) =
    ((C) ++ (C) ++ ("aa") ++ (C) ++ (C) ++ ("a") ++ (C) ++ (C) ++ (C) ++ ("a") ++ (D));
constraint
  ((B) ++ (E) ++ (F) ++ ("a") ++ (E)) =
    ((F) ++ ("a") ++ (C) ++ (C) ++ ("a") ++ (C) ++ (C) ++ (C) ++ (D));
constraint
  ((B) ++ (B) ++ (B) ++ (C)) =
    ((F) ++ (C) ++ ("aaaaaa") ++ (C) ++ ("aaa") ++ (C) ++ (F) ++ ("a") ++ (C));
constraint
  ((B) ++ (E) ++ ("aa") ++ (B) ++ (C)) = ((D) ++ (F) ++ (F) ++ ("aa") ++ (C));
constraint
  ((B) ++ (F) ++ (C) ++ (E) ++ (F) ++ (C)) =
    (("aa") ++ (C) ++ (C) ++ (C) ++ ("a") ++ (C) ++ ("a") ++ (F) ++ (F) ++ ("aa") ++ (C) ++ (C) ++ ("aa"));
constraint
  ((E) ++ ("aa") ++ (B) ++ (B) ++ (C)) =
    ((F) ++ (F) ++ ("a") ++ (C) ++ ("a") ++ (D));
constraint
  ((F) ++ (C) ++ (E) ++ (F) ++ (B) ++ (C)) =
    (("a") ++ (C) ++ ("a") ++ (C) ++ (F) ++ (F) ++ ("a") ++ (C) ++ (C) ++ (C) ++ (F) ++ (C) ++ ("a"));
constraint
  ((F) ++ (F) ++ (F) ++ (C) ++ ("a") ++ (B) ++ (C)) =
    (("aa") ++ (C) ++ ("a") ++ (F) ++ (C) ++ ("aa") ++ (D));
constraint
  (("a") ++ (C) ++ (C) ++ (C) ++ ("a") ++ (E) ++ (F) ++ (B) ++ ("a")) =
    ((D) ++ (D));
constraint
  ((F) ++ ("aaa") ++ (B) ++ (B) ++ ("a")) =
    ((F) ++ ("aa") ++ (C) ++ ("a") ++ (C) ++ (C) ++ ("a") ++ (D));
constraint
  ((B) ++ (B) ++ (B) ++ ("a")) =
    (("a") ++ (C) ++ (C) ++ ("a") ++ (C) ++ (E) ++ (C) ++ (D));
constraint
  ((B) ++ (B) ++ (B) ++ ("a")) =
    ((D) ++ (F) ++ ("aa") ++ (C) ++ ("a") ++ (C) ++ ("a") ++ (C));
constraint ((B) ++ (B) ++ (E) ++ (C) ++ (C) ++ ("a")) = ((D) ++ (D));
constraint ((F) ++ (F) ++ ("aa") ++ (E) ++ (E) ++ (C) ++ (C)) = ((D) ++ (D));
constraint ((B) ++ (B) ++ (B) ++ (C)) = ((D) ++ (D));
constraint
  ((E) ++ (C) ++ ("a") ++ (B) ++ (E) ++ (C) ++ (C) ++ (C)) =
    ((F) ++ (F) ++ (F) ++ (F) ++ (F) ++ ("a") ++ (C));
constraint
  ((C) ++ (C) ++ (C) ++ (C) ++ (C) ++ (C) ++ (C) ++ (B) ++ (E) ++ ("a") ++ (C) ++ ("a"))
    = ((D) ++ (D));
solve satisfy;
