(set-logic QF_SLIA)
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

; ---------- Fixed-length motifs ----------
; All motifs below accept words of one fixed length. For a motif of length m,
; an occurrence selected at 0-based index i is the first occurrence iff:
;   1. x[i .. i+m-1] matches the motif; and
;   2. the prefix ending immediately before the selected occurrence's final
;      character contains no complete occurrence of the motif.
; Any occurrence starting before i would end inside that prefix.

(define-const rho1 RegLan (str.to_re "CACGTG"))
(define-const rho2 RegLan
  (re.++ (re.union (str.to_re "A") (str.to_re "G"))
         (str.to_re "CGTG")))
(define-const rho3 RegLan
  (re.++
    (re.union (str.to_re "A") (str.to_re "G"))
    (re.union (str.to_re "C") (str.to_re "T"))
    (str.to_re "AAA")
    (re.union (str.to_re "A") (str.to_re "C") (str.to_re "T"))))

; i = -1 denotes no occurrence; otherwise i is the 0-based first-match index.
(define-fun match_at_index1 ((s String) (i Int)) Bool
  (ite (= i (- 1))
    (not (str.in_re s (re.++ re.all rho1 re.all)))
    (and
      (>= i 0)
      (<= (+ i 6) (str.len s))
      (str.in_re (str.substr s i 6) rho1)
      (not
        (str.in_re
          (str.substr s 0 (+ i 5))
          (re.++ re.all rho1 re.all))))))

(define-fun match_at_index2 ((s String) (i Int)) Bool
  (ite (= i (- 1))
    (not (str.in_re s (re.++ re.all rho2 re.all)))
    (and
      (>= i 0)
      (<= (+ i 5) (str.len s))
      (str.in_re (str.substr s i 5) rho2)
      (not
        (str.in_re
          (str.substr s 0 (+ i 4))
          (re.++ re.all rho2 re.all))))))

(define-fun match_at_index3 ((s String) (i Int)) Bool
  (ite (= i (- 1))
    (not (str.in_re s (re.++ re.all rho3 re.all)))
    (and
      (>= i 0)
      (<= (+ i 6) (str.len s))
      (str.in_re (str.substr s i 6) rho3)
      (not
        (str.in_re
          (str.substr s 0 (+ i 5))
          (re.++ re.all rho3 re.all))))))

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
