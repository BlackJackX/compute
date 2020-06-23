#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TOKEN {
    int id;
    int value;
} token;

enum {Num};
token *tk;
int read;

void next(char *src, int *i) {
    if(read) {
        read = 0;
        return;
    }
    int k = *i;
    while(src[k] == ' ')
        k++;
    if(src[k] > '9' || src[k] < '0') {
        tk->id = src[k];
        *i = ++k;
        return;
    }
    int val = 0;
    while(src[k] <= '9' && src[k] >= '0') {
        val *= 10;
        val += src[k++] - '0';
    }
    tk->id = Num;
    tk->value = val;
    *i = k;
}

int factor(char *src, int *i) {
    next(src, i);
    if(tk->id == '(') {
        int val = expr(src, i);
        next(src, i);
        if(tk->id != ')')
            printf("error: bracket not match!");
        return val;
    }
    else {
        if(tk->id != Num)
            printf("error: not a Num!");
        return tk->value;
    }
}

int term_tail(int lval, char *src, int *i) {
    next(src, i);
    int val;
    if(tk->id == '*') {
        val = lval * factor(src, i);
        return term_tail(val, src, i);
    }
    else if(tk->id == '/') {
        val = lval / factor(src, i);
        return term_tail(val, src, i);
    }
    else if(tk->id == '\n' || tk->id == '\0') {
        return lval;
    }
    else {
        read = 1;
        return lval;
    }
        
    
}

int expr_tail(int lval, char *src, int *i) {
    next(src, i);
    int val;
    if(tk->id == '+') {
        val = lval + term(src, i);
        return expr_tail(val, src, i);
    }
    else if(tk->id == '-') {
        val = lval - term(src, i);
        return expr_tail(val, src, i);
    }
    else if(tk->id == '\n') {
        return lval;
    }
    else {
        read = 1;
        return lval;
    }
}

int term(char *src, int *i) {
    int val = factor(src, i);
    return term_tail(val, src, i);
}

int expr(char *src, int *i) {
    int val = term(src, i);
    return expr_tail(val, src, i);
}

int program(char *src) {
    int *i = malloc(sizeof(int));
    *i = 0;
    int val = expr(src, i);
    free(i);
    return val;
}


int main() {
    size_t linecap = 0;
    ssize_t linelen;
    char *line = NULL;
    tk = malloc(sizeof(token));
    read = 0;
    while((linelen = getline(&line, &linecap, stdin)) > 0) {
        if(line[0] == '\n')
            continue;
        int ans = program(line);
        read = 0;
        if(line[linelen-1] == '\n')
            line[linelen-1] = '\0';
        printf("%s = %d\n", line, ans);
    }
    free(tk);
    return 0;
}