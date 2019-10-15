var string: sigmaStar_2;
var string: epsilon;
var string: x_3;
constraint (epsilon) = ("");
constraint
  (x_3) =
    (str_replace(epsilon, "\x68\x74\x74\x70\x3a\x2f\x2f",
       "\x68\x74\x74\x70\x73\x3a\x2f\x2f"));
constraint str_reg(x_3, "((.)*)(\x2f\x65\x76\x69\x6c)((.)*)");
solve satisfy;
