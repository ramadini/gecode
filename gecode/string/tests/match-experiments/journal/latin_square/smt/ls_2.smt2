(declare-fun row_0 () String)
(declare-fun row_1 () String)
(declare-fun col_0 () String)
(declare-fun col_1 () String)

(declare-fun r_0_0 () Int)
(declare-fun c_0_0 () Int)
(declare-fun r_0_1 () Int)
(declare-fun c_0_1 () Int)
(declare-fun r_1_0 () Int)
(declare-fun c_1_0 () Int)
(declare-fun r_1_1 () Int)
(declare-fun c_1_1 () Int)



(assert (>= r_0_0 0))
(assert (>= r_0_1 0))
(assert (>= r_1_0 0))
(assert (>= r_1_1 0))

(assert (>= c_0_0 0))
(assert (>= c_0_1 0))
(assert (>= c_1_0 0))
(assert (>= c_1_1 0))

(assert (= r_0_0 (str.indexof row_0 "A" 0)))
(assert (= r_0_1 (str.indexof row_0 "B" 0)))
(assert (= r_1_0 (str.indexof row_1 "A" 0)))
(assert (= r_1_1 (str.indexof row_1 "B" 0)))

(assert (= c_0_0 (str.indexof col_0 "A" 0)))
(assert (= c_0_1 (str.indexof col_0 "B" 0)))
(assert (= c_1_0 (str.indexof col_1 "A" 0)))
(assert (= c_1_1 (str.indexof col_1 "B" 0)))

(assert (= (= r_0_0 0) (= c_0_0 0)))
(assert (= (= r_0_0 1) (= c_1_0 0)))
(assert (= (= r_1_0 0) (= c_0_0 1)))
(assert (= (= r_1_0 1) (= c_1_0 1)))
(assert (= (= r_0_1 0) (= c_0_1 0)))
(assert (= (= r_0_1 1) (= c_1_1 0)))
(assert (= (= r_1_1 0) (= c_0_1 1)))
(assert (= (= r_1_1 1) (= c_1_1 1)))

(check-sat)
