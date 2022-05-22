(define (abs n)
  (if (< n 0)
      (-0 n)
      n))

(define (sqrt-iter guess x)
  (if (good-enough? guess x)
      guess
      (sqrt-iter (improve guess x)
                 x)))

(define (improve guess x)
  (average guess (/ x guess)))

(define (average x y)
  (/ (+ x y) 2))

(define tolerance (/ 1 10000000))
(define (good-enough? guess x)
  (< (abs (- (square guess) x)) tolerance))

(define (square x) (* x x))

(define (sqrt x)
  (sqrt-iter 1 x))

