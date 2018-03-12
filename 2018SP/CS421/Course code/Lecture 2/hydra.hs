chop :: [Int] -> [Int]
chop xx = aux xx [] (length xx) 1
  where
    aux x a 0 c = reverse a
    aux (x:xs) a n c = 
      if x == 0
      then aux xs (x:a) (n-1) c
      else aux (grow xs (n-1) c) ((x-(1*c)):a) (n-1) 0
      	where grow (x:xs) n c = (x+(n*c)):xs
