//calculator2.c

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h> //문자열 처리

typedef struct NODE {
	int data;
	struct Node* next; //다음 노드
}Node;

typedef struct STACK {
	Node* head; //typedef한 이름으로 선언해야 함
}Stack;

#define p_MAX_SIZE 1000

void init(Stack* stack) //스택 초기화 함수
{
	stack->head = NULL;
}

// 스택이 비어있는지 검사하는 함수
int is_empty(Stack* stack)
{
	return (stack->head == NULL); //NULL이면(비어있으면) 1, 아니면 0 반환
}

void push(Stack* stack, int data) { //새로운 노드 할당
	Node* node = (Node*)malloc(sizeof(Node)); //Node 메모리 동적할당

	if (node == NULL) {
		printf("Memory Allocation Error!");
		return;
	}
	else {
		node->data = data;
		node->next = stack->head;
		stack->head = node;
	}
}

int pop(Stack* stack) { //문자열 반환
	if (is_empty(stack)) {
		printf("Stack is empty!");
		exit(1);
	}
	else {
		Node* node = stack->head;
		int item = node->data;
		stack->head = stack->head->next;
		free(node);
		return item;
	}
}

int getTop(Stack* stack) { //stack의 top을 반환
	Node* head = stack->head;
	return stack->head->data;
}

int getPriority(char i) { //연산자의 우선순위를 정해줌
	switch (i) {
	case '(': case ')':
		return 0;
	case '+': case '-':
		return 1;
	case '*': case '/':
		return 2;
	}
	return -1;
}

void to_postfix(char in[], char post[]) { //입력받은 배열을 후위표기법으로 변환. post에 변환된 배열을 저장한다.
	Node* node;
	int len = strlen(in); //입력받은 배열의 길이를 계산
	int k = 0;
	char ch; //배열문자 임시 저장
	char top; //스택값 임시 저장

	init(&node); //스택 초기화

	for (int i = 0; i < len; i++) {
		ch = in[i];
		switch (ch) {
		case '+': case '-': case '*': case '/': //연산자의 경우
			//스택에 있는 연산자의 우선순위(숫자)가 더 크거나 같으면 출력(post에 저장)
			while (!is_empty(&node) && (getPriority(ch) <= getPriority(getTop(&node)))) {
				post[k++] = pop(&node); //1->2->3.. 순으로 후위표기배열에 출력(post에 저장)
			}
			push(&node, ch); //우선순위가 작으면 스택에 쌓기
			break;
		case '(':
			push(&node, ch); //스택으로..
			break;
		case ')':
			top = pop(&node); //맨 위의 요소가 팝됨 top에 임시저장
			while (top != '(') //왼쪽 괄호를 만날 때까지 출력(post에 저장)
			{
				post[k++] = top;
				top = pop(&node);
			}
			break;
		default:
			post[k++] = ch; //숫자는 바로 출력
			break;
		}
	}

	while (!is_empty(&node)) //stack이 다 빌때까지
	{
		post[k++] = pop(&node);
	}
}


int calculate(char in[]) { //후위표기법으로 변환된 식을 계산
	int o1, o2 = 0; //연산될 숫자를 pop한 변수
	int value = 0; //연산될 숫자를 stack에 다시 push할 변수
	char ch; //배열 문자가 저장될 임시 변수
	Stack* stack;

	init(&stack);	//스택 초기화

	for (int i = 0; in[i] != '\0'; i++) //배열 마지막까지
	{
		ch = in[i]; //후위 표기법으로 변환됨.
		if (ch != '+' && ch != '-' && ch != '*' && ch != '/') //연산자가 아니라면=>숫자라면
		{
			value = ch - '0'; //숫자의 문자 표현을 정수값으로 변환
			push(&stack, value); //숫자를 stack으로
		}
		else //연산자라면
		{
			o2 = pop(&stack); 
			o1 = pop(&stack); //stack에서 맨 위 두개 꺼냄
			switch (ch)	//연산자에 맞게 처리.
			{
			case '+':
				push(&stack, o1 + o2);
				break;
			case '-':
				push(&stack, o1 - o2);
				break;
			case '*':
				push(&stack, o1 * o2);
				break;
			case '/':
				push(&stack, o1 / o2);
				break;
			}
		}
	}
	return pop(&stack);
}

int main() {
	char* data;	//입력받을 중위표기식이 저장될 변수
	char post[100] = { '\0' };	//후위표기식으로 변경후 저장할 배열
	int result;	//결과값이 저장될 변수

	data = (char*)malloc(sizeof(char) * 100);	//배열의 크기 할당

	printf("식을 입력하세요: ");
	gets(data);

	to_postfix(data, post);	//후위표기전환함수 호출 
							//init함수를 사용하여 초기화한 스택변수의 주소로 대체해야 함

	result = calculate(post); //후위표기식 연산함수 호출

	printf("\n=> %s = %d\n", post, result);

	return 0;
}