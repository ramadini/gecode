(declare-fun row_0 () String)
(declare-fun row_1 () String)
(declare-fun row_2 () String)
(declare-fun row_3 () String)
(declare-fun row_4 () String)
(declare-fun row_5 () String)
(declare-fun col_0 () String)
(declare-fun col_1 () String)
(declare-fun col_2 () String)
(declare-fun col_3 () String)
(declare-fun col_4 () String)
(declare-fun col_5 () String)

(declare-fun r_0_0 () Int)
(declare-fun c_0_0 () Int)
(declare-fun r_0_1 () Int)
(declare-fun c_0_1 () Int)
(declare-fun r_0_2 () Int)
(declare-fun c_0_2 () Int)
(declare-fun r_0_3 () Int)
(declare-fun c_0_3 () Int)
(declare-fun r_0_4 () Int)
(declare-fun c_0_4 () Int)
(declare-fun r_0_5 () Int)
(declare-fun c_0_5 () Int)
(declare-fun r_1_0 () Int)
(declare-fun c_1_0 () Int)
(declare-fun r_1_1 () Int)
(declare-fun c_1_1 () Int)
(declare-fun r_1_2 () Int)
(declare-fun c_1_2 () Int)
(declare-fun r_1_3 () Int)
(declare-fun c_1_3 () Int)
(declare-fun r_1_4 () Int)
(declare-fun c_1_4 () Int)
(declare-fun r_1_5 () Int)
(declare-fun c_1_5 () Int)
(declare-fun r_2_0 () Int)
(declare-fun c_2_0 () Int)
(declare-fun r_2_1 () Int)
(declare-fun c_2_1 () Int)
(declare-fun r_2_2 () Int)
(declare-fun c_2_2 () Int)
(declare-fun r_2_3 () Int)
(declare-fun c_2_3 () Int)
(declare-fun r_2_4 () Int)
(declare-fun c_2_4 () Int)
(declare-fun r_2_5 () Int)
(declare-fun c_2_5 () Int)
(declare-fun r_3_0 () Int)
(declare-fun c_3_0 () Int)
(declare-fun r_3_1 () Int)
(declare-fun c_3_1 () Int)
(declare-fun r_3_2 () Int)
(declare-fun c_3_2 () Int)
(declare-fun r_3_3 () Int)
(declare-fun c_3_3 () Int)
(declare-fun r_3_4 () Int)
(declare-fun c_3_4 () Int)
(declare-fun r_3_5 () Int)
(declare-fun c_3_5 () Int)
(declare-fun r_4_0 () Int)
(declare-fun c_4_0 () Int)
(declare-fun r_4_1 () Int)
(declare-fun c_4_1 () Int)
(declare-fun r_4_2 () Int)
(declare-fun c_4_2 () Int)
(declare-fun r_4_3 () Int)
(declare-fun c_4_3 () Int)
(declare-fun r_4_4 () Int)
(declare-fun c_4_4 () Int)
(declare-fun r_4_5 () Int)
(declare-fun c_4_5 () Int)
(declare-fun r_5_0 () Int)
(declare-fun c_5_0 () Int)
(declare-fun r_5_1 () Int)
(declare-fun c_5_1 () Int)
(declare-fun r_5_2 () Int)
(declare-fun c_5_2 () Int)
(declare-fun r_5_3 () Int)
(declare-fun c_5_3 () Int)
(declare-fun r_5_4 () Int)
(declare-fun c_5_4 () Int)
(declare-fun r_5_5 () Int)
(declare-fun c_5_5 () Int)

(assert (str.in_re row_2 (re.* (re.range "A" "F"))))
(assert (str.in_re col_2 (re.* (re.range "A" "F"))))
(assert (str.in_re row_3 (re.* (re.range "A" "F"))))
(assert (str.in_re col_3 (re.* (re.range "A" "F"))))
(assert (str.in_re row_4 (re.* (re.range "A" "F"))))
(assert (str.in_re col_4 (re.* (re.range "A" "F"))))
(assert (str.in_re row_5 (re.* (re.range "A" "F"))))
(assert (str.in_re col_5 (re.* (re.range "A" "F"))))

(assert (= (str.len row_2) 6))
(assert (= (str.len col_2) 6))
(assert (= (str.len row_3) 6))
(assert (= (str.len col_3) 6))
(assert (= (str.len row_4) 6))
(assert (= (str.len col_4) 6))
(assert (= (str.len row_5) 6))
(assert (= (str.len col_5) 6))

(assert (>= r_0_0 0))
(assert (>= r_0_1 0))
(assert (>= r_0_2 0))
(assert (>= r_0_3 0))
(assert (>= r_0_4 0))
(assert (>= r_0_5 0))
(assert (>= r_1_0 0))
(assert (>= r_1_1 0))
(assert (>= r_1_2 0))
(assert (>= r_1_3 0))
(assert (>= r_1_4 0))
(assert (>= r_1_5 0))
(assert (>= r_2_0 0))
(assert (>= r_2_1 0))
(assert (>= r_2_2 0))
(assert (>= r_2_3 0))
(assert (>= r_2_4 0))
(assert (>= r_2_5 0))
(assert (>= r_3_0 0))
(assert (>= r_3_1 0))
(assert (>= r_3_2 0))
(assert (>= r_3_3 0))
(assert (>= r_3_4 0))
(assert (>= r_3_5 0))
(assert (>= r_4_0 0))
(assert (>= r_4_1 0))
(assert (>= r_4_2 0))
(assert (>= r_4_3 0))
(assert (>= r_4_4 0))
(assert (>= r_4_5 0))
(assert (>= r_5_0 0))
(assert (>= r_5_1 0))
(assert (>= r_5_2 0))
(assert (>= r_5_3 0))
(assert (>= r_5_4 0))
(assert (>= r_5_5 0))

(assert (>= c_0_0 0))
(assert (>= c_0_1 0))
(assert (>= c_0_2 0))
(assert (>= c_0_3 0))
(assert (>= c_0_4 0))
(assert (>= c_0_5 0))
(assert (>= c_1_0 0))
(assert (>= c_1_1 0))
(assert (>= c_1_2 0))
(assert (>= c_1_3 0))
(assert (>= c_1_4 0))
(assert (>= c_1_5 0))
(assert (>= c_2_0 0))
(assert (>= c_2_1 0))
(assert (>= c_2_2 0))
(assert (>= c_2_3 0))
(assert (>= c_2_4 0))
(assert (>= c_2_5 0))
(assert (>= c_3_0 0))
(assert (>= c_3_1 0))
(assert (>= c_3_2 0))
(assert (>= c_3_3 0))
(assert (>= c_3_4 0))
(assert (>= c_3_5 0))
(assert (>= c_4_0 0))
(assert (>= c_4_1 0))
(assert (>= c_4_2 0))
(assert (>= c_4_3 0))
(assert (>= c_4_4 0))
(assert (>= c_4_5 0))
(assert (>= c_5_0 0))
(assert (>= c_5_1 0))
(assert (>= c_5_2 0))
(assert (>= c_5_3 0))
(assert (>= c_5_4 0))
(assert (>= c_5_5 0))

(assert (= r_0_0 (str.indexof row_0 "A" 0)))
(assert (= r_0_1 (str.indexof row_0 "B" 0)))
(assert (= r_0_2 (str.indexof row_0 "C" 0)))
(assert (= r_0_3 (str.indexof row_0 "D" 0)))
(assert (= r_0_4 (str.indexof row_0 "E" 0)))
(assert (= r_0_5 (str.indexof row_0 "F" 0)))
(assert (= r_1_0 (str.indexof row_1 "A" 0)))
(assert (= r_1_1 (str.indexof row_1 "B" 0)))
(assert (= r_1_2 (str.indexof row_1 "C" 0)))
(assert (= r_1_3 (str.indexof row_1 "D" 0)))
(assert (= r_1_4 (str.indexof row_1 "E" 0)))
(assert (= r_1_5 (str.indexof row_1 "F" 0)))
(assert (= r_2_0 (str.indexof row_2 "A" 0)))
(assert (= r_2_1 (str.indexof row_2 "B" 0)))
(assert (= r_2_2 (str.indexof row_2 "C" 0)))
(assert (= r_2_3 (str.indexof row_2 "D" 0)))
(assert (= r_2_4 (str.indexof row_2 "E" 0)))
(assert (= r_2_5 (str.indexof row_2 "F" 0)))
(assert (= r_3_0 (str.indexof row_3 "A" 0)))
(assert (= r_3_1 (str.indexof row_3 "B" 0)))
(assert (= r_3_2 (str.indexof row_3 "C" 0)))
(assert (= r_3_3 (str.indexof row_3 "D" 0)))
(assert (= r_3_4 (str.indexof row_3 "E" 0)))
(assert (= r_3_5 (str.indexof row_3 "F" 0)))
(assert (= r_4_0 (str.indexof row_4 "A" 0)))
(assert (= r_4_1 (str.indexof row_4 "B" 0)))
(assert (= r_4_2 (str.indexof row_4 "C" 0)))
(assert (= r_4_3 (str.indexof row_4 "D" 0)))
(assert (= r_4_4 (str.indexof row_4 "E" 0)))
(assert (= r_4_5 (str.indexof row_4 "F" 0)))
(assert (= r_5_0 (str.indexof row_5 "A" 0)))
(assert (= r_5_1 (str.indexof row_5 "B" 0)))
(assert (= r_5_2 (str.indexof row_5 "C" 0)))
(assert (= r_5_3 (str.indexof row_5 "D" 0)))
(assert (= r_5_4 (str.indexof row_5 "E" 0)))
(assert (= r_5_5 (str.indexof row_5 "F" 0)))

(assert (= c_0_0 (str.indexof col_0 "A" 0)))
(assert (= c_0_1 (str.indexof col_0 "B" 0)))
(assert (= c_0_2 (str.indexof col_0 "C" 0)))
(assert (= c_0_3 (str.indexof col_0 "D" 0)))
(assert (= c_0_4 (str.indexof col_0 "E" 0)))
(assert (= c_0_5 (str.indexof col_0 "F" 0)))
(assert (= c_1_0 (str.indexof col_1 "A" 0)))
(assert (= c_1_1 (str.indexof col_1 "B" 0)))
(assert (= c_1_2 (str.indexof col_1 "C" 0)))
(assert (= c_1_3 (str.indexof col_1 "D" 0)))
(assert (= c_1_4 (str.indexof col_1 "E" 0)))
(assert (= c_1_5 (str.indexof col_1 "F" 0)))
(assert (= c_2_0 (str.indexof col_2 "A" 0)))
(assert (= c_2_1 (str.indexof col_2 "B" 0)))
(assert (= c_2_2 (str.indexof col_2 "C" 0)))
(assert (= c_2_3 (str.indexof col_2 "D" 0)))
(assert (= c_2_4 (str.indexof col_2 "E" 0)))
(assert (= c_2_5 (str.indexof col_2 "F" 0)))
(assert (= c_3_0 (str.indexof col_3 "A" 0)))
(assert (= c_3_1 (str.indexof col_3 "B" 0)))
(assert (= c_3_2 (str.indexof col_3 "C" 0)))
(assert (= c_3_3 (str.indexof col_3 "D" 0)))
(assert (= c_3_4 (str.indexof col_3 "E" 0)))
(assert (= c_3_5 (str.indexof col_3 "F" 0)))
(assert (= c_4_0 (str.indexof col_4 "A" 0)))
(assert (= c_4_1 (str.indexof col_4 "B" 0)))
(assert (= c_4_2 (str.indexof col_4 "C" 0)))
(assert (= c_4_3 (str.indexof col_4 "D" 0)))
(assert (= c_4_4 (str.indexof col_4 "E" 0)))
(assert (= c_4_5 (str.indexof col_4 "F" 0)))
(assert (= c_5_0 (str.indexof col_5 "A" 0)))
(assert (= c_5_1 (str.indexof col_5 "B" 0)))
(assert (= c_5_2 (str.indexof col_5 "C" 0)))
(assert (= c_5_3 (str.indexof col_5 "D" 0)))
(assert (= c_5_4 (str.indexof col_5 "E" 0)))
(assert (= c_5_5 (str.indexof col_5 "F" 0)))

(assert (= (= r_0_0 0) (= c_0_0 0)))
(assert (= (= r_0_0 1) (= c_1_0 0)))
(assert (= (= r_0_0 2) (= c_2_0 0)))
(assert (= (= r_0_0 3) (= c_3_0 0)))
(assert (= (= r_0_0 4) (= c_4_0 0)))
(assert (= (= r_0_0 5) (= c_5_0 0)))
(assert (= (= r_1_0 0) (= c_0_0 1)))
(assert (= (= r_1_0 1) (= c_1_0 1)))
(assert (= (= r_1_0 2) (= c_2_0 1)))
(assert (= (= r_1_0 3) (= c_3_0 1)))
(assert (= (= r_1_0 4) (= c_4_0 1)))
(assert (= (= r_1_0 5) (= c_5_0 1)))
(assert (= (= r_2_0 0) (= c_0_0 2)))
(assert (= (= r_2_0 1) (= c_1_0 2)))
(assert (= (= r_2_0 2) (= c_2_0 2)))
(assert (= (= r_2_0 3) (= c_3_0 2)))
(assert (= (= r_2_0 4) (= c_4_0 2)))
(assert (= (= r_2_0 5) (= c_5_0 2)))
(assert (= (= r_3_0 0) (= c_0_0 3)))
(assert (= (= r_3_0 1) (= c_1_0 3)))
(assert (= (= r_3_0 2) (= c_2_0 3)))
(assert (= (= r_3_0 3) (= c_3_0 3)))
(assert (= (= r_3_0 4) (= c_4_0 3)))
(assert (= (= r_3_0 5) (= c_5_0 3)))
(assert (= (= r_4_0 0) (= c_0_0 4)))
(assert (= (= r_4_0 1) (= c_1_0 4)))
(assert (= (= r_4_0 2) (= c_2_0 4)))
(assert (= (= r_4_0 3) (= c_3_0 4)))
(assert (= (= r_4_0 4) (= c_4_0 4)))
(assert (= (= r_4_0 5) (= c_5_0 4)))
(assert (= (= r_5_0 0) (= c_0_0 5)))
(assert (= (= r_5_0 1) (= c_1_0 5)))
(assert (= (= r_5_0 2) (= c_2_0 5)))
(assert (= (= r_5_0 3) (= c_3_0 5)))
(assert (= (= r_5_0 4) (= c_4_0 5)))
(assert (= (= r_5_0 5) (= c_5_0 5)))
(assert (= (= r_0_1 0) (= c_0_1 0)))
(assert (= (= r_0_1 1) (= c_1_1 0)))
(assert (= (= r_0_1 2) (= c_2_1 0)))
(assert (= (= r_0_1 3) (= c_3_1 0)))
(assert (= (= r_0_1 4) (= c_4_1 0)))
(assert (= (= r_0_1 5) (= c_5_1 0)))
(assert (= (= r_1_1 0) (= c_0_1 1)))
(assert (= (= r_1_1 1) (= c_1_1 1)))
(assert (= (= r_1_1 2) (= c_2_1 1)))
(assert (= (= r_1_1 3) (= c_3_1 1)))
(assert (= (= r_1_1 4) (= c_4_1 1)))
(assert (= (= r_1_1 5) (= c_5_1 1)))
(assert (= (= r_2_1 0) (= c_0_1 2)))
(assert (= (= r_2_1 1) (= c_1_1 2)))
(assert (= (= r_2_1 2) (= c_2_1 2)))
(assert (= (= r_2_1 3) (= c_3_1 2)))
(assert (= (= r_2_1 4) (= c_4_1 2)))
(assert (= (= r_2_1 5) (= c_5_1 2)))
(assert (= (= r_3_1 0) (= c_0_1 3)))
(assert (= (= r_3_1 1) (= c_1_1 3)))
(assert (= (= r_3_1 2) (= c_2_1 3)))
(assert (= (= r_3_1 3) (= c_3_1 3)))
(assert (= (= r_3_1 4) (= c_4_1 3)))
(assert (= (= r_3_1 5) (= c_5_1 3)))
(assert (= (= r_4_1 0) (= c_0_1 4)))
(assert (= (= r_4_1 1) (= c_1_1 4)))
(assert (= (= r_4_1 2) (= c_2_1 4)))
(assert (= (= r_4_1 3) (= c_3_1 4)))
(assert (= (= r_4_1 4) (= c_4_1 4)))
(assert (= (= r_4_1 5) (= c_5_1 4)))
(assert (= (= r_5_1 0) (= c_0_1 5)))
(assert (= (= r_5_1 1) (= c_1_1 5)))
(assert (= (= r_5_1 2) (= c_2_1 5)))
(assert (= (= r_5_1 3) (= c_3_1 5)))
(assert (= (= r_5_1 4) (= c_4_1 5)))
(assert (= (= r_5_1 5) (= c_5_1 5)))
(assert (= (= r_0_2 0) (= c_0_2 0)))
(assert (= (= r_0_2 1) (= c_1_2 0)))
(assert (= (= r_0_2 2) (= c_2_2 0)))
(assert (= (= r_0_2 3) (= c_3_2 0)))
(assert (= (= r_0_2 4) (= c_4_2 0)))
(assert (= (= r_0_2 5) (= c_5_2 0)))
(assert (= (= r_1_2 0) (= c_0_2 1)))
(assert (= (= r_1_2 1) (= c_1_2 1)))
(assert (= (= r_1_2 2) (= c_2_2 1)))
(assert (= (= r_1_2 3) (= c_3_2 1)))
(assert (= (= r_1_2 4) (= c_4_2 1)))
(assert (= (= r_1_2 5) (= c_5_2 1)))
(assert (= (= r_2_2 0) (= c_0_2 2)))
(assert (= (= r_2_2 1) (= c_1_2 2)))
(assert (= (= r_2_2 2) (= c_2_2 2)))
(assert (= (= r_2_2 3) (= c_3_2 2)))
(assert (= (= r_2_2 4) (= c_4_2 2)))
(assert (= (= r_2_2 5) (= c_5_2 2)))
(assert (= (= r_3_2 0) (= c_0_2 3)))
(assert (= (= r_3_2 1) (= c_1_2 3)))
(assert (= (= r_3_2 2) (= c_2_2 3)))
(assert (= (= r_3_2 3) (= c_3_2 3)))
(assert (= (= r_3_2 4) (= c_4_2 3)))
(assert (= (= r_3_2 5) (= c_5_2 3)))
(assert (= (= r_4_2 0) (= c_0_2 4)))
(assert (= (= r_4_2 1) (= c_1_2 4)))
(assert (= (= r_4_2 2) (= c_2_2 4)))
(assert (= (= r_4_2 3) (= c_3_2 4)))
(assert (= (= r_4_2 4) (= c_4_2 4)))
(assert (= (= r_4_2 5) (= c_5_2 4)))
(assert (= (= r_5_2 0) (= c_0_2 5)))
(assert (= (= r_5_2 1) (= c_1_2 5)))
(assert (= (= r_5_2 2) (= c_2_2 5)))
(assert (= (= r_5_2 3) (= c_3_2 5)))
(assert (= (= r_5_2 4) (= c_4_2 5)))
(assert (= (= r_5_2 5) (= c_5_2 5)))
(assert (= (= r_0_3 0) (= c_0_3 0)))
(assert (= (= r_0_3 1) (= c_1_3 0)))
(assert (= (= r_0_3 2) (= c_2_3 0)))
(assert (= (= r_0_3 3) (= c_3_3 0)))
(assert (= (= r_0_3 4) (= c_4_3 0)))
(assert (= (= r_0_3 5) (= c_5_3 0)))
(assert (= (= r_1_3 0) (= c_0_3 1)))
(assert (= (= r_1_3 1) (= c_1_3 1)))
(assert (= (= r_1_3 2) (= c_2_3 1)))
(assert (= (= r_1_3 3) (= c_3_3 1)))
(assert (= (= r_1_3 4) (= c_4_3 1)))
(assert (= (= r_1_3 5) (= c_5_3 1)))
(assert (= (= r_2_3 0) (= c_0_3 2)))
(assert (= (= r_2_3 1) (= c_1_3 2)))
(assert (= (= r_2_3 2) (= c_2_3 2)))
(assert (= (= r_2_3 3) (= c_3_3 2)))
(assert (= (= r_2_3 4) (= c_4_3 2)))
(assert (= (= r_2_3 5) (= c_5_3 2)))
(assert (= (= r_3_3 0) (= c_0_3 3)))
(assert (= (= r_3_3 1) (= c_1_3 3)))
(assert (= (= r_3_3 2) (= c_2_3 3)))
(assert (= (= r_3_3 3) (= c_3_3 3)))
(assert (= (= r_3_3 4) (= c_4_3 3)))
(assert (= (= r_3_3 5) (= c_5_3 3)))
(assert (= (= r_4_3 0) (= c_0_3 4)))
(assert (= (= r_4_3 1) (= c_1_3 4)))
(assert (= (= r_4_3 2) (= c_2_3 4)))
(assert (= (= r_4_3 3) (= c_3_3 4)))
(assert (= (= r_4_3 4) (= c_4_3 4)))
(assert (= (= r_4_3 5) (= c_5_3 4)))
(assert (= (= r_5_3 0) (= c_0_3 5)))
(assert (= (= r_5_3 1) (= c_1_3 5)))
(assert (= (= r_5_3 2) (= c_2_3 5)))
(assert (= (= r_5_3 3) (= c_3_3 5)))
(assert (= (= r_5_3 4) (= c_4_3 5)))
(assert (= (= r_5_3 5) (= c_5_3 5)))
(assert (= (= r_0_4 0) (= c_0_4 0)))
(assert (= (= r_0_4 1) (= c_1_4 0)))
(assert (= (= r_0_4 2) (= c_2_4 0)))
(assert (= (= r_0_4 3) (= c_3_4 0)))
(assert (= (= r_0_4 4) (= c_4_4 0)))
(assert (= (= r_0_4 5) (= c_5_4 0)))
(assert (= (= r_1_4 0) (= c_0_4 1)))
(assert (= (= r_1_4 1) (= c_1_4 1)))
(assert (= (= r_1_4 2) (= c_2_4 1)))
(assert (= (= r_1_4 3) (= c_3_4 1)))
(assert (= (= r_1_4 4) (= c_4_4 1)))
(assert (= (= r_1_4 5) (= c_5_4 1)))
(assert (= (= r_2_4 0) (= c_0_4 2)))
(assert (= (= r_2_4 1) (= c_1_4 2)))
(assert (= (= r_2_4 2) (= c_2_4 2)))
(assert (= (= r_2_4 3) (= c_3_4 2)))
(assert (= (= r_2_4 4) (= c_4_4 2)))
(assert (= (= r_2_4 5) (= c_5_4 2)))
(assert (= (= r_3_4 0) (= c_0_4 3)))
(assert (= (= r_3_4 1) (= c_1_4 3)))
(assert (= (= r_3_4 2) (= c_2_4 3)))
(assert (= (= r_3_4 3) (= c_3_4 3)))
(assert (= (= r_3_4 4) (= c_4_4 3)))
(assert (= (= r_3_4 5) (= c_5_4 3)))
(assert (= (= r_4_4 0) (= c_0_4 4)))
(assert (= (= r_4_4 1) (= c_1_4 4)))
(assert (= (= r_4_4 2) (= c_2_4 4)))
(assert (= (= r_4_4 3) (= c_3_4 4)))
(assert (= (= r_4_4 4) (= c_4_4 4)))
(assert (= (= r_4_4 5) (= c_5_4 4)))
(assert (= (= r_5_4 0) (= c_0_4 5)))
(assert (= (= r_5_4 1) (= c_1_4 5)))
(assert (= (= r_5_4 2) (= c_2_4 5)))
(assert (= (= r_5_4 3) (= c_3_4 5)))
(assert (= (= r_5_4 4) (= c_4_4 5)))
(assert (= (= r_5_4 5) (= c_5_4 5)))
(assert (= (= r_0_5 0) (= c_0_5 0)))
(assert (= (= r_0_5 1) (= c_1_5 0)))
(assert (= (= r_0_5 2) (= c_2_5 0)))
(assert (= (= r_0_5 3) (= c_3_5 0)))
(assert (= (= r_0_5 4) (= c_4_5 0)))
(assert (= (= r_0_5 5) (= c_5_5 0)))
(assert (= (= r_1_5 0) (= c_0_5 1)))
(assert (= (= r_1_5 1) (= c_1_5 1)))
(assert (= (= r_1_5 2) (= c_2_5 1)))
(assert (= (= r_1_5 3) (= c_3_5 1)))
(assert (= (= r_1_5 4) (= c_4_5 1)))
(assert (= (= r_1_5 5) (= c_5_5 1)))
(assert (= (= r_2_5 0) (= c_0_5 2)))
(assert (= (= r_2_5 1) (= c_1_5 2)))
(assert (= (= r_2_5 2) (= c_2_5 2)))
(assert (= (= r_2_5 3) (= c_3_5 2)))
(assert (= (= r_2_5 4) (= c_4_5 2)))
(assert (= (= r_2_5 5) (= c_5_5 2)))
(assert (= (= r_3_5 0) (= c_0_5 3)))
(assert (= (= r_3_5 1) (= c_1_5 3)))
(assert (= (= r_3_5 2) (= c_2_5 3)))
(assert (= (= r_3_5 3) (= c_3_5 3)))
(assert (= (= r_3_5 4) (= c_4_5 3)))
(assert (= (= r_3_5 5) (= c_5_5 3)))
(assert (= (= r_4_5 0) (= c_0_5 4)))
(assert (= (= r_4_5 1) (= c_1_5 4)))
(assert (= (= r_4_5 2) (= c_2_5 4)))
(assert (= (= r_4_5 3) (= c_3_5 4)))
(assert (= (= r_4_5 4) (= c_4_5 4)))
(assert (= (= r_4_5 5) (= c_5_5 4)))
(assert (= (= r_5_5 0) (= c_0_5 5)))
(assert (= (= r_5_5 1) (= c_1_5 5)))
(assert (= (= r_5_5 2) (= c_2_5 5)))
(assert (= (= r_5_5 3) (= c_3_5 5)))
(assert (= (= r_5_5 4) (= c_4_5 5)))
(assert (= (= r_5_5 5) (= c_5_5 5)))

(check-sat)
