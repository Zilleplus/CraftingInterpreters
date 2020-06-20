from sys import argv
from tokens import *
from tokenPrinter import TokenPrinter

def isStartOfWord(char: str):
    x = char.lower()
    alphabet = 'abcdefghijklmnopqrstuvwxyz'
    specialSymbols = '@_'
    return x in alphabet+specialSymbols and len(x)==1

def getReservedKeywordToken(word:str) -> TokenType:
    key_words = {
        "def" : TokenType.DEF,
        "if" : TokenType.IF,
        "else" : TokenType.ELSE,
        "then" : TokenType.THEN,
        "extern" : TokenType.EXTERN
    }
    if word in key_words:
        return key_words[word]
    return None # is not a reserved keyword

def getSpecialChars():
    return {
        "<" : TokenType.SMALLER_THEN,
        ">" : TokenType.GREATER_THEN,
        "=" : TokenType.EQUALS,
        "+" : TokenType.PLUS,
        "-" : TokenType.MINUS,
        "(" : TokenType.OPEN_BRACKET,
        ")" : TokenType.CLOSED_BRACKET,
        ' ' : TokenType.SPACE,
        '#' : TokenType.HASH,
        '*' : TokenType.ASTERIX,
        "'" : TokenType.SINGLE_QUOTE,
        '.' : TokenType.POINT,
        ',' : TokenType.COMMA
    }

def Lex(code:str):
    """
    input: text in string format
    output: list of tokens
    """
    iterChar = iter(code)
    char = next(iterChar)

    special_chars = getSpecialChars()

    position=Position("filename",0,0)
    empty= False
    while(not empty):
        token = None
        if(char in special_chars):
            token = Token(special_chars[char],position)
            char = next(iterChar)
            position.position_on_line = position.position_on_line + 1
        elif(char == '\n'):
            token = Token(TokenType.EOL,position)
            position.line_number = position.line_number + 1
            position.position_on_line = 0
            char = next(iterChar)
        elif(char.isdigit()):
            # this dude starts with a digit, so must be a number, as we don't support floats
            number = char
            char = next(iterChar)
            while(char.isdigit()):
                number = number + char
                # The end of an iterator causes an exception to be thrown.
                try:
                    char = next(iterChar)
                except:
                    break
            token = NumberToken(int(number),position)
            position.position_on_line = position.position_on_line + len(str(number))
        elif(isStartOfWord(char)):
            # this dude start with a l
            word = char
            char = next(iterChar)
            while(char.isalnum()):
                word = word + char
                # The end of an iterator causes an exception to be thrown.
                try:
                    char = next(iterChar)
                except:
                    break
            # check if this is a reserved word
            potential_token_type = getReservedKeywordToken(word)
            if ( potential_token_type==None):
                token = WordToken(word, position)
            else:
                token = Token(potential_token_type,position)
            position.position_on_line = position.position_on_line + len(word)
        else:
            # This is an invalid token, return invalid token and continue
            token = InvalidToken(char,position)
            position.position_on_line = position.position_on_line + 1
        yield token

def main(args):
    file_name = args[1]
    print("reading from file:"+file_name)
    file = open(file_name,"r")
    source_code = file.read()

    tokens = list(Lex(source_code))

    for t in tokens:
        print(TokenPrinter.toString(t))

if __name__ == "__main__":
    main(argv)
