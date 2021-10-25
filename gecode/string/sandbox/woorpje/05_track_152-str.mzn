var string: H;
var string: B;
var string: D;
constraint
  (("ee") ++ (D) ++ ("de")) = (("ee") ++ (B) ++ ("bdbaefaabdcbeebde"));
constraint
  (("adbddafebafaaaedefcedccddadadffafdfaaaef") ++ ("")) =
    (("adbddafebafaaaedef") ++ (H) ++ ("ddadadffafdfaaaef"));
constraint
  (("bcbaaeeeaaeadbbeacddfbcedccbdefbcbadcdef") ++ ("")) =
    (("bcbaaeeeaaeadbbeacddfb") ++ (H) ++ ("bdefbcbadcdef"));
constraint ((str_len (B)) * (16)) <= (1728);
constraint ((str_len (H)) * (14)) >= (42);
constraint ((str_len (H)) * (5)) <= (50);
constraint ((str_len (D)) * (19)) <= (5605);
solve satisfy;
