#lang plait


(define-type-alias Value Number)

(define (run [s : S-Exp]): Value
  (type-case Answer (eval (parse s) mt-env)
  [(valueA a) a]
   [else (error 'run "run problem")]))


(define-type Op
  (add) (sub) (mul) (leq))


(define-type Exp
  (num [n : Number])
  (var [s : Symbol])
  (fun [name : Symbol] [args : (Listof Symbol)] [body : Exp])
  (prog [xs : (Listof Exp) ] [e : Exp])  
  (ifz [b : Exp] [l : Exp] [r : Exp])
  (op  [e1 : Exp] [o : Op] [e2 : Exp])
  (letE [x : Symbol] [e1 : Exp] [e2 : Exp])
  (app [f : Symbol] [args : (Listof Exp)]))


(define (parse [s : S-Exp]) : Exp
  (cond
    [(s-exp-match? `NUMBER s)
     (num (s-exp->number s))]
    [(s-exp-match? `{ifz ANY then ANY else ANY} s)
     (ifz (parse (second (s-exp->list s)))
          (parse (list-ref (s-exp->list s) 3))
          (parse (list-ref (s-exp->list s) 5)))]
    [(s-exp-match? `{let SYMBOL be ANY in ANY} s)
     (letE (s-exp->symbol (second (s-exp->list s)))
           (parse (fourth (s-exp->list s)))
           (parse (list-ref (s-exp->list s) 5)))]
     [(s-exp-match? `{ANY SYMBOL ANY} s)
     (op  (parse (first (s-exp->list s)))
           (parse-op (s-exp->symbol (second (s-exp->list s))))
          (parse (third (s-exp->list s))))]
    [(s-exp-match? `(fun SYMBOL (SYMBOL ...) =  ANY) s)
      (check-fun(fun (s-exp->symbol (second (s-exp->list s)))
           (map  s-exp->symbol (s-exp->list(third(s-exp->list s))))
           (parse (list-ref (s-exp->list s) 4))))]
     [(s-exp-match? `{define {ANY ...} for  ANY} s)
      (check-prog (prog (map  parse (s-exp->list(second(s-exp->list s))))
             (parse (list-ref (s-exp->list s) 3))))]
     [(s-exp-match? `SYMBOL s)
     (var (s-exp->symbol s))]
     [(s-exp-match? `{SYMBOL  (ANY ...)} s)
      (app (s-exp->symbol (first (s-exp->list s)))
          (map  parse (s-exp->list(second(s-exp->list s)))))]
    [else (error 'parse "invalid input")]))


(define (remove-duplicates list)
  (foldr (lambda (x y) (cons x (filter (lambda (z) (not (equal? x z))) y))) empty list))


(define (check-prog p);sprawdza czy wszystkie definy to funkcje i czy nie ma dwoch o tej samej nazwie
(if (check-prog2 (length (prog-xs p)) (prog-xs p) '())
    p
    (error 'parse "functions' names be different")
    ))

  (define (check-prog2  x xs ys)
(if (empty? xs)(equal? x (length (remove-duplicates ys)))
        
    (type-case Exp (first xs)
            [(fun name a b) (check-prog2  x (rest xs) (cons (first xs) ys)) ]
            [else (error 'parse "after 'define' there should be functions")])))



(define (check-fun p)
(if (equal? (length (fun-args p)) (length (remove-duplicates (fun-args p))))
    p
    (error 'parse "formal arguments should be different"))) ;sprawdza czy pareamtery formalne nie zawieraja powtorzen

(define (parse-op [op : Symbol]) : Op
  (cond
    [(eq? op '+) (add)]
    [(eq? op '-) (sub)]
    [(eq? op '*) (mul)]
    [(eq? op '<=) (leq)]
    [else (error 'parse "unknown operator")]))


(define-type Answer
  (valueA [v : Value])
  (closureA [f : Symbol] [args : (Listof Symbol)] [body : Exp][env : Env]))
(define-type Storable
  (valS [v : Answer])
  (undefS))

(define-type Binding
  (bind [name : Symbol]
        [ref : (Boxof Storable)]))

(define-type-alias Env (Listof Binding))

(define mt-env empty)

(define (extend-env-undef [env : Env] [x : Symbol]) : Env
  (cons (bind x (box (undefS))) env))

(define (extend-env [env : Env] [x : Symbol] [v : Answer]) : Env
  (cons (bind x (box (valS v))) env))

(define (find-var [env : Env] [x : Symbol]) : (Boxof Storable)
  (type-case (Listof Binding) env
    [empty (error x "unbound variable")]
    [(cons b rst-env) (cond
                        [(eq? x (bind-name b))
                         (bind-ref b)]
                        [else
                         (find-var rst-env x)])]))
  
(define (lookup-env [x : Symbol] [env : Env]) : Answer
  (type-case Storable (unbox (find-var env x))
    [(valS v) v]
    [(undefS) (error 'lookup-env "undefined variable")]))
   
(define (update-env! [env : Env] [x : Symbol] [v : Answer]) : Void
  (set-box! (find-var env x) (valS v)))



(define (op->proc [op : Op]) 
  (type-case Op op
    [(add) (lambda (x y)  (valueA (+ (valueA-v x) (valueA-v y))))]
    [(sub) (lambda (x y)  (valueA (- (valueA-v x) (valueA-v y))))]
    [(mul) (lambda (x y)  (valueA (* (valueA-v x) (valueA-v y))))]
    [(leq) (lambda (x y) (if (<= (valueA-v x) (valueA-v y)) (valueA 0) (valueA 42)))]))



(define (eval [e : Exp] [env : Env]) : Answer
  (type-case Exp e
    [(num n) (valueA n)]
    [(fun name args body)
     (closureA name args body env)]
    [(var x)
     (lookup-env x env)]
    [(prog xs e) 
    (eval e (make-closures xs (update-undef xs env) ))] 
    [(ifz b l r) (if (equal? (eval b env) (valueA 0)) (eval l env) (eval r env))]
    [(op  e1 o e2) (let* ([v1 (eval e1 env)]
                          [v2 (eval e2 env)])
                    ((op->proc o) v1 v2))]
    [(letE x e1 e2)
     (let ([v1 (eval e1 env)])
       (eval e2 (extend-env env x v1)))]
    [(app f args) (apply  (lookup-env f env) (map (lambda(x) (eval x env)) args) ) ]))



(define (update-undef xs env)
  (if (empty? xs)
      env
      (update-undef (rest xs) (extend-env-undef env (fun-name (first xs))))))


(define (make-closures xs env)
  (if (empty? xs)
      env
      (begin (update-env! env (fun-name (first xs)) (eval (first xs) env)) 
      (make-closures (rest xs) env))))


(define (apply f args2 )
  (type-case Answer f
    [(closureA name args1 body env)
     (eval body (pom args1 args2 (update-undef2 args1 env)))]
    [else (error 'apply "not a function")]))


(define (update-undef2 xs env)
  (if (empty? xs)
      env
      (update-undef2 (rest xs) (extend-env-undef env  (first xs)))))


(define (pom a1 a2 env)
  (if(equal? (length a1) (length a2))
          (if (empty? a1)
      env
      (begin (update-env! env  (first a1) (first a2)) (pom (rest a1) (rest a2) env )))
          (error 'eval "arity mismatch")))


