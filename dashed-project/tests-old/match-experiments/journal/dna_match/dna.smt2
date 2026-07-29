(set-logic ALL)
(set-option :produce-models true)

; ---------- Parameters ----------
; Length of the target string
(define-const L Int 100)

; Number of motifs
(define-const K Int 3)

; The allowed structure of the target string (S), as a regex
; S = "CAC(T|G)TGCGTGGATAAAC"
(define-const S_re RegLan 
  (re.++
    (str.to_re "CAC")
    (re.union (str.to_re "T") (str.to_re "G"))
    (str.to_re "TGCGTGGATAAACCGCGCGGGGTCTTGATGGCCATGTACGTCTCTCCACCATGAATATCCGGCTGCGTGGAACCGCGCGGGGTCTAACCGTCGCAT")
  )
)

; ---------- Target string ----------
(declare-const x String)
(assert (= (str.len x) L))
(assert (str.in_re x S_re)) ; x ∈ L(S)

; ---------- match_at_index function ----------
; Regex for motif 1
(define-const rho1 RegLan (str.to_re "CACGTG"))
(define-fun match_at_index1
  ((x String) (i Int)) Bool
  (and (>= i (- 1)) (< i (str.len x)) 
    (ite (= i (- 1))
      (not (str.in_re x (re.++ re.all rho1 re.all)))
      (and
        (not (str.in_re (str.substr x 0 i) (re.++ re.all rho1 re.all)))
        (str.in_re (str.substr x i (- (str.len x) i)) (re.++ rho1 re.all))
        (forall ((j Int))
          (=> (and (<= 0 j) (< j i))
              (not (str.in_re (str.substr x j (- (str.len x) j)) (re.++ rho1 re.all)))
          )
        )
      )
    )
  )
)
; Regex for motif 2
(define-const rho2 RegLan (re.++ (re.union (str.to_re "A") (str.to_re "G")) (str.to_re "CGTG")))
(define-fun match_at_index2
  ((x String) (i Int)) Bool
  (and (>= i (- 1)) (< i (str.len x)) 
    (ite (= i (- 1))
      (not (str.in_re x (re.++ re.all rho2 re.all)))
      (and
        (not (str.in_re (str.substr x 0 i) (re.++ re.all rho2 re.all)))
        (str.in_re (str.substr x i (- (str.len x) i)) (re.++ rho2 re.all))
        (forall ((j Int))
          (=> (and (<= 0 j) (< j i))
              (not (str.in_re (str.substr x j (- (str.len x) j)) (re.++ rho2 re.all)))
          )
        )
      )
    )
  )
)
; Regex for motif 3
(define-const rho3 RegLan (re.++ 
  (re.union (str.to_re "A") (str.to_re "G"))
  (re.union (str.to_re "C") (str.to_re "T"))
  (str.to_re "AAA")
  (re.union (str.to_re "A") (str.to_re "C") (str.to_re "T"))
))
(define-fun match_at_index3
  ((x String) (i Int)) Bool
  (and (>= i (- 1)) (< i (str.len x)) 
    (ite (= i (- 1))
      (not (str.in_re x (re.++ re.all rho3 re.all)))
      (and
        (not (str.in_re (str.substr x 0 i) (re.++ re.all rho3 re.all)))
        (str.in_re (str.substr x i (- (str.len x) i)) (re.++ rho3 re.all))
        (forall ((j Int))
          (=> (and (<= 0 j) (< j i))
              (not (str.in_re (str.substr x j (- (str.len x) j)) (re.++ rho3 re.all)))
          )
        )
      )
    )
  )
)

; ---------- Motif match positions ----------
(declare-const i1 Int)
(declare-const i2 Int)
(declare-const i3 Int)
; ---------- Match constraints ----------
(assert (match_at_index1 x i1))
(assert (match_at_index2 x i2))
(assert (match_at_index3 x i3))
; ---------- Count how many motifs matched (non-zero indexes) ----------
(define-const count_matches Int
  (+ (ite (>= i1 0) 1 0) (ite (>= i2 0) 1 0) (ite (>= i3 0) 1 0))
)

; ---------- Objective function ----------
; If < 2 matches, then objective is 0
; If ≥ 2 matches, then compute span between min and max non-zero match indices

(define-fun max3 ((a Int) (b Int) (c Int)) Int
  (ite (>= a b) (ite (>= a c) a c)
       (ite (>= b c) b c)))

(define-fun min_nonzero3 ((a Int) (b Int) (c Int)) Int
  (let ((a0 (ite (< a 0) (+ L 1) a))
        (b0 (ite (< b 0) (+ L 1) b))
        (c0 (ite (< c 0) (+ L 1) c)))
    (ite (<= a0 b0) (ite (<= a0 c0) a0 c0) (ite (<= b0 c0) b0 c0))
  )
)

(define-const obj Int (ite 
  (< count_matches 2) 0 (- (max3 i1 i2 i3) (min_nonzero3 i1 i2 i3))
))

; ---------- Optional: Check model ----------
(check-sat)
(get-value (x i1 i2 i3 obj))
(assert (< obj 11))
(check-sat)
(get-value (x i1 i2 i3 obj))
(assert (< obj 6))
(check-sat)

