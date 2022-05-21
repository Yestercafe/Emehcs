; fib
(define fib (lambda (n)
              (cond ((= n 0) 1)
                    ((= n 1) 1)
                    (#t (+ (fib (- n 1)) (fib (- n 2)))))))
