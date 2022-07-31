(define (abs n)
  (if (< n 0)
      (-0 n)
      n))

(define tolerance .000001)
(define (fixed-point f first-guess)
  (define (close-enough? v1 v2)
    (< (abs (- v1 v2)) tolerance))
  (define (try guess)
    (let ((next (f guess)))
      (print f)
      (debug-get-function-env f)
      (print next)
      (if (close-enough? guess next)
          next
          (try next))))
  (try first-guess))

(define dx .000000001)
(define (deriv g)
  (lambda (x)
    (/ (- (g (+ x dx)) (g x))
       dx)))

(define (newton-transform g)
  (lambda (x)
    (- x (/ (g x) ((deriv g) x)))))

(define (newtons-method g guess)
  (fixed-point (newton-transform g) guess))

(define (sqrt x)
  (newtons-method (lambda (y) (- (* y y) x))
                  1))

(define kk (lambda (y) (- (* y y) 2)))

(sqrt 2)
