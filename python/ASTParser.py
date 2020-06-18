from AST import *
from lexer import Lex
from tokenPrinter import TokenPrinter
from itertools import *

"""
Define operator priority here,
Higher number means higher priority
"""
def operatorPriority(toke_type:TokenType) -> int:
    priors = {
        TokenType.SPACE : 0,
        TokenType.DEF : 0,
        TokenType.WORD : 0,
        TokenType.NUMBER : 0,
        TokenType.HASH : 0,
        TokenType.EXTERN : 0,
        TokenType.SMALLER_THEN : 0,
        TokenType.GREATER_THEN : 0,
        TokenType.EQUALS : 0,
        TokenType.OPEN_BRACKET : 0,
        TokenType.CLOSED_BRACKET : 0,
        TokenType.EOL : 0,
        TokenType.IF : 0,
        TokenType.ELSE : 0,
        TokenType.THEN : 0,
        TokenType.PLUS : 0
    }
    if toke_type in priors:
        return priors[token_type]
    return 0

class ASTParser:
    def parse(tokens):
        ...

    """
    input: iterator with tokens of one expression
    output: expression tree of expression
    """
    def recClimbExpression(tokens):
        expr=None
        priority=0
        try:
            while(True):
                expr=expressionRec(tokens,expr,priority)
        except StopIteration:
            return expr

    """
    By recursively going throught the high priority parsing first,
    operator prioirty is implicitly taking into account.
    This however increases the number of recursive call's and has bad 
    performance.
    """
    def expressionRecSlow(tokens,ast,current_position,priority=0):
        ...
    def parseSumExpr():
        ...
    def parseProdExpr():
        ...


    def statement():
        ...

def main(args):
    file_name = args[1]
    print("reading from file:"+file_name)
    file = open(file_name,"r")
    source_code = file.read()

    tokens = Lex(source_code)

    for t in tokens:
        print(TokenPrinter.toString(t))

def testExpression():
    expr = "5 + 10 - 2"
    tokens = filter(lambda x: x.toke_type != TokenType.SPACE, Lex(expr)) # remove the spaces as they have no meaning atm
    for t in tokens:
        print(TokenPrinter.toString(t))
    parsedExpression = ASTParser.expression(tokens)

if __name__ == "__main__":
    testExpression()
    #main(argv)

