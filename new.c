#include <stdio.h>
#include <stdlib.h>

int cnt = -1;
typedef struct Node //노드 정의
{
	char data;
	struct Node *next;
}Node;

typedef struct Stack //Stack 구조체 정의
{
	Node *top; //가장 최근 생성한 노드 == 맨 앞 노드
}Stack;

void InitStack(Stack *stack) 
{
	return stack -> top = NULL; //top을 NULL로 설정해서 초기화
}

int IsEmpty(Stack *stack) 
{
	return stack -> top == NULL: //top이 NULL이면 빈 상태임
}

void Push(Stack *stack, int data) 
{
	Node *now = (Node *)malloc(sizeof(Node)); // 노드 생성
	now -> data = data; // 데이터 설정
	now -> next = stack -> top; // now의 next링크를 현재 top으로 설정  
	stack -> top = now; // 스택의 맨 앞은 now
	count ++;
}

int Pop(Stack *stack) 
{
	Node *now;
	Node *prevtop;
	int re; // 꺼낼 값
	if (IsEmpty(stack))
	{
		return 0; //stack이 비어있다면 pop할 수 없으므로 종료
	}
	for (int i = 0; i < cnt -1; i++) 
	{
		now = now -> next;
	}
	*prevtop = now;
	now = now -> next;

	re = now -> data;
	cnt --;
	free(now);
	return re;
}



