from tokens import *

class TokenPrinter:
    tokenName = {
        TokenType.SPACE : "SPACE",
        TokenType.DEF : "DEF",
        TokenType.NUMBER : "NUMBER",
        TokenType.WORD : "WORD",
        TokenType.HASH : "HASH",
        TokenType.EXTERN : "EXTERN",
        TokenType.SMALLER_THEN : "SMALLER_THEN",
        TokenType.GREATER_THEN : "GREATER_THEN",
        TokenType.EQUALS : "EQUALS",
        TokenType.OPEN_BRACKET : "OPEN_BRACKET",
        TokenType.CLOSED_BRACKET : "CLOSED_BRACKET",
        TokenType.IF : "IF",
        TokenType.ELSE : "ELSE",
        TokenType.THEN : "THEN",
        TokenType.MINUS : "MINUS",
        TokenType.PLUS : "PLUS",
        TokenType.EOL : "EOL",
        TokenType.SINGLE_QUOTE : "SINGLE_QUOTE",
        TokenType.POINT : "POINT",
        TokenType.COMMA : "COMMA"
        }

    def toStringPosition(position:Position):
        return "[file name: "+position.file_name \
            +";line number: "+str(position.line_number) \
            +";position on line:"+str(position.position_on_line)+";]"

    def toString(token:Token):
        result = "Type="+TokenPrinter.tokenName[token.token_type]
        result = result + " " + TokenPrinter.toStringPosition(token.position)
        if(token.token_type==TokenType.WORD):
            result = result + " word="+token.word
        if(token.token_type==TokenType.NUMBER):
            result = result + " number="+str(token.number)
        return result


