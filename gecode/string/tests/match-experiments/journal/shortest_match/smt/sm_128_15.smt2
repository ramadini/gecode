(declare-fun L () Int)
(declare-fun W () String)
(assert (= (str.len W) L))
(assert (<= L 128))
(assert (str.in_re W (re.++ re.all (str.to_re "C")  (re.* (re.union (str.to_re "ae") (str.to_re "Ix*"))) (str.to_re "g")  re.all)))
(assert (str.in_re W (re.++ re.all (re.* (re.union (str.to_re "9") (str.to_re "E") (str.to_re "A") (str.to_re "F*"))) (str.to_re "BE3fjK")  (re.union (str.to_re "DJ7XC") (str.to_re "RMZFa") (str.to_re "rkq3Y") (str.to_re "X9VrC") (str.to_re "vSBsO") (str.to_re "TQGX6")) (str.to_re "Wv3d")  (re.union (str.to_re "akdO") (str.to_re "5g5g")) (str.to_re "N")  (re.* (re.union (str.to_re "Ix") (str.to_re "Sy") (str.to_re "H5") (str.to_re "q1*"))) (str.to_re "hPl")  re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "g")  (re.* (re.union (str.to_re "9") (str.to_re "9") (str.to_re "p") (str.to_re "T") (str.to_re "d") (str.to_re "2*"))) (str.to_re "C")  re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "zI")  (re.* (re.union (str.to_re "EY") (str.to_re "bp") (str.to_re "31") (str.to_re "Xc") (str.to_re "lu*"))) (str.to_re "blVl")  re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "ll")  (re.union (str.to_re "Vlik") (str.to_re "eaao") (str.to_re "gsmO")) (str.to_re "C")  re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "gzKWIIkd")  (re.* (re.union (str.to_re "ON") (str.to_re "2t") (str.to_re "Iw") (str.to_re "pQ") (str.to_re "ed") (str.to_re "J1*"))) (str.to_re "Ixh")  re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "Ig")  (re.* (re.union (str.to_re "z") (str.to_re "d*"))) re.all)))
(assert (str.in_re W (re.++ re.all (re.* (re.union (str.to_re "kC") (str.to_re "uO") (str.to_re "7l") (str.to_re "Pw*"))) re.all)))
(assert (str.in_re W (re.++ re.all (re.* (re.union (str.to_re "kC") (str.to_re "je") (str.to_re "t3") (str.to_re "CQ*"))) (str.to_re "KWII")  re.all)))
(assert (str.in_re W (re.++ re.all (re.* (re.union (str.to_re "Cz") (str.to_re "uZ") (str.to_re "eF") (str.to_re "9L*"))) re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "Vl")  (re.* (re.union (str.to_re "f") (str.to_re "H") (str.to_re "B") (str.to_re "Q") (str.to_re "U*"))) (str.to_re "6KllVli")  (re.union (str.to_re "kCae") (str.to_re "0x6g") (str.to_re "Gf85") (str.to_re "VpZn")) (str.to_re "NIIhCu")  re.all)))
(assert (str.in_re W (re.++ re.all (re.* (re.union (str.to_re "Cz") (str.to_re "IQ") (str.to_re "wa") (str.to_re "Qa") (str.to_re "RG") (str.to_re "S4*"))) re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "k")  (re.union (str.to_re "C") (str.to_re "4") (str.to_re "D") (str.to_re "V")) re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "CEY")  (re.* (re.union (str.to_re "blVlf6") (str.to_re "Z7cbq4") (str.to_re "ivWtqn") (str.to_re "hWA25O") (str.to_re "TyZr7K*"))) (str.to_re "KllVlikCae")  re.all)))
(assert (str.in_re W (re.++ re.all (re.union (str.to_re "KllVli") (str.to_re "cpQPjN") (str.to_re "buMRLg") (str.to_re "2gP8po")) (str.to_re "kCaeW")  re.all)))
(check-sat)
