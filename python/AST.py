from enum import Enum,auto
from tokens import TokenType

class NodeType(Enum):
    CONSTANT = auto()
    UNITARY_OPERATOR = auto()
    BINARY_OPERATOR = auto()
    FUNCTION_CALL = auto()
    FUNCTION_DEFINITION = auto()

class Node:
    def __init__(self,node_type:NodeType):
        self.node_type=node_type

class Number(Node):
    def __init__(self,number: int):
        super().__init__(NodeType.NUMBER)
        self.number=number

class BinaryOperator(Node):
    def __init__(self,operator_type,left_expression, right_expression):
        super().__init__(NodeType.BINARY_OPERATOR)
        self.operator_type= operator_type
        self.left_expression=left_expression
        self.right_expression=right_expression

class BinaryOperatorType(Enum):
        SMALLER_THEN = auto()
        GREATER_THEN = auto()
        EQUALS = auto()
        MINUS = auto()
        PLUS = auto()
    
class TokenToOperator:
    binary_operator ={
        TokenType.SMALLER_THEN : BinaryOperatorType.SMALLER_THEN,
        TokenType.GREATER_THEN : BinaryOperatorType.GREATER_THEN,
        TokenType.EQUALS : BinaryOperatorType.EQUALS,
        TokenType.MINUS : BinaryOperatorType.MINUS,
        TokenType.PLUS : BinaryOperatorType.PLUS
    }

class ASTPrinter:
    """
    Prints out the ast in S-expressions
    """
    def print(ast):
        ...
