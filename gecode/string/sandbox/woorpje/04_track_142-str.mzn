var string: B;
var string: A;
var string: C;
var string: D;
constraint ((D) ++ ("")) = ((A) ++ (A) ++ (A) ++ ("aaa"));
constraint ((D) ++ ("")) = ((A) ++ (B) ++ (B) ++ (A) ++ (B) ++ ("a"));
constraint ((C) ++ (B)) = ((B) ++ ("aaaa") ++ (B) ++ ("aaaaaaa"));
constraint ((D) ++ ("")) = ((A) ++ ("aaaaaa") ++ (B) ++ (B) ++ ("a"));
constraint
  (("aa") ++ (B) ++ ("aaaa") ++ (B) ++ ("aa") ++ (B) ++ ("a")) =
    ((A) ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((D) ++ ("")) = (("aa") ++ (B) ++ (A) ++ ("aa") ++ (B) ++ ("aaa"));
constraint ((D) ++ ("")) = ((A) ++ ("aaaaaa") ++ (B) ++ ("aaa"));
constraint ((D) ++ ("")) = ((A) ++ (A) ++ (A) ++ (B) ++ ("a"));
constraint
  ((A) ++ ("aaaa") ++ (B) ++ (B) ++ (B) ++ ("a")) =
    (("aa") ++ (B) ++ ("aaaa") ++ (A) ++ ("aaa"));
constraint
  ((D) ++ ("")) =
    (("aa") ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("a"));
constraint
  ((B) ++ ("aa") ++ (A) ++ (A) ++ ("aaa")) =
    (("aa") ++ (B) ++ (A) ++ (A) ++ ("aaa"));
constraint
  ((C) ++ ("aa") ++ ("")) =
    ((A) ++ ("aa") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("a"));
constraint
  ((D) ++ ("")) = ((B) ++ ("aa") ++ (A) ++ ("aaaa") ++ (B) ++ ("a"));
constraint
  ((B) ++ ("aa") ++ (A) ++ ("aa") ++ (B) ++ (B) ++ ("a")) =
    ((B) ++ ("aa") ++ (B) ++ (B) ++ (A) ++ (B) ++ ("a"));
constraint ((D) ++ ("")) = ((A) ++ (B) ++ ("aaaaaa") ++ (B) ++ ("a"));
constraint
  ((A) ++ (B) ++ ("aa") ++ (A) ++ (B) ++ ("a")) =
    ((B) ++ ("aa") ++ (B) ++ (B) ++ (A) ++ (B) ++ ("a"));
constraint ((C) ++ (B)) = ((A) ++ ("aaaaaa") ++ (B) ++ ("aaa"));
constraint ((C) ++ (B)) = (("aa") ++ (B) ++ (A) ++ (B) ++ ("aaaaa"));
constraint ((D) ++ ("")) = (("aa") ++ (B) ++ (A) ++ (A) ++ (B) ++ ("a"));
constraint
  ((A) ++ (A) ++ (B) ++ ("aa") ++ (B) ++ ("a")) =
    ((A) ++ ("aaaa") ++ (A) ++ ("aaa"));
constraint ((D) ++ ("")) = ((B) ++ (B) ++ (A) ++ (A) ++ ("aaa"));
constraint ((D) ++ ("")) = ((B) ++ (B) ++ (A) ++ (B) ++ (B) ++ ("aaa"));
constraint ((D) ++ ("")) = (("aaaa") ++ (A) ++ (B) ++ ("aaaaa"));
constraint
  ((A) ++ (B) ++ ("aa") ++ (B) ++ ("aaaaa")) = ((A) ++ (A) ++ ("aaaaaaa"));
constraint
  ((D) ++ ("")) = (("aa") ++ (B) ++ (B) ++ ("aa") ++ (A) ++ (B) ++ ("a"));
constraint ((D) ++ ("")) = ((A) ++ (B) ++ ("aa") ++ (A) ++ ("aaa"));
constraint
  ((A) ++ (A) ++ (A) ++ ("aaa")) =
    (("aa") ++ (B) ++ (A) ++ (A) ++ (B) ++ ("a"));
constraint ((C) ++ ("aa") ++ ("")) = ((A) ++ (A) ++ (A) ++ ("aaa"));
constraint ((C) ++ (B)) = ((A) ++ ("aa") ++ (B) ++ ("aa") ++ (B) ++ ("aaa"));
constraint
  ((D) ++ ("")) =
    (("aa") ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("a"));
constraint
  ((D) ++ ("")) = ((B) ++ ("aa") ++ (A) ++ (B) ++ (B) ++ (B) ++ ("a"));
constraint ((C) ++ (B)) = ((B) ++ (B) ++ (A) ++ (A) ++ (B) ++ ("a"));
constraint ((C) ++ ("aa") ++ ("")) = ((A) ++ ("aaaa") ++ (A) ++ ("aaa"));
constraint
  ((D) ++ ("")) = ((B) ++ ("aa") ++ (A) ++ (B) ++ ("aa") ++ (B) ++ ("a"));
constraint
  ((B) ++ ("aaaaaa") ++ (A) ++ (B) ++ ("a")) =
    ((A) ++ (B) ++ (B) ++ (A) ++ ("aaa"));
constraint ((C) ++ ("aa") ++ ("")) = ((A) ++ (B) ++ (B) ++ (A) ++ ("aaa"));
constraint
  (("aa") ++ (B) ++ (A) ++ (A) ++ (B) ++ ("a")) =
    (("aaaa") ++ (A) ++ ("aaaaaaa"));
constraint ((C) ++ (B)) = ((A) ++ ("aaaa") ++ (A) ++ ("aaa"));
constraint ((D) ++ ("")) = ((A) ++ (A) ++ (A) ++ (B) ++ ("a"));
constraint
  ((D) ++ ("")) = (("aaaa") ++ (B) ++ ("aaaa") ++ (B) ++ (B) ++ ("a"));
solve satisfy;
