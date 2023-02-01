#ifndef PARSER_H
#define PARSER_H

#include "AST.h"
#include "Lexer.h"

#include "llvm/Support/raw_ostream.h"

class Parser {
    Lexer& Lex;
    Token Tok;  // next token
    bool HasError;

private:
    void error() {
        llvm::errs() << "Unexcepted: " << Tok.getText()
                    << "\n";
        HasError = true;
    }

    void advance() { Lex.next(Tok); }

    bool expect(Token::TokenKind Kind) {
        if (Tok.getKind() != Kind) {
            error();
            return true;
        }
        return false;
    }

    bool consume(Token::TokenKind Kind) {
        if (expect(Kind))
            return true;
        advance();
        return false;
    }

    /*
    example: with a,b: a * (b + 4)
    the grammer rule as below:
    calc: ("with" ident (, ident)* ":") ? expr;
    expr: term (("+" | "-") term)*
    term: factor (("*" | "/") factor)*
    factor: ident | number | "(" expr ")"
    ident: ([a-zA-Z])+
    number: ([0-9])+
    */
    AST* parseCalc();
    Expr* parseExpr();
    Expr* parseTerm();
    Expr* parseFactor();

public:
    Parser(Lexer& Lex) : Lex(Lex), HasError(false) {
        advance();
    }

    bool hasError() { return HasError; }

    AST* parse();
};

#endif  // PARSER_H
