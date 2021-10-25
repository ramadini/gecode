var string: B;
var string: F;
var string: A;
var string: C;
constraint
  (("adddf") ++ (F) ++ (B) ++ ("ec") ++ (F) ++ (C) ++ (A) ++ (F) ++ ("a") ++ (F) ++ ("aaeba"))
    =
    (("adddfb") ++ (F) ++ ("ececb") ++ (C) ++ (A) ++ (F) ++ ("abaae") ++ (F) ++ ("a"));
constraint
  (("d") ++ (F) ++ ("dcafccdea") ++ (F) ++ ("aefdd") ++ (F) ++ ("c") ++ (F) ++ ("cd") ++ (F) ++ ("cfadcaedfaaecefce"))
    =
    (("dbdcafccdeabaefdd") ++ (F) ++ ("c") ++ (F) ++ ("cdbcfadcaedfaaecefce"));
constraint
  (("fcbdeecbcd") ++ (F) ++ ("f") ++ (F) ++ ("ddcacefb") ++ (F) ++ ("afacfbacab") ++ (F) ++ ("aaeaeee"))
    =
    (("fcbdeecbcd") ++ (F) ++ ("f") ++ (F) ++ ("ddcacefbbafacfbacab") ++ (F) ++ ("aaeaeee"));
constraint ((str_len (F)) * (12)) <= (60);
constraint ((str_len (C)) * (10)) >= (70);
constraint ((str_len (B)) * (3)) >= (6);
solve satisfy;
