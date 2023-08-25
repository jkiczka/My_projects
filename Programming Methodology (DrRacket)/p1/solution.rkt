
#lang racket

(provide (struct-out column-info)
         (struct-out table)
         (struct-out and-f)
         (struct-out or-f)
         (struct-out not-f)
         (struct-out eq-f)
         (struct-out eq2-f)
         (struct-out lt-f)
         table-insert
         table-project
         table-sort
         table-select
         table-rename
         table-cross-join
         table-natural-join)

(define-struct column-info (name type) #:transparent)

(define-struct table (schema rows) #:transparent)



(define-struct and-f (l r))
(define-struct or-f (l r))
(define-struct not-f (e))
(define-struct eq-f (name val))
(define-struct eq2-f (name name2))
(define-struct lt-f (name val))

(define cities
  (table
   (list (column-info 'city    'string)
         (column-info 'country 'string)
         (column-info 'area    'number)
         (column-info 'capital 'boolean))
   (list (list "Wrocław" "Poland"  293 #f)
         (list "Warsaw"  "Poland"  517 #t)
         (list "Poznań"  "Poland"  262 #f)
         (list "Berlin"  "Germany" 892 #t)
         (list "Munich"  "Germany" 310 #f)
         (list "Paris"   "France"  105 #t)
         (list "Rennes"  "France"   50 #f))))



(define countries
  (table
   (list (column-info 'country 'string)
         (column-info 'population 'number))
   (list (list "Poland" 38)
         (list "Germany" 83)
         (list "France" 67)
         (list "Spain" 47))))

(define (table-insert row tab)
    (if(check row tab)
        (table (table-schema tab) (cons row (table-rows tab)))
           (error 'error)));"Niezgodnosc_typow"



(define (check row tab)
(if (= (length(table-schema tab)) (length row))
(check-type row (table-schema tab))
#f))


(define (type-check x)
    (cond[(number? x) 'number]
         [(string? x) 'string]
         [(symbol? x) 'symbol]
         [(boolean? x) 'boolean]
         [else 'other]))

(define(check-type row tschm)
    (cond[(and (null? row) (null? tschm)) #t]
         [(or (null? row) (null? tschm)) #f]
         [else (if (equal? (column-info-type (car tschm)) (type-check (car row)))
             (check-type (cdr row) (cdr tschm))
             #f)]))




(define (table-rename col ncol tab)
  (if (symbol? ncol)
      (table (find-name col ncol (table-schema tab) '()) (table-rows tab))
       (error 'Opis_kolumny_musi_miec_typ_symbol)
  ))



(define (find-name col ncol tschm xs)
    (cond [(null? tschm)  (reverse xs)]
          [(equal? (column-info-name (car tschm)) col) (find-name col ncol (cdr tschm) (cons (column-info ncol (column-info-type(car tschm))) xs)) ]
          [else (find-name col ncol (cdr tschm) (cons  (car tschm) xs ))]))




(define (check-log row f tab)
    (cond [(and-f? f) (and (check-log row (and-f-l f) tab) (check-log row (and-f-r f) tab))]
          [(or-f? f) (or (check-log row (or-f-l f) tab ) (check-log row (or-f-r f) tab))]
          [(not-f? f) (not (check-log row (not-f-e f) tab ))]
          [(eq-f? f) (equal? (eq-f-val f) (col-val (eq-f-name f) row (table-schema tab)))]
          [(eq2-f? f)(equal? (col-val (eq2-f-name f) row (table-schema tab) ) (col-val (eq2-f-name2 f) row (table-schema tab) ))]
          [(lt-f? f) (smaller? (col-val (lt-f-name f) row (table-schema tab))(lt-f-val f)  )]))



(define (col-val name row tschm)
    (cond [(not (symbol? name)) (error 'error)];"Opis_kolumny_musi_miec_typ_symbol"
          [(null? tschm)  (error 'error) ];"Kolumna_o_tej_nazwie_nie_istieje"
          [(equal? (column-info-name (car tschm)) name) (car row)  ]
          [else (col-val name (cdr row) (cdr tschm)  )]))





(define (smaller? a b)
    (if (equal? (type-check a) (type-check b))
                (cond [(equal? (type-check a) 'number) (if (< a b) #t #f)]
                      [(equal? (type-check a) 'string) (string<? a b )]
                      [(equal? (type-check a) 'symbol) (smaller? (symbol->string a) (symbol->string b))]
                      [(equal? (type-check a) 'boolean) (if (and(not a) b) #t #f )]
                     [(equal? (type-check a) 'other) (error 'error) ]); "Typy_danych_nieporownywalne"
                (error 'Typy_danych_nieporownywalne)))



(define (col-number name tschm)
  (define (it name tschm x)
    (cond [(not (symbol? name)) (error 'error)]; "Opis_kolumny_musi_miec_typ_symbol"
          [(null? tschm)  (error 'error ) ]; "Kolumna_o_tej_nazwie_nie_istieje"
          [(equal? (column-info-name (car tschm)) name) x  ]
           [else (it name (cdr tschm) (+ 1 x) )]))
  (it name tschm 0))


(define (nth lst x)
  (cond ((null? lst) (error 'error ));"nie istnieje taki indeks na liscie"
        ((= x 0) (first lst))
        (else (nth (rest lst) (- x 1)))))


(define (col-val2 name row tschm)
  (nth row (col-number name tschm)))
 

(define (table-select form tab)
(table 
 (table-schema tab)
  (filter (lambda(x) (check-log x form tab)) (table-rows tab))))


(define (table-cross-join tab1 tab2)
  (table
   (append (table-schema tab1) (table-schema tab2))
   (ap2 (table-rows tab1) (table-rows tab2))
   ))

(define(app xs ys)
  (define(it xs ys w)
    (if  (null? ys) (reverse w) (it xs (cdr ys) (cons (append (car xs) (car ys)) w))))
  (it xs ys null)
  )

 (define (ap2 xs ys)
   (define (it xs ys w)
     (if (null? xs)
         w
         (append (app (list(car xs)) ys) (it (cdr xs) ys w) )))
   (it xs ys null))





(define (list-n-col cols tschm) ;tworzy liste numerow column z listy nazw
    (define(it cols xs)
      (if (empty? cols)
          (sort xs >);sort, zeby kolejnosc byla taka jakw  wyjsciowej tabeli
          (it (cdr cols) (cons (col-number (car cols) tschm) xs))))
    (it cols null))


(define lista-nr-kolumn (list-n-col (list 'country 'area) (table-schema cities)))

(define (row-col-n row lcols) ;zwraca wiersz o kolumnach o numerach z listy
    (define(it lcols xs)
      (if (empty? lcols)
          xs
          (it (cdr lcols) (cons (nth row (car lcols) ) xs))))
    (it lcols null))



(define (table-project cols tab)
 (let ([lista (list-n-col cols (table-schema tab))])
   (table
   (row-col-n (table-schema tab) lista)
   (map (lambda(x)(row-col-n x lista)) (table-rows tab))))) 
   

(define (comp-rows w1 w2 lp)
(cond [(null? lp) #f]
      [(equal? (nth w1 (car lp)) (nth w2 (car lp))) (comp-rows w1 w2 (cdr lp))]
      [(smaller? (nth w1 (car lp)) (nth w2 (car lp))) #t]
      [else #f]
      ))
(define (komparator lp)
(lambda ( x y) (comp-rows x y lp)))


;(define (insert-generic lt n xs lp)
 ; (if (null? xs)
  ;    (list n)
   ;   (if (lt n (car xs) lp)
    ;      (cons n xs)
     ;     (cons (car xs) (insert-generic lt n (cdr xs) lp)))))

;(define (insertion-sort-generic lt xs lp)
 ; (define (it xs ys)
  ;  (if (null? xs)
   ;     ys
    ;    (it (cdr xs) (insert-generic lt (car xs) ys lp))))
  ;(it xs null))

;(define (insertion-sort-new xs lp)
 ; (insertion-sort-generic comp-rows xs lp))
(define (table-sort cols tab)
  (table
   (table-schema tab)
   (sort (table-rows tab) (komparator (list-n-col2 cols (table-schema tab)))))) 




(define (list-n-col2 cols tschm) ;tworzy liste numerow column z listy nazw
    (define(it cols xs)
      (if (empty? cols)
          (reverse xs) 
          (it (cdr cols) (cons (col-number (car cols) tschm) xs))))
    (it cols null))



(define (intersection-cols a b)
 (map (lambda(y) (column-info-name y))(filter (lambda(x) (member x b) ) a)))


(define (dodaj-jedynke x)
  (string->symbol(string-append "!" (symbol->string x))))

(define (new-name xs)
  (map (lambda(x) (dodaj-jedynke x)) xs ))




(define (f1 lp nlp tab) ;zmiana nazw powtarzajacych sie kolumn
       (if(null? lp)
          tab
          (f1 (cdr lp) (cdr nlp)
              (table-rename (car lp) (car nlp) tab))))



(define (f2 tab1 tab2) ;zlaczanie kartezjanskie
    (table-cross-join tab1
       (f1 (intersection-cols (table-schema tab1) (table-schema tab2))
           (new-name (intersection-cols (table-schema tab1) (table-schema tab2)))
           tab2)))

(define (gen-form xs ys ) ;generowanie formuly do selecta wierszy
    (define (it xs ys fm)
       (if(null? xs)
          fm
          (it (cdr xs) (cdr ys)
               (and-f(eq2-f (car xs)(car ys) ) fm)



              )))
    (it (cdr xs) (cdr ys) (eq2-f (car xs)
                              (car ys)) ))


(define (f3 tab1 tab2)
    (table-select (gen-form
                   (intersection-cols (table-schema tab1) (table-schema tab2))
                   (new-name (intersection-cols (table-schema tab1) (table-schema tab2)))

                   ) (f2 tab1 tab2)))




(define (rem-el xs ys)
  (filter (lambda (x) (not (member x xs))) ys))

(define (gen-col-n tschm)
  (map (lambda (x) (column-info-name x)) tschm))


(define (table-natural-join tab1 tab2)
    (table-project (rem-el (new-name(intersection-cols (table-schema tab1) (table-schema tab2))) (gen-col-n(table-schema (f3 tab1 tab2))))
    
     (f3 tab1 tab2)))



