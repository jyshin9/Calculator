#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)

// 노드 정의 선언
typedef struct
{
	int item;
	struct StackNode* link;
} StackNode;

typedef struct
{
	StackNode* top;
} LinkedStackType;


// 스택 초기화 함수
void init(LinkedStackType* s)
{
	s->top = NULL;
}

// 스택이 비어있는지 검사하는 함수
int is_empty(LinkedStackType* s)
{
	return (s->top == NULL);
}

// 스택에 노드를 삽입할 함수
void push(LinkedStackType* s, int item)
{
	StackNode* temp = (StackNode*)malloc(sizeof(StackNode));
	if (temp == NULL)	// 메모리 할당이 되지 않았으면 에러를 발생시킴.
	{
		fprintf(stderr, "메모리 할당 에러\n");
		return;
	}
	else	// 할당이 됐으면 기존 노드와 연결시켜준다.
	{
		temp->item = item;
		temp->link = s->top;
		s->top = temp;
	}
}

// 스택에 마지막 값을 리턴하고 제거하는 함수
int pop(LinkedStackType* s)
{
	if (is_empty(s))	// 노드가 비었으면 실행시키지 않는다.
	{
		fprintf(stderr, "스택이 비어있음.\n");
		exit(1);
	}
	else	// 노드가 존재하면 최상위 노드를 불러내어 값을 리턴하고 제거함.
	{
		StackNode* temp = s->top;
		int item = temp->item;
		s->top = s->top->link;
		free(temp);
		return item;
	}
}

// 스택 마지막 값을 확인, 리턴하는 함수
int peek(LinkedStackType* s)
{
	if (is_empty(s))	// 노드가 비었으면 실행시키지 않는다.
	{
		fprintf(stderr, "스택이 비어있음.\n");
		exit(1);
	}
	else	// 최상위 노드의 값을 리턴함
	{
		return s->top->item;
	}
}


// 연산자의 우선순위를 반환하는 함수
int prec(char op)
{
	switch (op)	// 각각의 우선순위를 0,1,2로 리턴시킴.
	{
	case '(': case ')':
		return 0;
	case '+': case '-':
		return 1;
	case '*': case '/':
		return 2;
	}
	return -1;
}

// 입력받은 중위표기된 배열을 후위표기로 바꿔주는 함수. 2번째 인자에 변환된 식을 저장한다.
void infix_to_postfix(char exp[], char str[])
{
	int i = 0, k = 0;	// i : loop문 제어 변수, k : 배열 위치에 관한 변수
	char ch, top_op;	// ch : 배열문자가 저장될 임시변수, top_op : 스택값이 저장될 임시변수
	int len = strlen(exp);	// 배열의 길이를 저장할 변수
	StackNode* s;

	init(&s);	// 스택 초기화
	for (i = 0; i < len; i++)
	{
		ch = exp[i];	// 입력받은 배열을 1칸씩 전진시키면서 문자를 확인하고
		switch (ch)		// 아래의 case문대로 조건실행, 혹은 default에 따라 처리함.
		{
		case '+': case '-': case '*': case '/':	// 연산자
			// 스택에 있는 연산자의 우선순위가 더 크거나 같으면 출력
			while (!is_empty(&s) && (prec(ch) <= prec(peek(&s))))
			{
				str[k++] = pop(&s);
				//printf("%c", pop(&s));
			}
			push(&s, ch);
			break;
		case '(':
			push(&s, ch);
			break;
		case ')':
			top_op = pop(&s);	// 왼쪽 괄호를 만날 때까지 출력
			while (top_op != '(')
			{
				str[k++] = top_op;
				//printf("%c", top_op);
				top_op = pop(&s);
			}
			break;
		default:
			str[k++] = ch;
			//	printf("%c", ch);
			break;
		}
	}
	while (!is_empty(&s))
	{
		str[k++] = pop(&s);
		//printf("%c", pop(&s));
	}
}

// 후위표기식을 연산할 함수
int eval(char exp[])
{
	int op1, op2, value, i = 0;	// op1,2 : 연산될 숫자를 pop한 변수, value : 연산될 숫자를 push할 변수, i : loop문 제어 변수
	char ch;	// 배열 문자가 저장될 임시 변수
	StackNode* s;

	init(&s);	// 스택 초기화
	for (i = 0; exp[i] != '\0'; i++)
	{
		ch = exp[i];
		if (ch != '+' && ch != '-' && ch != '*' && ch != '/')
		{
			value = ch - '0';		// int형으로 변환
			push(&s, value);
		}
		else
		{
			op2 = pop(&s);
			op1 = pop(&s);
			switch (ch)	// 연산자를 조사하여 그에 맞게 처리함.
			{
			case '+':
				push(&s, op1 + op2);
				break;
			case '-':
				push(&s, op1 - op2);
				break;
			case '*':
				push(&s, op1 * op2);
				break;
			case '/':
				push(&s, op1 / op2);
				break;
			}
		}
	}
	return pop(&s);
}


int main()
{
	char* data;	// 입력받을 중위표기식이 저장될 char형 포인터변수
	char str[100] = { '\0' };	// 바뀐 후위표기식을 저장할 char형 배열
	int result;	// 연산값이 저장될 변수
	int i = 0;	// loop문 제어변수
	data = (char*)malloc(sizeof(char) * 100);	// 배열의 크기 할당
	printf("후위표기식으로 바꿀 중위표기식을 입력하세요. : ");
	gets(data);


	infix_to_postfix(data, str);	// 후위표기전환함수 호출

	printf("\n%s\n", str);
	for (i = 0; str[i] != '\0'; i++)	// str배열에 제대로 들어갔는지 확인.
	{
		printf("str[%d] = %c\n", i, str[i]);
	}
	result = eval(str);	// 후위표기식 연산함수 호출
	printf("\n계산 결과 : %d\n", result);

	return 0;
}