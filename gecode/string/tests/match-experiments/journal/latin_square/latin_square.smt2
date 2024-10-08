(declare-fun row0 () String)
(declare-fun row1 () String)
(declare-fun col0 () String)
(declare-fun col1 () String)
(declare-fun r_00 () Int)
(declare-fun r_01 () Int)
(declare-fun r_10 () Int)
(declare-fun r_11 () Int)
(declare-fun c_00 () Int)
(declare-fun c_01 () Int)
(declare-fun c_10 () Int)
(declare-fun c_11 () Int)

(assert (str.in_re row0 (re.* (re.union (str.to_re "A") (str.to_re "B")))))
(assert (str.in_re row1 (re.* (re.union (str.to_re "A") (str.to_re "B")))))
(assert (str.in_re col0 (re.* (re.union (str.to_re "A") (str.to_re "B")))))
(assert (str.in_re col1 (re.* (re.union (str.to_re "A") (str.to_re "B")))))

(assert (= (str.len row0) 2))
(assert (= (str.len row1) 2))
(assert (= (str.len col0) 2))
(assert (= (str.len col1) 2))

(assert (>= r_00 0))
(assert (>= r_01 0))
(assert (>= r_10 0))
(assert (>= r_11 0))

(assert (>= c_00 0))
(assert (>= c_01 0))
(assert (>= c_10 0))
(assert (>= c_11 0))

(assert (= r_00 (str.indexof row0 "A" 0)))
(assert (= r_10 (str.indexof row1 "A" 0)))
(assert (= r_01 (str.indexof row0 "B" 0)))
(assert (= r_11 (str.indexof row1 "B" 0)))

(assert (= c_00 (str.indexof col0 "A" 0)))
(assert (= c_10 (str.indexof col1 "A" 0)))
(assert (= c_01 (str.indexof col0 "B" 0)))
(assert (= c_11 (str.indexof col1 "B" 0)))

(assert (xor (= r_00 0) (= c_00 0)))
(assert (xor (= r_00 1) (= c_10 0)))
(assert (xor (= r_01 0) (= c_01 0)))
(assert (xor (= r_01 1) (= c_11 0)))
(assert (xor (= r_10 0) (= c_00 1)))
(assert (xor (= r_10 1) (= c_10 1)))
(assert (xor (= r_11 0) (= c_01 1)))
(assert (xor (= r_11 1) (= c_11 1)))

(check-sat)
(get-model)

