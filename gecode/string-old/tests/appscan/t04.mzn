var string: cookie;
var string: cookie_part1;
var string: cookie_part2;
var string: cookie_part3;
var string: t1;
var string: l1;
constraint
  (cookie) =
    (("expires=Thu, 18 Dec 2013 12:00:00 UTC;searchLang=") ++ (t1) ++ ("domain=www.somesite.com"));
constraint (cookie) = ((cookie_part1) ++ (cookie_part2) ++ (cookie_part3));
constraint
  str_reg(cookie_part2,
    "((?)|(;))(searchLang=)(((a)|(b)|(c)|(d)|(e)|(f)|(g)|(h)|(i)|(j)|(k)|(l)|(m)|(n))*)");
constraint
  (not (("") = (cookie_part3))) -> ((cookie_part3) = ((";") ++ (l1)));
constraint (str_len (cookie_part2)) > (12);
constraint not (("") = (cookie_part1));
solve satisfy;
