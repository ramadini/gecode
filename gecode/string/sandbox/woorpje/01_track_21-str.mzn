var string: H;
var string: F;
constraint
  (("abdcbddabcdda") ++ (H) ++ ("ccdcdaaaccddbcaacaacacccabbbbcbbc")) =
    (("abdcbddabcdd") ++ (F) ++ ("aacaacacccabbbbcbbc"));
solve satisfy;
