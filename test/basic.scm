* test
(assert 1 1)
(assert #t #t)
(assert #\a #\a)

(assert (-0 1) (- 0 1))

(assert (+ 1 1) 2)
(assert (/ (* (+ 2 1) 4) (* 2 2)) 3)
(assert (- 10 100) (-0 90))

(assert (car '(1 2 3 (1 2 3) . 4)) 1)
(assert (cdr '(1 2 3 (1 2 3) . 4)) '(2 3 (1 2 3) . 4))
(assert (cons 1 '(1 2 3 . 4)) '(1 1 2 3 . 4))
(assert (cons '(1 . 2) '(1 2 3 . (1 2 3))) '((1 . 2) 1 2 3 . (1 2 3)))

(assert (&& #t #f) #f)
(assert (&& #t #t) #t)
(assert (|| #t #f) #t)
(assert (|| #f #f) #f)
(assert (not (not #t)) #t)

(assert (> 2 1) #t)
(assert (/= 2 1) #t)
(assert (>= 3 2) #t)
(assert (= 2 1) #f)
(assert (eq? 1 1) #t)

(define m3 (-0 3))
(define m7 (-0 7))
(define m2 (-0 2))
(define m1 (-0 1))

(assert (mod 7 3) 1)
(assert (mod 7 m3) m2)
(assert (mod m7 3) m1)
(assert (mod m7 m3) 2)

(assert (remainder 7 3) 1)
(assert (remainder 7 m3) 1)
(assert (remainder m7 3) m1)
(assert (remainder m7 m3) m1)

(assert (quotient 7 3) 2)
(assert (quotient 7 m3) m2)
(assert (quotient m7 3) m2)
(assert (quotient m7 m3) 2)

(assert (if #t "Yes" "No") "Yes")
(assert (cond ((> 1 2) "Yes") ((&& #t #f) "Maybe") (#t "No")) "No")
(assert (cond ((> 1 2) "Yes") ((&& #t #t) "Maybe") (#t "No")) "Maybe")

