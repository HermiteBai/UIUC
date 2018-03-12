--max element in list
maxList (x:xs) = aux xs x
	where
		aux [] a = a
		aux (x:xs) a = aux xs (max a x) 

--factorial
fact n = aux n 1
	where	
		aux 0 a = a
		aux n a = aux (n - 1) (n * a)

--If all element in list satisfy some condition
allList p [] = True
allList p (x:xs) | p x = allList p xs
						 		 | otherwise = False

--fibonacci
fib n = aux 1 1 n
	where
		aux f1 f2 1 = f1
		aux f1 f2 n = aux f2 (f1 + f2) (n - 1) 