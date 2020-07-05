from enum import Enum,auto
from copy import deepcopy

class TokenType(Enum):
    SPACE = auto()
    DEF = auto()
    NUMBER = auto()
    WORD = auto()
    HASH = auto()
    EXTERN = auto()
    SMALLER_THEN = auto()
    GREATER_THEN = auto()
    EQUALS = auto()
    OPEN_BRACKET = auto()
    CLOSED_BRACKET = auto()
    IF = auto()
    ELSE = auto()
    THEN = auto()
    MINUS = auto()
    ASTERIX = auto()
    PLUS = auto()
    EOL = auto()
    SINGLE_QUOTE = auto()
    POINT = auto()
    COMMA = auto()
    INVALID = auto()
    WEDGE = auto()

class Position:
    def __init__(self,file_name:str,line_number:int,position_on_line:int):
        self.line_number=line_number
        self.file_name=file_name
        self.position_on_line=position_on_line

class Token:
    def __init__(self,token_type:TokenType,position: Position):
        self.token_type=token_type
        self.position=deepcopy(position)

class WordToken(Token):
    def __init__(self,word:str,position:Position):
        super().__init__(TokenType.WORD,position)
        self.word=word

class NumberToken(Token):
    def __init__(self,number: int,position:Position):
        super().__init__(TokenType.NUMBER,position)
        self.number = number

class InvalidToken(Token):
    def __init__(self,invalid_char,position:Position):
        super().__init__(TokenType.INVALID,position)
        self.invalidChar = invalid_char
