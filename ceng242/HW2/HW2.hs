module HW2 (
    parse, -- reexport for easy terminal use
    foldAndPropagateConstants,
    assignCommonSubexprs,
    reducePoly
) where

import Expression
import Parser
import Data.Char
import Data.Text
-- import Data.Maybe
import Data.List
import Data.Function (on)
-- import Pprint

-- Do not change the module definition and imports above! Feel free
-- to modify the parser, but please remember that it is just a helper
-- module and is not related to your grade. You should only submit
-- this file. Feel free to import other modules, especially Data.List!

foldIfConstants :: ExprV -> ExprV
foldIfConstants (Leaf x) = (Leaf x)
foldIfConstants (UnaryOperation op (Leaf (Constant node))) = (Leaf (Constant (-1 * node)))
foldIfConstants (BinaryOperation op (Leaf (Constant a)) (Leaf (Constant b))) = case op of
    Plus -> (Leaf (Constant (a + b)))
    Times -> (Leaf (Constant (a * b)))
foldIfConstants (BinaryOperation op left right) =  foldIfConstants' (BinaryOperation op (foldIfConstants left) (foldIfConstants right))

foldIfConstants' :: ExprV -> ExprV
foldIfConstants' (BinaryOperation op (Leaf (Constant a)) (Leaf (Constant b))) = case op of
    Plus -> (Leaf (Constant (a + b)))
    Times -> (Leaf (Constant (a * b)))
foldIfConstants' x = x

replaceOne :: (String, Int) -> ExprV -> ExprV
replaceOne (str,val) (Leaf (Variable x)) = if (str == x) then (Leaf (Constant val)) else (Leaf (Variable x))
replaceOne (str,val)  (Leaf x) = (Leaf x)
replaceOne (str,val) (UnaryOperation Minus node) = (UnaryOperation Minus (replaceOne (str,val) node))
replaceOne (str,val)  (BinaryOperation opn left right) = ( BinaryOperation opn (replaceOne (str,val) left) (replaceOne (str,val) right) )

replaceAllLeft :: (String,Int) -> [(String, ExprV)] -> [(String, ExprV)]
replaceAllLeft _ [] = []
replaceAllLeft (str,val) ((s,exprv):xs) = [(s,(replaceOne (str,val) exprv))] ++ (replaceAllLeft (str,val) xs)

doesReplaceAllLeft :: (String, ExprV) -> [(String, ExprV)] -> [(String, ExprV)] 
doesReplaceAllLeft _ [] = []
doesReplaceAllLeft (str, (Leaf (Constant val))) x = replaceAllLeft (str, val) x
doesReplaceAllLeft _ x = x

foldH :: (String,ExprV) -> (String,ExprV)
foldH (x,y) = (x,foldIfConstants y)

foldAndPropagateConstants :: [(String, ExprV)] -> [(String, ExprV)]
foldAndPropagateConstants [] = []
foldAndPropagateConstants ((str,expv):xs) = [foldH (str,expv)] ++ foldAndPropagateConstants (doesReplaceAllLeft (foldH (str,expv)) xs)

-------------------------------------------------------------------------------------------------------------------------------------------

nameVar :: ExprV -> String
nameVar (UnaryOperation Minus (Leaf (Constant a))) = "$-" ++ (show a)
nameVar (UnaryOperation Minus (Leaf (Variable a))) = "$-" ++ a
nameVar (BinaryOperation Plus (Leaf (Constant a)) (Leaf (Constant b))) = "$" ++ (show a) ++ "+" ++ (show b)
nameVar (BinaryOperation Times (Leaf (Constant a)) (Leaf (Constant b))) = "$" ++ (show a) ++ "*" ++ (show b)
nameVar (BinaryOperation Plus (Leaf (Constant a)) (Leaf (Variable b))) = "$" ++ (show a) ++ "+" ++ b
nameVar (BinaryOperation Times (Leaf (Constant a)) (Leaf (Variable b))) = "$" ++ (show a) ++ "*" ++ b
nameVar (BinaryOperation Plus (Leaf (Variable a)) (Leaf (Constant b))) = "$" ++ a ++ "+" ++ (show b)
nameVar (BinaryOperation Times (Leaf (Variable a)) (Leaf (Constant b))) = "$" ++ a ++ "*" ++ (show b)
nameVar (BinaryOperation Plus (Leaf (Variable a)) (Leaf (Variable b))) = "$" ++ a ++ "+" ++ b
nameVar (BinaryOperation Times (Leaf (Variable a)) (Leaf (Variable b))) = "$" ++ a ++ "*" ++ b

findSubTrees :: ExprV -> [(ExprV)]
findSubTrees (Leaf a) = []
findSubTrees (UnaryOperation Minus (Leaf x)) = [(UnaryOperation Minus (Leaf x))]
findSubTrees (BinaryOperation op (Leaf a) (Leaf b)) = [((BinaryOperation op (Leaf a) (Leaf b)))]
findSubTrees (UnaryOperation op node) = findSubTrees node
findSubTrees (BinaryOperation op left right) = findSubTrees left ++ findSubTrees right

findCommonSubTree'' :: ExprV -> [(ExprV)] -> ExprV
findCommonSubTree'' _ [] = (Leaf (Constant 0)) 
findCommonSubTree'' exp (x:xs) = if (exp == x) then exp else findCommonSubTree'' exp xs

findCommonSubTree' :: [(ExprV)] -> [(ExprV)] -> ExprV
findCommonSubTree' [] _ = (Leaf (Constant 0))
findCommonSubTree' _ [] = (Leaf (Constant 0))
findCommonSubTree' (x:xs) y 
    | ( findCommonSubTree'' x y ) == (Leaf (Constant 0)) = findCommonSubTree' xs y
    | otherwise = x

findCommonSubTree :: [(ExprV)] -> ExprV
findCommonSubTree [] = (Leaf (Constant 0))
findCommonSubTree [a] = (Leaf (Constant 0))
findCommonSubTree x = findCommonSubTree' x (Data.List.tail x)

assignVarSubTree :: ExprV -> [(String, ExprV)]
assignVarSubTree (Leaf (Constant 0)) = []
assignVarSubTree exp = [(nameVar exp, exp)]

substituteVarBack :: (String, ExprV) -> ExprV -> ExprV
substituteVarBack (str,exp) (BinaryOperation oper (Leaf a) (Leaf b)) = if (exp == (BinaryOperation oper (Leaf a) (Leaf b))) then (Leaf (Variable str)) else ((BinaryOperation oper (Leaf a) (Leaf b)))
substituteVarBack (str, exp) (UnaryOperation Minus node) = if (exp == (UnaryOperation Minus node)) then (Leaf (Variable str)) else (UnaryOperation Minus (substituteVarBack (str,exp) node))
substituteVarBack (str, exp) (BinaryOperation oper left (Leaf a))
    |(left == exp) = (BinaryOperation oper (Leaf (Variable str)) (Leaf a))
    | otherwise = (BinaryOperation oper (substituteVarBack (str,exp) left) (Leaf a))
substituteVarBack (str, exp) (BinaryOperation oper (Leaf a) right)
    |(right == exp) = (BinaryOperation oper (Leaf a) (Leaf (Variable str)))
    | otherwise = (BinaryOperation oper (Leaf a) (substituteVarBack (str,exp) right))
substituteVarBack (str,exp) (BinaryOperation oper left right) = (BinaryOperation oper (substituteVarBack (str,exp) left) (substituteVarBack (str,exp) right) )

commonFinder :: ExprV -> ExprV
commonFinder x = findCommonSubTree (findSubTrees x)
nameSubTree :: ExprV -> (String, ExprV)
nameSubTree x = Data.List.head (assignVarSubTree (commonFinder x))

assignAll :: ExprV -> [(String, ExprV)] -> ([(String, ExprV)], ExprV)
assignAll exp list = if ((commonFinder exp) /= (Leaf (Constant 0))) then (assignAll (substituteVarBack (nameSubTree exp) exp) (list ++ [nameSubTree exp])) else (list,exp)

assignCommonSubexprs :: ExprV -> ([(String, ExprV)], ExprV)
assignCommonSubexprs exp = assignAll exp []

--------------------------------------------------------------------------------------------------------------------------------------------------

findLeaves :: ExprV -> [ExprV]
findLeaves (Leaf x) = [(Leaf x)]
findLeaves (BinaryOperation op l r) = findLeaves l ++ findLeaves r
findLeaves (UnaryOperation op node) = findLeaves node

findVariable' :: [ExprV] -> String
findVariable' [] = ""
findVariable' ((Leaf (Variable x)):xs) = x
findVariable' (x:xs) = findVariable' xs

findVariable :: ExprV -> String
findVariable ex = findVariable' (findLeaves ex)

multiplyOneWithAll :: (Int, Int) -> [(Int, Int)] -> [(Int, Int)]
multiplyOneWithAll (0,pow) [(coef2,pow2)] = [(0, pow)]
multiplyOneWithAll (coef,pow) [(coef2,pow2)] = [((coef * coef2), (pow + pow2))]
multiplyOneWithAll (c,p) (x:xs) = (multiplyOneWithAll (c,p) [x]) ++ (multiplyOneWithAll (c,p) xs)

multiplyLists :: [(Int, Int)] -> [(Int, Int)] -> [(Int, Int)]
multiplyLists [a] x = multiplyOneWithAll a x
multiplyLists (x:xs) y = addLists (multiplyOneWithAll x y) (multiplyLists xs y)

addOneToList :: (Int, Int) -> [(Int, Int)] -> [(Int, Int)]
addOneToList x [] = [x]
addOneToList (co,pow) (x:xs) = if ((snd x) == pow) then ([((co + (fst x)), pow)] ++ xs) else ([x] ++ (addOneToList (co,pow) xs))

addLists :: [(Int, Int)] -> [(Int, Int)] -> [(Int, Int)]
addLists [] y = y
addLists (x:xs) y = addLists xs (addOneToList x y)

treeToList :: ExprV -> [(Int,Int)]
treeToList (Leaf (Constant num)) = [(num,0)]
treeToList (Leaf (Variable var)) = [(1,1)]
treeToList (UnaryOperation Minus rest) = treeToList (BinaryOperation Times (Leaf (Constant (-1))) rest)
treeToList (BinaryOperation op left right) 
    | (op == Plus) = (addLists (treeToList left) (treeToList right))
    | (op == Times) = (multiplyLists (treeToList left) (treeToList right))

finalList :: ExprV -> [(Int, Int)]
finalList x = sortBy (flip compare `on` snd) (treeToList x)

nodeToBranch :: (Int,Int) -> String -> ExprV
nodeToBranch (x,0) _ = (Leaf (Constant x))
nodeToBranch (1,1) var = (Leaf (Variable var))
nodeToBranch (-1,1) var = (UnaryOperation Minus (Leaf (Variable var)))
nodeToBranch (x,1) var = (BinaryOperation Times (Leaf (Constant x)) (Leaf (Variable  var)))
nodeToBranch (x,n) var = (BinaryOperation Times (nodeToBranch (x,(n - 1)) var) (Leaf (Variable  var)))

listToTree :: [(Int, Int)] -> ExprV -> ExprV
listToTree [a] exp = nodeToBranch a (findVariable exp)
listToTree ((a,b):xs) exp = if (a==0) then (listToTree xs exp) else (BinaryOperation Plus ( listToTree xs exp ) (nodeToBranch (a,b) (findVariable exp)))

reducePoly :: ExprV -> ExprV
reducePoly x = listToTree (finalList x) x 

-- an extra dummy variable, so as to not crash the GUI
notImpl :: ExprV
notImpl = Leaf $ Variable "Not Implemented"

