var string: B;
var string: A;
constraint ((A) ++ (B) ++ ("a")) = (("aaaaaaaa") ++ (B) ++ ("aaa"));
constraint ((A) ++ ("aaaaaaaaa") ++ ("")) = ((A) ++ ("aaaaaaaaa") ++ (""));
constraint ((A) ++ (B) ++ ("a")) = (("aaaaaaaa") ++ (B) ++ ("aaa"));
constraint ((A) ++ (B) ++ ("a")) = (("aaaaaaaa") ++ (B) ++ ("aaa"));
constraint ((A) ++ ("aaaaaaaaa") ++ ("")) = (("aaaaaaaa") ++ (B) ++ ("aaa"));
constraint ((A) ++ (B) ++ ("a")) = ((B) ++ ("aaaaaaaaaaa") ++ (""));
constraint (("aaaaaaaa") ++ (B) ++ ("aaa")) = ((B) ++ (A) ++ ("a"));
constraint (("aaaaaaaaaaaaaaaaaaa") ++ ("")) = ((B) ++ (B) ++ ("aaa"));
constraint ((A) ++ ("aaaaaaaaa") ++ ("")) = ((A) ++ ("aaaaaaaaa") ++ (""));
constraint ((B) ++ (B) ++ ("aaa")) = ((B) ++ (B) ++ ("aaa"));
constraint ((A) ++ (B) ++ ("a")) = (("aaaaaaaaaaaaaaaaaaa") ++ (""));
constraint (("aaaaaaaa") ++ (B) ++ ("aaa")) = ((B) ++ (B) ++ ("aaa"));
constraint ((A) ++ ("aaaaaaaaa") ++ ("")) = ((B) ++ (B) ++ ("aaa"));
constraint ((A) ++ ("aaaaaaaaa") ++ ("")) = ((B) ++ (B) ++ ("aaa"));
constraint
  ((B) ++ ("aaaaaaaaaaa") ++ ("")) = ((B) ++ ("aaaaaaaaaaa") ++ (""));
constraint
  (("aaaaaaaa") ++ (B) ++ ("aaa")) = (("aaaaaaaaaaaaaaaaaaa") ++ (""));
constraint ((A) ++ ("aaaaaaaaa") ++ ("")) = (("aaaaaaaa") ++ (B) ++ ("aaa"));
constraint ((B) ++ (B) ++ ("aaa")) = (("aaaaaaaa") ++ (B) ++ ("aaa"));
constraint ((A) ++ (B) ++ ("a")) = ((B) ++ (B) ++ ("aaa"));
constraint ((B) ++ (B) ++ ("aaa")) = (("aaaaaaaa") ++ (B) ++ ("aaa"));
constraint (("aaaaaaaa") ++ (B) ++ ("aaa")) = ((B) ++ (A) ++ ("a"));
constraint
  ((B) ++ ("aaaaaaaaaaa") ++ ("")) = (("aaaaaaaa") ++ (B) ++ ("aaa"));
constraint ((A) ++ (B) ++ ("a")) = ((B) ++ ("aaaaaaaaaaa") ++ (""));
constraint ((B) ++ (B) ++ ("aaa")) = (("aaaaaaaaaaaaaaaaaaa") ++ (""));
constraint ((B) ++ (B) ++ ("aaa")) = ((B) ++ (B) ++ ("aaa"));
constraint ((A) ++ (B) ++ ("a")) = (("aaaaaaaaaaaaaaaaaaa") ++ (""));
constraint ((A) ++ (B) ++ ("a")) = ((B) ++ ("aaaaaaaaaaa") ++ (""));
constraint ((A) ++ ("aaaaaaaaa") ++ ("")) = (("aaaaaaaa") ++ (B) ++ ("aaa"));
constraint
  (("aaaaaaaa") ++ (B) ++ ("aaa")) = (("aaaaaaaaaaaaaaaaaaa") ++ (""));
constraint ((A) ++ (B) ++ ("a")) = ((B) ++ (B) ++ ("aaa"));
constraint ((A) ++ ("aaaaaaaaa") ++ ("")) = (("aaaaaaaa") ++ (B) ++ ("aaa"));
constraint ((A) ++ (B) ++ ("a")) = ((A) ++ ("aaaaaaaaa") ++ (""));
constraint ((A) ++ (B) ++ ("a")) = ((A) ++ ("aaaaaaaaa") ++ (""));
constraint ((B) ++ (B) ++ ("aaa")) = (("aaaaaaaaaaaaaaaaaaa") ++ (""));
constraint ((B) ++ (B) ++ ("aaa")) = (("aaaaaaaa") ++ (B) ++ ("aaa"));
constraint
  ((B) ++ ("aaaaaaaaaaa") ++ ("")) = ((B) ++ ("aaaaaaaaaaa") ++ (""));
constraint ((A) ++ (B) ++ ("a")) = ((B) ++ (B) ++ ("aaa"));
constraint
  (("aaaaaaaaaaaaaaaaaaa") ++ ("")) = ((A) ++ ("aaaaaaaaa") ++ (""));
constraint (("aaaaaaaaaaaaaaaaaaa") ++ ("")) = ((B) ++ (A) ++ ("a"));
constraint ((A) ++ (B) ++ ("a")) = (("aaaaaaaaaaaaaaaaaaa") ++ (""));
constraint
  ((B) ++ ("aaaaaaaaaaa") ++ ("")) = ((B) ++ ("aaaaaaaaaaa") ++ (""));
constraint ((B) ++ (B) ++ ("aaa")) = ((B) ++ ("aaaaaaaaaaa") ++ (""));
constraint ((A) ++ ("aaaaaaaaa") ++ ("")) = ((B) ++ (B) ++ ("aaa"));
constraint ((A) ++ ("aaaaaaaaa") ++ ("")) = (("aaaaaaaa") ++ (B) ++ ("aaa"));
constraint
  (("aaaaaaaa") ++ (B) ++ ("aaa")) = (("aaaaaaaaaaaaaaaaaaa") ++ (""));
constraint ((A) ++ ("aaaaaaaaa") ++ ("")) = (("aaaaaaaa") ++ (B) ++ ("aaa"));
constraint ((A) ++ (B) ++ ("a")) = ((B) ++ (B) ++ ("aaa"));
constraint
  (("aaaaaaaaaaaaaaaaaaa") ++ ("")) = ((A) ++ ("aaaaaaaaa") ++ (""));
constraint ((A) ++ ("aaaaaaaaa") ++ ("")) = ((B) ++ ("aaaaaaaaaaa") ++ (""));
constraint ((A) ++ ("aaaaaaaaa") ++ ("")) = ((B) ++ (A) ++ ("a"));
constraint
  ((A) ++ ("aaaaaaaaa") ++ ("")) = (("aaaaaaaaaaaaaaaaaaa") ++ (""));
constraint ((A) ++ (B) ++ ("a")) = ((B) ++ (B) ++ ("aaa"));
constraint ((A) ++ ("aaaaaaaaa") ++ ("")) = (("aaaaaaaa") ++ (B) ++ ("aaa"));
constraint
  (("aaaaaaaa") ++ (B) ++ ("aaa")) = (("aaaaaaaa") ++ (B) ++ ("aaa"));
solve satisfy;
