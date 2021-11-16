var string: s;
var string: filename_0;
var string: filename_1;
var string: filename_2;
var int: i1;
var int: i2;
var int: i3;
var string: tmpStr0;
var string: tmpStr1;
var string: tmpStr2;
constraint (filename_0) = (s);
constraint
  [(i1) = ((0) - (1)),((filename_0) = ((tmpStr0) ++ (("/") ++ (tmpStr1)))) /\ (not (str_contains(tmpStr1, "/"))) /\ (
  (i1) = (str_len (tmpStr0)))][1 + (str_contains(filename_0, "/"))];
constraint
  [(filename_1) = (filename_0),((i2) = ((str_len (filename_0)) - (i1))) /\ (
  (filename_1) = (str_sub(filename_0, (i1)+(1), ((i1)+(1))+(i2))))][1 + (not (
  (i1) = ((0) - (1))))];
constraint (i3) = ((str_find_offset(".", filename_1, (0)+(1)))-(1));
constraint
  [(filename_2) = (filename_1),(filename_2) =
                                 (str_sub(filename_1, (0)+(1), ((0)+(1))+(i3)))][1 + (not (
  (i3) = ((0) - (1))))];
solve ::int_search([str_contains(filename_0, "/")], input_order, indomain_min, complete) satisfy;
