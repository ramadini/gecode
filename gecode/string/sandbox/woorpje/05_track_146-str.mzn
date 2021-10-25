var string: G;
var string: C;
var string: D;
constraint
  (("eddda") ++ (D) ++ ("bcbaa")) =
    (("edddaddfbabeafd") ++ (C) ++ ("fadeacdcaeaccbabcbaa"));
constraint
  (("fdedacfdaddabddfebacdcccfa") ++ (G) ++ ("dbcdeeecce")) =
    (("fdedacfdaddabddfebacdcccfa") ++ (G) ++ ("dbcdeeecce"));
constraint
  (("adaadecdcceaceeecebfcbcdffefbabccf") ++ (G) ++ ("cc")) =
    (("adaadecdcceaceeecebfcbcdffefbabccfafddcc") ++ (""));
constraint ((str_len (C)) * (12)) <= (576);
solve satisfy;
