var string: selKeyword_value;
var string: selKeyword_value_trimed;
var string: p1;
var string: p2;
var string: p3;
var string: url;
var string: ret;
var string: houseCityKeyName;
constraint (selKeyword_value) = ((p1) ++ (p2) ++ (p3));
constraint str_reg(p1, "(( )|(\t))*");
constraint str_reg(p3, "(( )|(\t))*");
constraint not (str_suffixof(" ", p2));
constraint not (str_suffixof("\t", p2));
constraint not (str_prefixof("\t", p2));
constraint not (str_prefixof(" ", p2));
constraint (selKeyword_value_trimed) = (p2);
constraint
  (url) =
    (("http://xf.house.somesite.com/") ++ (houseCityKeyName) ++ ("/search/0-0-0-0-0-0-0-0-0-1-1.html"));
constraint
  [(ret) = (url),(ret) =
                   ((url) ++ ("?keyword=") ++ (selKeyword_value_trimed))][1 + (not (
  ("") = (selKeyword_value_trimed)))];
solve satisfy;
