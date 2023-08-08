
/* scanner .h*/
#ifndef __SCANNER__H
#define __SCANNER__H
#define _CRT_SECURE_NO_WARNINGS
#include "FileDescriptor.cpp"


//Token types enumeration
typedef enum
{
	/* Literals */
	lx_identifier, lx_integer, lx_string, lx_float, lex_char, lex_double,
	/* Keywords */
	kw_program,
	kw_var, kw_constant, kw_integer, kw_boolean, kw_string, kw_float,
	kw_true, kw_false, kw_if, kw_fi, kw_then, kw_else,
	kw_while, kw_do, kw_od,
	kw_and, kw_or,
	kw_read, kw_write,
	kw_for, kw_from, kw_to, kw_by,
	kw_function, kw_procedure, kw_return, kw_not, kw_begin, kw_end,
	/* Operators */
	lx_lparen, lx_rparen, lx_lbracket, lx_rbracket,
	lx_colon, lx_dot, lx_semicolon, lx_comma, lx_colon_eq,
	lx_plus, lx_minus, lx_star, lx_slash,
	lx_eq, lx_neq, lx_lt, lx_le, lx_gt, lx_ge, lx_eof
}
LEXEME_TYPE;

// Definition of TOKEN, you can use a clase if you  want 
struct	TOKEN
{
	LEXEME_TYPE type;
	int value;  // can be used instead of the str_ptr for IDs and strings
	float float_value;

	char* str_ptr; // points to strings or Identifiers, can use value
	//instead but with type casting
};

class SCANNER {
private:
	
	// define your functions ..
	//I would define the following functions..
	void skip_comments();
	TOKEN* check_keyword(const char* word);
	TOKEN* get_id(const char* word);
	TOKEN* get_op(const char* word);
	TOKEN* get_string(const char* word);
	TOKEN* get_int(const char* word); // gets integers and floats

public:
	FileDescriptor* Fd;
	SCANNER();
	SCANNER(FileDescriptor* fd); //{ Fd = fd; /* write other code if needed*/ };
	TOKEN* Scan(FileDescriptor* fd);
	TOKEN* Scan();
	TOKEN* get_token(const char* word);
	void skip_whitespace();
	// You may define it as TOKEN *Scan(FileDescriptor *fd);

};
extern int keys; /* number of keywords */
extern const char* keyword[];

extern LEXEME_TYPE key_type[];



extern const char* operator_list[];

// Array for corresponding operator types
extern LEXEME_TYPE operator_type[];


extern int operators;

// It is helpful to define the following arrays in your .cpp file
// not in this header file

/* The keyword list is and must be kept in alphabetical order. To
add a keyword, insert it at the appropriate place in keyword[],
insert its type in the appropriate place in key type[], and
increment the var keys. Order is needed for binary search
*/


#endif




