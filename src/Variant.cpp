/*
 * Variant.cpp
 *
 *  Created on: 04 февр. 2014 г.
 *      Author: maxim
 */
#include "Variant.h"
using namespace std;
#define SIZE_ERR 5

int Variant::TestMePlease(Student stud)
{
	int point = 0;
	int ans;
	int k = 10;
	for (int i = 0; i < (int)questions.size(); i++)
	{
		cout << endl << "Вопрос:"<<endl;
		cout<<questions[i];
		vector <Answer> answers = questions[i].get_answers();
		cout<<"Варианты ответа:"<<endl;
		for (int j = 0; j < static_cast<int>(answers.size()); j++)
		{
			cout<<j+1<<") "<< answers[j];
		}
		string s;
		getline(cin, s);
		int j = 0;
		vector <int> student_answers;
		while (j < s.length())
		{
			string new_s = "";
			while ((s[j] < '0') || (s[j] > '9'))
				j++;
			while (!((s[j] < '0') || (s[j] > '9')))
			{
				new_s += s[j];
				j++;
			}
			ans = atoi(new_s.c_str());
			if (ans != 0 && !has(student_answers.begin(), student_answers.end(), ans))
			{
				student_answers.push_back(ans);
			}
		}
		int correct_answers = 0;
		for (int j = 0; j < answers.size(); j++)
		{
			if (answers[j].get_correct())
				correct_answers++;
		}
		if (correct_answers != student_answers.size())
			continue;
		int iter;
		for (iter = 0; iter < student_answers.size(); iter++)
		{
			if (student_answers[iter] >= 1 && !answers[student_answers[iter] - 1].get_correct())
				break;
		}
		if  (iter == student_answers.size())
			point += k;
	}
	cout.setf(ios::fixed);
	cout.precision(1);
	float percent = (float)point * 100/(float)(k*questions.size());
	int mark = -1;
	if (percent >= 90)
		mark = 5;
	else
	if (percent >= 75)
		mark = 4;
	else
	if (percent >= 60)
		mark = 3;
	else
		mark = 2;
	cout <<"Вы набрали "<< point <<" очков, правильных ответов " << percent << "%" << " Оценка: " << mark;
	add_result(stud, point, mark);
	return point;
}

Variant::Variant(const Variant& V)
{
	number = V.number;
	questions = V.questions;
}

void Variant::print()
{
	for (vector<Question>::iterator iter = questions.begin(); iter != questions.end(); ++iter)
			iter->print();
};

Variant get_variant(Test T)
{
	vector <pair < Question, bool > > test_questions(T.get_test_size());
	for(int i = 0; i < T.get_test_size(); i++)
	{
		test_questions[i].second = false;
		test_questions[i].first = T[i];

	}
	Variant v;
	if (!T.compare_size())
	{
		//Исключение
	}
	else
	{
		for (int j = 0; j < T.get_variant_size();)
		{
			int q = rand()%(T.get_test_size());
			if (test_questions[q].second == false)
			{
				v.getVector().push_back(test_questions[q].first);
				test_questions[q].second = true;
				j++;
			}
		}
	}
	return v;
};

bool has(const vector <int> ::iterator begin , const vector <int> ::iterator end, int key)
{
	for (vector<int>::iterator i = begin; i!= end; i++)
		if (*i == key)
			return true;
	return false;
}



