(declare-fun L () Int)
(declare-fun W () String)
(assert (= (str.len W) L))
(assert (<= L 64))
(assert (str.in_re W (re.++ re.all (str.to_re "QW")  (re.* (re.union (str.to_re "u") (str.to_re "5*"))) (str.to_re "n")  re.all)))
(assert (str.in_re W (re.++ re.all (re.* (re.union (str.to_re "nv") (str.to_re "gG") (str.to_re "x7") (str.to_re "us") (str.to_re "Mt*"))) re.all)))
(assert (str.in_re W (re.++ re.all (re.* (re.union (str.to_re "m") (str.to_re "e") (str.to_re "e") (str.to_re "b*"))) (str.to_re "1")  re.all)))
(assert (str.in_re W (re.++ re.all (re.union (str.to_re "vlv") (str.to_re "F8d") (str.to_re "fzz") (str.to_re "iVI")) (str.to_re "Vlv")  (re.* (re.union (str.to_re "fsk") (str.to_re "3GN") (str.to_re "ZQH") (str.to_re "6G9") (str.to_re "IXa") (str.to_re "DeS*"))) (str.to_re "1m1")  re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "Wu")  (re.union (str.to_re "n") (str.to_re "t") (str.to_re "4") (str.to_re "j")) (str.to_re "v")  re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "Vl")  (re.union (str.to_re "vf") (str.to_re "9A") (str.to_re "Pw") (str.to_re "go")) (str.to_re "sk1IJ")  re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "sk1")  (re.* (re.union (str.to_re "m1K1LlvXLNTZl") (str.to_re "kUvGOW8rkEqvD*"))) (str.to_re "9")  re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "I")  (re.union (str.to_re "J") (str.to_re "m") (str.to_re "v") (str.to_re "Q") (str.to_re "F") (str.to_re "a")) re.all)))
(assert (str.in_re W (re.++ re.all (re.union (str.to_re "IJU") (str.to_re "KGD") (str.to_re "OG4") (str.to_re "sIA")) (str.to_re "GH")  re.all)))
(assert (str.in_re W (re.++ re.all (re.* (re.union (str.to_re "JoHQW") (str.to_re "QHkGV") (str.to_re "IBfqT") (str.to_re "s9hNz") (str.to_re "BVgZs") (str.to_re "oW80w*"))) (str.to_re "un")  re.all)))
(check-sat)
