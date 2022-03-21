(/ (* (+ 2 1) 4) (* 2 2))
(car '(1 2 3 (1 2 3) . 4))
(cdr '(1 2 3 (1 2 3) . 4))
(cons 1 '(1 2 3 . 4))
(cons '(1 . 2) '(1 2 3 . (1 2 3)))

(if #t "Yes" "No")
(cond ((> 1 2) "Yes") ((&& #t #f) "Maybe") (else "No"))
(cond ((> 1 2) "Yes") ((&& #t #t) "Maybe") (else "No"))