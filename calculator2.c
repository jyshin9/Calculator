//calculator2.c

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h> //���ڿ� ó��

typedef struct NODE {
	int data;
	struct Node* next; //���� ���
}Node;

typedef struct STACK {
	Node* head; //typedef�� �̸����� �����ؾ� ��
}Stack;

#define p_MAX_SIZE 1000

void init(Stack* stack) //���� �ʱ�ȭ �Լ�
{
	stack->head = NULL;
}

// ������ ����ִ��� �˻��ϴ� �Լ�
int is_empty(Stack* stack)
{
	return (stack->head == NULL); //NULL�̸�(���������) 1, �ƴϸ� 0 ��ȯ
}

void push(Stack* stack, int data) { //���ο� ��� �Ҵ�
	Node* node = (Node*)malloc(sizeof(Node)); //Node �޸� �����Ҵ�

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

int pop(Stack* stack) { //���ڿ� ��ȯ
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

int getTop(Stack* stack) { //stack�� top�� ��ȯ
	Node* head = stack->head;
	return stack->head->data;
}

int getPriority(char i) { //�������� �켱������ ������
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

void to_postfix(char in[], char post[]) { //�Է¹��� �迭�� ����ǥ������� ��ȯ. post�� ��ȯ�� �迭�� �����Ѵ�.
	Node* node;
	int len = strlen(in); //�Է¹��� �迭�� ���̸� ���
	int k = 0;
	char ch; //�迭���� �ӽ� ����
	char top; //���ð� �ӽ� ����

	init(&node); //���� �ʱ�ȭ

	for (int i = 0; i < len; i++) {
		ch = in[i];
		switch (ch) {
		case '+': case '-': case '*': case '/': //�������� ���
			//���ÿ� �ִ� �������� �켱����(����)�� �� ũ�ų� ������ ���(post�� ����)
			while (!is_empty(&node) && (getPriority(ch) <= getPriority(getTop(&node)))) {
				post[k++] = pop(&node); //1->2->3.. ������ ����ǥ��迭�� ���(post�� ����)
			}
			push(&node, ch); //�켱������ ������ ���ÿ� �ױ�
			break;
		case '(':
			push(&node, ch); //��������..
			break;
		case ')':
			top = pop(&node); //�� ���� ��Ұ� �˵� top�� �ӽ�����
			while (top != '(') //���� ��ȣ�� ���� ������ ���(post�� ����)
			{
				post[k++] = top;
				top = pop(&node);
			}
			break;
		default:
			post[k++] = ch; //���ڴ� �ٷ� ���
			break;
		}
	}

	while (!is_empty(&node)) //stack�� �� ��������
	{
		post[k++] = pop(&node);
	}
}


int calculate(char in[]) { //����ǥ������� ��ȯ�� ���� ���
	int o1, o2 = 0; //����� ���ڸ� pop�� ����
	int value = 0; //����� ���ڸ� stack�� �ٽ� push�� ����
	char ch; //�迭 ���ڰ� ����� �ӽ� ����
	Stack* stack;

	init(&stack);	//���� �ʱ�ȭ

	for (int i = 0; in[i] != '\0'; i++) //�迭 ����������
	{
		ch = in[i]; //���� ǥ������� ��ȯ��.
		if (ch != '+' && ch != '-' && ch != '*' && ch != '/') //�����ڰ� �ƴ϶��=>���ڶ��
		{
			value = ch - '0'; //������ ���� ǥ���� ���������� ��ȯ
			push(&stack, value); //���ڸ� stack����
		}
		else //�����ڶ��
		{
			o2 = pop(&stack); 
			o1 = pop(&stack); //stack���� �� �� �ΰ� ����
			switch (ch)	//�����ڿ� �°� ó��.
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
	char* data;	//�Է¹��� ����ǥ����� ����� ����
	char post[100] = { '\0' };	//����ǥ������� ������ ������ �迭
	int result;	//������� ����� ����

	data = (char*)malloc(sizeof(char) * 100);	//�迭�� ũ�� �Ҵ�

	printf("���� �Է��ϼ���: ");
	gets(data);

	to_postfix(data, post);	//����ǥ����ȯ�Լ� ȣ�� 
							//init�Լ��� ����Ͽ� �ʱ�ȭ�� ���ú����� �ּҷ� ��ü�ؾ� ��

	result = calculate(post); //����ǥ��� �����Լ� ȣ��

	printf("\n=> %s = %d\n", post, result);

	return 0;
}