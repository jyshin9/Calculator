#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // ���ڿ��� ó���� �ű� ������ 
#define MAX 100

typedef struct Node {
    char data[MAX]; // �ϳ��� ���ڿ��� ���� �� �ֵ���. ���� �Ǵ� ������ �� �پ��� ���°� ���� ������.
    struct Node* next; // ���� ���� ����� �� �ֵ���.
} Node;

typedef struct Stack
{
    Node* top;
}Stack;

void push(Stack* stack, char *data) { //Stack����ü ������ 
    Node* node = (Node*)malloc(sizeof(Node));
    strcpy(node->data, data); // �Ű������� ���� ���ڿ��� node �ȿ� �����
    node->next = stack->top; // ���� ������ top�� node�� next��
    stack->top = node; // top�� ���� ���� �ٲ���
}

char* getTop(Stack* stack) {
    Node* top = stack->top;
    return top->data;
}

char pop(Stack* stack) {
    if (stack->top == NULL)
    {
        printf("���� ����÷ο�");
    }
    Node* node = stack->top;
    char* data = (Node*)malloc(sizeof(Node));
    strcpy(data, node->data); // ���� ���ÿ� �ִ� �����͸� ��ȯ�ϵ��� �����, 

    stack->top = node->next; // ���� ������ top���� �ϳ��� �̵��� �Ѵ�.
    free(node);
    return data;
}

int getPriority(char i) {
    if (i=='+'||i=='-') return 1;
    if (i == '*' || i == '/') return 2;
    return 0;
    // '('�� ���� �켱������ ���� ������ top�� ')'�� �ִ� ��츦 �������� �ϱ� ������ 0���� ����
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
    stack.top = NULL; // ���� �ʱ�ȭ

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
    printf("���� ǥ���: &s\n", b);

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