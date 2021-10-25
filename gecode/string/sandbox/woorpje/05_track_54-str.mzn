var string: H;
var string: D;
var string: F;
constraint
  (("bfbbedbcbedebfb") ++ (H) ++ ("bffcabbfebfedafffbdaa")) =
    (("bfb") ++ (D) ++ ("bbfebfedafffbdaa"));
constraint
  (("abbdeccecbf") ++ (F) ++ ("ddcbedbacfbfbbeccedbdfbdbc")) =
    (("abbdeccecbf") ++ (F) ++ ("ddcbedbacfbfbbeccedbdfbdbc"));
constraint
  (("abebfdcadedfdbdab") ++ (F) ++ ("eafbdeeadecefffcdffc")) =
    (("abebfdcadedfdbdabeebeafbdeeadecefffcdffc") ++ (""));
constraint ((str_len (D)) * (15)) <= (615);
constraint ((str_len (F)) * (18)) >= (54);
solve satisfy;
