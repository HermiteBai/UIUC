-- Problem 1: Length of Tuple
--Can't do it

-- Problem 2: Insert pair of key and value into associative list
assoc :: Ord a => a -> t -> [(a, t)] -> [(a, t)]
assoc key value [] = [(key,value)]
assoc key value xx@((k,v):xs)
    | key < k = (key, value):xx
    | otherwise = (k, v) : assoc key value xs

-- Problem 3:
get 0 0 ((a, b),(c, d)) = a
get 1 0 ((a, b),(c, d)) = c
get 0 1 ((a, b),(c, d)) = b
get 1 1 ((a, b),(c, d)) = d

-- Problem 4:
data Direction = GoLeft | GoRight
get2 GoLeft GoLeft ((a, b), (c, d)) = a
get2 GoRight GoLeft ((a, b), (c, d)) = c
get2 GoLeft GoRight ((a, b), (c, d)) = b
get2 GoRight GoRight ((a, b), (c, d)) = d

-- Problem 5:
maybePlus (Just a) (Just b) = Just $ a + b
maybePlus _ _ = Nothing

lift f (Just x) (Just y) = Just $ f x y
lift f _ _ = Nothing

-- BST
data Tree a = Node a (Tree a) (Tree a)
            | Empty
    deriving (Show)

del victim Empty = Empty
del victim n@(Node x Empty Empty)
    | x == victim = Empty
    | otherwise = n
del victim n@(Node x c Empty)
    | x <= victim = c
    | otherwise = Node x (del victim c) fEmpty
del victim n@(Node x Empty c)
    | x <= victim = c
    | otherwise = Node x Empty (del victim c)
