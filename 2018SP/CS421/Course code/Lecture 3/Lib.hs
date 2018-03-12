module Lib
    ( swap
    , calc
    , Tree
    ) where

data Tree =
     TInt Integer
   | TOp String Tree Tree
   deriving (Show)

swap :: Tree -> Tree
swap (TInt x) = TInt x
swap (TOp s t1 t2) = TOp s (swap t2) (swap t1)

-- t1 = TOp "*" (TOp "+" (TInt 20) (TInt 1))
--             (TOp "-" (TInt 10) (TInt 9))

calc :: Tree -> Integer
calc (TInt x) = x
calc (TOp "+" t1 t2) = (calc t1) + (calc t2)
calc (TOp "-" t1 t2) = (calc t1) - (calc t2)
calc (TOp "*" t1 t2) = (calc t1) * (calc t2)