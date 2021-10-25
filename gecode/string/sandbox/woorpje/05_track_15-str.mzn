var string: B;
var string: A;
var string: C;
constraint
  (("aecabdd") ++ (B) ++ ("cfbcb")) =
    (("aecabddedbefcc") ++ (A) ++ ("aafbccbeecfbcb"));
constraint
  (("edcddddbbebeaaffaceaaeeecfbfceacacccdefb") ++ ("")) =
    (("edcddddbbebeaaffaceaaeeecfbfceaca") ++ (C) ++ ("defb"));
constraint
  (("ffeedadccbfdbe") ++ (C) ++ ("bebaaccfbfefcebeafafcee")) =
    (("ffeedadccbfdbe") ++ (C) ++ ("bebaaccfbfefcebeafafcee"));
constraint ((str_len (A)) * (14)) >= (14);
constraint ((str_len (C)) * (6)) <= (168);
constraint ((str_len (B)) * (10)) >= (250);
solve satisfy;
