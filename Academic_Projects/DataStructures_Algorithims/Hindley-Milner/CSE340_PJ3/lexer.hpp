/*
 * Copyright (C) Rida Bazzi, 2016
 * Edited by Joshua Elkins, 2023
 *
 * Do not share this file with anyone
 */
#ifndef __LEXER__H__
#define __LEXER__H__

#include <vector>
#include <string>

#include "inputbuf.hpp"

// ------- token types -------------------

typedef enum { END_OF_FILE = 0, INT, REAL,
   BOO, TR, FA, IF, WHILE, SWITCH, CASE,
   PUBLIC, PRIVATE, NUM, REALNUM, NOT, PLUS,
   MINUS, MULT, DIV, GTEQ, GREATER, LTEQ,
   NOTEQUAL, LESS, LPAREN, RPAREN, EQUAL,
   COLON, COMMA, SEMICOLON, LBRACE,
   RBRACE, ID, ERROR
} TokenType;

class Token {
  public:
    void Print();

    std::string lexeme;
    TokenType token_type;
    int line_no;
};

class LexicalAnalyzer {
  public:
    Token GetToken();
    TokenType UngetToken(Token);
    LexicalAnalyzer();

  private:
    std::vector<Token> tokens;
    int line_no;
    Token tmp;
    InputBuffer input;

    bool SkipSpace();
    bool SkipComments();
    bool IsKeyword(std::string);
    TokenType FindKeywordIndex(std::string);
    Token ScanIdOrKeyword();
    Token ScanNumber();
};

#endif  //__LEXER__H__

