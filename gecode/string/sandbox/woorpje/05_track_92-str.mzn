var string: I;
var string: F;
var string: H;
constraint
  (("dafbedafebd") ++ (I) ++ ("abecdbdbdeaceef")) =
    (("dafbedafe") ++ (H) ++ ("ae") ++ (F) ++ ("fbdbecabecdbdbdeaceef"));
constraint
  (("dbfabafbfeceabdeddfffcbfebcaeddddfabaaaf") ++ ("")) =
    (("dbfabafbfecea") ++ (F) ++ ("dfffcbfebcaeddddfabaaaf"));
constraint
  (("ecdfabdedfcccebdaeeafaedeebfbdccbbfebeff") ++ ("")) =
    (("ecdfa") ++ (F) ++ ("fcccebdaeeafaedeebfbdccbbfebeff"));
constraint ((str_len (H)) * (7)) >= (21);
constraint ((str_len (H)) * (10)) <= (60);
constraint ((str_len (F)) * (3)) >= (9);
constraint ((str_len (F)) * (20)) <= (420);
constraint ((str_len (I)) * (20)) <= (1460);
solve satisfy;
