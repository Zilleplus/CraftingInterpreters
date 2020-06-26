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
        NodeType.NUMBER : "number",
        NodeType.UNITARY_OPERATOR : "untary operator",
        NodeType.BINARY_OPERATOR : "binary operator",
        NodeType.FUNCTION_CALL : "function call",
        NodeType.FUNCTION_DEFINITION : "function definition"
    }

    def getDetailsNode(node):
        """
        input:
            node
        returns string with details of the node
        """
        ...

    def createBinExpression(operator_type,leftSExpr,rightSExpr):
        expr = "("
        expr = expr + leftSExpr + " "
        expr = expr + ASTPrinter.BinaryOperatorName[operator_type] + " "
        expr = expr + rightSExpr + " "  
        return expr + ")"

    def astToString(node):
        """ 
        input:
            ast node
        return string representing ast from this node on
        """ 

        if(node.node_type == NodeType.BINARY_OPERATOR):
            return ASTPrinter.createBinExpression(node.operator_type,\
                ASTPrinter.astToString(node.left_expression),\
                ASTPrinter.astToString(node.right_expression))

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
        print(ASTPrinter.astToString(ast))

