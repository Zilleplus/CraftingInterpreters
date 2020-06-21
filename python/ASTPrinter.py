from AST import *

class ASTPrinter:
    BinaryOperatorName = {
        BinaryOperatorType.SMALLER_THEN : "<",
        BinaryOperatorType.GREATER_THEN : ">",
        BinaryOperatorType.EQUALS : "=",
        BinaryOperatorType.MINUS : "-",
        BinaryOperatorType.PLUS : "+",
        BinaryOperatorType.ASTERIX : "*"
    }

    NodeName = {
        NUMBER : "number",
        UNITARY_OPERATOR : "untary operator",
        BINARY_OPERATOR : "binary operator",
        FUNCTION_CALL : "function call",
        FUNCTION_DEFINITION : "function definition"
    }

    def getDetailsNode(node):
        """
        input:
            node
        returns string with details of the node
        """
        ...

    def createBinExpression(node_type,leftSExpr,rightSExpr):
        expr = ""
        expr = expr + leftSExpr + " "
        expr = expr + ASTPrinter.BinaryOperatorName[node.node_type] + " "
        expr = expr + rightSExpr + " "  
        return expr

    def astToString(ast):
        if(node.node_type == NodeType.BINARY_OPERATOR):
            return createBinExpression(node.operator_type,\
                astToString(node.left_expression),\
                astToString(node.right_expression)

        if(node.node_type==NodeType.NUMBER):
            return str(node.number)

        raise Exception("node type not implemented in ASTPrinter")

    def print(ast):
        """
        Prints out the ast in S-expressions
        -> only do expressions atm, cuz i haven't gotten to the rest yet
        input: 
            ast
        """
        print(astToString(ast))

