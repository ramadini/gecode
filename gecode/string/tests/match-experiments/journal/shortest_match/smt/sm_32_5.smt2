(declare-fun L () Int)
(declare-fun W () String)
(assert (= (str.len W) L))
(assert (<= L 32))
(assert (str.in_re W (re.++ re.all (re.* (re.union (str.to_re "e") (str.to_re "a") (str.to_re "x") (str.to_re "S") (str.to_re "g") (str.to_re "p*"))) (str.to_re "h")  re.all)))
(assert (str.in_re W (re.++ re.all (re.union (str.to_re "sq") (str.to_re "Qz") (str.to_re "xA") (str.to_re "lS") (str.to_re "Mf") (str.to_re "0Y")) re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "zXZ")  (re.* (re.union (str.to_re "Ok21je") (str.to_re "4V0QLc") (str.to_re "ysBJNC*"))) re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "hA")  (re.* (re.union (str.to_re "n67") (str.to_re "CBO*"))) re.all)))
(assert (str.in_re W (re.++ re.all (str.to_re "sqAYITf")  (re.* (re.union (str.to_re "q") (str.to_re "A") (str.to_re "Y") (str.to_re "p") (str.to_re "a") (str.to_re "A*"))) (str.to_re "V573yC")  re.all)))
(check-sat)
