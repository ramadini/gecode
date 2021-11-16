var string: v1;
var string: v2;
var int: v3;
var string: ret;
constraint (v2) = ("<");
constraint
  [(ret) = (v1),((v3) = ((str_find_offset(v1, v2, (0)+(1)))-(1))) /\ (
  (ret) = (str_sub(v1, (0)+(1), ((0)+(1))+(v3))))][1 + (str_contains(v1, v2))];
constraint (str_contains(ret, "<")) \/ (str_contains(ret, ">"));
solve satisfy;
