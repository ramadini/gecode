var string: cookie;
var string: cookie_part1;
var string: cookie_part2;
var string: cookie_part3;
var string: lang;
var int: idx1;
var int: len1;
var string: l1;
constraint (cookie) = ((cookie_part1) ++ (cookie_part2) ++ (cookie_part3));
constraint
  str_reg(cookie_part2,
    "((?)|(;))(searchLang=)(((a)|(b)|(c)|(d)|(e)|(f)|(g)|(h)|(i)|(j)|(k)|(l)|(m)|(n))*)");
constraint
  (not (("") = (cookie_part3))) -> ((cookie_part3) = ((";") ++ (l1)));
constraint (str_len (cookie_part2)) > (11);
constraint
  (cookie) =
    ("expires=Thu, 18 Dec 2013 12:00:00 UTC;searchLang=nb;domain=local;");
constraint
  (idx1) = (((str_find_offset("=", cookie_part2, (0)+(1)))-(1)) + (1));
constraint (len1) = (str_len (cookie_part2));
constraint (idx1) > (0);
constraint (len1) > (idx1);
constraint
  (lang) =
    (str_sub(cookie_part2, (idx1)+(1), ((idx1)+(1))+((len1) - (idx1))));
solve satisfy;
