module Main where
import System.Environment

main :: IO()
main = do args <- getArgs
          let a = read(args !! 0) :: Int
          let b = read(args !! 1) :: Int
          putStrLn (args !! 0 ++ " + " ++ args !! 1 ++ " = " ++ show(a + b))
