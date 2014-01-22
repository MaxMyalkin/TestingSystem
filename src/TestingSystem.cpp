#include <iostream>
#include "Test.h"
#include "Variant.h"
#include <string.h>
#include <fstream>
#include "TestingSystem.pb.h"
#include "Group.h"
#include "time.h"

using namespace std;

int main() {
	freopen("errors", "w", stderr);
	srand(time(NULL));
	Configuration::configuration->Instance("questions1.pb", "answer1.pb", "students1.pb", "groups1.pb", "results1.pb");
	//1.    Возможность изменения конфигурационного файла.
	Configuration::configuration->Instance("questions.pb", "answers.pb", "students.pb", "groups.pb", "results.pb");
	//2.	Возможность добавления вопроса.
	Question q;
	cout << "-------------------------------------Ввод вопроса: -----------------------------------------------" << endl;
	cin >> q;	//Перегрузка оператора ввода.
	q.add();	//Использование метода add для добавления в файл.
	q.add("TCP/IP", "How many levels does TCP/IP have? "); 	//Добавление в файл методом add
	Question q1("RIP", "What metric does protocol RIP have?");
	q1.add();
	getchar();getchar();
	//3.	Печать всех объектов
	cout << "-----------------------------------Печать всех вопросов: -----------------------------------------" << endl;
	q.print_all();
	getchar();getchar();
	//4.	Получение и печать объекта
	cout << "----------------------------------Получение вопроса по ID: --------------------------------------" << endl;
	q.get(1);
	cout << "---------------------------------Печать информации об объекте: ----------------------------------" << endl;
	q.print();		//Печать для просмотра информации об объекте, как объекте БД
	getchar();getchar();
	cout << "---------------------------------Печать в пользовательском формате: -----------------------------" << endl;
	cout << q;		//Печать в пользовательском формате
	//5.	Обновление информации об объекте
	getchar();getchar();
	cout << "--------------------------------Обновление информации о вопросе: -------------------------------" << endl;
	q.update("Routing Protocols", "What protocols in this list are routing?");
	q.print_all();
	getchar();getchar();
	//Аналогичные действия могут быть произведены со всеми объектами, имеющими возможность работы с файлами
	cout << "--------------------------------Добавление ответов: ---------------------------------------------" << endl;
	Answer a;
	//Добавление ответа на 0 вопрос
	a.add("Only one", false, 0);
	a.add("Two", true, 0);
	//Добавим ответы на другие вопросы
	a.add("seconds per transmission 100 Mbit of information", true, 2);
	a.add("length of cable between stations", false, 2);
	a.add("there is no metric in RIP", false, 2);

	Answer a1;
	//Вопрос с множественными ответами
	a1.set_contents("RIP");
	a1.set_correct(true);
	a1.set_q_id(3);			//После обновления вопрос стал последним, значит его ID = 3
	a1.add();
	a1.add("ICMP", false, 3);
	a1.add("OSPF", true, 3);

	//Выведем все ответы.

	cout << "-------------------------------Вывод всех ответов в режиме дебага -------------------------------" << endl;
	a.print_all();
	getchar();getchar();
	//Также существует возможность получить вектор ответов на вопрос.
	cout << "-------------------------------Вывод вектора ответов на вопрос: ---------------------------------" << endl;
	vector <Answer> answers;
	answers = q.get_answers();
	cout << "Ответы на вопрос Routing Protocol: " << endl;
	for (int i = 0; i < answers.size(); i++)
	{
		cout << answers[i];
		cout << endl;
	}
	getchar();getchar();
	cout << "-----------------------------------Создание группы и студентов: -----------------------------" << endl;
	//Теперь создадим группы и студентов.
	Group g("IU5-52");
	g.add();
	Student st;
	st.add("Myalkin", 3, g.getID());
	st.add("Abashin", 3, g.getID());
	st.add("Chernobrovkin", 3, g.getID());

	//Теперь создадим тест.
	cout << "------------------------------------Создание теста: -----------------------------------------" << endl;
	Test t;
	t.get_questions();		//Тест выступает в роли хранилища всех вопросов.
	t.set_count(3);			//Установка количества вопросов в каждом варианте.
	t.print();
	getchar();getchar();
	cout << "-----------------------------------Тестирование первого студента: -------------------------------------------" << endl;
	//Теперь нужно собрать варианты по тесту.
	Variant v1;
	v1 = get_variant(t);	//Получение варианта по рандомным вопросам.
	st.get("Abashin");
	v1.TestMePlease(st);
	getchar();getchar();
	cout << "----------------------------------Тестирование второго студента: -----------------------------------------" << endl;
	Variant v2 = get_variant(t);
	st.get("Myalkin");
	v2.TestMePlease(st);
	getchar();getchar();
	//Вывод результатов
	cout << "---------------------------------Вывод результатов: ---------------------------------------" << endl;
	cout << "---------------------------------Результаты по группе: -------------------------------------" << endl;
	g.get("IU5-52");
	group_result(g);
	getchar();getchar();
	cout << "---------------------------------Все результаты: -------------------------------------------" << endl;
	all_results();
	getchar();getchar();
	cout << "---------------------------------Результаты студента: -------------------------------------------" << endl;
	st.get("Abashin");
	student_result(st);
	getchar();getchar();
	return 0;
}
