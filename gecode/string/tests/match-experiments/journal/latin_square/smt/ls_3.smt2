(declare-fun row_0 () String)
(declare-fun row_1 () String)
(declare-fun row_2 () String)
(declare-fun col_0 () String)
(declare-fun col_1 () String)
(declare-fun col_2 () String)

(declare-fun r_0_0 () Int)
(declare-fun c_0_0 () Int)
(declare-fun r_0_1 () Int)
(declare-fun c_0_1 () Int)
(declare-fun r_0_2 () Int)
(declare-fun c_0_2 () Int)
(declare-fun r_1_0 () Int)
(declare-fun c_1_0 () Int)
(declare-fun r_1_1 () Int)
(declare-fun c_1_1 () Int)
(declare-fun r_1_2 () Int)
(declare-fun c_1_2 () Int)
(declare-fun r_2_0 () Int)
(declare-fun c_2_0 () Int)
(declare-fun r_2_1 () Int)
(declare-fun c_2_1 () Int)
(declare-fun r_2_2 () Int)
(declare-fun c_2_2 () Int)

(assert (str.in_re row_2 (re.* (re.range "A" "C"))))
(assert (str.in_re col_2 (re.* (re.range "A" "C"))))

(assert (= (str.len row_2) 3))
(assert (= (str.len col_2) 3))

(assert (>= r_0_0 0))
(assert (>= r_0_1 0))
(assert (>= r_0_2 0))
(assert (>= r_1_0 0))
(assert (>= r_1_1 0))
(assert (>= r_1_2 0))
(assert (>= r_2_0 0))
(assert (>= r_2_1 0))
(assert (>= r_2_2 0))

(assert (>= c_0_0 0))
(assert (>= c_0_1 0))
(assert (>= c_0_2 0))
(assert (>= c_1_0 0))
(assert (>= c_1_1 0))
(assert (>= c_1_2 0))
(assert (>= c_2_0 0))
(assert (>= c_2_1 0))
(assert (>= c_2_2 0))

(assert (= r_0_0 (str.indexof row_0 "A" 0)))
(assert (= r_0_1 (str.indexof row_0 "B" 0)))
(assert (= r_0_2 (str.indexof row_0 "C" 0)))
(assert (= r_1_0 (str.indexof row_1 "A" 0)))
(assert (= r_1_1 (str.indexof row_1 "B" 0)))
(assert (= r_1_2 (str.indexof row_1 "C" 0)))
(assert (= r_2_0 (str.indexof row_2 "A" 0)))
(assert (= r_2_1 (str.indexof row_2 "B" 0)))
(assert (= r_2_2 (str.indexof row_2 "C" 0)))

(assert (= c_0_0 (str.indexof col_0 "A" 0)))
(assert (= c_0_1 (str.indexof col_0 "B" 0)))
(assert (= c_0_2 (str.indexof col_0 "C" 0)))
(assert (= c_1_0 (str.indexof col_1 "A" 0)))
(assert (= c_1_1 (str.indexof col_1 "B" 0)))
(assert (= c_1_2 (str.indexof col_1 "C" 0)))
(assert (= c_2_0 (str.indexof col_2 "A" 0)))
(assert (= c_2_1 (str.indexof col_2 "B" 0)))
(assert (= c_2_2 (str.indexof col_2 "C" 0)))

(assert (= (= r_0_0 0) (= c_0_0 0)))
(assert (= (= r_0_0 1) (= c_1_0 0)))
(assert (= (= r_0_0 2) (= c_2_0 0)))
(assert (= (= r_1_0 0) (= c_0_0 1)))
(assert (= (= r_1_0 1) (= c_1_0 1)))
(assert (= (= r_1_0 2) (= c_2_0 1)))
(assert (= (= r_2_0 0) (= c_0_0 2)))
(assert (= (= r_2_0 1) (= c_1_0 2)))
(assert (= (= r_2_0 2) (= c_2_0 2)))
(assert (= (= r_0_1 0) (= c_0_1 0)))
(assert (= (= r_0_1 1) (= c_1_1 0)))
(assert (= (= r_0_1 2) (= c_2_1 0)))
(assert (= (= r_1_1 0) (= c_0_1 1)))
(assert (= (= r_1_1 1) (= c_1_1 1)))
(assert (= (= r_1_1 2) (= c_2_1 1)))
(assert (= (= r_2_1 0) (= c_0_1 2)))
(assert (= (= r_2_1 1) (= c_1_1 2)))
(assert (= (= r_2_1 2) (= c_2_1 2)))
(assert (= (= r_0_2 0) (= c_0_2 0)))
(assert (= (= r_0_2 1) (= c_1_2 0)))
(assert (= (= r_0_2 2) (= c_2_2 0)))
(assert (= (= r_1_2 0) (= c_0_2 1)))
(assert (= (= r_1_2 1) (= c_1_2 1)))
(assert (= (= r_1_2 2) (= c_2_2 1)))
(assert (= (= r_2_2 0) (= c_0_2 2)))
(assert (= (= r_2_2 1) (= c_1_2 2)))
(assert (= (= r_2_2 2) (= c_2_2 2)))

(check-sat)