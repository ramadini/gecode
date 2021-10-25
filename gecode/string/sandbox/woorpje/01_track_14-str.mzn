var string: B;
var string: A;
var string: C;
constraint ((A) ++ (B) ++ ("aaaa")) = ((C) ++ (B));
solve satisfy;
