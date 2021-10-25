var string: A;
constraint (("ebdddce") ++ ("")) = (("ebd") ++ (A) ++ ("e"));
constraint (("dec") ++ (A) ++ ("e")) = (("decddce") ++ (""));
constraint (("c") ++ (A) ++ ("cde")) = (("c") ++ (A) ++ ("cde"));
constraint (("dcccddc") ++ ("")) = (("dccc") ++ (A));
constraint (("dcca") ++ (A)) = (("dcca") ++ (A));
constraint ((A) ++ ("dbdb") ++ ("")) = (("ddcdbdb") ++ (""));
constraint (("ecb") ++ (A) ++ ("a")) = (("ecb") ++ (A) ++ ("a"));
solve satisfy;
