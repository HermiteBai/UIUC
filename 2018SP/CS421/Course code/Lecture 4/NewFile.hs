sumL :: [Integer] -> Integer
sumL = foldr (\a b -> (+) a b) 0

sumSqr :: [Integer] -> Integer
sumSqr = foldr (\a b -> (a*a) + b) 0