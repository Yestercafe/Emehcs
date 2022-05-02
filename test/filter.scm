* test
(define (empty? lst) (eq? lst '()))

(define (filter lst pred)
  (if (empty? lst)
      '()
      (let ((fst (car lst)))
        (if (pred fst)
            (cons fst (filter (cdr lst) pred))
            (filter (cdr lst) pred)))))

(assert (filter '(1 2 3 4 5 6) (lambda (x) (= 0 (mod x 2)))) '(2 4 6))

(define (is-odd x)
  (= 1 (mod x 2)))
(assert (filter '(1 2 3 4 5 6) is-odd) '(1 3 5))
