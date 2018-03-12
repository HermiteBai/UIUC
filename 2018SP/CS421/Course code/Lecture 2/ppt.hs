calc n i | n == 2 = i
         | odd n = calc (n*3+1) (i+1)
         | otherwise = calc (n `div` 2) (i+1)
