--factorial
fact 0 = 1
fact n | n > 0 = n * fact (n-1)

--remove negatives from list
remNeg [] = []
remNeg (x:xs) | x < 0 = result
							| otherwise = x : result
	where result = remNeg xs

--reverse a list
reverseList xx = aux xx []
	where 
		aux [] a = a
		aux (x:xs) a = aux xs (x:a)

--decrease all elements in a list
decList [] = []
decList (x:xs) = x - 1 : decList xs