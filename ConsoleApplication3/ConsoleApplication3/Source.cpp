//Заменить все положительные числа квадратом минимального числа.
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <list>   
#include <sstream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <fstream>
#include <cstdio>
#include <time.h>
#include <cstring>
using namespace std;

int diapozon = 10;

class MultValue
{
private:
	int Factor;   
public:
	MultValue(const int& _Val) : Factor(_Val) {
	}

	int operator ( ) (int& elem) const
	{
		if (elem > 0)
			elem = Factor*Factor;
		return elem ;
	}
};

int Random()
{
	return rand() % (2 * diapozon + 1) - diapozon;
}

// проверка корректности символа в имени файла
bool CorrectSymbol(char c)
{
	switch (c)
	{
		case '.': 
		case '*': 
		case '?': 
		case '<': 
		case '>': 
		case ':': 
		case '|': 
			return false;
		default: 
			return true;
	}
}


// проверка проверка корректности имени
bool CorrectName(char *name)
{
	int i = 1;
	bool goodsm=false;
	if (name[0] != '\0')
	{
		goodsm = CorrectSymbol(name[0]);
		while (name[i] != '\0' && goodsm)
			goodsm = CorrectSymbol(name[i++]);
	}

	return goodsm;
}

//ввод имени файла
char* InputFileName()
{
	char *filename;
    filename= new char[45];

	do
	{
		cout << "Введите имя файла(без расширения):  ";
		cin >> filename;
		if (CorrectName(filename))
		{
			strcat(filename, ".txt");
			break;
		}
		else
			cout << "Файл не удалось создать\n";
	} while (true);

	return filename;
}

// заполнение файла случайными числами
FILE *LoadInTextFile(int &N, int &M)
{
	FILE *file;
	char* filename=InputFileName();

	file = fopen(filename, "w+");

	srand((unsigned int)time(0));

	fstream in(file);
	for (int i = 1;i <= N;i++)
	{
		in << rand() % (2 * M + 1) - M << ' ';
	}
	cout << "Файл создан!" << endl;
	return file;
}

// заполнение файла случайными числами через generate
FILE *LoadInTextFileInGenerate(int &N, int &M)
{
	FILE *file;
	char* filename = InputFileName();

	file = fopen(filename, "w+");
	fstream in(file);

	srand((unsigned int)time(0));

	list<int> numbers(N);
	diapozon = M;
	generate(numbers.begin(), numbers.end(), Random);

	copy(numbers.begin(), numbers.end(), ostream_iterator<int>(in, " "));
	cout << "Файл создан!" << endl;
	return file;
}

// загрузка чисел из файла в список
list<int> LoadFromFile(FILE *File)
{
	ifstream in(File);
	in.seekg(0);

	int x;
	list <int> numbers;
	if (!in.eof())
	{
		in >> x;

	}
	while (!in.eof()) 
	{
		numbers.push_back(x);
		in >> x;
	}
	in.close();
	return numbers;
}

// печать списка в файл
void OutputFile(list<int> &items)
{
	char* newfname = InputFileName();
	ofstream fout(newfname);
	if (fout)
	{
		list<int>::const_iterator pos;

		for (pos = items.begin(); pos != items.end(); ++pos)
		{
			fout << *pos << ' ';
		}
		cout << endl << "Файл выведен!" << endl;
		fout.close();
	}
	else
	{
		cout << "Ошибка открытия файла!" << endl;
	}
}

// печать списка на экран
void OutputScreen(list<int> &items)
{
	list<int>::const_iterator pos;

	for (pos = items.begin(); pos != items.end(); ++pos) 
	{
		cout << *pos << ' ';
	}
	cout << endl << "Список выведен!" << endl;
}

// поиск минимального элемента в списке
int SearchMin(list<int> &numbers)
{
	int min = numbers.front();
	for (auto iter = ++numbers.begin(); iter != numbers.end(); iter++)
	{
		if (*iter < min)
			min = *iter;
	}
	return min;
}


// поиск минимального элемента в списке (через итераторы)
int SearchMin(list<int>::iterator begin, list<int>::iterator end)
{
	int min = *begin;
	for (auto iter = ++begin; iter != end; iter++)
	{
		if (*iter < min)
			min = *iter;
	}
	return min;
}

// функция modify для изменения списка
list<int> Modify(list<int> &numbers)
{
	if (numbers.empty())
		return numbers;
	list<int> numbers2;
	int min = SearchMin(numbers);
	int sqmn = min*min;
	for (auto iter = numbers.begin(); iter != numbers.end(); iter++)
	{
		if (*iter > 0)
		{
			*iter = sqmn;
		}
		numbers2.push_back(*iter);
	}
	return numbers2;
}

// перегруженная функция modify для изменения списка
list<int> Modify(list<int>:: iterator begin, list<int>::iterator end)
{
	list<int> numbers2;
	int min = SearchMin(begin, end);
	int sqmn = min*min;
	for (auto iter = begin; iter != end; iter++)
	{
		if (*iter > 0)
		{
			*iter = sqmn;
		}
		numbers2.push_back(*iter);
	}
	return numbers2;
}

// функция  для изменения списка через transform
list<int> TransformList(list<int> &numbers)
{
	int min = SearchMin(numbers);
	list<int> numbers2;
	for (auto iter = numbers.begin(); iter != numbers.end(); iter++)
	{
		numbers2.push_back(*iter);
	}
	transform(numbers2.begin(), numbers2.end(), numbers2.begin(), MultValue(min));
	return numbers2;  //?
}

// функция  для изменения списка через for_each
list<int> ForEachList(list<int> &numbers)
{
	int min = SearchMin(numbers);
	list<int> numbers2;
	for (auto iter = numbers.begin(); iter != numbers.end(); iter++)
	{
		numbers2.push_back(*iter);
	}
	for_each(numbers.begin(), numbers.end(),  MultValue(min));
    return numbers2;  //?
}

// функция  для вычисления суммы
int Sum(list<int> &numbers)
{
	if (numbers.empty())
	{
		cout << "Список пустой" << endl;
		return 0;
	}
	int sum = 0;
	for (auto iter = numbers.begin(); iter != numbers.end(); iter++)
	{
		sum += *iter;
	}
	return sum;
}

// функция  для вычисления среднего арифметоческого
double Average(list<int> &numbers)
{
	if (numbers.empty())
	{
		cout << "Список пустой" << endl;
		return 0;
	}
	else
	{
		int sums = Sum(numbers);
		int col = numbers.size();
		return (double)sums / col;
	}
}

int InputNumber(int min, int max)
{
	int n;
	do
	{
		cout << "Ваш выбор: ";
		while ((!(cin >> n)) || cin.get() != '\n')
		{
			cout << "Неправильный ввод !!  Повторите снова!\n";
			cin.clear(); // сбрасывает все биты потока, тоесть поток становится "хорошим"
			cin.sync();//Удалим данные из буффера
			cout << "Ваш выбор : ";
			getchar();
			getchar();
		}
	} while ((n < min) || (n > max));
	return n;
}

int Menu()
{
	cout << " --------------------Меню---------------------  " << endl;
	cout << "1 - Modify(преобразование списка )" << endl;
	cout << "2 - Modify(преобразование списка - перегруженный вариант) " << endl;
	cout << "3 - Преобразование с помощью transform" << endl;
	cout << "4 - Преобразование с помощью for_each" << endl;
	cout << "5 - Посчитать сумму" << endl;
	cout << "6 - Посчитать среднее арифметическое" << endl;
	cout << "7 - Вывод на экран " << endl;
	cout << "8 - Вывод в файл" << endl;
	cout << "0 - Выход " << endl;
	int n = InputNumber(0, 8);
	cout << "\n";
	return n;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	FILE *file;
	list<int> numbers;
	int N = 10;
	int M = 50;
	char filename[40];
	char newfname[40];
	int changemenu;
	bool enterfile = true;

	cout << " --------------------Меню---------------------  " << endl;
	cout << "1 - Заполнение текстового файла случайными числами(через цикл)" << endl;
	cout << "2 - Заполнение текстового файла случайными числами(через generate) " << endl;
	cout << "0 - Выход" << endl;
	changemenu = InputNumber(0, 2);
	switch (changemenu)
	{
		case 1:
			file = LoadInTextFile( N, M);
			break;

		case 2:
			file = LoadInTextFileInGenerate( N, M);
			break;

		default:
			enterfile=false;
			break;
	}
	if (enterfile)
	{
		numbers = LoadFromFile(file);
		for (;;)
		{
			int n = Menu();
			switch (n)
			{
				case 1:
					numbers = Modify(numbers);
				break;

				case 2:
					numbers = Modify(numbers.begin(), numbers.end());
				break;

				case 3:
					numbers = TransformList(numbers);
				break;

				case 4:
					numbers = ForEachList(numbers);
				break;

				case 5:
					cout << "Сумма: " << Sum(numbers) << endl;
				break;

				case 6:
					cout << "Среднее арифметическое: " << Average(numbers) << endl;
				break;

				case 7:
					cout << "Список: " << endl;
					OutputScreen(numbers);
				break;

				case 8:
					OutputFile(numbers);
				break;

				default:
					cout << "Выход " << endl; exit(0);
				break;

			}
		}
	}
	else
		cout << "Досвидания\n";
	system("pause");
}




