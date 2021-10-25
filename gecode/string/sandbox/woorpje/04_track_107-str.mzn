var string: B;
var string: E;
var string: A;
var string: D;
constraint ((B) ++ (E) ++ (A)) = ((D) ++ (A) ++ ("a"));
constraint
  (("aaaa") ++ (E) ++ (E) ++ (B) ++ ("aa")) =
    (("aaaaaa") ++ (B) ++ (E) ++ ("aaaa"));
constraint ((B) ++ (B) ++ (E) ++ ("aaaa")) = ((A) ++ (A));
constraint
  ((B) ++ (E) ++ (E) ++ ("aaaaaa")) = ((E) ++ ("aaaaa") ++ (D) ++ ("aa"));
constraint ((B) ++ (B) ++ (B) ++ ("aa")) = ((D) ++ (B) ++ ("aaaaa"));
constraint (("aaaaaa") ++ (B) ++ (B) ++ ("aa")) = ((A) ++ (A));
constraint ((E) ++ ("aa") ++ (B) ++ (E) ++ (E)) = ((A) ++ (B) ++ ("aaaa"));
constraint
  ((E) ++ ("aa") ++ (B) ++ (B) ++ ("aa")) = (("aaaaaaaaa") ++ (D) ++ ("aa"));
constraint ((B) ++ ("aaaaaa") ++ (B) ++ ("aa")) = ((D) ++ (B) ++ ("aaaaa"));
constraint ((D) ++ ("aaa") ++ (E) ++ ("aaaa")) = ((D) ++ (D) ++ ("aa"));
constraint ((B) ++ (E) ++ ("aa") ++ (B) ++ ("aa")) = ((D) ++ (A) ++ ("a"));
constraint
  ((B) ++ (B) ++ ("aaaa") ++ (E)) = ((B) ++ ("aaa") ++ (D) ++ ("aa"));
constraint (("aaaaaa") ++ (B) ++ ("aaaaaaaa")) = ((D) ++ (A) ++ ("a"));
constraint ((B) ++ ("aaaaaa") ++ (B) ++ ("aa")) = ((D) ++ (D) ++ ("aa"));
constraint
  ((B) ++ (B) ++ (E) ++ ("aaaa")) = ((B) ++ ("aaa") ++ (D) ++ ("aa"));
constraint
  (("aaaaaa") ++ (B) ++ (E) ++ ("aaaa")) =
    ((D) ++ ("aaaa") ++ (E) ++ ("aaa"));
constraint ((B) ++ (B) ++ (E) ++ ("aaaa")) = ((D) ++ (D) ++ ("aa"));
constraint
  (("aaaaaa") ++ (B) ++ (B) ++ ("aa")) = ((D) ++ (E) ++ ("aaaaaaa"));
constraint
  ((B) ++ (E) ++ ("aa") ++ (B) ++ ("aa")) = ((D) ++ (B) ++ ("aaaaa"));
constraint
  (("aaaaaa") ++ (B) ++ ("aaaa") ++ (E)) = ((D) ++ (B) ++ ("aaaaa"));
constraint ((E) ++ ("aa") ++ (B) ++ (B) ++ ("aa")) = ((D) ++ (D) ++ ("aa"));
constraint (("aaaaaaaa") ++ (E) ++ ("aaaaaaaa")) = ((A) ++ (B) ++ (E));
constraint
  (("aaaaaa") ++ (B) ++ (B) ++ ("aa")) = ((B) ++ ("aaa") ++ (D) ++ ("aa"));
constraint
  (("aaaaaaaa") ++ (E) ++ (B) ++ ("aa")) =
    (("aaaa") ++ (E) ++ (E) ++ (E) ++ (E));
constraint
  ((B) ++ (B) ++ ("aaaa") ++ (E)) = ((E) ++ ("aaaaa") ++ (D) ++ ("aa"));
constraint ((B) ++ ("aaaaaa") ++ (B) ++ ("aa")) = ((D) ++ (D) ++ ("aa"));
constraint ((B) ++ (B) ++ (B) ++ ("aa")) = ((D) ++ (D) ++ ("aa"));
constraint
  ((B) ++ (E) ++ ("aa") ++ (B) ++ ("aa")) = ((E) ++ ("aaaaaa") ++ (A));
constraint ((B) ++ (B) ++ (B) ++ ("aa")) = ((D) ++ (D) ++ ("aa"));
constraint
  ((B) ++ (B) ++ (B) ++ ("aa")) = ((D) ++ ("aaaa") ++ (E) ++ ("aaa"));
constraint
  ((B) ++ (B) ++ (B) ++ ("aa")) = (("aaaa") ++ (E) ++ ("a") ++ (D) ++ ("aa"));
constraint
  ((B) ++ (E) ++ (E) ++ (E) ++ ("aa")) = ((D) ++ ("aaaa") ++ (E) ++ ("aaa"));
constraint
  (("aaaaaa") ++ (B) ++ ("aaaaaaaa")) = ((B) ++ ("aaa") ++ (D) ++ ("aa"));
constraint
  (("aaaaaa") ++ (B) ++ (B) ++ ("aa")) = ((A) ++ ("aaaaaaaaaa") ++ (""));
constraint
  ((E) ++ ("aa") ++ (B) ++ (E) ++ ("aaaa")) = ((A) ++ (B) ++ ("aaaa"));
constraint ((B) ++ (B) ++ (B) ++ ("aa")) = ((D) ++ (A) ++ ("a"));
constraint ((E) ++ (E) ++ (E) ++ ("aaaa") ++ (E)) = ((D) ++ (A) ++ ("a"));
constraint ((B) ++ ("aaaaaa") ++ (B) ++ ("aa")) = ((D) ++ (D) ++ ("aa"));
constraint (("aaaa") ++ (E) ++ (E) ++ ("aaaaaaaa")) = ((D) ++ (D) ++ ("aa"));
constraint ((B) ++ (E) ++ (E) ++ (E) ++ ("aa")) = ((D) ++ (B) ++ ("aaaaa"));
constraint ((D) ++ ("aaaaaaa") ++ (E)) = ((A) ++ (E) ++ (E) ++ ("aa"));
constraint ((E) ++ (E) ++ ("aaaaaaaa") ++ (E)) = ((D) ++ (D) ++ ("aa"));
constraint ((B) ++ (B) ++ (B) ++ ("aa")) = ((D) ++ (B) ++ ("aaaaa"));
constraint
  ((E) ++ (E) ++ ("aaaa") ++ (E) ++ ("aaaa")) = ((A) ++ (B) ++ ("aaaa"));
constraint ((E) ++ (E) ++ (E) ++ (E) ++ ("aaaa")) = ((A) ++ (A));
constraint ((B) ++ (B) ++ (B) ++ ("aa")) = ((D) ++ (D) ++ ("aa"));
solve satisfy;
