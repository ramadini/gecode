var string: F;
var string: E;
constraint
  (("d") ++ (F) ++ ("bddddaaaaabacaddbdbaca") ++ (F) ++ ("c") ++ (E) ++ ("cad") ++ (F))
    =
    (("db") ++ (F) ++ ("ddddaaaaabacadd") ++ (F) ++ ("d") ++ (F) ++ ("acabc") ++ (F) ++ ("aba") ++ (F) ++ ("caaddbda") ++ (F) ++ ("acada") ++ (F) ++ ("babaaabdabcdacad") ++ (F));
solve satisfy;
