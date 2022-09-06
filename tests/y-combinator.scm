; https://zhuanlan.zhihu.com/p/24237196

(define Y
  (lambda (fn)
    ((lambda (f)
       (f f)) (lambda (f)
                (fn (lambda (s) ((f f) s)))))))

((Y (lambda (g)
      (lambda (s)
        (cond
          ((eq? s '()) '())
          (#t
            (cons (string-upcase (car s)) (g (cdr s)))))))) '("a" "b" "c" "d" "e"))

