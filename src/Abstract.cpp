/*
 * Abstract.cpp
 *
 *  Created on: 04 февр. 2014 г.
 *      Author: maxim
 */

#include "Abstract.h"
using namespace std;

Object::Object( int id = -1 ):id ( id ) { }

Object::Object( const Object& T) : id(T.id) { }

void Object::print(){
	cout << endl;
	cout << "Вывод объекта" << endl;
	initialize_print();
}

void Object::print_all()
{
	cout << endl;
	cout << "Вывод всех объектов: " << endl;
	initialize_print_all();
}



