* test
(define (foo x) (+ x 1))

(define (add-with-3-4)
  (let ((x 3)
        (y 4))
    (+ x y)))

(assert (add-with-3-4) 7)

(define k 2)
(define (return-k k) k)
(assert (return-k 3) 3)
