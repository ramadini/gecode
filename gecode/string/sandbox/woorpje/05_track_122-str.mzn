var string: B;
var string: E;
var string: A;
constraint
  (("dfcecaffbe") ++ (B) ++ ("e") ++ (E) ++ ("fed") ++ (E) ++ ("ff") ++ (E) ++ ("bfc") ++ (E) ++ ("b"))
    =
    (("dfcecaffbe") ++ (B) ++ ("eafed") ++ (E) ++ ("ff") ++ (E) ++ ("bfcab"));
constraint
  (("faec") ++ (E) ++ ("bbcccdbabfcccbacebbbc") ++ (E) ++ ("ffedeffc") ++ (E) ++ ("cdfe"))
    =
    (("f") ++ (E) ++ ("ec") ++ (E) ++ ("bbcccdbabfcccb") ++ (E) ++ ("cebbbc") ++ (E) ++ (A) ++ ("deffcacdfe"));
constraint
  (("dcb") ++ (E) ++ ("cccdb") ++ (E) ++ (E) ++ ("dacfec") ++ (E) ++ ("ccdf") ++ (E) ++ ("fccfdbeeafeddf") ++ (E) ++ ("bf"))
    =
    (("dcbacccdbaad") ++ (E) ++ ("cfecaccdf") ++ (E) ++ ("fccfdbee") ++ (E) ++ ("feddf") ++ (E) ++ ("bf"));
constraint ((str_len (B)) * (11)) >= (77);
constraint ((str_len (A)) * (17)) >= (34);
constraint ((str_len (A)) * (1)) <= (6);
solve satisfy;
