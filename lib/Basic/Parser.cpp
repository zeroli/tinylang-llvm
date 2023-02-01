#include "tinylang/Basic/Parser.h"

AST* Parser::parse() {
    AST* Res = parseCalc();
    expect(Token::eoi);
    return Res;
}

// calc: ("with" ident (, ident)* ":") ? expr;
AST* Parser::parseCalc() {
    Expr* E;
    llvm::SmallVector<llvm::StringRef, 8> Vars;

    if (Tok.is(Token::KW_with)){
        advance();

        if (expect(Token::ident)) 
            goto _error;
        Vars.push_back(Tok.getText());
        advance();

        // more identifier?
        while (Tok.is(Token::comma)) {
            advance();
            if (expect(Token::ident))
                goto _error;
            Vars.push_back(Tok.getText());
            advance();
        }
        if (consume(Token::colon))
            goto _error;
    }
    E = parseExpr();
    if (Vars.empty()) {
        return E;
    } else {
        return new WithDecl(Vars, E);
    }
_error:
    while (!Tok.is(Token::eoi))
        advance();
    return nullptr;
}

// expr: term (("+" | "-") term)*
Expr* Parser::parseExpr() {
    Expr* Left = parseTerm();
    while (Tok.isOneOf(Token::plus, Token::minus)) {
        BinaryOp::Operator Op =
            Tok.is(Token::plus) ? BinaryOp::Plus :
                                    BinaryOp::Minus;
        advance();
        Expr* Right = parseTerm();
        Left = new BinaryOp(Op, Left, Right);
    }
    return Left;
}

// term: factor (("*" | "/") factor)*
Expr* Parser::parseTerm() {
    Expr* Left = parseFactor();
    while (Tok.isOneOf(Token::star, Token::slash)) {
        BinaryOp::Operator Op =
            Tok.is(Token::star) ? BinaryOp::Mul:
                                    BinaryOp::Div;
        advance();
        Expr* Right = parseFactor();
        Left = new BinaryOp(Op, Left, Right);
    }
    return Left;
}

Expr* Parser::parseFactor() {
    Expr* Res = nullptr;
    switch (Tok.getKind()) {
    case Token::number: {
        Res = new Factor(Factor::Number, Tok.getText());
        advance();
        break;
    }
    case Token::ident: {
        Res = new Factor(Factor::Ident, Tok.getText());
        advance();
        break;
    }
    case Token::l_paren: {
        advance();
        Res = parseExpr();
        if (!consume(Token::r_paren)) break;
        // fall through
    }
    default: {
        if (!Res) error();
        while (!Tok.isOneOf(Token::r_paren, Token::star,
                            Token::plus, Token::minus,
                            Token::slash, Token::eoi)) {
            advance();
        }
        break;
    }
    }  // end of switch
    return Res;
}