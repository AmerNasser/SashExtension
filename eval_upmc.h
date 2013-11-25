//
//  eval_upmc.h
//  SashProject
//
//  Created by Amer Nasser on 25/11/13.
//  Copyright (c) 2013 ____. All rights reserved.
//

#ifndef SashProject_eval_upmc_h
#define SashProject_eval_upmc_h



typedef union {
    int terminal;
    struct expression* expr[2];
} Data;

typedef struct expression {
    char op;
    Data data;
} Expr;


/*
typedef struct { 
    
    const char *str; 
    
    int        len;
    
    int        pos;
    
    jmp_buf		err_jmp_buf;
    
    const char *error;
    
    void						*user_data;
    
    parser_variable_callback	variable_cb;
    
    parser_function_callback	function_cb;
} parser_data;*/

char* getExpression(char* arg);

void error_message(const char* string) ;

char eat_char(const char* string, char c) ;

int eat_int(const char* string) ;

Expr* factor(const char* string, Expr* expr) ;

Expr* factor_tail(const char* string, Expr* expr) ;

Expr* term(const char* string, Expr* expr) ;

Expr* term_tail(const char* string, Expr* expr) ;

Expr* parse(const char* string) ;


int eval(Expr* expr) ;



#endif
