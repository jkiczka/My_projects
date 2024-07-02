#lang racket
(require data/heap)
(provide sim? wire?
         (contract-out
          [make-sim        (-> sim?)]
          [sim-wait!       (-> sim? positive? void?)]
          [sim-time        (-> sim? real?)]
          [sim-add-action! (-> sim? positive? (-> any/c) void?)]

          [make-wire       (-> sim? wire?)]
          [wire-on-change! (-> wire? (-> any/c) void?)]
          [wire-value      (-> wire? boolean?)]
          [wire-set!       (-> wire? boolean? void?)]

          [bus-value (-> (listof wire?) natural?)]
          [bus-set!  (-> (listof wire?) natural? void?)]

          [gate-not  (-> wire? wire? void?)]
          [gate-and  (-> wire? wire? wire? void?)]
          [gate-nand (-> wire? wire? wire? void?)]
          [gate-or   (-> wire? wire? wire? void?)]
          [gate-nor  (-> wire? wire? wire? void?)]
          [gate-xor  (-> wire? wire? wire? void?)]

          [wire-not  (-> wire? wire?)]
          [wire-and  (-> wire? wire? wire?)]
          [wire-nand (-> wire? wire? wire?)]
          [wire-or   (-> wire? wire? wire?)]
          [wire-nor  (-> wire? wire? wire?)]
          [wire-xor  (-> wire? wire? wire?)]

          [flip-flop (-> wire? wire? wire? void?)]
          ))



(struct sim ([time  #:mutable] [heap  #:mutable]))
(struct wire ([value #:mutable] [actions  #:mutable] [sim #:mutable] ))

(define (cmp pair1 pair2)
  (<= (car pair1) (car pair2)))


(define (make-sim)
 (sim 0 (make-heap cmp)))

(define (sim-wait! sim ile)
  (define limit  (+ (sim-time sim ) ile))
  (define (pom)
    (if (and (> (heap-count(sim-heap sim)) 0) (<= (car(heap-min(sim-heap sim))) limit))
        (begin
          (set-sim-time! sim (car(heap-min(sim-heap sim))))
          ((cdr (heap-min (sim-heap sim))))
          (heap-remove-min! (sim-heap sim))
          (pom))
        (void)))
  (begin
    (pom)
    (set-sim-time! sim limit)
  ))


(define (sim-add-action! sim zaile zdarzenie)
   (heap-add! (sim-heap sim) (cons (+ (sim-time sim) zaile) zdarzenie)))

(define (make-wire sim)
  (wire #f '() sim))

(define (wire-on-change! w a) 
  (begin (a)
         (set-wire-actions! w (cons a (wire-actions w)))))

;(define (wire-value w)
 ; (wire-value w))

(define (wire-set! w b)
  (if (equal? b (wire-value w))
      (void)
      (begin  (set-wire-value! w b)
      (call-actions (wire-actions w) w))))

(define (call-actions xs w)
  (if(empty? xs)
     (void)
     (begin
       ((car xs))
       (call-actions (cdr xs) w))))
  


;------------------------------

(define (bus-set! wires value)
  (match wires
    ['() (void)]
    [(cons w wires)
     (begin
       (wire-set! w (= (modulo value 2) 1))
       (bus-set! wires (quotient value 2)))]))

(define (bus-value ws)
  (foldr (lambda (w value) (+ (if (wire-value w) 1 0) (* 2 value)))
         0
         ws))


;------------------------------------




(define (gate-not out in)            
   (define not-action (lambda()(wire-set! out (not (wire-value in)))))
   (wire-on-change! in (lambda() (sim-add-action! (wire-sim in) 1 not-action))))

(define (gate-and out in1 in2)            
   (define and-action (lambda()(wire-set! out (and  (wire-value in1) (wire-value in2)))))
   (wire-on-change! in1 (lambda() (sim-add-action! (wire-sim in1) 1 and-action)))
   (wire-on-change! in2 (lambda() (sim-add-action! (wire-sim in2) 1 and-action))))


(define (gate-nand out in1 in2)            
   (define nand-action (lambda()(wire-set! out (nand  (wire-value in1) (wire-value in2)))))
   (wire-on-change! in1 (lambda() (sim-add-action! (wire-sim in1) 1 nand-action)))
   (wire-on-change! in2 (lambda() (sim-add-action! (wire-sim in2) 1 nand-action))))


(define (gate-or out in1 in2)            
   (define or-action (lambda()(wire-set! out (or  (wire-value in1) (wire-value in2)))))
   (wire-on-change! in1 (lambda() (sim-add-action! (wire-sim in1) 1 or-action)))
   (wire-on-change! in2 (lambda() (sim-add-action! (wire-sim in2) 1 or-action))))


(define (gate-nor out in1 in2)            
   (define nor-action (lambda()(wire-set! out (nor  (wire-value in1) (wire-value in2)))))
   (wire-on-change! in1 (lambda() (sim-add-action! (wire-sim in1) 1 nor-action)))
   (wire-on-change! in2 (lambda() (sim-add-action! (wire-sim in2) 1 nor-action))))


(define (gate-xor out in1 in2)            
   (define xor-action (lambda()(wire-set! out (xor  (wire-value in1) (wire-value in2)))))
   (wire-on-change! in1 (lambda() (sim-add-action! (wire-sim in1) 2 xor-action)))
   (wire-on-change! in2 (lambda() (sim-add-action! (wire-sim in2) 2 xor-action))))

(define (wire-not in)
  (define out (make-wire (wire-sim in)))
(wire-set! out (not(wire-value in)))
  (gate-not out in)
  out)

(define (wire-and in1 in2)
  (define out (make-wire (wire-sim in1)))
  (wire-set! out (and(wire-value in1) (wire-value in2)))
  (gate-and out in1 in2)
  out)

(define (wire-nand in1 in2)
  (define out (make-wire (wire-sim in1)))
  (wire-set! out (nand(wire-value in1) (wire-value in2)))
  (gate-nand out in1 in2)
  out)

(define (wire-or in1 in2)
  (define out (make-wire (wire-sim in1)))
  (wire-set! out (or(wire-value in1) (wire-value in2)))
  (gate-or out in1 in2)
  out)

(define (wire-nor in1 in2)
  (define out (make-wire (wire-sim in1)))
  (wire-set! out (nor(wire-value in1) (wire-value in2)))
  (gate-nor out in1 in2)
  out)

(define (wire-xor in1 in2)
  (define out (make-wire (wire-sim in1)))
  (wire-set! out (xor(wire-value in1) (wire-value in2)))
  (gate-xor out in1 in2)
  out)



(define (flip-flop out clk data)
  (define sim (wire-sim data))
  (define w1  (make-wire sim))
  (define w2  (make-wire sim))
  (define w3  (wire-nand (wire-and w1 clk) w2))
  (gate-nand w1 clk (wire-nand w2 w1))
  (gate-nand w2 w3 data)
  (gate-nand out w1 (wire-nand out w3)))

