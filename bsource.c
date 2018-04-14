#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXPR_SIZE	100		// 수식 최대 사이즈
int cnt = -1;

int isp[] = { 0, 19, 12, 12, 13, 13, 13, 0 };
int icp[] = { 20, 19, 12, 12, 13, 13, 13, 0 };
char expr[MAX_EXPR_SIZE];		// 수식을 저장하는 문자배열.

typedef enum {
	lparen, rparen, plus, minus, times, divide, mod, eos, operand
} precedence; // 우선순위

// 후위표현으로 표현 된 수식중 연산자를 문자로 표현
void print_token(precedence token, char** str);
// 토큰을 받아옴. 토큰의 종류
precedence	get_token(char* symbol, int* n);

typedef struct Node //노드 정의
{
	char data;
	struct Node *next;
}Node;

Node * head = NULL; // head 노드


int	eval(void);					// 후위 표현 수식을 계산.
void Push(int data);	// 스택 푸쉬.
int Pop();				// 스택 팝
void postfix(void);					// 수식을 후위 표현으로 변환

// 후위표현으로 표현 된 수식중 연산자를 문자로 표현
void print_token(precedence token, char** str);
// 토큰을 받아옴. 토큰의 종류
precedence get_token(char* symbol, int* n);

int main(void)
{
	printf("----------------------------------\n");
	printf("|   Converting infix to postfix   |\n");
	printf("----------------------------------\n\n");
	printf("수식을 입력하세요\nInput : ");
	scanf("%s", expr);	// 수식 입력 받음
	postfix();			// 후위표기로 변환

	printf("\n\n결과 : %d \n\n", eval()); // 후위식을 계산.
	return 0;
}

int IsEmpty()
{
	if (cnt == -1) {
		return -1;  // cnt == -1이면 빈 상태임
	}
	return 0;
}

void Push(int data)
{
	Node *now = (Node *)calloc(sizeof(Node), 1); // 노드 생성
	Node *temp = head;

	if (IsEmpty()) {
		head = now;
		now->data = data; // 데이터 설정
	}
	else {
		while ((temp->next) != NULL){
			temp = temp->next;
		}
		now->data = data; // 데이터 설정
		temp->next = now; // temp의 next링크를 현재 top으로 설정  
	}

	cnt++;
}

int Pop()
{
	Node *now = head;
	int re; // 꺼낼 값

	if (IsEmpty())
	{
		return eos; //stack이 비어있다면 pop할 수 없으므로 종료
	}

	while ((now->next) != NULL)
	{
		now = now->next;
	}
	re = now->data;
	cnt--;
	free(now);
	return re;
}

char FindTopV()
{
	Node *now = head;
	char re; // 꺼낼 값

	if (IsEmpty())
	{
		return 0; //stack이 비어있다면 pop할 수 없으므로 종료
	}
	for (int i = 0; i < cnt; i++)
	{
		now = now->next;
	}
	re = now->data;
	return re;
}


precedence get_token(char* symbol, int* n)
{
	switch (*symbol = expr[(*n)++])
	{
	case '(': return lparen;
	case ')': return rparen;
	case '+': return plus;
	case '-': return minus;
	case '/': return divide;
	case '*': return times;
	case '%': return mod;
	case '\0': return eos;
	
	default: return operand;
	}
}
// 해당 토큰에 해당하는 기호(연산자)로 문자로 삽입.
void print_token(precedence token, char* str)
{
	switch (token)
	{
	case plus: *str = '+';	return;       // +
	case minus: *str = '-'; return;      // -
	case divide: *str = '/'; return;     // /
	case times:	*str = '*'; return;      // *
	case mod: *str = '%'; return;        // %

	case eos: return;
	default: return;
	}
}

void postfix(void)
{
	int count = 0;
	char symbol;				// 연산자, 피연산자 
	char expr_post[MAX_EXPR_SIZE] = { 0, }; // 후위 표기식으로 저장하는 문자열.
	precedence token;			// 토큰을 받는 변수
	int n = 0;	// 문자열의 인덱스

	// 중위 -> 후위 변환 알고리즘
	// 토큰이 eos, 즉 문자의 끝 일때 까지 반복
	for (token = get_token(&symbol, &n); token != eos; token = get_token(&symbol, &n))
	{
		if (token == operand)	// 피연산자라면 출력.
			expr_post[count++] = symbol;

		else if (token == rparen)
		{			// 폐괄호라면.
			while ( FindTopV() != lparen)	// 스택탑이 개괄호 전까지 
				print_token(Pop(), &expr_post); // 스택의 내용을 차례대로 출력.
			Pop();	// 스택의 탑 , 즉 개괄호는 버린다.
		}
		else {
			// 폐괄호가 아니라면
			// 연산자 우선순위를 검사.
			// 들어오는 쪽이 스택안의 것보다 우선순위가 높거나 같을때 까지.
			while (isp[FindTopV()] > icp[token])
				print_token(Pop(), &expr_post); // 스택내용을 출력.
			Push(token);			// 들어갈려하는 토큰 삽입.
		}
	}
	// 나머지 스택 내의 내용들을 문자열에 저장.
	while ((token = Pop()) != eos && cnt > -1)
		print_token(token, &expr_post);

	// 후위 표현 출력.
	printf("postfix expression : %s \n", expr_post);
	strcpy(expr, expr_post);	// 전역변수에 저장.(계산)
}

int eval(void)
{
	precedence token;	// 토큰
	char symbol;	// 문자
	int	op1, op2;	// 피연산자 1 ,2
	int	n = 0;		// 문자열의 인덱스

	// 토큰을 받아옴
	token = get_token(&symbol, &n);
	while (token != eos)
	{
		// 피연산자라면 문자를 숫자로 변환해서 스택에 넣는다.
		if (token == operand)
			Push(symbol - '0');/*문자로된 값을 숫자로 변환*/
		else{
			// symbol이 연산자라면
			// 스택에 저장 해놨던 피연산자를 꺼내온다.
			// 예를 들어 a b c + d * - 에서 ‘+’ 연산 을 하는 피연산자는
			// 스택 탑과 바로 밑의 피연산자이다.
			// 스택에는 밑에서 부터 a , b , c 상으로 저장 되어있는데.
			// c + b 가 아니라 b + c 이기 때문에 역순으로 받아 와야 한다.
			op2 = Pop();  // 두번째 피연산자.
			op1 = Pop();  // 첫번째 피연산자.
			switch (token)
			{	// 해당연산자에 맞게 연산하여 그 결과값을 스택에 저장.
			case plus: Push(op1 + op2);	break;
			case minus: Push(op1 - op2); break;
			case times:	Push(op1 * op2); break;
			case divide: Push(op1 / op2); break;
			case mod: Push(op1 % op2); break;

			default: break;
			}
		}
		// 연산후 해당 문자의 토큰값을 받아온다
		token = get_token(&symbol, &n);
	}
	// 최종 결과 값을 반환.
	return Pop(); // 결과 반환
}


