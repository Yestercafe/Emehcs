* test
(define (factorial-rec x)
  (if (= x 0)
      1
      (* x (factorial-rec (- x 1)))))

(define (factorial-iter-seg x i n)
  (if (= i n)
      x
      (factorial-iter-seg (* x (+ 1 i)) (+ 1 i) n)))
(define (factorial-iter x)
  (factorial-iter-seg 1 0 x))

(define (two-fact x)
  (print-ln (factorial-rec x))
  (print-ln (factorial-iter x))
  (assert (factorial-rec x) (factorial-iter x)))

(define (test-two-fact-aux i n)
  (if (> i n)
      #t
      (exec ((two-fact i)
             (test-two-fact-aux (+ 1 i) n)))))
(define (test-two-fact x)
  (test-two-fact-aux 0 x))

(test-two-fact 10)

