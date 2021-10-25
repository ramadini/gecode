var string: H;
var string: B;
var string: G;
var string: A;
var string: D;
constraint
  (("db") ++ (A) ++ ("eefddcf") ++ (H) ++ ("afebacbdacacaefededd")) =
    (("dba") ++ (G) ++ ("ddcf") ++ (H) ++ (B) ++ ("cacaefededd"));
constraint
  (("ddfffcaaefeedbefdb") ++ (D) ++ ("fdeebabdcdebaacfcb")) =
    (("ddfffcaaefeedbefdb") ++ (D) ++ ("fdeebabdcdebaacfcb"));
constraint
  (("fefcbcbaadccfeffbc") ++ (D) ++ ("cdcfacdbeddcddefbc")) =
    (("fefcbcbaadccfeffbccfcdcdcfacdbeddcddefbc") ++ (""));
constraint ((str_len (G)) * (10)) >= (20);
constraint ((str_len (H)) * (4)) >= (8);
constraint ((str_len (B)) * (17)) >= (17);
solve satisfy;
