from AST import *
from lexer import Lex
from tokenPrinter import TokenPrinter
from itertools import *
from ASTPrinter import ASTPrinter
from ASTExprParser import ASTExprParser
from sys import *

def printTokens(tokens):
    for t in tokens:
        print(TokenPrinter.toString(t))

class ASTParser:
    """
    Convert tokens into an ast
    """
    def parse(tokens):
        """
        input: 
            tokens
        returns ast
        """
        ...

def main(args):
    file_name = args[1]
    print("reading from file:"+file_name)
    file = open(file_name,"r")
    source_code = file.read()

    tokens = Lex(source_code)

    for t in tokens:
        print(TokenPrinter.toString(t))

if __name__ == "__main__":
    main(argv)

