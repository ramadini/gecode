var string: B;
var string: E;
var string: H;
var string: I;
var string: F;
var string: G;
var string: C;
var string: D;
constraint
  ((C) ++ (C) ++ ("ab") ++ (B) ++ (F) ++ ("aaa")) =
    ((C) ++ ("b") ++ (F) ++ (F) ++ ("a") ++ (C) ++ ("aaaa"));
constraint
  ((H) ++ (B) ++ (C) ++ ("aa") ++ (F)) =
    (("a") ++ (G) ++ (G) ++ ("aaaa") ++ (C) ++ (F) ++ ("a") ++ (G) ++ ("a"));
constraint
  ((D) ++ ("aaaaba") ++ (C) ++ ("a")) =
    (("ba") ++ (G) ++ ("aaaaaa") ++ (F) ++ (C) ++ ("a"));
constraint
  (("a") ++ (G) ++ (G) ++ (I) ++ (G) ++ (B) ++ (G) ++ ("b")) =
    (("a") ++ (C) ++ (G) ++ ("b") ++ (G) ++ ("ab") ++ (F) ++ ("abb") ++ (G));
constraint
  ((D) ++ ("a") ++ (C) ++ ("a") ++ (C) ++ ("b") ++ (G) ++ (G)) =
    ((G) ++ ("a") ++ (F) ++ ("aabba") ++ (G) ++ (G) ++ (C) ++ (G));
constraint
  ((F) ++ (F) ++ (G) ++ (F) ++ (C) ++ ("a") ++ (F) ++ (C)) =
    ((F) ++ (F) ++ ("b") ++ (F) ++ (G) ++ (G) ++ ("a") ++ (F) ++ (C));
constraint
  ((F) ++ ("b") ++ (F) ++ ("b") ++ (B) ++ ("a") ++ (G) ++ ("aa") ++ (G)) =
    (("ba") ++ (G) ++ (F) ++ (G) ++ ("aa") ++ (F) ++ (F) ++ ("a") ++ (G));
constraint
  ((C) ++ ("a") ++ (B) ++ (C) ++ (C) ++ (G) ++ ("aa") ++ (G)) =
    ((G) ++ (F) ++ ("aab") ++ (G) ++ (C) ++ (G) ++ (F) ++ ("ab"));
constraint
  (("a") ++ (B) ++ ("a") ++ (C) ++ ("a") ++ (G) ++ ("ababa")) =
    (("aaa") ++ (F) ++ (C) ++ ("a") ++ (F) ++ (G) ++ ("a") ++ (G) ++ ("a"));
constraint
  (("a") ++ (C) ++ (F) ++ (F) ++ (C) ++ ("ab") ++ (F) ++ ("a")) =
    (("a") ++ (G) ++ (G) ++ ("babab") ++ (G) ++ ("a") ++ (G) ++ (F) ++ ("a"));
constraint
  ((I) ++ (C) ++ ("a") ++ (G) ++ ("a") ++ (G) ++ (B)) =
    ((I) ++ ("bba") ++ (G) ++ ("a") ++ (F) ++ ("ab"));
constraint
  ((F) ++ ("a") ++ (G) ++ (I) ++ ("bb") ++ (G) ++ (F)) =
    (("baa") ++ (C) ++ ("bba") ++ (C) ++ ("b") ++ (G) ++ (F));
constraint
  ((F) ++ (I) ++ (E)) = ((F) ++ (G) ++ (G) ++ (F) ++ (F) ++ ("bbaaaa"));
constraint
  ((B) ++ ("a") ++ (B) ++ (B) ++ ("a") ++ (G) ++ ("ab")) =
    (("aa") ++ (F) ++ ("aa") ++ (G) ++ ("aa") ++ (F) ++ (G) ++ ("ab"));
constraint
  ((G) ++ ("aaaa") ++ (B) ++ (B) ++ (F) ++ ("a")) =
    ((F) ++ ("aaaaa") ++ (G) ++ ("aa") ++ (C) ++ ("aa"));
constraint
  ((F) ++ (C) ++ (G) ++ (F) ++ (G) ++ (G) ++ ("b") ++ (B) ++ ("a")) =
    ((F) ++ (C) ++ ("bba") ++ (C) ++ (F) ++ ("a") ++ (G) ++ ("a"));
constraint
  (("baab") ++ (F) ++ (G) ++ (G) ++ (B) ++ (G) ++ (F)) =
    (("baa") ++ (G) ++ (F) ++ ("bbaa") ++ (C) ++ (F));
constraint
  (("aa") ++ (B) ++ ("bb") ++ (C) ++ (F) ++ (C) ++ ("a")) =
    (("aaaa") ++ (G) ++ (G) ++ (C) ++ (G) ++ (F) ++ ("b") ++ (F));
constraint
  (("a") ++ (D) ++ (G) ++ (B) ++ ("aaaa")) =
    (("a") ++ (F) ++ (G) ++ ("aabaa") ++ (F) ++ ("aaa"));
constraint
  (("aa") ++ (C) ++ ("aaa") ++ (G) ++ (G) ++ (B) ++ (C)) =
    (("aa") ++ (G) ++ (F) ++ ("aa") ++ (C) ++ ("aa") ++ (G) ++ ("b") ++ (G));
constraint
  ((F) ++ ("aa") ++ (C) ++ ("aa") ++ (F) ++ (G) ++ (F) ++ (G)) =
    (("baaa") ++ (G) ++ (G) ++ (B) ++ ("a") ++ (G) ++ (F) ++ ("b"));
constraint
  ((G) ++ ("a") ++ (G) ++ (G) ++ (H) ++ (C) ++ (C) ++ ("b")) =
    (("ba") ++ (C) ++ ("ab") ++ (F) ++ ("a") ++ (C) ++ (C) ++ ("b"));
constraint
  (("a") ++ (G) ++ ("b") ++ (F) ++ (C) ++ (G) ++ (F) ++ ("a") ++ (C) ++ (G))
    = (("a") ++ (C) ++ (F) ++ (G) ++ (G) ++ (G) ++ ("baa") ++ (C) ++ (G));
constraint
  (("b") ++ (F) ++ ("aa") ++ (C) ++ (C) ++ (F) ++ ("aaa")) =
    ((C) ++ ("aaa") ++ (C) ++ ("b") ++ (G) ++ (F) ++ ("aaa"));
constraint
  (("a") ++ (C) ++ ("b") ++ (F) ++ ("ab") ++ (B) ++ (G) ++ ("bb")) =
    (("a") ++ (G) ++ ("bb") ++ (F) ++ ("a") ++ (F) ++ ("a") ++ (G) ++ ("b") ++ (G) ++ ("b"));
constraint
  (("b") ++ (G) ++ ("baa") ++ (F) ++ ("a") ++ (I) ++ (G)) =
    ((C) ++ (F) ++ ("a") ++ (G) ++ ("aa") ++ (C) ++ (F) ++ ("b") ++ (G));
constraint
  ((G) ++ (B) ++ (F) ++ (C) ++ (G) ++ ("a") ++ (G) ++ ("aaa")) =
    ((F) ++ ("a") ++ (G) ++ ("ba") ++ (C) ++ (G) ++ ("abaaa"));
constraint
  ((F) ++ (G) ++ (F) ++ ("b") ++ (G) ++ (C) ++ (F) ++ (B)) =
    ((G) ++ ("ab") ++ (F) ++ (C) ++ ("bb") ++ (G) ++ ("aaa") ++ (G));
constraint
  ((G) ++ ("a") ++ (F) ++ (D) ++ ("aa") ++ (F) ++ ("a")) =
    ((G) ++ ("aba") ++ (F) ++ (F) ++ ("aaa") ++ (G) ++ ("aa"));
constraint
  (("a") ++ (B) ++ (G) ++ ("a") ++ (G) ++ ("b") ++ (G) ++ ("b") ++ (G) ++ (B))
    = (("aaa") ++ (G) ++ (F) ++ ("b") ++ (G) ++ (C) ++ (G) ++ ("aa") ++ (G));
constraint
  ((B) ++ ("b") ++ (G) ++ (C) ++ (C) ++ ("babab")) =
    (("aa") ++ (C) ++ (G) ++ ("b") ++ (G) ++ (G) ++ (G) ++ (F) ++ (F) ++ ("b"));
constraint
  (("abbbb") ++ (F) ++ ("a") ++ (B) ++ ("aaa")) =
    (("a") ++ (C) ++ (C) ++ (G) ++ ("aaaa") ++ (G) ++ ("aaa"));
constraint
  ((F) ++ (F) ++ (C) ++ (C) ++ (B) ++ (F) ++ ("a")) =
    ((F) ++ (G) ++ ("a") ++ (C) ++ ("bb") ++ (B) ++ (G) ++ ("aa"));
constraint
  (("a") ++ (G) ++ ("aba") ++ (F) ++ (B) ++ ("ba") ++ (F)) =
    (("aba") ++ (G) ++ ("a") ++ (G) ++ ("aaa") ++ (G) ++ ("ba") ++ (F));
constraint
  ((F) ++ ("ba") ++ (G) ++ (F) ++ (G) ++ (G) ++ (C) ++ ("a") ++ (F)) =
    ((G) ++ ("a") ++ (F) ++ ("bba") ++ (C) ++ (C) ++ ("aba"));
constraint
  (("aa") ++ (I) ++ ("aa") ++ (B) ++ ("aa")) =
    (("aa") ++ (G) ++ (G) ++ (F) ++ (F) ++ ("a") ++ (B) ++ ("aa"));
constraint
  (("aaa") ++ (G) ++ ("aa") ++ (B) ++ (C) ++ ("b") ++ (F)) =
    (("aaa") ++ (F) ++ ("aaa") ++ (C) ++ (C) ++ (F));
constraint
  (("aaab") ++ (G) ++ (G) ++ ("a") ++ (G) ++ ("a") ++ (G) ++ ("aa") ++ (G) ++ ("a"))
    = (("aaa") ++ (G) ++ (G) ++ (F) ++ (G) ++ ("a") ++ (F) ++ ("a") ++ (F));
constraint
  ((G) ++ (G) ++ ("a") ++ (I) ++ ("ba") ++ (G) ++ (F) ++ (G)) =
    ((G) ++ (G) ++ ("abb") ++ (G) ++ ("a") ++ (G) ++ (F) ++ (G) ++ (G) ++ ("ab"));
constraint
  (("a") ++ (G) ++ ("bb") ++ (F) ++ ("a") ++ (F) ++ ("aa") ++ (C) ++ ("a")) =
    (("a") ++ (C) ++ (C) ++ ("aa") ++ (G) ++ ("aaa") ++ (C) ++ ("a"));
constraint
  (("aa") ++ (C) ++ ("baabbba") ++ (F) ++ ("b")) =
    (("aa") ++ (C) ++ (F) ++ ("a") ++ (C) ++ (F) ++ (F) ++ ("b"));
constraint
  (("ba") ++ (G) ++ ("baaa") ++ (B) ++ ("a") ++ (G) ++ (F)) =
    (("bab") ++ (F) ++ ("aaaa") ++ (F) ++ (C) ++ ("a"));
constraint
  (("aa") ++ (G) ++ ("a") ++ (G) ++ (G) ++ ("a") ++ (G) ++ (G) ++ ("b") ++ (F) ++ ("aa"))
    = (("aa") ++ (G) ++ ("a") ++ (C) ++ ("abb") ++ (C) ++ ("aaa"));
constraint
  (("ba") ++ (H) ++ (G) ++ ("aaa") ++ (G) ++ ("ba")) =
    ((G) ++ ("aa") ++ (G) ++ ("baabaaa") ++ (C) ++ ("a"));
constraint
  (("b") ++ (F) ++ ("aa") ++ (G) ++ ("ab") ++ (G) ++ (G) ++ ("b") ++ (B)) =
    ((C) ++ ("aaa") ++ (F) ++ (C) ++ ("b") ++ (G) ++ (B));
constraint
  (("b") ++ (I) ++ (G) ++ (F) ++ ("a") ++ (B) ++ ("a")) =
    ((C) ++ (G) ++ (F) ++ ("bbbaaaa") ++ (F));
constraint
  ((F) ++ (B) ++ ("baa") ++ (C) ++ ("aab") ++ (G)) =
    (("baaab") ++ (G) ++ ("aab") ++ (F) ++ ("ab") ++ (G));
constraint
  ((C) ++ (B) ++ (F) ++ ("aa") ++ (I)) =
    (("bb") ++ (B) ++ (F) ++ ("aa") ++ (G) ++ (G) ++ (F) ++ (G));
constraint
  (("aaa") ++ (C) ++ (G) ++ ("a") ++ (F) ++ ("a") ++ (C) ++ (F)) =
    (("aaa") ++ (C) ++ (F) ++ (F) ++ ("ab") ++ (G) ++ (F));
constraint
  (("aa") ++ (F) ++ ("aaa") ++ (F) ++ ("a") ++ (F) ++ (C)) =
    (("aa") ++ (G) ++ ("aaaabaa") ++ (F) ++ (C));
constraint
  ((F) ++ (C) ++ ("a") ++ (G) ++ (B) ++ ("a") ++ (F) ++ (G) ++ (G)) =
    ((G) ++ ("a") ++ (C) ++ ("a") ++ (G) ++ ("aa") ++ (F) ++ (F) ++ (G) ++ ("b"));
constraint
  ((B) ++ ("aba") ++ (B) ++ (G) ++ ("a") ++ (G) ++ ("ab")) =
    (("aababa") ++ (B) ++ ("baba") ++ (G));
constraint
  (("aaab") ++ (D) ++ ("aa") ++ (G) ++ (G) ++ ("b")) =
    (("aaabba") ++ (G) ++ ("aaaa") ++ (G) ++ ("b") ++ (G));
constraint
  ((C) ++ ("a") ++ (C) ++ (G) ++ ("aaa") ++ (B) ++ (C)) =
    ((C) ++ ("a") ++ (C) ++ (F) ++ ("aaaabbb"));
constraint
  (("baaaaaa") ++ (G) ++ (B) ++ ("aa") ++ (G)) =
    ((F) ++ ("aaaaa") ++ (F) ++ ("a") ++ (F) ++ ("ab"));
constraint
  (("aaaa") ++ (C) ++ ("aba") ++ (F) ++ ("ba") ++ (G)) =
    (("aaaab") ++ (F) ++ (G) ++ ("a") ++ (G) ++ ("a") ++ (G) ++ ("ab"));
constraint
  ((F) ++ ("aaaaaaabaaaa") ++ ("")) = ((F) ++ ("aaaaaaa") ++ (G) ++ ("aaaa"));
constraint
  ((C) ++ ("aaaa") ++ (G) ++ ("bbaaa") ++ (F)) =
    ((C) ++ ("aaaa") ++ (G) ++ (G) ++ (G) ++ ("aaa") ++ (F));
constraint
  (("aa") ++ (G) ++ ("ba") ++ (G) ++ ("b") ++ (C) ++ ("b") ++ (G) ++ ("b") ++ (F))
    = (("aab") ++ (F) ++ ("b") ++ (G) ++ (C) ++ ("bb") ++ (G) ++ ("ba"));
constraint
  ((G) ++ ("a") ++ (G) ++ ("ba") ++ (I) ++ ("a") ++ (F) ++ ("a")) =
    ((F) ++ (C) ++ ("a") ++ (C) ++ ("ba") ++ (F) ++ (F) ++ ("a"));
constraint
  ((I) ++ ("aaa") ++ (C) ++ ("ba") ++ (C)) =
    ((G) ++ ("b") ++ (F) ++ ("baaa") ++ (G) ++ ("b") ++ (F) ++ (G) ++ (G));
constraint
  ((G) ++ ("a") ++ (F) ++ (C) ++ (C) ++ ("aaaa") ++ (G) ++ (G)) =
    ((F) ++ (F) ++ (C) ++ (G) ++ (F) ++ ("aaa") ++ (G) ++ ("b"));
constraint
  ((D) ++ (G) ++ (B) ++ (G) ++ ("a") ++ (B)) =
    ((F) ++ (G) ++ ("aa") ++ (G) ++ (B) ++ (F) ++ ("aab"));
constraint
  (("aa") ++ (G) ++ (G) ++ (F) ++ ("b") ++ (G) ++ ("bab") ++ (F) ++ (G)) =
    ((B) ++ (G) ++ ("ba") ++ (C) ++ (F) ++ (G) ++ (F) ++ (G));
constraint
  (("b") ++ (B) ++ ("bb") ++ (I) ++ ("a") ++ (F)) =
    ((F) ++ ("a") ++ (C) ++ (G) ++ (G) ++ ("b") ++ (F) ++ (F) ++ (F));
constraint
  (("a") ++ (B) ++ ("aa") ++ (F) ++ ("aa") ++ (B) ++ ("a")) =
    (("a") ++ (B) ++ ("aa") ++ (G) ++ ("aaaaa") ++ (G) ++ ("a"));
constraint
  ((F) ++ ("aaaaab") ++ (B) ++ ("aba")) =
    ((F) ++ ("aaaaa") ++ (F) ++ ("a") ++ (F) ++ ("ba"));
constraint
  (("aa") ++ (F) ++ ("aab") ++ (B) ++ (G) ++ ("a") ++ (C)) =
    (("aabaaa") ++ (G) ++ ("aa") ++ (C) ++ ("a") ++ (C));
constraint
  ((B) ++ ("a") ++ (F) ++ ("aa") ++ (G) ++ (B) ++ (F)) =
    ((B) ++ ("a") ++ (F) ++ ("aa") ++ (F) ++ ("ab") ++ (F));
constraint
  ((I) ++ ("b") ++ (F) ++ ("b") ++ (F) ++ (F) ++ ("a")) =
    ((C) ++ (F) ++ (G) ++ ("b") ++ (F) ++ (C) ++ ("a") ++ (F) ++ ("a"));
constraint
  ((G) ++ (G) ++ ("b") ++ (B) ++ ("aa") ++ (C) ++ ("a") ++ (C) ++ ("b")) =
    ((C) ++ ("baa") ++ (G) ++ ("aa") ++ (G) ++ ("bab") ++ (G) ++ ("b"));
constraint
  (("a") ++ (G) ++ ("a") ++ (B) ++ (G) ++ (H) ++ ("a") ++ (G)) =
    (("a") ++ (F) ++ ("aabba") ++ (C) ++ ("aaa") ++ (G));
constraint
  (("a") ++ (G) ++ (B) ++ ("a") ++ (C) ++ (B) ++ (G) ++ ("aa")) =
    (("a") ++ (F) ++ ("a") ++ (F) ++ (C) ++ ("aa") ++ (G) ++ ("baa"));
constraint
  ((G) ++ ("aa") ++ (C) ++ ("abb") ++ (F) ++ ("abab")) =
    ((F) ++ ("a") ++ (G) ++ (F) ++ ("b") ++ (G) ++ (G) ++ ("aa") ++ (F) ++ (G));
constraint
  (("a") ++ (F) ++ (D) ++ (B) ++ ("aab")) = (("abababaaaa") ++ (F) ++ ("ab"));
constraint
  (("b") ++ (B) ++ ("aaaaa") ++ (F) ++ ("aaa")) =
    ((G) ++ ("aa") ++ (F) ++ ("aaaa") ++ (F) ++ ("aaa"));
constraint
  (("abaa") ++ (F) ++ ("aaa") ++ (C) ++ (G) ++ ("bb")) =
    (("a") ++ (G) ++ ("aa") ++ (G) ++ ("aa") ++ (B) ++ (G) ++ (C) ++ (G));
constraint
  (("a") ++ (C) ++ ("aaa") ++ (G) ++ ("b") ++ (H) ++ ("a")) =
    (("a") ++ (G) ++ (F) ++ ("aab") ++ (F) ++ (C) ++ ("aaa"));
constraint
  ((F) ++ (C) ++ ("baaa") ++ (F) ++ (B) ++ ("a")) =
    ((G) ++ ("ab") ++ (G) ++ (G) ++ ("aaa") ++ (F) ++ ("aa") ++ (G) ++ ("a"));
solve satisfy;
