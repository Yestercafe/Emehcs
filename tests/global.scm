* test
(define a 3)
(define half-b 2)
(define b (+ half-b half-b))
(define (sq x) (* x x))
(define sq-plus (lambda (x y) (+ (sq x) (sq y))))

(assert (sq-plus a b) 25)
