var string(1) of {"a"}: prefix0;
var string(1) of {"A", "a"}: prefix1;
var string(3) of {"A", "a", "b"}: text;

constraint text = prefix0 ++ prefix1 ++ "b";
constraint text >= "b";

solve satisfy;

output ["text = ", show(text), "\n"];
