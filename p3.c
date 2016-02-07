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

//Global Variables
char *input;
int idSize = 0;
int intSize = 0;
int first = 0;
int variableCount = 0;
int count = 0;

//Struct
struct Entry {
    struct Entry *next;
    char *name;
};

//Global struct
struct Entry *table;

static void error() {
    fprintf(stderr,"error at %d:%d\n", line,pos);
    //printf("error");
    longjmp(escape, 1);
}

//MIGHT NOT NEED THIS FUNCTION
void get(char *id) {
    
}

void set(char *id) {
    int same = 0;
    struct Entry *current = (struct Entry *) malloc(sizeof(struct Entry));
    if (first == 0) {
        table -> name = id;
        table -> next = NULL;
        first++;
    }
    else {
        current = table;
        while (current -> next != NULL) {
            if (strcmp(current -> name, id) == 0) {
                same = 1;
            }
            current = current -> next;
        }
        if (strcmp(current -> name, id) == 0) {
            same = 1;
        }
    }
    if (!same) {
        current -> next = malloc(sizeof(struct Entry));
        //No memory left
        if (current -> next == NULL) {
            printf("Out of memory\n");
        }
        //Assign and print values
        else {
            current = current -> next;
            current -> name = id;
            current -> next = NULL;
        }
    }
    /*//No memory left
    if (current -> next == NULL) {
        printf("Out of memory\n");
    }
    //Assign and print values
    else {
        current = current -> next;
        current -> name = id;
        current -> next = NULL;*/
        printf("    mov %%r15, ");
        printf("%s\n", id);
        printf("    mov $p3_format, %%rdi\n");
        printf("    mov $");
        printf("%s", id);
        printf("%s", "var");
        printf(", %%rsi\n");
        printf("    mov %%r15, %%rdx\n");
        printf("    mov $0, %%rax\n");
        printf("    call printf\n");
        //variableCount++;
        /*printf("    mov p3_format, %%rdi\n");
        printf("    mov ");
        printf("%s", id);
        printf(", %%rsi\n");
        printf("    mov %%r15, %%rdx\n");
        printf("    call printf\n");*/
    //}
}

char peekChar(void) {
    const char nextChar = getchar();
    pos ++;
    if (nextChar == 10) {
        line ++;
        pos = 0;
    }
    return nextChar;
}

void consume(int length) {
    input += length;
    while (input[0] == ' ' || input[0] == 9 || input[0] == 10) {
        input++;
    }
}

int isWhile() {
    if (strncmp(input, "while", 5) == 0) {
        if (input[5] != 0 && ((input[5] >= 'a' && input[5] <= 'z') || 
            (input[5] >= '0' && input[5] <= '9'))) {
            return 0;
        }
        else {
            return 1;
        }
    }
    return 0;
}

int isIf() {
    if (strncmp(input, "if", 2) == 0) {
        if (input[2] != 0 && ((input[2] >= 'a' && input[2] <= 'z') || 
            (input[2] >= '0' && input[2] <= '9'))) {
            return 0;
        }
        else {
            return 1;
        }
    }
    return 0;
}

int isElse() {
    if (strncmp(input, "else", 4) == 0) {
        if (input[4] != 0 && ((input[4] >= 'a' && input[4] <= 'z') || 
            (input[4] >= '0' && input[4] <= '9'))) {
            return 0;
        }
        else {
            return 1;
        }
    }
    return 0;
}

int isSemi() {
    if (strncmp(input, ";", 1) == 0) {
        return 1;
    }
    return 0;
}

int isLeftBlock() {
    if (strncmp(input, "{", 1) == 0) {
        return 1;
    }
    return 0;
}

int isRightBlock() {
    if (strncmp(input, "}", 1) == 0) {
        return 1;
    }
    return 0;
}

int isEq() {
    if (strncmp(input, "=", 1) == 0) {
        return 1;
    }
    return 0;
}

int isEqEq() {
    if (strncmp(input, "==", 2) == 0) {
        return 1;
    }
    return 0;
}

int isLeft() {
    if (strncmp(input, "(", 1) == 0) {
        return 1;
    }
    return 0;
}

int isRight() {
    if (strncmp(input, ")", 1) == 0) {
        return 1;
    }
    return 0;
}

int isEnd() {
    if (input[0] == 0) {
        return 1;
    }
    return 0;
}

int isId() {
    idSize = 0;
    if (input[idSize] >= 'a' && input[idSize] <= 'z') {
        idSize++;
    }
    else {
        return 0;
    }
    while ((input[idSize] >= 'a' && input[idSize] <= 'z') || 
        (input[idSize] >= '0' && input[idSize] <= '9')) {
        idSize++;
    }
    if (idSize == 2) {
        if (input[0] == 'i' && input[1] == 'f') {
            return 0;
        }
    }
    if (idSize == 4) {
        if (input[0] == 'e' && input[1] == 'l' && 
            input[2] == 's' && input[3] == 'e') {
            return 0;
        }
    }
    if (idSize == 4) {
        if (input[0] == 'w' && input[1] == 'h' && input[2] == 'i' && 
            input[3] == 'l' && input[4] == 'e') {
            return 0;
        }
    }
    return 1;
}

int isMul() {
    if (strncmp(input, "*", 1) == 0) {
        return 1;
    }
    return 0;
}

int isPlus() {
    if (strncmp(input, "+", 1) == 0) {
        return 1;
    }
    return 0;
}

char *getId() {
    char *myId;
    myId = malloc(idSize);
    for (int i = 0; i < idSize; i++) {
        myId[i] = input[i];
    }
    return myId;
}

int isInt() {
    intSize = 0;
    if (input[intSize] >= '0' && input[intSize] <= '9') {
        intSize++;
    }
    else {
        return 0;
    }
    while ((input[intSize] >= '0' && input[intSize] <= '9') || 
        (input[intSize] == '_')) {
        intSize++;
    }
    return 1;
}

int getInt() {
    int myInt = input[0] - '0';
    for (int i = 1; i < intSize; i++) {
        if (input[i] >= '0' && input[i] <= '9') {
            int temp = input[i] - '0';
            myInt = myInt * 10 + temp;
        }
    }
    return myInt;
}

void expression(void);
void seq(void);

/* handle id, literals, and (...) */
void e1() {
    if (isLeft()) {
        consume(1);
        //printf("    push %%r13\n");
        expression();
        if (!isRight()) {
            error();
        }
        consume(1);
        //printf("    pop %%r13\n");
    } else if (isInt()) {
        int v = getInt();
        consume(intSize);
        printf("    push %%r13\n");
        //printf("    mov %%r15, %%r14\n");
        printf("    mov $");
        printf("%d", v);
        printf(", %%r13\n");
        printf("    mov %%r13, %%r15\n");
        //printf("    mov %%r14, %%r13\n");
        printf("    pop %%r13\n");
    } else if (isId()) {
        char *id = getId();
        consume(idSize);
        get(id);
    } else {
        error();
    }
}

/* handle '*' */
void e2(void) {
    printf("    push %%r13\n");
    e1();
    printf("    pop %%r13\n");
    while (isMul()) {
        consume(1);
        printf("    mov %%r15, %%r13\n");
        printf("    push %%r13\n");
        e1();
        printf("    pop %%r13\n");
        printf("    imul %%r13, %%r15\n");
    }
}

/* handle '+' */
void e3(void) {
    printf("    push %%r13\n");
    e2();
    printf("    pop %%r13\n");
    while (isPlus()) {
        consume(1);
        printf("    mov %%r15, %%r13\n");
        printf("    push %%r13\n");
        e2();
        printf("    pop %%r13\n");
        printf("    add %%r13, %%r15\n");
    }
}

/* handle '==' */
void e4(void) {
    e3();
    while (isEqEq()) {
        consume(2);
        e3();
    }
    //printf("    pop %%r13\n");
}

void expression(void) {
    e4();
    //printf("    pop %%r13\n");
}

int statement(void) {
    if (isId()) {
        char *id = getId();
        consume(idSize);
        if (!isEq())
            error();
        consume(1);
        expression();
        set(id);

        if (isSemi()) {
            consume(1);
        }

        return 1;
    } else if (isLeftBlock()) {
        consume(1);
        seq();
        if (!isRightBlock())
            error();
        consume(1);
        return 1;
    } else if (isIf()) {
        consume(2);
        expression();

        statement();

        if (isElse()) {
            consume(4);
            //printf("    else:");
            statement();
        }
        return 1;
    } else if (isWhile()) {
        consume(5);
        expression();
        statement();
        return 1;
    } else if (isSemi()) {
        consume(1);
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
    table = (struct Entry *) malloc(sizeof(struct Entry));
    int pos = 0;
    int size = 16;
    input = (char *) malloc(16);
    while (1) {
        const char c = peekChar();
        if (c == -1) break;
        input[pos] = c;
        if (pos + 1 == size) {
            char *temp;
            if ((temp = (char *) realloc(input, 2 * size)) == NULL) {
                printf("ERROR: realloc failed");
            }
            input = temp;
            size *= 2;
        }
        pos++;
    }
    /*char *temp1;
    if ((temp1 = (char *) realloc(input, size + 1)) == NULL) {
        printf("ERROR: realloc failed");
    }
    input = temp1;*/
    //printf("%s\n", "test");
    input[pos] = '\0';
    printf("    .text\n");
    printf("    .global main\n");
    printf("    .extern printf\n");
    printf("main:\n");
    printf("    push %%r13\n");
    printf("    push %%r14\n");
    printf("    push %%r15\n");
    /*printf("    push %%rdi\n");
    printf("    push %%rsi\n");
    printf("    push %%rdx\n");*/
    /*while (1) {
        const int c = peekChar();
        if (c == -1) break;
        printf("    mov $p3_format,%%rdi\n");
        printf("    mov $%d,%%rsi\n",c);
        printf("    mov $%d,%%rdx\n",c);
        printf("    call printf\n");
    }*/
    int x = setjmp(escape);
    if (x == 0) {
        program();
    }
    printf("    mov $0,%%rax\n");
    /*printf("    pop %%rdx\n");
    printf("    pop %%rsi\n");
    printf("    pop %%rdi\n");*/
    printf("    pop %%r15\n");
    printf("    pop %%r14\n");
    printf("    pop %%r13\n");
    printf("    ret\n");
    printf("    .data\n");

    printf("p3_format: .string\"%%s:%%d\\n\"\n");
    /*char formatString[] = "[%d]%c";
    for (int i=0; i<sizeof(formatString); i++) {
        printf("    .byte %d\n",formatString[i]);
    }*/
    /*printf("%s\n", "table is not null");
    if (table -> name == NULL) {
        printf("%s\n", "table is null");
    }
    printf("%s\n", "table is not null");
    printf("%s\n", table -> name);*/
    while (table -> next != NULL) {
        printf("    ");
        printf("%s", table -> name);
        printf(": .quad 0\n");
        printf("    ");
        printf("%s", table -> name);
        printf("%s", "var");
        printf(": .string ");
        printf("\"");
        printf("%s", table -> name);
        printf("\"\n");
        table = table -> next;
    }
    printf("    ");
    printf("%s", table -> name);
    printf(": .quad 0\n");
    printf("    ");
    printf("%s", table -> name);
    printf("%s", "var");
    printf(": .string ");
    printf("\"");
    printf("%s", table -> name);
    printf("\"\n");
    table = table -> next;
}

int main(int argc, char *argv[]) {
    compile();
    return 0;
}
