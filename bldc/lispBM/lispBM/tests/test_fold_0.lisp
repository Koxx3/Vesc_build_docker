
(define fold (lambda (f i xs)
               (if (= xs nil)
                   i
                   (fold f (f i (car xs)) (cdr xs)))))

(= (fold '+ 0 (list 1 2 3 4 5 6 7 8 9 10)) 55)

