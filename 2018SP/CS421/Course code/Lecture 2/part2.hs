--sum all elements in a list
sumList [] a = a
sumList (x:xs) a = sumList xs $ a + x

--increase all the elements in a list
incList xx = aux xx []
	where 
		aux [] a = reverse a
		aux (x:xs) a = aux xs (x + 1 : a)

--product all elements in a list
prodList xx = aux xx 1
	where
		aux [] a = a
		aux (x:xs) a = aux xs (x * a)
