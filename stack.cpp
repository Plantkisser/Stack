
/*
	В целом уточнение. Открывать-закрывать логфайл каждый раз -
	дорого. Ну то есть функция fopen() дорогая. Посему лучше все-таки
	в init() открывать и в destruct() закрывать. И тип прописать в 
	документации, что надо обязательно вызывать init() и destroy(),
	иначе сам дурак, если ломается :)
 */


#include <stdio.h>
#include <stdlib.h>

/*
	Тут уж явно можно не сокращать буквы)
 */
# define ERROR 535
# define CPCTY 1
# define KAN 1987
# define WERROR 365
# define TEST 7

# define UNITTEST(what, op, ref, type) \
{\
	type result = what;\
	type refcpy = ref;\
	if (result op refcpy)\
		printf (#what" [passed]\n");\
	else\
	{\
		printf (#what "is ");\
		print_##type(result);\
		printf ("but %s be ", result);\
		print_##type(refcpy);\
		printf("\n");\
	}\
}

/*
	Запили тут тайпдеф, чтоб у тебя был тип stk_elem
	Ну или что-то в этом роде
 */

struct stack
{
	int kan1;
	int *data;
	int size;
	int capacity;
	int kan2;
};

void print_int(int a)
{
	printf("%d ", a);
	return; 
}
void print_double(double a)
{
	printf("%lf ", a);
	return; 
}

/** compare canary with constant KAN
*/

int check_stack(stack* test)
{
	int i = 0;
/*
	Не лучше ли?

	FILE* f = fopen("log.txt", "a");
 */
	FILE * f;
	f = fopen("log.txt", "a");
/*
	Проверку бы сюда
 */
/*
	Что-то мне подсказывает, что строчка длинновата)
 */
	fprintf(f, "Stack text\n{\n\tsize=%d\n\tcount=%d\n\tdata[%d]\n\t{\n", test->capacity, test->size, test->capacity);
	for (i = 1; i <= test->capacity; i++)
		fprintf(f, "\t\t[%d] : %d\n", i, test->data[i]);

	fprintf(f, "\t}\n}\n");
/*
	Кажется, если ошибка, можно прям тут сигнализировать в логи.
	И даже если нет, то ты выходишь и не закрываешь файл, а зря.
 */
	if (test->data[0] != KAN)
		return ERROR;
	if (test->data[test->capacity + 1] != KAN)
		return ERROR;
	if (test->capacity <= test->size)
	{
		fprintf(f,"capacity less then data pointer\n");
		return ERROR;
	}	
	fclose(f);
	return 0;
}

/*
	Оычно такие функции называют isnull(). То есть isсвойство()
 */
/** compare pointer with NULL
*/

int checkptr (stack * ptr)
{
	if (ptr == NULL)
	{
		printf("Pointer test is NULL"); //log
		return ERROR;
	}
	return 0;
}

/** increase size of stack by multiplying on 2
*/

int incrcapacity(stack *test)
{
	int rezch = check_stack(test);
	if (rezch == ERROR)
	{
/*
	Про сие уже говорил, кажется)
	Сразу инициализировать
 */
		FILE * f;
		f = fopen("log.txt", "a");
/*
	Проверкуууу
 */
		fprintf(f, "Problem with check_stack\n");
		fclose(f);
	}
	
	
	if (checkptr(test) == ERROR) return ERROR;
	int *newdata =(int*) calloc(test->capacity * 2 + 2, sizeof(int));
/*
	Проверкаааааа :)
 */
	test->capacity = test->capacity * 2;
	int i = 0;

/*
	Вообще говоря, если realloc для этого. Но я им не пользовался...)
 */
	for (i = 0; i <= test->size; i++)
	{
		newdata[i] = test->data[i];
	}
	newdata[test->capacity + 1] = KAN; 

	free(test->data);
	test->data = newdata;
	return 0;
}

/** add a number with type int
*/

/*
	То же, что и для incrcapacity
 */
int push_back(stack *test, int a)
{
	int rezch = check_stack(test);
	if (rezch == ERROR)
	{
		FILE * f;
		f = fopen("log.txt", "a");
		fprintf(f, "Problem with check_stack\n");
		fclose(f);
		return WERROR;
	}
	
	
	if (checkptr(test) == ERROR) return ERROR;
	if (test->capacity <= test->size )
	{
		int rez = incrcapacity(test); // log
		if (rez == ERROR) 
		{
			FILE *f;
			f = fopen ("log.txt", "a");
			fprintf(f, "Buy new processor\n");
			fclose(f);	
		}
	}
	test->size++;
	test->data[test->size] = a;
	
	rezch = check_stack(test);
	if (rezch == ERROR)
	{
		FILE * f;
		f = fopen("log.txt", "a");
		fprintf(f, "Problem with check_stack\n");
		fclose(f);
		return WERROR;
	}
	
	return 0;
}

/** delete the last cell and return cells value
*/

/*
	Вообще говоря, надо бы сжимать стек, когда там мало остается. 
	Подсказка - переделать incrcapacity в changecapacity)
 */
int pop (stack *test)
{
	if (checkptr(test) == ERROR) return ERROR;
	int a = test->data[test->size];
	test->size--;
	return a;
}

/** print all cells value
*/

int print_data(stack* test)
{
	if (checkptr(test) == ERROR) return ERROR;
	int i = 0;
	for (i = 1; i <= test->size; i++)
	{
		printf("%d\n", test->data[i]);
	}
	return 0;
}

/** open file "input.txt" and read all values
WARNING value must be type int!!!
after reading print it on screen
all errors and values of stack will be written in log.txt 
*/
int main ()
{
	stack test;
/*
	Для этого делается оберточка init():

	int init(stack* stk);
 */
	test.kan1 = KAN;
	test.kan2 = KAN;
	test.size = 0;
	test.capacity = CPCTY;
	test.data = (int*) calloc (test.capacity + 2, sizeof(int));
	test.data[0] = KAN;
	test.data[test.capacity+1] = KAN;
	if (test.data == NULL) 
	{
		FILE * f1;
		f1 = fopen("log.txt", "a");
		fprintf(f1, "Buy new processor\n");
		fclose(f1);
		return 0;
	}
	
	
	FILE *f;
	f = fopen("input.txt", "r");
	if (f == NULL)
	{
		FILE * f1;
		f1 = fopen("log.txt", "a");
		fprintf(f1, "Cannot open file fo reading\n");
		fclose(f1);
		return 0;
	}
	int a = 0;
	int rez = 0;
	while (fscanf(f, "%d", &a) != -1)
	{
		rez = push_back(&test, a);
		if (rez == ERROR)
		{
			FILE *f1;
			f1 = fopen("log.txt", "a");
			fprintf(f1, "Problem with push_back\n");
			fclose(f);
		}
	}
	rez = push_back(&test, TEST);
	UNITTEST(pop(&test), ==, TEST, int);
	int b = pop(&test);
	printf("%d\n", b);
	print_data(&test);
	fclose(f);
	free(test.data);
	return 0;
}


