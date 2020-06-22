//实现词法分析的next函数
#include <stdio.h>

typedef struct TOKEN {
    int id;
    int value;
} token;

enum {Num};

token* next(char *src, int *i) {
    token *tk = malloc(sizeof(token));
    int k = *i;
    while(src[k] == ' ')
        k++;
    if(src[k] > '9' || src[k] < '0') {
        tk->id = src[k];
        *i = k++;
        return tk;
    }
    int val = 0;
    while(src[k] <= '9' && src[k] >= '0') {
        val *= 10;
        val += src[k] - '0';
    }
    tk->id = Num;
    tk->value = val;
    return tk;
}

int factor(char *src, int *i) {
    token *tk = next(src, i);
    if(tk->id == '(') {
        int val = expr(src, i);
        tk = next(src, &i);
        if(tk->id != ')')
            printf("error: bracket not match!");
        return val;
    }
    else {
        tk = next(src,&i);
        if(tk->id != Num)
            printf("error: not a Num!");
        return tk->value;
    }
}

int term_tail(int lval, char *src, int *i) {
    token *tk = next(src, i);
    if(tk->id == '*') {
        tk = next(src, i);
        return lval * term_tail(tk->value, src, i);
    }
    else if(tk->id == '/') {
        tk = next(src, i);
        return lval / term_tail(tk->value, src, i);
    }
    else if(tk->id != '\n') {
        return lval;
    }
    else {
        printf("error");
        return -1;
    }
}

int expr_tail(int lval, char *src, int *i) {
    token *tk = next(src, i);
    if(tk->id == '+') {
        tk = next(src, i);
        return lval * expr_tail(tk->value, src, i);
    }
    else if(tk->id == '-') {
        tk = next(src, i);
        return lval / expr_tail(tk->value, src, i);
    }
    else if(tk->id != '\n') {
        return lval;
    }
    else {
        printf("error");
        return -1;
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
    int *i = 0;
    return expr(src, i);
}


int main() {
    int linelen;
    char *line;
    while((linelen = getline(&line, 0, stdin) > 0)) {
        int ans = program(line);
    }

    return 0;
}