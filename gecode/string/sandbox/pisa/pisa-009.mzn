%include "replace.mzn";
var string: s;
var string: ret0;
var string: ret1;
constraint (ret0) = (str_replace(str_replace(s, "<", "&lt;"), ">", "&gt;"));
constraint (ret1) = ((ret0) ++ ("<br/>"));
constraint (str_contains(ret1, "<")) \/ (str_contains(ret1, ">"));
solve satisfy;
