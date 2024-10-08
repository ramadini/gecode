(declare-fun L () Int)
(declare-fun W () String)
(assert (= (str.len W) L))
(assert (<= L 32))
(assert (str.in_re W (re.++ re.all (re.union (str.to_re "b") (str.to_re "X") (str.to_re "G") (str.to_re "w")) re.all)))
(assert (str.in_re W (re.++ re.all (re.* (re.union (str.to_re "b") (str.to_re "u") (str.to_re "J") (str.to_re "k") (str.to_re "Q*"))) re.all)))
(assert (str.in_re W (re.++ re.all (re.* (re.union (str.to_re "L") (str.to_re "B") (str.to_re "B") (str.to_re "f*"))) re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "l")  (re.union (str.to_re "L") (str.to_re "0") (str.to_re "D") (str.to_re "2") (str.to_re "L") (str.to_re "p")) re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "L")  (re.* (re.union (str.to_re "S") (str.to_re "G") (str.to_re "N") (str.to_re "K") (str.to_re "x") (str.to_re "o*"))) re.all)))
(assert (str.in_re W (re.++ re.all (re.union (str.to_re "l") (str.to_re "a") (str.to_re "A") (str.to_re "D") (str.to_re "8") (str.to_re "c")) (str.to_re "L")  re.all)))
(assert (str.in_re W (re.++ re.all (re.union (str.to_re "b") (str.to_re "M") (str.to_re "n") (str.to_re "r") (str.to_re "W") (str.to_re "d")) re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "bl")  (re.* (re.union (str.to_re "L") (str.to_re "F*"))) re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "S")  (re.* (re.union (str.to_re "Z9") (str.to_re "Q8") (str.to_re "C7") (str.to_re "cw*"))) (str.to_re "0S")  re.all)))
(assert (str.in_re W (re.++ re.all (re.* (re.union (str.to_re "S") (str.to_re "V") (str.to_re "C*"))) re.all)))
(assert (str.in_re W (re.++ re.all (re.* (re.union (str.to_re "0") (str.to_re "c") (str.to_re "u") (str.to_re "h") (str.to_re "r") (str.to_re "A*"))) re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "90SQC")  (re.union (str.to_re "jo") (str.to_re "pP") (str.to_re "Zt") (str.to_re "rC") (str.to_re "83") (str.to_re "bO")) (str.to_re "O")  re.all)))
(assert (str.in_re W (re.++ re.all (re.* (re.union (str.to_re "O") (str.to_re "C*"))) re.all)))
(assert (str.in_re W (re.++ re.all (re.union (str.to_re "S") (str.to_re "p") (str.to_re "n") (str.to_re "s") (str.to_re "x") (str.to_re "l")) re.all)))
(assert (str.in_re W (re.++ re.all (re.* (re.union (str.to_re "0") (str.to_re "t") (str.to_re "p") (str.to_re "T") (str.to_re "W*"))) (str.to_re "S")  re.all)))
(check-sat)
