var string: N;
var string: O;
var string: E;
constraint
  ((E) ++ ("bbbdbcdddbbdbdabaccdddbcbccdbd") ++ ("")) =
    (("cabbcac") ++ (N) ++ ("cdaddbbacb") ++ (O) ++ ("dddbcbccdbd"));
solve satisfy;
