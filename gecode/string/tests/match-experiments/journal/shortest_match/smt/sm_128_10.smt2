(declare-fun L () Int)
(declare-fun W () String)
(assert (= (str.len W) L))
(assert (<= L 128))
(assert (str.in_re W (re.++ re.all (str.to_re "h")  (re.union (str.to_re "zTh") (str.to_re "6bz") (str.to_re "Aaz") (str.to_re "2yQ")) (str.to_re "9")  re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "I")  (re.* (re.union (str.to_re "Eh") (str.to_re "Zv") (str.to_re "rr") (str.to_re "cg") (str.to_re "4j*"))) re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "KhrpQ37knB")  (re.union (str.to_re "i") (str.to_re "K") (str.to_re "p") (str.to_re "P") (str.to_re "a")) (str.to_re "Icz")  re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "o")  (re.union (str.to_re "4trpQ37") (str.to_re "S5qtylS")) (str.to_re "9")  re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "kT")  (re.* (re.union (str.to_re "P1") (str.to_re "75") (str.to_re "dZ") (str.to_re "Fn") (str.to_re "g3*"))) (str.to_re "u6wM")  (re.* (re.union (str.to_re "k") (str.to_re "B*"))) (str.to_re "xjcz")  re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "aRK")  (re.* (re.union (str.to_re "D") (str.to_re "q") (str.to_re "f*"))) (str.to_re "xI")  (re.* (re.union (str.to_re "E") (str.to_re "K*"))) (str.to_re "zx")  (re.* (re.union (str.to_re "V") (str.to_re "x") (str.to_re "O") (str.to_re "e*"))) (str.to_re "C0C")  (re.union (str.to_re "mknBiIczPo4t") (str.to_re "MDYch6gatJVz") (str.to_re "NHgAA2wBTEeY") (str.to_re "gpSeFgvQkemp") (str.to_re "ns0X7cMgIihy")) (str.to_re "r")  (re.union (str.to_re "pQ37") (str.to_re "Mmq1")) re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "zcz6w")  (re.union (str.to_re "Mkx") (str.to_re "OPq") (str.to_re "an2") (str.to_re "blb") (str.to_re "Nau") (str.to_re "G8p")) (str.to_re "j")  re.all)))
(assert (str.in_re W (re.++ re.all (re.union (str.to_re "Dx") (str.to_re "tA") (str.to_re "Pf")) (str.to_re "IE")  re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "cz")  (re.union (str.to_re "BiIcz") (str.to_re "z9LRJ") (str.to_re "oHty5") (str.to_re "KdAmO") (str.to_re "6SdB7") (str.to_re "2xMn1")) (str.to_re "P")  (re.union (str.to_re "o") (str.to_re "d") (str.to_re "V") (str.to_re "R") (str.to_re "h") (str.to_re "C")) (str.to_re "4trpQ379N")  (re.* (re.union (str.to_re "5BC") (str.to_re "E3X*"))) (str.to_re "H")  (re.union (str.to_re "u") (str.to_re "Z")) (str.to_re "kTP1u6wMkxj")  re.all)))
(assert (str.in_re W (re.++ re.all (re.* (re.union (str.to_re "kxjczc") (str.to_re "KEk6Ha*"))) (str.to_re "z")  re.all)))
(check-sat)
