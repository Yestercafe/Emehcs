module Main where
import Control.Monad
import System.Environment
import Numeric (readOct, readHex)
import Text.ParserCombinators.Parsec hiding (spaces)

main :: IO()
main = getArgs >>= putStrLn . show . eval . readExpr . (!! 0)

symbol :: Parser Char
symbol = oneOf "!$%&|*+-/:<=?>@^_~"

readExpr :: String -> LispVal
readExpr input = case parse parseExpr "lisp" input of
    Left err -> String $ "No match: " ++ show err
    Right val -> val

spaces :: Parser ()
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

parseList :: Parser LispVal
parseList = liftM List $ sepBy parseExpr spaces

parseDottedList :: Parser LispVal
parseDottedList = do
    head <- endBy parseExpr spaces
    tail <- char '.' >> spaces >> parseExpr
    return $ DottedList head tail

parseQuoted :: Parser LispVal
parseQuoted = do
    char '\''
    x <- parseExpr
    return $ List [Atom "quote", x]

parseExpr :: Parser LispVal
parseExpr = try parseChar
        <|> parseNumber
        <|> parseString
        <|> parseAtom
        <|> parseQuoted
        <|> do char '('
               x <- (try parseList) <|> parseDottedList
               char ')'
               return x

showVal :: LispVal -> String
showVal (String contents) = "\"" ++ contents ++ "\""
showVal (Atom name) = name
showVal (Number contents) = show contents
showVal (Bool True) = "#t"
showVal (Bool False) = "#f"
showVal (List contents) = "(" ++ unwordsList contents ++ ")"
showVal (DottedList head tail) = "(" ++ unwordsList head ++ " . " ++ showVal tail ++ ")"

unwordsList :: [LispVal] -> String
unwordsList = unwords . map showVal

instance Show LispVal where show = showVal

eval :: LispVal -> LispVal
eval val@(String _) = val
eval val@(Number _) = val
eval val@(Bool _) = val
eval (List [Atom "quote", val]) = val
eval (List (Atom func : args)) = apply func $ map eval args

apply :: String -> [LispVal] -> LispVal
apply func args = maybe (Bool False) ($ args) $ lookup func primitives

primitives :: [(String, [LispVal] -> LispVal)]
primitives = [("+", numericBinop (+)),
              ("-", numericBinop (-)),
              ("*", numericBinop (*)),
              ("/", numericBinop div),
              ("mod", numericBinop mod),
              ("quotient", numericBinop quot),
              ("remainder", numericBinop rem)]

numericBinop :: (Integer -> Integer -> Integer) -> [LispVal] -> LispVal
numericBinop op params = Number $ foldl1 op $ map unpackNum params

unpackNum :: LispVal -> Integer
unpackNum (Number n) = n
unpackNum (String n) = let parsed = reads n in
                           if null parsed
                              then 0
                              else fst $ parsed !! 0
unpackNum (List [n]) = unpackNum n
unpackNum _ = 0
