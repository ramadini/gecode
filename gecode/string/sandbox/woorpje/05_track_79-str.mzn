var string: H;
var string: I;
var string: B;
var string: J;
constraint
  (("cfbfbbdaaabcabebe") ++ (J) ++ ("eccacacfeaccfbbdeaaf")) =
    (("cfbf") ++ (H) ++ (H) ++ ("daaabcabeb") ++ (I) ++ ("accfb") ++ (H) ++ ("deaaf"));
constraint
  (("cafacccecae") ++ (H) ++ ("dabdbdafbdadbecbeebdafdcecc") ++ (H)) =
    (("cafacccecae") ++ (H) ++ ("da") ++ (H) ++ ("dbdaf") ++ (H) ++ ("dad") ++ (H) ++ ("ecbeebdafdcecc") ++ (H));
constraint
  ((B) ++ ("eafd") ++ (H) ++ ("ebcfdafcebbfddaaefadfbb") ++ (H) ++ ("ffceecba"))
    =
    (("faeeafdbebcfdafce") ++ (H) ++ (H) ++ ("fddaaefadf") ++ (H) ++ (H) ++ (H) ++ ("ffceec") ++ (H) ++ ("a"));
constraint ((str_len (I)) * (16)) >= (144);
constraint ((str_len (I)) * (18)) <= (1782);
constraint ((str_len (J)) * (14)) >= (28);
constraint ((str_len (H)) * (15)) <= (60);
solve satisfy;
