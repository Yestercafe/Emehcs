module Main where

main :: IO()
main = do name <- getLine
          putStrLn $ "Hello, " ++ name
