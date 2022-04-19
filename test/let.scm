* test
(define foo (lambda ()
              (let ((add (lambda (x y) (+ x y))))
                (let ((x (add 2 3))
                      (y 1))
                  (+ x y)))))
(assert (foo) 6)
