//calculator.h

#pragma once
#include <stdlib.h>

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

void to_postfix(Stack* stack, char in[], char post[]) { //�Է¹��� �迭�� ����ǥ������� ��ȯ. post�� ��ȯ�� �迭�� �����Ѵ�.
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
			while (stack->head != NULL && getPriority(ch) <= getPriority(getTop(stack))) {
				post[k++] = pop(&node); //1->2->3.. ������ ����ǥ��迭�� ���(post�� ����)
			}
			push(&node, &ch); //�켱������ ������ ���ÿ� �ױ�
			break;
		case '(':
			push(&node, &ch); //��������..
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
	int value= 0; //����� ���ڸ� stack�� �ٽ� push�� ����
	char ch; //�迭 ���ڰ� ����� �ӽ� ����
	Stack* stack;

	init(&stack);	// ���� �ʱ�ȭ

	for (int i = 0; in[i] != '\0'; i++) //�迭 ����������
	{
		ch = in[i]; //���� ǥ������� ��ȯ��.
		if (ch != '+' && ch != '-' && ch != '*' && ch != '/') //�����ڰ� �ƴ϶��=>���ڶ��
		{
			value = ch - '0'; // ������ ���� ǥ���� ���������� ��ȯ
			push(&stack, &value); //���ڸ� stack����
		}
		else //�����ڶ��
		{	
			int o3 = 0;
			o1 = pop(&stack);
			o2 = pop(&stack); //stack���� �� �� �ΰ� ����
			switch (ch)	//�����ڿ� �°� ó��.
			{
			case '+':
				o3 = o1 + o2;
				push(&stack, &(o3));
				break;
			case '-':
				o3 = o1 - o2;
				push(&stack, &(o3));
				break;
			case '*':
				o3 = o1 * o2;
				push(&stack, &(o3));
				break;
			case '/':
				o3 = o1 / o2;
				push(&stack, &(o3));
				break;
			}
		}
	}
	return pop(&stack);
}
