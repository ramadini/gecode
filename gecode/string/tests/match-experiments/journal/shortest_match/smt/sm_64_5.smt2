(declare-fun L () Int)
(declare-fun W () String)
(assert (= (str.len W) L))
(assert (<= L 64))
(assert (str.in_re W (re.++ re.all (str.to_re "xL")  (re.* (re.union (str.to_re "Gz7Dl") (str.to_re "2dybR") (str.to_re "G3F1a") (str.to_re "5eQq4*"))) (str.to_re "t5")  re.all)))
(assert (str.in_re W (re.++ re.all (re.union (str.to_re "8G") (str.to_re "wm") (str.to_re "c8") (str.to_re "Op") (str.to_re "bz")) (str.to_re "z6G")  re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "tuiHrjn")  (re.union (str.to_re "QYC6WSZ") (str.to_re "wrXmxOb")) (str.to_re "L7")  re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "cYC6")  (re.union (str.to_re "WSZL") (str.to_re "ZuqZ") (str.to_re "KU9T") (str.to_re "CPsP") (str.to_re "4yKk")) (str.to_re "7npfe")  (re.union (str.to_re "29p1v") (str.to_re "dylE5") (str.to_re "RA2q8") (str.to_re "CZ0S2") (str.to_re "SPMgm") (str.to_re "Ur1fX")) (str.to_re "J8u")  (re.* (re.union (str.to_re "CMS") (str.to_re "FcK") (str.to_re "xOh*"))) (str.to_re "7J8Gz")  (re.* (re.union (str.to_re "6") (str.to_re "k") (str.to_re "p") (str.to_re "f") (str.to_re "h") (str.to_re "6*"))) re.all)))
(assert (str.in_re W (re.++ re.all (re.union (str.to_re "r") (str.to_re "4") (str.to_re "6") (str.to_re "N") (str.to_re "o")) (str.to_re "jnQ")  re.all)))
(check-sat)