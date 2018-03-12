-- Problem 1
negList x = map negate x

-- Problem 2
sumSqr :: [Integer] -> Integer
sumSqr = foldr (\a b -> (a*a) + b) 0

-- Problem 3
-- Can't do it. map :: (a -> b) -> [a] -> [b]
-- has to keep the shape of the input data structure

-- Problem 4
myzipWith f (x:xx) (y:yy) = (f x y) : (myzipWith f xx yy)
myzipWith f _ _ = []

fibs = 1 : 1 : zipWith (+) fibs (tail fibs)

-- Problem 5
-- foldl is tail recursive, save the memory

-- Problem 6
fix f x = 
  if x == result
    then result
    else fix f result
  where result = f x

fix_with_eps f eps x = aux x
  where aux x = 
    let result = f x
      in if (abs $ result - x) < eps
      	then result
      	else fix f result
      	
-- Problem 7
curry f x y = f (x, y)

-- Problem 8
flip f x y = f y x