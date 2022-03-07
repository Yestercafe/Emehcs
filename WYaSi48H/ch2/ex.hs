module Main where
import Control.Monad
import System.Environment
import Numeric (readOct, readHex)
import Text.ParserCombinators.Parsec hiding (spaces)

main :: IO()
main = do args <- getArgs
          putStrLn (readExpr (args !! 0))

symbol :: Parser Char
symbol = oneOf "!$%&|*|=/:<=?>@^_~#"

readExpr :: String -> String
readExpr input = case parse parseExpr "lisp" input of
    Left err -> "No match: " ++ show err
    Right val -> "Found value"

spaces :: Parser()
spaces = skipMany1 space

data LispVal = Atom String
             | List [LispVal]
             | DottedList [LispVal] LispVal
             | Number Integer
             | String String
             | Bool Bool
             | Char Char

parseChar :: Parser LispVal
parseChar = do char '\''
               c <- noneOf "'"
               char '\''
               return $ Char c

parseString :: Parser LispVal
parseString = do char '"'
                 x <- many chars
                 char '"'
                 return $ String x
                 where chars = escaped <|> noneOf "\""
                       escaped = char '\\' >> choice (zipWith escapedChar codes replacements)
                       escapedChar :: Char -> Char -> Parser Char
                       escapedChar code replacement = char code >> return replacement
                       codes        = ['b',  'n',  'f',  'r',  't',  '\\', '\"', '/']
                       replacements = ['\b', '\n', '\f', '\r', '\t', '\\', '\"', '/']

parseAtom :: Parser LispVal
parseAtom = do first <- letter <|> symbol
               rest <- many (letter <|> digit <|> symbol)
               let atom = [first] ++ rest
               return $ case atom of
                          "#t" -> Bool True
                          "#f" -> Bool False
                          otherwise -> Atom atom

parseDec :: Parser LispVal
parseDec = do n <- many1 digit
              return (Number (read n))

parseOct :: Parser LispVal
parseOct = do char '#'
              char 'o'
              rest <- many1 octDigit
              return (Number (fst (readOct rest !! 0)))

parseHex :: Parser LispVal
parseHex = do char '#'
              char 'x'
              rest <- many1 hexDigit
              return (Number (fst (readHex rest !! 0)))

parseNumber :: Parser LispVal
parseNumber = parseDec
          <|> try parseOct
          <|> try parseHex

parseExpr :: Parser LispVal
parseExpr = parseChar
        <|> parseNumber
        <|> parseString
        <|> parseAtom
