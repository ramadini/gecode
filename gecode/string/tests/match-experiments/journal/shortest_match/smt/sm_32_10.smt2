(declare-fun L () Int)
(declare-fun W () String)
(assert (= (str.len W) L))
(assert (<= L 32))
(assert (str.in_re W (re.++ re.all (re.* (re.union (str.to_re "G") (str.to_re "k") (str.to_re "D*"))) re.all)))
(assert (str.in_re W (re.++ re.all (re.* (re.union (str.to_re "G") (str.to_re "m") (str.to_re "j") (str.to_re "r*"))) re.all)))
(assert (str.in_re W (re.++ re.all (re.* (re.union (str.to_re "n") (str.to_re "2") (str.to_re "H") (str.to_re "q") (str.to_re "4*"))) re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "KG")  (re.* (re.union (str.to_re "n") (str.to_re "X") (str.to_re "O*"))) re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "G")  (re.* (re.union (str.to_re "zC") (str.to_re "si") (str.to_re "Fn") (str.to_re "pm*"))) (str.to_re "Ojc")  re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "v")  (re.* (re.union (str.to_re "G") (str.to_re "m") (str.to_re "h") (str.to_re "R*"))) re.all)))
(assert (str.in_re W (re.++ re.all (re.union (str.to_re "ggZn") (str.to_re "brxj") (str.to_re "2he6") (str.to_re "t8Ut") (str.to_re "cAJZ") (str.to_re "jjaa")) re.all)))
(assert (str.in_re W (re.++ re.all (re.* (re.union (str.to_re "n") (str.to_re "e") (str.to_re "8") (str.to_re "m*"))) (str.to_re "XGO")  re.all)))
(assert (str.in_re W (re.++ re.all (re.union (str.to_re "c") (str.to_re "d") (str.to_re "m")) (str.to_re "n")  re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "nmy")  (re.union (str.to_re "c") (str.to_re "j") (str.to_re "5") (str.to_re "P")) (str.to_re "nnXG")  re.all)))
(check-sat)
