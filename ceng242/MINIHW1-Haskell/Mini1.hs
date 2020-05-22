module Mini1 (
    gridMap,
    gridMapIf,
    evalExpr,
    getVars,
    evalDeriv,
    parse -- reexported to allow use
    ) where

import Expression
import Parser

-- Do not modify the module declaration and imports above!
-- Also do not change the function signatures and do not
-- remove the dummy implementations of the functions if
-- you want your code to compile.

-- Feel free to import anything else here
import Data.List

-- gridMap (20 points), map function over grid elements

gridMap :: (a -> b) -> [[a]] -> [[b]]
gridMap f [] = []
gridMap func (x:xs) = (map func x) : (gridMap func xs)

-- gridMapIf (20 points), map functions over grid elements 
-- that satisfy the predicate provided as the first arg.
listMapIf :: (a -> Bool) -> (a -> a) -> [a] -> [a]
listMapIf _ _ [] = []
listMapIf bf af (x:xs) = if (bf x) then ((af x):(listMapIf bf af xs)) else ([x]++(listMapIf bf af xs))

gridMapIf :: (a -> Bool) -> (a -> a) -> [[a]] -> [[a]]
gridMapIf _ _ [] = []
gridMapIf bool func (x:xs) = (listMapIf bool func x):(gridMapIf bool func xs)

-- evalExpr (20 points), evaluate the expression by
-- substituting (var, value) pairs given in the first arg.

replaceOneVariable :: (String, Int) -> ExprV -> ExprV
replaceOneVariable (str,val) (Leaf (Variable a)) = if (str==a) then ((Leaf (Constant val))) else (Leaf (Variable a))
replaceOneVariable _ (Leaf (Constant a)) = (Leaf (Constant a))
replaceOneVariable (str,val) (UnaryOperation Minus x) = (UnaryOperation Minus (replaceOneVariable (str,val) x))
replaceOneVariable (str,val) (BinaryOperation opr l r) = (BinaryOperation opr (replaceOneVariable (str,val) l) (replaceOneVariable (str,val) r))

replaceAllVars :: [(String, Int)] -> ExprV -> ExprV
replaceAllVars [] x = x
replaceAllVars (x:xs) expr = (replaceAllVars xs (replaceOneVariable x expr))

evalTree :: ExprV -> Int
evalTree (Leaf (Constant x)) = x
evalTree (UnaryOperation Minus x) = evalTree (BinaryOperation Times (Leaf (Constant (-1))) x)
evalTree (BinaryOperation op left right) = case op of
    Plus -> (evalTree left) + (evalTree right)
    Times -> (evalTree left) * (evalTree right)

evalExpr :: [(String, Int)] -> ExprV -> Int
evalExpr args expr = evalTree (replaceAllVars args expr)

-- getVars (20 points), return the variables contained
-- in the expression in a list (ordered, no duplicates)

getVarList :: ExprV -> [String]
getVarList (Leaf (Variable x)) = [x]
getVarList (Leaf x) = []
getVarList (UnaryOperation Minus x) = (getVarList x)
getVarList (BinaryOperation op l r) = (getVarList l) ++ (getVarList r)

getVars :: ExprV -> [String]
getVars ex = Data.List.sort (nub (getVarList ex))

-- evalDeriv (20 points), evaluate the first derivative
-- with respect to the variable given in the second
-- arg. using (var, value) pairs given in the first arg.

takeDeriv :: String -> ExprV -> ExprV
takeDeriv str (Leaf (Variable var)) = if (var == str) then (Leaf (Constant 1)) else (Leaf (Constant 0))
takeDeriv _ (Leaf (Constant num)) = (Leaf (Constant 0))
takeDeriv str (UnaryOperation Minus x) = (takeDeriv str (BinaryOperation Times (Leaf (Constant (-1))) x))
takeDeriv str (BinaryOperation op left right) = case op of
    Times -> (BinaryOperation Plus (BinaryOperation Times (takeDeriv str left) right) (BinaryOperation Times left (takeDeriv str right)))
    Plus -> (BinaryOperation Plus (takeDeriv str left) (takeDeriv str right))
    
evalDeriv :: [(String, Int)] -> String -> ExprV -> Int
evalDeriv elist str expr = (evalTree (replaceAllVars elist (takeDeriv str expr)))

-- Looks like that's all! 
