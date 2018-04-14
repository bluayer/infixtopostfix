#include<st#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EXPR_SIZE	100		// 수식 최대 사이즈
int isp[] = { 0, 19, 12, 12, 13, 13, 0 };
int icp[] = { 20, 19, 12, 12, 13, 13, 0 };
char expr[MAX_EXPR_SIZE];		// 수식을 저장하는 문자배열.

typedef enum {
	lparen, rparen, plus, minus, times, divide, eos, operand
} precedence; // 우선순위

typedef struct Node //노드 정의
{
	double data;
	struct Node *next;
}Node;

typedef struct{
	Node *top;
}Stack;


void InitStack(Stack *stack);
int IsEmpty(Stack *stack);
void Push(Stack *stack,double data);	// 스택 푸쉬.
double Pop(Stack *stack);				// 스택 팝
void postfix(void);					// 수식을 후위 표현으로 변환
precedence get_token(char* symbol, int* n);
void print_token(precedence token, char** str);
double eval(void);
double TopV(Stack *stack);


int main()
{
	Stack s;
	InitStack(&s);
	printf("----------------------------------\n");
	printf("|   Converting infix to postfix   |\n");
	printf("----------------------------------\n\n");
	printf("수식을 입력하세요\nInput : ");
	scanf("%s", expr);	// 수식 입력 받음

						
	postfix();			// 후위표기로 변환

	printf("\n\n결과 : %lf \n\n", eval()); // 후위식을 계산.
	system("pause");
	return 0;
}

int IsEmpty(Stack *stack)
{
	return stack->top == NULL;
}

void InitStack(Stack *stack)
{
	stack->top = NULL; //스택 초기화에서는 top을 NULL로 설정
}

void Push(Stack *stack, double data)
{
	Node *now = (Node *)calloc(sizeof(Node), 1); // 노드 생성

	if (IsEmpty(stack)) {
		stack->top = now;
		now->data = data; // 데이터 설정
	}
	else {
		now->data = data; // 데이터 설정
		now->next = stack->top;
		stack->top = now;
	}
}

double Pop(Stack *stack)
{
	Node *now;
	double re;
	if (IsEmpty(stack))
	{
		return eos;
	}

	now = stack->top;//now를 top으로 설정
	
	re = now->data;//꺼낼 값은 now의 data로 설정
	stack->top = stack->top->next;

	free(now);//필요없으니 메모리 해제
	return re;
}

double TopV(Stack *stack)
{
	Node *now;
	double re;
	if (IsEmpty(stack))
	{
		return eos;
	}

	now = stack->top;//now를 top으로 설정
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
	case '\0': return eos;

	default: return operand;
	}
}
// 해당 토큰에 해당하는 기호(연산자)로 문자로 삽입.
void print_token(precedence token, char** str)
{
	switch (token)
	{
	case plus: *(*str) = '+';	return;       // +
	case minus: *(*str) = '-'; return;		  // -
	case divide: *(*str) = '/'; return;		  // /
	case times:	*(*str) = '*'; return;        // *

	case eos: return;
	default: return;
	}
}

void postfix(void)
{	
	Stack st;
	char symbol;				// 연산자, 피연산자 
	char expr_post[MAX_EXPR_SIZE] = { 0, }; // 후위 표기식으로 저장하는 문자열.
	char* str = expr_post;
	precedence token;			// 토큰을 받는 변수
	int n = 0;	// 문자열의 인덱스

	InitStack(&st);
	// 중위 -> 후위 변환 알고리즘
	// 토큰이 eos, 즉 문자의 끝 일때 까지 반복
	for (token = get_token(&symbol, &n); token != eos; token = get_token(&symbol, &n))
	{
		if (token == operand) {	// 피연산자라면 출력.
			*str++ = symbol;
		}
		else if (token == rparen)
		{			// 폐괄호라면.
			while ( TopV(&st) != lparen) {	// 스택탑이 개괄호 전까지 
				print_token(Pop(&st), &str);
				str++;
				*str = ' ';
				// 스택의 내용을 차례대로 출력.
			}
			Pop(&st);	// 스택의 탑 , 즉 개괄호는 버린다.
			str++;
		}
		else {
			*str = ' ';
			str++;
			// 폐괄호가 아니라면
			// 연산자 우선순위를 검사.
			// 들어오는 쪽이 스택안의 것보다 우선순위가 높거나 같을때 까지.
			while (isp[(char)(TopV(&st))] >= icp[token]) {
				print_token(Pop(&st), &str); // 스택내용을 출력.
				str++;
			}
			Push(&st,token);			// 들어갈려하는 토큰 삽입.
		}
	}
	// 나머지 스택 내의 내용들을 문자열에 저장.
	while (!IsEmpty(&st)) {
		*str = ' ';
		str++;
		print_token(Pop(&st), &str);
		str++;
	}

	*str = '\0';
		
	// 후위 표현 출력.
	printf("postfix expression : %s \n", expr_post);
	strcpy(expr, expr_post);	// 전역변수에 저장.(계산)
}


double eval(void)
{	
	Stack st;
	double value =0.0;
	char *ppostfix = expr;
	double	op1 = 0.0, op2 = 0.0;	// 피연산자 1 ,2
	int	n = 0;		// 문자열의 인덱스

		while (*ppostfix != '\0') {
			// 피연산자라면 문자를 숫자로 변환해서 스택에 넣는다.		
			if (isspace(*ppostfix)) {
					ppostfix++;
					continue;
			}
			if (isdigit(*ppostfix)) {
				value = strtod(ppostfix, &ppostfix);
				Push(&st, value);
			}
			else if (*ppostfix == '+' || *ppostfix == '-' || *ppostfix == '*' || *ppostfix == '/') {

				op2 = Pop(&st);  // 두번째 피연산자.
				op1 = Pop(&st);  // 첫번째 피연산자.
				switch (*ppostfix)
				{	// 해당연산자에 맞게 연산하여 그 결과값을 스택에 저장.
				case '+': Push(&st, op1 + op2);	break;
				case '-': Push(&st, op1 - op2); break;
				case '*': Push(&st, op1 * op2); break;
				case '/': Push(&st, op1 / op2); break;
				}
				ppostfix++;
			}
			// 연산후 해당 문자의 토큰값을 받아온다
		}
	// 최종 결과 값을 반환.
	return Pop(&st); // 결과 반환
}
