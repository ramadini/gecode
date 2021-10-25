var string: H;
var string: B;
var string: E;
var string: A;
var string: J;
constraint
  (("dddeabc") ++ (E) ++ ("b") ++ (B) ++ (B) ++ (A) ++ (A) ++ ("efffc") ++ (B) ++ ("affe") ++ (B) ++ (B) ++ ("ebabea"))
    =
    (("dd") ++ (H) ++ ("c") ++ (J) ++ ("ed") ++ (B) ++ ("b") ++ (B) ++ (B) ++ (A) ++ (A) ++ ("efff") ++ (B) ++ (B) ++ ("affe") ++ (B) ++ (B) ++ ("ebabea"));
constraint
  (("bfaedbedcdbb") ++ (B) ++ ("bdefeebd") ++ (B) ++ ("edcebfbdebfe") ++ (B) ++ (B) ++ ("dabe"))
    =
    (("bfaedbed") ++ (B) ++ ("dbb") ++ (B) ++ ("bdefeebd") ++ (B) ++ ("ed") ++ (B) ++ ("ebfbdebfeccdabe"));
constraint
  (("ebe") ++ (B) ++ ("dafbdebaaeacefebbffdacaffefee") ++ (B) ++ ("ddedba"))
    =
    (("ebe") ++ (B) ++ ("dafbdebaaeacefebbffda") ++ (B) ++ ("affefee") ++ (B) ++ ("ddedba"));
constraint ((str_len (H)) * (8)) >= (32);
constraint ((str_len (H)) * (14)) <= (112);
constraint ((str_len (J)) * (15)) >= (60);
constraint ((str_len (B)) * (3)) <= (9);
constraint ((str_len (E)) * (18)) >= (72);
constraint ((str_len (E)) * (9)) <= (81);
solve satisfy;
