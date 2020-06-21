from AST import *
from lexer import Lex
from tokenPrinter import TokenPrinter
from itertools import *
from ASTPrinter import ASTPrinter

def printTokens(tokens):
    for t in tokens:
        print(TokenPrinter.toString(t))

"""
Define operator priority here,
Higher number means higher priority
"""
def operatorPriority(toke_type:TokenType) -> int:
    priors = {
        TokenType.SMALLER_THEN : 0,
        TokenType.GREATER_THEN : 0,
        TokenType.EQUALS : 0,
        TokenType.OPEN_BRACKET : 0,
        TokenType.CLOSED_BRACKET : 0,
        TokenType.ASTERIX : 1,
        TokenType.PLUS : 0
    }
    if toke_type in priors:
        return priors[token_type]
    return 0

class ASTExpressionParser:
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
            raise Exception("expected unitary token, but got something else")

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
        except:
            raise Exception("expected binary operator, but got something else")


    def parseExpression(tokens):
        """
        input:
            - tokens: iterator of tokens -> should not contain error tokens at this point
        returns ast
        """
        firstToken = next(tokens)
        expr = ASTExpressionParser.parseUniExpr(firstToken)
        try: 
            secondToken = next(tokens)
            thirdToken = next(tokens)
            return parseRec(expr,secondToken,thirdToken,tokens)
        except:
            return expr

    def parseRec(left,operator,right,tokens):
        """
        Parse rec down till the next operator is not stronger then the previous one

        input:
            - leftExpr : ast
            - operatorToken : token
            - rightExpr : ast
            - tokens : iterator of tokens
        """
        try:
            nextOperator = next(tokens)

            try:
                nextRightToken = next(tokens)

                if(operatorPriority(nextOperator)>operatorPriority(operator)):
                    priorityRight = parseRec(parseUniExpr(rightToken),nextOperator,next(tokens),tokens)
                    return parseBinExpr(left,operator,priorityRight)
                else:
                    newLeft = parseBinExpr(left,operator,right)
                    return parseRec(newLeft,nextOperator,nextRight,tokens)

            except: # no next right token available
                nextRightToken = nextOperator # this must be uni, as it's the final token
                return parseBinExpr(left,operator,parseUniExpr(nextRightToken)) # end of the recursion
        except:
            # doe not take into account invalid expr such as "3 + 3 -"
            return left # apparently there is nothing left to do

def main(args):
    file_name = args[1]
    print("reading from file:"+file_name)
    file = open(file_name,"r")
    source_code = file.read()

    tokens = Lex(source_code)

    for t in tokens:
        print(TokenPrinter.toString(t))

def testExpression():
    expr = "5 + 10 * 2"

    unfilteredTokens = Lex(expr)

    # remove the spaces as they have no meaning atm
    tokens = list(filter(lambda x: x.token_type != TokenType.SPACE, unfilteredTokens)) 
    printTokens(tokens)

    print("pasrsing tokens into expr: \n")
    parsedExpression = ASTExpressionParser.parseExpression(iter(tokens))
    ASTPrinter.print(parsedExpression)

if __name__ == "__main__":
    testExpression()
    #main(argv)

