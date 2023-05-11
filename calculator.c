#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // 문자열을 처리할 거기 때문에 
#define MAX 100

typedef struct Node {
    char data[MAX]; // 하나의 문자열을 담을 수 있도록. 숫자 또는 연산자 등 다양한 형태가 들어가기 떄문에.
    struct Node* next; // 다음 노드로 연결될 수 있도록.
} Node;

typedef struct Stack
{
    Node* top;
}Stack;

void push(Stack* stack, char *data) { //Stack구조체 포인터 
    Node* node = (Node*)malloc(sizeof(Node));
    strcpy(node->data, data); // 매개변수로 들어온 문자열을 node 안에 담아줌
    node->next = stack->top; // 현재 스택의 top을 node의 next로
    stack->top = node; // top을 지금 노드로 바꿔줌
}

char* getTop(Stack* stack) {
    Node* top = stack->top;
    return top->data;
}

char pop(Stack* stack) {
    if (stack->top == NULL)
    {
        printf("스택 언더플로우");
    }
    Node* node = stack->top;
    char* data = (Node*)malloc(sizeof(Node));
    strcpy(data, node->data); // 현재 스택에 있던 데이터를 반환하도록 만들고, 

    stack->top = node->next; // 기존 스택의 top에서 하나를 뽑도록 한다.
    free(node);
    return data;
}

int getPriority(char i) {
    if (i=='+'||i=='-') return 1;
    if (i == '*' || i == '/') return 2;
    return 0;
    // '('는 원래 우선순위가 가장 높지만 top에 ')'가 있는 경우를 고려해줘야 하기 때문에 0으로 설정
}

char transition(Stack* stack, char** s, int size) {
    char res[1000] = "";
    for (int i = 0; i < size; i++) {
        if (!strcmp(s[i], "+") || !strcmp(s[i], "-") || !strcmp(s[i], "/") || !strcmp(s[i], "*") || ) {
            while (stack->top != NULL && getPrioity(getTop(stack)) >= getPriority(s[i])) {
                strcat(res, pop(stack));
                strcat(res, " ");
            }
        }
        else if (!strcmp(s[i], "("))push(stack, s[i]);
        else if (!strcmp(s[i], ")")) {
            while (strcmp(getTop(stack), "(")) {
                strcat(res, pop(stack));
                strcat(res, " ");
            }
            pop(stack);
        }
        else {
            strcat(res, s[i]);
            strcat(res, " ");
        }
    }
    while (stack->top != NULL) {
        strcat(res, pop(stack));
        strcat(res, " ");
    }
}

void calculate(Stack* stack, char** s, int size) {
    int x, y, z;
    for (int i = 0; i < size; i++)
    {
        if (!strcmp(s[i], "+") || !strcmp(s[i], "-") || !strcmp(s[i], "/") || !strcmp(s[i], "*")) {
            x = atoi(pop(stack));
            y = atoi(pop(stack));

            if (!strcmp(s[i], "+")) z = y + x;
            if (!strcmp(s[i], "-")) z = y - x;
            if (!strcmp(s[i], "*")) z = y * x;
            if (!strcmp(s[i], "/")) z = y / x;

            char buffer[100];
            sprintf(buffer, "%d", z);
            push(stack, buffer);
        }
        else {
            push(stack, s[i]);
        }
    }
    printf("%d\n", pop(stack));
}

int main(void)
{
    Stack stack;
    stack.top = NULL; // 스택 초기화

    char a[100] = "( ( 3 + 4 ) * 5 ) - 5 * 7 * 5 - 5 * 10";
    int size = 1;
    for (int i = 0; i < strlen(a); i++) {
        if (a[i] == ' ') size++;
    }

    char* ptr = strtok(a, " ");
    char** input = (char**)malloc(sizeof(char*) * size);
    for (int i = 0; i < size; i++) {
        input[i] = (char*)malloc(sizeof(char) * 100);
    }
    for (int i = 0; i < size; i++) {
        strcpy(input[i], ptr);
        ptr = strtok(NULL, " ");
    }

    char b[1000] = "";
    strcpy(b, transition(&stack, input, size));
    printf("후위 표기법: &s\n", b);

    size = 1;
    for (int i = 0; i < strlen(b) - 1; i++) { // 
        if (b[i] == " ") size++;
    }

    char* ptr2 = strtok(b, " ");
    for (int i = 0; i < size; i++) {
        strcpy(input[i], ptr2);
        ptr2 = strtok(NULL, " ");
    }
    calculate(&stack, input, size); // -190
}