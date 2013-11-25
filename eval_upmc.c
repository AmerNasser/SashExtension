//
//  eval_upmc.c
//  SashProject
//
//  Created by Amer Nasser on 25/11/13.
//  Copyright (c) 2013 ____. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "eval_upmc.h"

int VAL_STRING =0;


char* getExpression(char* arg)
{
	int i;
    char * res = malloc(strlen((const char*)arg)*sizeof(char));
    char * tmp = res;
    for(i =0;i<strlen(arg) - 2;i++){
        *tmp = *(arg +i + 1);
        tmp++;
    }
    *tmp ='\0';
    
    return res;
}


void error_message(const char* string) {
    unsigned int i;
    fprintf(stderr, "operateur non connu '%c' a la position : %u.\n\n", string[VAL_STRING], VAL_STRING);
    fprintf(stderr, "expression :: '%s'\n", string);
    fprintf(stderr, "Probleme: ");
    for(i = 0; i < VAL_STRING; ++i) {
        fprintf(stderr, " ");
    }
    fprintf(stderr, "^\n");
    exit(1);
}


//ne sont plus utilisé ....
//remplacé par eat char
/*
char parser_eat( parser_data *pd ){
	if( pd->pos < pd->len )
		return pd->str[pd->pos++];
	parser_error( pd, "Tried to read past end of string!" );
	return '\0';
}

void parser_eat_whitespace( parser_data *pd ){
	while( isspace( parser_peek( pd ) ) )
		parser_eat( pd );
}
*/


char eat_char(const char* string, char c) {
    if(string[VAL_STRING] != c) {
        error_message(string);
    }
    ++VAL_STRING;
    return c;
}

int eat_int(const char* string) {
    int i;
    
    if(!isdigit(string[VAL_STRING])) {
        error_message(string);
    }
    
    i = atoi(string + VAL_STRING);
    while(isdigit(string[VAL_STRING])) {
        ++VAL_STRING;
    }
    return i;
}


Expr* factor(const char* string, Expr* expr) {
    if(string[VAL_STRING] == '(') {
        expr->op = eat_char(string, '(');
        expr->data.expr[0] = parse(string);
        eat_char(string, ')');
    } else if(isdigit(string[VAL_STRING])) {
        expr->op = 'd';
        expr->data.terminal = eat_int(string);
    }
    return expr;
}

Expr* factor_tail(const char* string, Expr* expr) {
    Expr* new_expr;
    
    switch(string[VAL_STRING]) {
        case '*':
        case '/':
            if(NULL == (new_expr = (Expr*)malloc(sizeof(Expr)))) {
                exit(1);
            }
            if(NULL == (new_expr->data.expr[1] = (Expr*)malloc(sizeof(Expr)))) {
                exit(1);
            }
            new_expr->op = eat_char(string, string[VAL_STRING]);
            new_expr->data.expr[0] = expr;
            
            new_expr->data.expr[1] = factor(string, new_expr->data.expr[1]);
            new_expr = factor_tail(string, new_expr);
            return new_expr;
        case '+':
        case '-':
        case ')':
        case 0:
            return expr;
        default:
            error_message(string);return NULL;
    }
}

Expr* term(const char* string, Expr* expr) {
    if(string[VAL_STRING] == '(' || isdigit(string[VAL_STRING])) {
        expr = factor(string, expr);
        expr = factor_tail(string, expr);
        return expr;
    } else {
        error_message(string);return NULL;
    }
}

Expr* term_tail(const char* string, Expr* expr) {
    Expr* new_expr;
    
    switch(string[VAL_STRING]) {
        case '+':
        case '-':
            if(NULL == (new_expr = (Expr*)malloc(sizeof(Expr)))) {
                exit(1);
            }
            if(NULL == (new_expr->data.expr[1] = (Expr*)malloc(sizeof(Expr)))) {
                exit(1);
            }
            new_expr->op = eat_char(string, string[VAL_STRING]);
            new_expr->data.expr[0] = expr;
            
            new_expr->data.expr[1] = term(string, new_expr->data.expr[1]);
            new_expr = term_tail(string, new_expr);
            return new_expr;
        case ')':
        case 0:
            return expr;
        default:
            error_message(string);return NULL;
    }
}

Expr* parse(const char* string) {
    Expr* expr;
    
    if(string[VAL_STRING] == '(' || isdigit(string[VAL_STRING])) {
        if(NULL == (expr = (Expr*)malloc(sizeof(Expr)))) {
            exit(1);
        }
        
        expr = term(string, expr);
        expr = term_tail(string, expr);
        return expr;
    } else {
        error_message(string);return NULL;
    }
}



int eval(Expr* expr) {
    int ret;
    
    switch(expr->op) {
        case '(':
            ret = eval(expr->data.expr[0]);
            free(expr->data.expr[0]);
            break;
        case '*':
            ret =
            eval(expr->data.expr[0])
            *
            eval(expr->data.expr[1])
            ;
            free(expr->data.expr[0]);
            free(expr->data.expr[1]);
            break;
        case '/':
            ret =
            eval(expr->data.expr[0])
            /
            eval(expr->data.expr[1])
            ;
            free(expr->data.expr[0]);
            free(expr->data.expr[1]);
            break;
        case '+':
            ret =
            eval(expr->data.expr[0])
            +
            eval(expr->data.expr[1])
            ;
            free(expr->data.expr[0]);
            free(expr->data.expr[1]);
            break;
        case '-':
            ret =
            eval(expr->data.expr[0])
            -
            eval(expr->data.expr[1])
            ;
            free(expr->data.expr[0]);
            free(expr->data.expr[1]);
            break;
        case 'd':
            ret = expr->data.terminal;
            break;
        default:
            exit(1);
    }
    return ret;
}


