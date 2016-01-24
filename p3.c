#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>

#define MISSING() do { \
    fprintf(stderr,"missing code at %s:%d\n",__FILE__,__LINE__); \
    error(); \
} while (0)

static jmp_buf escape;

static int line = 0;
static int pos = 0;

static void error() {
    fprintf(stderr,"error at %d:%d\n", line,pos);
    longjmp(escape, 1);
}

void get(char *id) {
    MISSING();
}

void set(char *id) {
    MISSING();
}


int peekChar(void) {
    const char nextChar = getchar();
    pos ++;
    if (nextChar == 10) {
        line ++;
        pos = 0;
    }
    return nextChar;
}

void consume() {
    MISSING();
}

int isWhile() {
    MISSING();
    return 0;
}

int isIf() {
    MISSING();
    return 0;
}

int isElse() {
    MISSING();
    return 0;
}

int isSemi() {
    MISSING();
    return 0;
}

int isLeftBlock() {
    MISSING();
    return 0;
}

int isRightBlock() {
    MISSING();
    return 0;
}

int isEq() {
    MISSING();
    return 0;
}

int isEqEq() {
    MISSING();
    return 0;
}

int isLeft() {
    MISSING();
    return 0;
}

int isRight() {
    MISSING();
    return 0;
}

int isEnd() {
    MISSING();
    return 0;
}

int isId() {
    MISSING();
    return 0;
}

int isMul() {
    MISSING();
    return 0;
}

int isPlus() {
    MISSING();
    return 0;
}

char *getId() {
    MISSING();
    return 0;
}

int isInt() {
    MISSING();
    return 0;
}

int getInt() {
    MISSING();
    return 0;
}

void expression(void);
void seq(void);

/* handle id, literals, and (...) */
void e1() {
    if (isLeft()) {
        consume();
        expression();
        if (!isRight()) {
            error();
        }
        consume();
    } else if (isInt()) {
        /*int v =*/ getInt();
        consume();
    } else if (isId()) {
        char *id = getId();
        consume();
        get(id);
    } else {
        error();
    }
}

/* handle '*' */
void e2(void) {
    e1();
    while (isMul()) {
        consume();
        e1();
    }
}

/* handle '+' */
void e3(void) {
    e2();
    while (isPlus()) {
        consume();
        e2();
    }
}

/* handle '==' */
void e4(void) {
    e3();
    while (isEqEq()) {
        consume();
        e3();
    }
}

void expression(void) {
    e4();
}

int statement(void) {
    if (isId()) {
        char *id = getId();
        consume();
        if (!isEq())
            error();
        consume();
        expression();
        set(id);

        if (isSemi()) {
            consume();
        }

        return 1;
    } else if (isLeftBlock()) {
        consume();
        seq();
        if (!isRightBlock())
            error();
        consume();
        return 1;
    } else if (isIf()) {
        consume();
        expression();

        statement();

        if (isElse()) {
            consume();
            statement();
        }
        return 1;
    } else if (isWhile()) {
        consume();
        expression();
        statement();
        return 1;
    } else if (isSemi()) {
        consume();
        return 1;
    } else {
        return 0;
    }
}

void seq(void) {
    while (statement()) { fflush(stdout); }
}

void program(void) {
    seq();
    if (!isEnd())
        error();
}

void compile(void) {
    printf("    .text\n");
    printf("    .global main\n");
    printf("main:\n");
    printf("    push %%r13\n");
    printf("    push %%r14\n");
    printf("    push %%r15\n");
    while (1) {
        const int c = peekChar();
        if (c == -1) break;
        printf("    mov $p3_format,%%rdi\n");
        printf("    mov $%d,%%rsi\n",c);
        printf("    mov $%d,%%rdx\n",c);
        printf("    call printf\n");
    }
    int x = setjmp(escape);
    if (x == 0) {
        program();
    }
    printf("    mov $0,%%rax\n");
    printf("    pop %%r15\n");
    printf("    pop %%r14\n");
    printf("    pop %%r13\n");
    printf("    ret\n");
    printf("    .data\n");

    printf("p3_format:\n");
    char formatString[] = "[%d]%c";
    for (int i=0; i<sizeof(formatString); i++) {
        printf("    .byte %d\n",formatString[i]);
    }
}

int main(int argc, char *argv[]) {
    compile();
    return 0;
}
