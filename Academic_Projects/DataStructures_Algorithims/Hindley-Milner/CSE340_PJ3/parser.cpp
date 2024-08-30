/*
 * Created by: Joshua Elkins
 * Date: June 24th, 2023
 */

#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>
#include <stdlib.h>
#include <string.h>


#include "lexer.hpp"
#include "inputbuf.hpp"
#include "parser.hpp"

using namespace std;


//primary types:
// 1 = integer (NUM)
// 2 = realnum (REAL)
// 3 = boolean (BOO)

LexicalAnalyzer lexer;
Token token;
TokenType tempTokenType;
int binNumCount = 4;


struct sTableEntry {
    string name;
    int lineNo;
    int type;
    bool printed;
};

struct sTable {
    sTableEntry* item;
    sTable *prev;
    sTable *next;
};

sTable* symbolTable;

void addList(string name, int line, int type) {
    if(symbolTable == NULL) {
        sTable* newEntry = new sTable();
        sTableEntry* newItem = new sTableEntry();

        newItem->name = name;
        newItem->lineNo = token.line_no;
        newItem->type = type;
        newItem->printed = false;

        newEntry->item = newItem;
        newEntry->next = NULL;
        newEntry->prev = NULL;

        symbolTable = newEntry;

    }
    else {
        sTable* temp = symbolTable;
        while(temp->next != NULL) {
            temp = temp->next;
        }

        sTable* newEntry = new sTable();
        sTableEntry* newItem = new sTableEntry();

        newItem->name = name;
        newItem->lineNo = token.line_no;
        newItem->type = type;
        newItem->printed = false;

        newEntry->item = newItem;
        newEntry->next = NULL;
        newEntry->prev = temp;
        temp->next = newEntry;
    }
}

//returns the type if found in struct, if not it adds the variable as UNDEFINED
int searchList(string name) {
    sTable* temp = symbolTable;
    bool found = false;
    if (temp == NULL) {
        addList(name, token.line_no, binNumCount);
        binNumCount++;
        return 4;
    }
    else {
        while(temp->next != NULL) {
            if(temp->item->name == name) {
                found = true;
                return(temp->item->type);
            }
            else {
                temp = temp->next;
            }
        }
        if(temp->item->name == name) {
            found = true;
            return(temp->item->type);
        }
        if(!found) {
            addList(name, token.line_no, binNumCount);
            binNumCount++;
            int t = binNumCount-1;
            return(t);
        }
    }
    return 0;
}

void printList() {
    sTable* temp = symbolTable;
    
    while(temp != nullptr) {
        if(temp->item->printed) {
            temp = temp->next;
            continue;
        }
        string outputHolder = "";
        int tempType = temp->item->type;
        if(tempType >= 4) {
            outputHolder += temp->item->name;
            temp->item->printed = true;
            sTable* iter = temp->next;
            while(iter != nullptr) {
                if(iter->item->printed) {
                    iter = iter->next;
                    continue;
                }
                if(iter->item->type == tempType) {
                    outputHolder += ", " + iter->item->name;
                    iter->item->printed = true;
                }
                iter = iter->next;
            }
            outputHolder += ": ? #";
            cout << outputHolder << endl;
            temp = symbolTable;
        } else if(tempType <= 3) {
            string varType = (tempType == 1) ? "int" : (tempType == 2) ? "real" : "bool";
            outputHolder = temp->item->name + ": " + varType + " #";
            temp->item->printed = true; // add cout statement here???
            cout << outputHolder << endl;

            sTable* iter = temp->next;

            while(iter != nullptr && iter->item->type == tempType) {
                if(iter->item->printed) {
                    iter = iter->next;
                    continue;
                }
                varType = (iter->item->type == 1) ? "int" : (iter->item->type == 2) ? "real" : "bool";
                outputHolder = iter->item->name + ": " + varType + " #";
                cout << outputHolder << endl;
                iter->item->printed = true;
                iter = iter->next;
            }
        }

        if(!outputHolder.empty()) {
            //cout << output << endl;
            outputHolder = "";
        }
        temp = temp->next;
    }
}

int parse_varlist(void) {
    token = lexer.GetToken();
    int tempI;
    char* lexeme = (char*)malloc(sizeof(token.lexeme)+1);
    memcpy(lexeme, (token.lexeme).c_str(), (token.lexeme).size()+1);
    searchList(lexeme);

    if(token.token_type == ID) {
        token = lexer.GetToken();
        if(token.token_type == COMMA) {
            //cout << "\n Rule Parsed: var_list -> ID COMMA var_list \n";
            tempI = parse_varlist();
        }
        else if(token.token_type == COLON) {
            tempTokenType = lexer.UngetToken(token);
            //cout << "\n Rule Parsed: var_list -> ID \n";
        }
        else {
            cout << "\n Syntax Error \n";
        }
    }
    else {
        cout << "\n Syntax Error \n";
    }
    return 0;
}

int parse_body(void);

int parse_unaryOperator(void) {
    token = lexer.GetToken();

    if(token.token_type == NOT) {
        //cout << "\n Rule parsed: unary_operator -> NOT";
        return 3;
    }
    else{
        cout << "\n Syntax Error \n";
        return 0;
    }
}

//returns the integer that corresponds to TokenType enum
int parse_binaryOperator(void) {
    token = lexer.GetToken();
    if(token.token_type == PLUS) {
        //cout << "\n Rule parsed: binary_operator -> PLUS\n";
        return 15;
    }
    else if(token.token_type == MINUS) {
        //cout << "\n Rule parsed: binary_operator -> MINUS \n";
        return 16;
    }
    else if(token.token_type == MULT) {
        //cout << "\n Rule parsed: binary_operator -> MULT\n";
        return 17;
    }
    else if(token.token_type == DIV) {
        //cout << "\n Rule parsed: binary_operator -> DIV \n";
        return 18;
    }
    else if(token.token_type == GREATER) {
        //cout << "\n Rule parsed: binary_operator -> GREATER \n";
        return 20;
    }
    else if(token.token_type == LESS){
        //cout << "\n Rule parsed: binary_operator -> LESS\n";
        return 23;
    }
    else if(token.token_type == GTEQ){
        //cout << "\n Rule parsed: binary_operator -> GTEQ \n";
        return 19;
    }
    else if(token.token_type == LTEQ){
        //cout << "\n Rule parsed: binary_operator -> LTEQ\n";
        return 21;
    }
    else if(token.token_type == EQUAL) {
        //cout << "\n Rule parsed: binary_operator -> EQUAL \n";
        return 26;
    }
    else if(token.token_type == NOTEQUAL) {
        //cout << "\n Rule parsed: binary_operator -> NOTEQUAL \n";
        return 22;
    }
    else {
        cout << "\n Syntax Error \n";
        return(0); // WAS -1
    }
}

//returns number corresponding to the 3 given binNo
int parse_primary(void) {
    token = lexer.GetToken();
    if(token.token_type == ID ) {
        return(searchList(token.lexeme));
        //cout << "\n Rule parsed: primary -> ID\n";
    }
    else if(token.token_type == NUM ) {
        //cout << "\n Rule parsed: primary -> NUM \n";
        return 1;
    }
    else if(token.token_type == REALNUM) {
        //cout << "\n Rule parsed: primary -> REALNUM\n";
        return 2;
    }
    else if(token.token_type == TR) {
        //cout << "\n Rule parsed: primary -> TRUE \n";
        return 3;
    }
    else if(token.token_type == FA) {
        //cout << "\n Rule parsed: primary -> FALSE \n";
        return 3;
    }
    else {
        cout << "\n Syntax Error \n";
        return 0;
    }
}

void tpyeUpdate(int currType, int newType) {
    sTable* table = symbolTable;

    while(table->next != NULL) {
        if(table->item->type == currType) {
            table->item->type = newType;
        }
        table = table->next;
    }
    if(table->item->type == currType) {
        table->item->type = newType;
    }
}

int parse_expression(void) {
    int tempI;
    int leftBinNo;
    int rightBinNo;
    token = lexer.GetToken();
    if(token.token_type == ID || token.token_type == NUM || token.token_type == REALNUM || token.token_type == TR || token.token_type == FA ) {
        //cout << "\n Rule parsed: expression -> primary \n";
        lexer.UngetToken(token);
        tempI = parse_primary();
    }
    else if(token.token_type == PLUS || token.token_type == MINUS || token.token_type == MULT || token.token_type == DIV || token.token_type == GREATER || token.token_type == LESS || token.token_type == GTEQ || token.token_type == LTEQ || token.token_type == EQUAL || token.token_type == NOTEQUAL) {
        //cout << "\n Rule parsed: expression -> binary_operator expression expression \n";
        tempTokenType = lexer.UngetToken(token);
        tempI = parse_binaryOperator();
        // in searchList and addList add an input argument that is the binNo
        // in each ID for which you do addList or searchList just update binNo.
        // if plus minus multiply divide
        leftBinNo = parse_expression();
        rightBinNo = parse_expression();
        //if(tempI1 != tempI2) type mismatch token.lineno C2 (this is not true if tempI1 == -1 or tempI2 == -1 )

        // both tempI1 and tempI2 are -1
        // is lType -1? if so then do nothing
        // if not then it implies that lType is 0 1 or 2. Then check if parser_expression returned an ID or a INT NUm or REAL.
        // if parse expression returned from INT NUM REAL do nothing
        // else if it returned from an ID, then searchList for the ID update the type of ID to lType. --> search for all IDs which have binNo same as binNo of the current ID
        // for each such ID if its type is -1 then change their types to lType, if not type mismatch token.line_no C2

        // if only one of tempI1 or tempI2 is -1
        // if tempI1 is -1, did tempI1 return from ID? if so then searchList for ID and make its type = tempI2 --> search for all IDs which have binNo same as binNo of the current ID
        // for each such ID if its type is -1 then change their types to lType, if not type mismatch token.line_no C2
        if((tempI != PLUS && tempI != MINUS && tempI != MULT && tempI != DIV && tempI != GREATER && tempI != LESS && tempI != GTEQ && tempI != LTEQ && tempI != EQUAL && tempI != NOTEQUAL) ||  leftBinNo != rightBinNo) {
          // if(tempI != PLUS && tempI != MINUS && tempI != MULT && tempI != DIV && tempI != GREATER && tempI != LESS && tempI != GTEQ && tempI != LTEQ && tempI != EQUAL && tempI != NOTEQUAL)
           // {
                if(tempI == PLUS || tempI == MINUS || tempI == MULT || tempI == DIV) {
                    if((leftBinNo == 1 || leftBinNo == 2) && rightBinNo >= 4) {
                        tpyeUpdate(rightBinNo, leftBinNo);
                        rightBinNo = leftBinNo;
                    }
                    else if(leftBinNo >= 4 && (rightBinNo == 1 || rightBinNo == 2)) {
                        tpyeUpdate(leftBinNo, rightBinNo);
                        leftBinNo = rightBinNo;
                    }
                    else if(leftBinNo >= 4 && rightBinNo >= 4) {
                        tpyeUpdate(rightBinNo, leftBinNo);
                        rightBinNo = leftBinNo;
                    }
                    
                    else if(leftBinNo == 3 && rightBinNo >= 4) {
                        tpyeUpdate(rightBinNo, leftBinNo);
                        rightBinNo = leftBinNo;
                    }
                    
                    else if(rightBinNo == 3 && leftBinNo >= 4){
                        tpyeUpdate(leftBinNo, rightBinNo);
                        leftBinNo = rightBinNo;
                    }
                    
                    else {
                        cout << "TYPE MISMATCH " << token.line_no << " C2"<<endl;
                        exit(1);
                    }
                //}//
            }
            else if(tempI == GREATER || tempI == LESS || tempI == GTEQ || tempI == LTEQ || tempI == NOTEQUAL || tempI == EQUAL) {
                if((rightBinNo >= 4 && leftBinNo >= 4)) {
                    tpyeUpdate(leftBinNo, rightBinNo);
                    leftBinNo = rightBinNo;
                    return 3;
                }
                
                else if(rightBinNo >= 4 && leftBinNo == 3) {
                    tpyeUpdate(rightBinNo, leftBinNo);
                    rightBinNo = leftBinNo;
                    return 3;
                }
                
                else if(rightBinNo == 3 && leftBinNo >= 4){
                    tpyeUpdate(leftBinNo, rightBinNo);
                    leftBinNo = rightBinNo;
                    return 3;
                }
                else {
                    cout << "TYPE MISMATCH " << token.line_no << " C2"<<endl;
                    exit(1);
                }
            }
            else {
                cout << "TYPE MISMATCH " << token.line_no << " C2"<<endl;
                exit(1);
            }
        } // end of if temp !=....
        if(tempI == GREATER || tempI == LESS || tempI == GTEQ || tempI == LTEQ || tempI == EQUAL ) {
            tempI = 3;
        }
        else {
            tempI = rightBinNo;
        }
    }
    else if(token.token_type == NOT) {
        //cout << "\n Rule parsed: expression -> unary_operator expression \n";
       // tempTokenType = lexer.UngetToken(token);
      //  lexer.UngetToken(token);
       tempI = parse_expression();
        if(tempI < 3) {
            cout << "TYPE MISMATCH " << token.line_no << " C3"<<endl;
            exit(1);
        }
        else if(tempI >= 4) {
            tpyeUpdate(rightBinNo, leftBinNo);          //updated this last (swapped left and right)
            rightBinNo = leftBinNo;
            return 3;
        }
    }
    else {
        cout << "\n Syntax Error \n";
        return 0;
    }
return tempI;
}



int parse_assstmt(void) {
   // int tempI;
    string name;
    int leftBinNo;
    int rightBinNo;
    token = lexer.GetToken();
    if(token.token_type == ID) {
        leftBinNo = searchList(token.lexeme);
        token = lexer.GetToken();
        if(token.token_type == EQUAL) {
            token = lexer.GetToken();
            if(token.token_type == ID || token.token_type == NUM || token.token_type == REALNUM || token.token_type == TR || token.token_type == FA || token.token_type == PLUS || token.token_type == MINUS || token.token_type == MULT || token.token_type == DIV || token.token_type == LESS || token.token_type == GREATER || token.token_type == GTEQ || token.token_type == LTEQ || token.token_type == EQUAL || token.token_type == NOTEQUAL || token.token_type == NOT) {
                lexer.UngetToken(token);
                rightBinNo = parse_expression();
                //rType right type of an assigment tempI.
                //check for C1. if ltype == rtype then fine if not then TYPE MISMATCH token.line_no C1
                // if any one of lType or rType is -1 then should not throw type mismatch.
                // if lType != -1 && rType is -1 then you search for left ID token to extract its type. searchList should return type.
                // you have to use search list again with the right token to update the right token's type to lType
                
                if(leftBinNo == rightBinNo) {
                    return 0;
                }
                
                else if(leftBinNo == 1 || leftBinNo == 2){
                    cout << "TYPE MISMATCH " << token.line_no << " C1" << endl;
                    exit(1);
                }
                
                else if (leftBinNo == 3){
                    tpyeUpdate(rightBinNo,leftBinNo);
                    rightBinNo = leftBinNo;
                }
                
                else if(rightBinNo == 3){
                    tpyeUpdate(leftBinNo, 3);
                    leftBinNo = rightBinNo;
                }

                else {
                    tpyeUpdate(leftBinNo,3);
                    tpyeUpdate(rightBinNo, 3);
                    leftBinNo = 3;
                    rightBinNo = 3;
                }
                token = lexer.GetToken();
                if(token.token_type == SEMICOLON) {
                    //cout << "\n Rule parsed: assignment_stmt -> ID EQUAL expression SEMICOLON"<<endl
                }
                else {
                    cout << "\n Syntax Error \n";
                }
            }
            else {
                cout << "\n Syntax Error \n";
            }
        }
        else {
            cout << "\n Syntax Error \n";
        }
    }
    else {
        cout << "\n Syntax Error \n";
    }
    return 0;
}

int parse_case(void) {
    int tempI;
    token = lexer.GetToken();
    if(token.token_type == CASE ) {
        token = lexer.GetToken();
        if(token.token_type == NUM) {
            token = lexer.GetToken();
            if(token.token_type == COLON) {
                //cout << "\n Rule parsed: case -> CASE NUM COLON body";
                tempI = parse_body();
            }
            else {
                cout << "\n Syntax Error \n";
            }
        }
        else {
            cout << "\n Syntax Error \n";
        }
    }
    else {
        cout << "\n Syntax Error \n";
    }
    
    return 0;
}

int parse_caselist(void) {
    int tempI;
    token = lexer.GetToken();
    if(token.token_type == CASE) {
        tempTokenType = lexer.UngetToken(token);
        tempI = parse_case();
        token = lexer.GetToken();
        if(token.token_type == CASE) {
            tempTokenType = lexer.UngetToken(token);
            //cout << "\n Rule parsed: case_list -> case case_list \n";
            tempI = parse_caselist();
        }
        else if(token.token_type == RBRACE) {
            tempTokenType = lexer.UngetToken(token);
            //cout << "\n Rule parsed: case_list -> case  \n";
        }
    }
    return 0;
}

int parse_switchstmt(void) {
    int tempI;
    Token token1;
    token = lexer.GetToken();
    if(token.token_type == SWITCH) {
        token = lexer.GetToken();
        if(token.token_type == LPAREN) {
            tempI = parse_expression();
            // if tempI != INT then throw type error
            // else if tempI = -1 ==> parse_expresssion retunred an ID, then go and change using searchList the type of ID to 1.
            if(tempI != 1 && tempI <= 3) {
                cout<< "TYPE MISMATCH " << token.line_no << " C5"<<endl;
                exit(1);
            }
            else if(tempI >= 4){
                //token = lexer.GetToken();
               // token1 = lexer.GetToken();
               int temp = searchList(token.lexeme);
                if(temp != 1) {
                    tpyeUpdate(temp, 1);
                }
               // lexer.UngetToken(token1);
               // lexer.UngetToken(token);
            }
            token = lexer.GetToken();
            if(token.token_type == RPAREN) {
                token = lexer.GetToken();
                if(token.token_type == LBRACE) {
                    tempI = parse_caselist();
                    token = lexer.GetToken();
                    if(token.token_type == RBRACE) {
                        //cout << "\n Rule parsed: switch_stmt -> SWITCH LPAREN expression RPAREN LBRACE case_list RBRACE \n";
                    }
                    else {
                        cout << "\n Syntax Error \n";
                    }
                }
                else {
                    cout << "\n Syntax Error \n";
                }
            }
            else {
                cout << "\n Syntax Error \n";
            }
        }
        else {
            cout << "\n Syntax Error \n";
        }
    }
    else {
        cout << "\n Syntax Error \n";
    }
    return 0;
}

int parse_whilestmt(void) {
    int tempI;

    token = lexer.GetToken();
    if(token.token_type == WHILE) {
        token = lexer.GetToken();
        if(token.token_type == LPAREN) {
            tempI = parse_expression();
            // if tempI != bool then throw type error
            // else if tempI = -1 ==> parse_expresssion retunred an ID, then go and change using searchList the type of ID to 2.
            if(tempI < 3) {
                cout<< "TYPE MISMATCH " << token.line_no << " C4" << endl;
                exit(1);
            }
            
            else if(tempI >= 4) {
                int temp = searchList(token.lexeme);
                if(temp != 3){
                    tpyeUpdate(temp, 3);
                }
                
            }
            token = lexer.GetToken();
            if(token.token_type == RPAREN) {
                //cout << "\n Rule parsed: whilestmt -> WHILE LPAREN expression RPAREN body \n";
                tempI = parse_body();
            }
            else {
                cout << "\n Syntax Error \n";
            }
        }
        else {
            cout << "\n Syntax Error \n";
        }
    }
    else {
        cout << "\n Syntax Error \n";
    }
    return 0;
}

int parse_ifstmt(void) {
    int tempI;
    token = lexer.GetToken();
    if(token.token_type == IF) {
        token = lexer.GetToken();
        if(token.token_type == LPAREN) {
            tempI = parse_expression();
            // if tempI != bool then throw type error
            // else if tempI = -1 ==> parse_expresssion retunred an ID, then go and change using searchList the type of ID to 2.
            if(tempI < 3) {
                cout<< "TYPE MISMATCH " << token.line_no << " C4" << endl;
                exit(1);
            }
            
            else if(tempI >= 4){
                int temp = searchList(token.lexeme);
                if(temp != 3) {
                    tpyeUpdate(temp, 3);
                }
                
            }
            token = lexer.GetToken();
            if(token.token_type == RPAREN) {
                //cout << "\n Rule parsed: ifstmt -> IF LPAREN expression RPAREN body \n";
                tempI = parse_body();
            }
            else {
                cout << "\n Syntax Error \n";
            }
        }
        else {
            cout << "\n Syntax Error \n";
        }
    }
    else {
        cout << "\n Syntax Error \n";
    }
    return 0;
}

int parse_stmt(void) {
    int tempI;
    token = lexer.GetToken();
    if(token.token_type == ID) {
        tempTokenType = lexer.UngetToken(token);
        //cout << "\n Rule parsed: stmt -> assignment_stmt \n";
        tempI = parse_assstmt();
    }
    else if(token.token_type == IF) {
        tempTokenType = lexer.UngetToken(token);
        //cout << "\n Rule parsed: stmt -> if_stmt";
        tempI = parse_ifstmt();
    }
    else if(token.token_type == WHILE) {
        tempTokenType = lexer.UngetToken(token);
        //cout << "\n Rule parsed: stmt -> while_stmt";
        tempI = parse_whilestmt();
    }
    else if(token.token_type == SWITCH) {
        tempTokenType = lexer.UngetToken(token);
        //cout << "\n Rule parsed: stmt -> switch_stmt";
        tempI = parse_switchstmt();
    }
    else{
        cout << "\n Syntax Error \n";
    }
    return 0;
}

int parse_stmtlist(void) {
    token = lexer.GetToken();
    int tempI;
    if(token.token_type == ID || token.token_type == IF || token.token_type == WHILE || token.token_type == SWITCH) {
        tempTokenType = lexer.UngetToken(token);
        tempI = parse_stmt();
        token = lexer.GetToken();
        if(token.token_type == ID || token.token_type == IF || token.token_type == WHILE || token.token_type == SWITCH) {
            tempTokenType = lexer.UngetToken(token);
            //cout << "\n Rule Parsed: stmt_list -> stmt stmt_list \n";
            tempI = parse_stmtlist();
        }
        else if (token.token_type == RBRACE) {
            tempTokenType = lexer.UngetToken(token);
            //cout << "\n Rule parsed: stmt_list -> stmt \n";
        }
    }
    else {
        cout << "\n Syntax Error \n";
    }
    return(0);
}

int parse_body(void) {
    token = lexer.GetToken();
    int tempI;
    if(token.token_type == LBRACE) {
        //cout << "\n Rule Parsed: scope -> ID LBRACE public_vars private_vars stmt_list RBRACE \n";
        tempI = parse_stmtlist();
        token = lexer.GetToken();
        if(token.token_type == RBRACE) {
            //cout << "\n Rule parsed: body -> LBRACE stmt_list RBRACE \n";
            return 0;
        }
        else {
            cout << "\n Syntax Error \n ";
            return 0;
        }
    }
    else if(token.token_type == END_OF_FILE) {
        tempTokenType = lexer.UngetToken(token);
        return 0;
    }
    else {
        cout << "\n Syntax Error \n ";
        return 0;
    }
}

int parse_typename(void) {
    token = lexer.GetToken();
    if(token.token_type == INT || token.token_type == REAL || token.token_type == BOO) {
        sTable* table = symbolTable;
        while(table->next != NULL) {
            if(table->item->lineNo == token.line_no) {
                table->item->type = token.token_type;
            }
            table = table->next;
        }
        if(table->item->lineNo == token.line_no) {
            table->item->type = token.token_type;
        }
    }
    else {
        cout << "\n Syntax Error \n";
    }
    return 0;
}

int parse_vardecl(void) {
    int tempI;
    token = lexer.GetToken();
    if(token.token_type == ID) {
        tempTokenType = lexer.UngetToken(token);
        tempI = parse_varlist();
        token = lexer.GetToken();
        if(token.token_type == COLON) {
            tempI = parse_typename();
            //use the searchList to update the types of variables that are already in the symbolTable
            token = lexer.GetToken();
            if(token.token_type == SEMICOLON) {
                //cout << "\n Rule parsed: var_decl -> var_list COLON type_name SEMICOLON"<<endl;
            }
            else {
                cout << "\n Syntax Error \n";
            }
        }
        else {
            cout << "\n Syntax Error \n";
        }
    }
    else {
        cout << "\n Syntax Error \n";
    }
    return 0;
}

int parse_vardecllist(void) {
    int tempI;
    token = lexer.GetToken();
    while(token.token_type == ID) {
        tempTokenType = lexer.UngetToken(token);
        tempI = parse_vardecl();
        token = lexer.GetToken();
        if(token.token_type != ID) {
            //cout << "\n Rule Parsed: var_decl_list -> var_decl \n";
        }
        else {
            //cout << "\n Rule Parsed: var_decl_list -> var_decl var_decl_list \n";
        }
    }
    tempTokenType = lexer.UngetToken(token);
    return 0;
}

string global = "::";
int parse_globalVars(void) {
    token = lexer.GetToken();
    int tempI;
    
    if(token.token_type == ID) {
        tempTokenType = lexer.UngetToken(token);
        //cout << "\n Rule parsed: globalVars -> var_decl_list \n";
        tempI = parse_vardecllist();
    }
    else{
        cout << "Syntax Error \n";
    }
    return 0;
}

int parse_program(void) {
    token = lexer.GetToken();
    int tempI;
    while (token.token_type != END_OF_FILE) {
        if(token.token_type == ID) {
            tempTokenType = lexer.UngetToken(token);
            //cout << "\n Rule parsed: program -> global_vars scope \n";
            tempI = parse_globalVars();
            tempI = parse_body();
        }
        else if(token.token_type == LBRACE) {
            tempTokenType = lexer.UngetToken(token);
            //cout << "\n Rule parsed: global_vars -> epsilon \n";
            tempI = parse_body();
        }
        else if(token.token_type == END_OF_FILE) {
            return 0;
        }
        else {
            cout << "\n Syntax Error \n";
            return 0;
        }
        token = lexer.GetToken();
    }
    return 0;
}


int main() {
    int start;
    start = parse_program();
    printList();
    //cout << "\n End of Program \n";
   
}
