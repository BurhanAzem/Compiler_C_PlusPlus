#include "scanner.h"
//#include "FileDescriptor.cpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <String>
using namespace std;
#define _CRT_SECURE_NO_WARNINGS

int keys = 30;
char c_eof = ' ';
const char* keyword[] = {
"and", "begin", "boolean", "by", "constant",
"do", "else", "end", "false", "fi", "float", "for", "from",
"function", "if", "integer", "not", "od", "or", "procedure",
"program", "read", "return", "string", "then", "to", "true",
"var", "while", "write"
};
LEXEME_TYPE key_type[] = {
kw_and, kw_begin, kw_boolean, kw_by, kw_constant,
kw_do, kw_else, kw_end, kw_false, kw_fi,kw_float,
kw_for, kw_from, kw_function, kw_if, kw_integer, kw_not,
kw_od, kw_or, kw_procedure, kw_program,kw_read, kw_return,
kw_string, kw_then, kw_to, kw_true, kw_var, kw_while, kw_write
};
const char* operator_list[] = {
    "(", ")", "[", "]",
    ":", ".", ";", ",", ":=",
    "+", "-", "*", "/",
    "=", "!=", "<", "<=", ">", ">=", ""
};
LEXEME_TYPE operator_type[] = {
    lx_lparen, lx_rparen, lx_lbracket, lx_rbracket,
    lx_colon, lx_dot, lx_semicolon, lx_comma, lx_colon_eq,
    lx_plus, lx_minus, lx_star, lx_slash,
    lx_eq, lx_neq, lx_lt, lx_le, lx_gt, lx_ge, lx_eof
};
int operators = 20;


SCANNER::SCANNER() {
    Fd = nullptr;
}

SCANNER::SCANNER(FileDescriptor* fd) {
    Fd = fd;
    keys = 30;
    operators = 20;
}

void SCANNER::skip_comments() {
    char ch;
    bool hashFlag = false;
    while (1) {
        ch = Fd->readChar();
        if (ch == '#') {
            if (hashFlag) {
                break;
            }
            hashFlag = true;
        }
        else if (ch == '\n' || ch == EOF) {

            break;
        }
        else {
            hashFlag = false;
        }
    }
}

TOKEN* SCANNER::check_keyword(const char* word) {
    if (word == nullptr) {
        printf("Error: Null pointer passed to check_keyword\n");
        return nullptr;
    }

    for (int i = 0; i < keys; ++i) {
        if (strcmp(word, keyword[i]) == 0) {
            TOKEN* newToken = new TOKEN();
            newToken->type = key_type[i];
            return newToken;
        }
    }
    return nullptr;
}

TOKEN* SCANNER::get_id(const char* word) {
    TOKEN* newToken = new TOKEN();
    newToken->type = lx_identifier;
    newToken->str_ptr = new char[strlen(word) + 1];
    strcpy(newToken->str_ptr, word);

    return newToken;
}

TOKEN* SCANNER::get_op(const char* op)
{
    for (int i = 0; i < operators; ++i) {
        if (strcmp(op, operator_list[i]) == 0) {
            TOKEN* newToken = new TOKEN();
            newToken->type = operator_type[i];
            return newToken;
        }
    }
    return nullptr;
}

TOKEN* SCANNER::get_string(const char* word) {
    TOKEN* newToken = new TOKEN();
    newToken->type = lx_string;


    newToken->str_ptr = new char[strlen(word) + 1];
    strcpy(newToken->str_ptr, word);

    return newToken;
}

TOKEN* SCANNER::get_int(const char* word) {
    TOKEN* newToken = new TOKEN();
    newToken->type = lx_integer;


    newToken->value = atoi(word);

    return newToken;
}

TOKEN* SCANNER::Scan(FileDescriptor* fd) {
    Fd = fd;
    if (Fd == nullptr) {
        printf("Error: File descriptor is null.\n");
        return nullptr;
    }
    char c = Fd->readChar();
    if (c_eof == EOF)
        return nullptr;
    if (c == EOF)
        return nullptr;
    if (c == '#') {
        skip_comments();
        return Scan(Fd);  // Call Scan again after skipping the comment
    }

    if (isdigit(c) || (c == '-' && isdigit(Fd->peek()))) {
        std::string num_string = "";
        num_string += c;

        while (isdigit(c = Fd->readChar())) {
            num_string += c;
        }

        if (c == EOF || isspace(c) || strchr("():=+-*/=!<>.;[],{}", c) != NULL) {
            Fd->unget();
            return get_int(num_string.c_str());
        }
        else {
            Fd->reportError("Unexpected character in number");
            return nullptr;
        }
    }
    else if (c == '\"') {
        std::string str = "";

        while ((c = Fd->readChar()) != '\"') {
            if (c == EOF || c == '\n') {
                Fd->reportError("Unexpected EOF or newline in string");
                return nullptr;
            }
            str += c;
        }

        return get_string(str.c_str());
    }
    else if (isalpha(c) || c == '_') {
        std::string word = "";
        word += c;

        while (isalpha(c = Fd->readChar()) || isdigit(c) || c == '_') {
            word += c;
        }

        if (c == EOF || isspace(c) || strchr("():=+-*/=!<>.;[],{}", c) != NULL) {
            Fd->unget();
            TOKEN* keyword_token = check_keyword(word.c_str());
            if (keyword_token != nullptr) {
                return keyword_token;
            }
            else {
                return get_id(word.c_str());
            }
        }
        else {
            Fd->reportError("Unexpected character in identifier/keyword");
            return nullptr;
        }
    }
    else if (strchr("():=+-*/=!<>.;[],{}", c) != NULL) {
        std::string op = "";
        op += c;
        if ((c_eof = Fd->peek()) != EOF && strchr("=:<>", c) != NULL) {
            op += c_eof;
            Fd->readChar(); // Consume the second character of the operator
        }
        return get_op(op.c_str());
    }
    else if (isspace(c)) {
        while ((c = Fd->readChar()) != EOF && isspace(c)) {
            // Just consume the whitespace
        }
        Fd->unget();
        return Scan(Fd);
    }
    else if (c == '\0') {
        return nullptr;
    }
    else {
        Fd->reportError("Unexpected character ---");
        return nullptr;
    }
}

/*void SCANNER::skip_whitespace() {
    char c = Fd->readLine()[0];
    while (isspace(c)) {
        c = Fd->readLine()[0];
    }
    Fd->unget();
}*/




