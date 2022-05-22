* test
(assert (make-string 1 #\a) "a")
(assert (make-string 3 #\a) "aaa")

(assert (string) "")
(assert (string #\a #\b #\c) "abc")

(assert (list->string '(#\a #\b #\c)) "abc")
(assert (list->string '()) "")
(assert (string->list "") '())
(assert (string->list "123") '(#\1 #\2 #\3))

(assert (string-copy "abc") "abc")

(assert (string=? "abc" "abc") #t)
(assert (string=? "" "") #t)
(assert (string=? "abc" "123") #f)
(assert (string<? "abc" "abc") #f)
(assert (string<? "abc" "123") #f)
(assert (string<? "" "asd") #t)
(assert (string<=? "abc" "abc") #t)
(assert (string<=? "" "") #t)
(assert (string-hash "abc") 2946)

(define s1 "hello")
(assert (string-capitalized? s1) #f)
(assert (string-capitalized? (string-capitalize s1)) #t)
(string-capitalize! s1)
(assert (string-capitalized? s1) #t)
(assert (string-upper-case? s1) #f)
(string-upcase! s1)
(assert (string-upper-case? s1) #t)
(assert (string-downcase s1) "hello")

(define s2 (string-append "hello" ", " "world"))
(assert s2 "hello, world")
(assert (string-head s2 5) "hello")
(assert (string-tail s2 7) "world")

(define s3 "hello")
(assert (string-pad-left s3 5) "hello")
(assert (string-pad-left s3 0) "")
(assert (string-pad-left s3 4) "ello")
(assert (string-pad-left s3 10) "     hello")
(assert (string-pad-right s3 5) "hello")
(assert (string-pad-right s3 3) "hel")
(assert (string-pad-right s3 0) "")
(assert (string-pad-right s3 10) "hello     ")

(define s4 " \t hello \n ")
(assert (string-trim-left s4) "hello \n ")
(assert (string-trim-right s4) " \t hello")
(assert (string-trim s4) "hello")
(assert (string-trim "") "")

(define s5 "abcde")
(assert (substring? "bcd" s5) #t)
(assert (substring? "bcb" s5) #f)

(assert (string-prefix? "abc" s5) #t)
(assert (string-suffix? "cde" s5) #t)

(assert (string-replace s5 #\a #\b) "bbcde")
(string-replace! s5 #\a #\b)
(assert s5 "bbcde")

(assert (vector-8b-ref s5 2) 99)
