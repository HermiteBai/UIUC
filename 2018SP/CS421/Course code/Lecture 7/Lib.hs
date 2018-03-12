report x = x

gcdstar xx k = aux xx k
  where 
    aux [] newk = newk 0
    aux (1:xs) newk = k 1
    aux (x:xs) newk = aux xs (\res -> newk (gcd x res))

fact 0 k = k 1
fact n k = fact (n - 1) (\v -> k (n * v))

sumList [] k = k 0
sumList (x:xs) k = sumList xs (\v -> k (x + v))

mymap f [] k = k []
mymap f (x:xs) k = mymap f xs (\v -> k ((f x):v))

mymin a b k
  | a < b = k a
  | otherwise = k b
mymin4 a b c d k  = mymin a b (\v1 -> k (mymin c d (\v2 -> k (mymin v1 v2 k))))

myif cond a b k
  | cond = a k
  | otherwise = b k