module HW1 (
    form,
    constGrid,
    flatten,
    access,
    slice,
    vcat,
    hcat,
    without,
    matches2d
) where

-- do not modify the module declaration above!
-- this will ensure that you cannot load (compile)
-- the module without implementing all of the functions.

-- If you have functions you do not want to implement,
-- leave them as undefined or make them have another
-- default value. If you fully remove any of their definitions,
-- that will be a compilation error during evaluation,
-- and you will be eligible for (yay!) a 5 point deduction
-- (that's bad for your grade). Runtime errors in your code 
-- (rather than compilation errors) are acceptable and will simply
-- result in you getting zero from the specific test case causing
-- an error.

-------------------------
-- Fellowship of the Grid (25, 5, 5, 5 points)
form :: [a] -> (Int, Int) -> [[a]] 
form _ (0,_) = []
form _ (_,0) = []
form (a:b) (x,y) = (take y (a:b)):(form (drop y (a:b)) (x-1,y))

----------------------------

constGrid :: a -> (Int, Int) -> [[a]]
constGrid _ (0,_) = []
constGrid n (x,y) = (constGrid' n y):(constGrid n (x-1,y))

constGrid' a 0 = []
constGrid' a n = a:(constGrid' a (n-1))

-------------------------------

flatten :: [[a]] -> [a]
flatten n = flatten' n

flatten' [] = []
flatten' (a:b) = a ++ flatten' b

--------------------------------

access :: [[a]] -> (Int, Int) -> a
access (a:b) (0,x) = access' a x
access (a:b) (x,y) = access b (x-1,y)


access' (a:b) 0 = a
access' (a:b) n = access' b (n-1)



----------------------------
------------------------------
------------------------------



-- The Two Signatures (10, 5, 5, 10 points)
slice :: [[a]] -> (Int, Int) -> (Int, Int) -> [[a]]
slice [] _ _ = []
slice xs (a,b) (x,y) = slice' (take (b-a) (drop a xs)) (x,y)

slice' :: [[a]] -> (Int, Int) -> [[a]]
slice' [[]] _ = [[]]
slice' [] _ = []
slice' (x:xs) (a,b) = (take (b-a) (drop a x)):(slice' xs (a,b))

---------------------------------

vcat :: [[a]] -> [[a]] -> [[a]]
vcat xs xy = xs ++ xy

---------------------------------

hcat :: [[a]] -> [[a]] -> [[a]]
hcat [] x = x
hcat x [] = x
hcat (x:xs) (y:ys) = (x ++ y):(hcat xs ys)

---------------------------------

without :: [[a]] -> (Int, Int) -> (Int, Int) -> [[a]]
without [] _ _ = []
without xs (a,b) (c,d) = without' (take a xs ++ (drop b xs)) (c,d)

without' :: [[a]] -> (Int, Int) -> [[a]]
without' [] _ = []
without' (x:xs) (a,b) = (take a x ++ (drop b x)):(without' xs (a,b))

----------------------------
-- Return of the Non-trivial (30 points, 15 subject to runtime constraints)

matches2d :: Eq a => [[a]] -> [[a]] -> [(Int, Int)]
matches2d x y = isIn (subgWithIndex (subGridMaker x (length (head y), length y)) (length (head x) - length (head y) + 1)) (upperBound x y) y


index :: (Int,Int) -> (Int,Int) -> [[(Int,Int)]]
index (endrow,endcol) (row,col) = [[(x,x+row),(y,y+col)] |  x <- [0..endrow-row], y<-[0..endcol-col]]


subGridMaker :: [[a]] -> (Int,Int) -> [[[a]]]
subGridMaker xs (a,b) = [slice xs (x!!0) (x!!1) | x <- (index ((length (head xs)), (length xs)) (a,b))]

subgWithIndex :: [[[a]]] -> Int -> [[[[a]]]]
subgWithIndex [] _ = []
subgWithIndex x a = (take a x):(subgWithIndex (drop a x) a)

isIn :: Eq a => [[[[a]]]] -> (Int,Int) -> [[a]] -> [(Int,Int)]
isIn a (er,ec) b = [(x,y) | x <- [0..er], y <- [0..ec], a!!x!!y == b]

upperBound :: [[a]] -> [[a]] -> (Int,Int)
upperBound a b = ((length (head a) - length (head b)), (length a - length b))


----------------------------
-- What is undefined? Just a value that will cause an error
-- when evaluated, from the GHC implementation:
-- undefined = error "Prelude.undefined"
-- But it allows your module to be compiled
-- since the function definitions will exist.
