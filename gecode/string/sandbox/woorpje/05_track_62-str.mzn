var string: B;
var string: A;
var string: C;
constraint
  (("ccbe") ++ (B) ++ ("beafbbf") ++ (A) ++ ("beadcccfdfe")) =
    (("ccbefacbeafbbf") ++ (C) ++ ("fdfe"));
constraint
  (("cccffefabcabbedcefabadbaacfbfbfacbccebfd") ++ ("")) =
    (("cccffefabcabbedcefabadbaacfbfb") ++ (B) ++ ("bccebfd"));
constraint
  (("ccfdaecaffacacdcdcdbfbaebedbffacbacfdfbf") ++ ("")) =
    (("ccfdaecaf") ++ (B) ++ ("acdcdcdbfbaebedbffacbacfdfbf"));
constraint ((str_len (A)) * (1)) >= (10);
constraint ((str_len (C)) * (16)) >= (144);
constraint ((str_len (B)) * (13)) >= (39);
solve satisfy;
