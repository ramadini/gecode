var string: H;
var string: C;
var string: D;
var string: E;
constraint
  (("dcffh") ++ (H) ++ ("cfeb") ++ (E) ++ ("i")) =
    (("dcf") ++ (C) ++ (D) ++ ("i"));
solve satisfy;
