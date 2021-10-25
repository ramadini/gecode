var string: B;
var string: E;
var string: H;
var string: F;
var string: A;
var string: C;
var string: D;
constraint ((A) ++ (E) ++ ("a")) = ((F) ++ (B));
constraint
  ((A) ++ (A) ++ (F)) =
    ((C) ++ (C) ++ ("a") ++ (C) ++ ("a") ++ (C) ++ (F) ++ (C) ++ (C));
constraint
  ((D) ++ ("a") ++ (A) ++ (F)) = (("aa") ++ (F) ++ (F) ++ (F) ++ (C) ++ (C));
constraint
  ((D) ++ ("a") ++ (A) ++ (F)) =
    ((F) ++ (C) ++ (C) ++ (C) ++ (C) ++ ("a") ++ (C) ++ (F));
constraint ((H) ++ (A) ++ (F)) = ((B) ++ (C) ++ (C));
constraint
  ((A) ++ (H) ++ (C) ++ ("a")) = ((F) ++ (F) ++ (F) ++ ("aa") ++ (F));
constraint ((A) ++ (A) ++ (F)) = ((C) ++ (C) ++ (A) ++ (C) ++ ("aaa"));
constraint ((A) ++ (H) ++ (C) ++ (C)) = (("aa") ++ (F) ++ (F) ++ (F) ++ (F));
constraint
  (("aa") ++ (C) ++ (D) ++ (D) ++ ("a")) =
    (("a") ++ (C) ++ (F) ++ (C) ++ (C) ++ (F) ++ (C) ++ ("a"));
constraint
  ((A) ++ (E) ++ ("a")) = (("a") ++ (C) ++ (F) ++ (F) ++ (F) ++ (F));
constraint
  ((H) ++ (A) ++ (C) ++ ("a")) = ((F) ++ ("aa") ++ (C) ++ (C) ++ (F) ++ (F));
constraint ((E) ++ (E)) = ((F) ++ (F) ++ (F) ++ (F) ++ (F));
constraint
  ((A) ++ (A) ++ (C) ++ (C)) = ((F) ++ ("aa") ++ (C) ++ ("aa") ++ (C) ++ (F));
constraint ((A) ++ (A) ++ (F)) = ((F) ++ (F) ++ (F) ++ (F) ++ (F));
constraint
  ((A) ++ (E) ++ (C)) = ((F) ++ (F) ++ ("a") ++ (C) ++ (F) ++ (C) ++ (C));
constraint
  ((F) ++ (F) ++ (A) ++ (C) ++ ("a")) =
    ((F) ++ (C) ++ (C) ++ (C) ++ ("a") ++ (F) ++ ("aa"));
constraint
  ((A) ++ (A) ++ (F)) =
    ((C) ++ ("a") ++ (F) ++ (C) ++ (C) ++ (C) ++ (C) ++ (C) ++ ("a"));
constraint
  ((H) ++ (A) ++ (F)) = (("aa") ++ (F) ++ ("aa") ++ (F) ++ (C) ++ ("a"));
constraint ((A) ++ (A) ++ (F)) = ((F) ++ (F) ++ (F) ++ (D) ++ (C));
constraint
  ((H) ++ (A) ++ (F)) = (("a") ++ (C) ++ (F) ++ (C) ++ (C) ++ (D) ++ (C));
constraint
  ((D) ++ (C) ++ (A) ++ ("aa")) =
    ((F) ++ (F) ++ ("aa") ++ (F) ++ (C) ++ ("a"));
constraint
  ((A) ++ (A) ++ (C) ++ (C)) = ((F) ++ (F) ++ (F) ++ (C) ++ ("aa") ++ (C));
constraint
  ((A) ++ (E) ++ (C)) = ((F) ++ (C) ++ ("a") ++ (F) ++ (C) ++ (C) ++ ("aa"));
constraint ((A) ++ (H) ++ (F)) = ((F) ++ (F) ++ ("aa") ++ (F) ++ (F));
constraint ((A) ++ (H) ++ (F)) = ((F) ++ (F) ++ (F) ++ (C) ++ ("a") ++ (F));
constraint
  ((H) ++ (A) ++ (C) ++ ("a")) =
    ((C) ++ ("aa") ++ (C) ++ (F) ++ (C) ++ ("a") ++ (F));
constraint
  ((H) ++ (A) ++ ("a") ++ (C)) =
    ((C) ++ (C) ++ (F) ++ (C) ++ (C) ++ (F) ++ (F));
constraint ((E) ++ (E)) = (("aa") ++ (F) ++ (F) ++ (F) ++ ("a") ++ (C));
constraint
  ((A) ++ (D) ++ (F) ++ (C)) = ((F) ++ (F) ++ (F) ++ ("aa") ++ (C) ++ (C));
constraint ((A) ++ (A) ++ (F)) = ((F) ++ (F) ++ (F) ++ (F) ++ (F));
constraint ((A) ++ (A) ++ (F)) = (("aa") ++ (F) ++ (F) ++ (F) ++ (F));
constraint
  ((A) ++ (E) ++ ("a")) = ((C) ++ (C) ++ (C) ++ (C) ++ (F) ++ (F) ++ (F));
constraint ((A) ++ (E) ++ ("a")) = ((H) ++ (F) ++ (F) ++ (C) ++ (C));
constraint ((E) ++ (E)) = ((F) ++ (F) ++ (F) ++ (F) ++ (F));
constraint ((A) ++ (D) ++ (D)) = ((F) ++ (F) ++ ("aa") ++ (F) ++ (F));
constraint
  ((A) ++ (H) ++ (F)) = ((F) ++ (F) ++ (C) ++ ("a") ++ (F) ++ (C) ++ (C));
constraint
  ((H) ++ (A) ++ (F)) = ((C) ++ (C) ++ ("aa") ++ (F) ++ (F) ++ ("a") ++ (C));
constraint ((E) ++ (E)) = ((F) ++ (C) ++ ("a") ++ (C) ++ (C) ++ (F) ++ (F));
constraint
  ((A) ++ (A) ++ ("aa")) =
    ((F) ++ (F) ++ ("a") ++ (C) ++ ("a") ++ (C) ++ ("aa"));
constraint
  ((E) ++ (E)) = ((F) ++ ("aa") ++ (F) ++ (C) ++ (C) ++ (C) ++ ("a"));
constraint ((A) ++ (D) ++ (D)) = ((F) ++ (F) ++ (F) ++ (C) ++ ("a") ++ (F));
constraint
  ((A) ++ (H) ++ (F)) = (("aa") ++ (F) ++ (F) ++ ("a") ++ (C) ++ (F));
constraint
  ((H) ++ ("a") ++ (E)) =
    (("aa") ++ (C) ++ ("a") ++ (F) ++ ("a") ++ (C) ++ (F));
constraint
  ((A) ++ (D) ++ (F) ++ (C)) =
    (("a") ++ (C) ++ (C) ++ (C) ++ ("a") ++ (C) ++ (F) ++ (C) ++ ("a"));
constraint
  ((A) ++ (E) ++ ("a")) =
    (("a") ++ (C) ++ (F) ++ ("a") ++ (C) ++ ("a") ++ (C) ++ (C) ++ (C));
constraint
  ((H) ++ (A) ++ ("a") ++ (C)) = (("a") ++ (C) ++ (F) ++ (F) ++ (D) ++ (C));
constraint
  ((A) ++ (D) ++ (C) ++ (C) ++ ("a")) =
    ((F) ++ (C) ++ ("a") ++ (F) ++ (F) ++ ("aa"));
constraint ((A) ++ (A) ++ (F)) = ((C) ++ (C) ++ ("aa") ++ (F) ++ (F) ++ (F));
constraint
  ((E) ++ (E)) = ((F) ++ (C) ++ ("a") ++ (F) ++ ("a") ++ (C) ++ (F));
constraint
  ((A) ++ (H) ++ (F)) =
    ((F) ++ (F) ++ (C) ++ (C) ++ (C) ++ ("a") ++ (C) ++ ("a"));
constraint ((H) ++ (A) ++ (F)) = ((F) ++ (F) ++ (F) ++ (C) ++ (C) ++ (F));
constraint
  ((H) ++ (A) ++ (F)) = ((C) ++ (C) ++ (F) ++ ("a") ++ (C) ++ (F) ++ (F));
constraint
  ((H) ++ (A) ++ ("aa")) =
    ((C) ++ (C) ++ ("a") ++ (C) ++ (C) ++ (C) ++ (F) ++ (F));
constraint
  ((A) ++ (H) ++ (F)) =
    ((C) ++ (C) ++ ("a") ++ (C) ++ (F) ++ (F) ++ (C) ++ (C));
constraint ((E) ++ (E)) = (("a") ++ (C) ++ (F) ++ (F) ++ (F) ++ (F));
constraint
  ((A) ++ (A) ++ ("aa")) = ((C) ++ (C) ++ (F) ++ (F) ++ ("aa") ++ (F));
constraint ((A) ++ (A) ++ (F)) = ((F) ++ (F) ++ (F) ++ (F) ++ (C) ++ (C));
constraint ((D) ++ (C) ++ (H) ++ (F)) = ((F) ++ (F) ++ (F) ++ (F) ++ ("aa"));
constraint
  ((D) ++ ("a") ++ (A) ++ (F)) = (("aa") ++ (F) ++ (F) ++ (F) ++ (F));
constraint
  ((A) ++ (E) ++ ("a")) = ((F) ++ (C) ++ (C) ++ (F) ++ (C) ++ ("aa") ++ (C));
constraint
  ((A) ++ (D) ++ (D)) = (("aa") ++ (F) ++ (F) ++ ("aa") ++ (C) ++ (C));
constraint ((E) ++ (E)) = (("aa") ++ (F) ++ (F) ++ (C) ++ ("aa") ++ (C));
constraint ((A) ++ (A) ++ ("aa")) = ((F) ++ ("aa") ++ (F) ++ (F) ++ (F));
constraint ((H) ++ (A) ++ (F)) = ((F) ++ (F) ++ (F) ++ (F) ++ (F));
constraint
  ((E) ++ (E)) = ((F) ++ (F) ++ (C) ++ ("aa") ++ (C) ++ ("a") ++ (C));
solve satisfy;
