from AST import *
from lexer import Lex
from tokenPrinter import TokenPrinter
from itertools import *
from ASTPrinter import ASTPrinter
from sys import *

def printTokens(tokens):
    for t in tokens:
        print(TokenPrinter.toString(t))

def operatorPriority(token_type:TokenType) -> int:
    """
    Define operator priority here,
    Higher number means higher priority
    """
    priors = {
        TokenType.SMALLER_THEN : 0,
        TokenType.GREATER_THEN : 0,
        TokenType.EQUALS : 0,
        TokenType.OPEN_BRACKET : 0,
        TokenType.CLOSED_BRACKET : 0,
        TokenType.ASTERIX : 1,
        TokenType.PLUS : 0
    }
    if token_type in priors:
        return priors[token_type]
    return 0

class ASTExprParser:
    def parseUniExpr(token):
        """
        convert number/var into ast

        input :
            - token
        returns ast
        """
        if(token.token_type==TokenType.NUMBER):
            return Number(token.number)
        else:
            raise Exception("expected unitary token, but got "+str(token.token_type)+" else")

    def parseBinExpr(leftExpr,operatorToken,rightExpr):
        """
        convert op token into ast

        input:
            - leftExpr : ast
            - operatorToken : token
            - rightExpr : ast
        returns ast
        """
        try:
            binOperatorType = TokenToOperator.binary_operator[operatorToken.token_type]
            return BinaryOperator(binOperatorType,leftExpr,rightExpr)
        except StopIteration:
            raise Exception("expected binary operator, but got "+str(operatorToken.token_type)+" else")

    def parseRec(leftExpr,operatorToken,rightToken,tokens):
        """
        Parse rec down 

        input:
            - leftExpr : ast
            - operatorToken : token
            - rightToken : token
            - tokens : iterator of tokens
        """
        try:
            nextOperatorToken = next(tokens)

            try:
                nextRightToken = next(tokens)

                if(operatorPriority(nextOperatorToken.token_type)>operatorPriority(operatorToken.token_type)):
                    newLeft = ASTExprParser.parseUniExpr(rightToken)
                    priorityRight = ASTExprParser.parseRec(newLeft,nextOperatorToken,nextRightToken ,tokens)
                    return ASTExprParser.parseBinExpr(leftExpr,operatorToken,priorityRight)
                else:
                    newRight = ASTExprParser.parseUniExpr(rightToken)
                    newLeft = ASTExprParser.parseBinExpr(leftExpr,operatorToken,newRight)
                    return ASTExprParser.parseRec(newLeft,nextOperatorToken,nextRightToken,tokens)

            except StopIteration: # no next right token available, but we did get an operator
                raise Exception("unexpected ending of expression: " + str(nextOperatorToken.token_type))
        except StopIteration:
            # apparently there is nothing left to do
            # does not take into account invalid expr such as "3 + 3 -"
            # leftExpr = left
            rightExpr = ASTExprParser.parseUniExpr(rightToken)
            return ASTExprParser.parseBinExpr(leftExpr, operatorToken, rightExpr)

    def parseExpression(tokens):
        """
        input:
            - tokens: iterator of tokens -> should not contain error tokens at this point
        returns ast
        """
        firstToken = next(tokens)
        expr = ASTExprParser.parseUniExpr(firstToken)
        try:
            secondToken = next(tokens)
            thirdToken = next(tokens)
            return ASTExprParser.parseRec(expr,secondToken,thirdToken,tokens)
        except StopIteration:
            return expr

def main(args):
    expr = "5 + 10 * 2"

    unfilteredTokens = Lex(expr)

    # remove the spaces as they have no meaning atm
    tokens = list(filter(lambda x: x.token_type != TokenType.SPACE, unfilteredTokens))
    printTokens(tokens)

    print("parsing tokens into expr: \n")
    parsedExpression = ASTExprParser.parseExpression(iter(tokens))
    ASTPrinter.print(parsedExpression)

if __name__ == "__main__":
    main(argv)
