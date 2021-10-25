var string: B;
var string: E;
var string: J;
constraint
  (("fffded") ++ (E) ++ ("ba")) =
    (("fffdedbaed") ++ (J) ++ (J) ++ ("faaadcfffbbdcbacffeaacbfba"));
constraint
  (("facecaffdedadffccbdfbfcedeebbfbbb") ++ (J) ++ ("cebfd")) =
    (("facecaffdedadffccbdfbfcedeebbfbbbaecebfd") ++ (""));
constraint
  (("fcaffceecfbcaba") ++ (J) ++ ("caabafeeeacafaafffefaaf")) =
    (("fcaffceecfbcaba") ++ (J) ++ ("caabafeeeacafaafff") ++ (B) ++ ("af"));
constraint ((str_len (J)) * (1)) <= (5);
constraint ((str_len (E)) * (13)) >= (364);
constraint ((str_len (E)) * (19)) <= (608);
constraint ((str_len (B)) * (3)) >= (9);
constraint ((str_len (B)) * (2)) <= (12);
solve satisfy;
