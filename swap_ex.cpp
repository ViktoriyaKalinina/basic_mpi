#include <iostream>
#include <cmath>
#include <stdio.h>
#include <vector>
#include <stdio.h>

using namespace std;
 
int main()
{
  vector <int> x, y; /// Создаем два вектора
  {
      x = { 1, 2 }; /// заполняем первый вектор
      y = { 3, 4 }; /// заполняем второй вектор
  }

	cout << "x: ";
  for (int i=0; i<x.size(); ++i)
  cout << x[i] << ends;
 
  cout << "\ny: ";
  for (int i=0; i<y.size(); ++i)
  cout << y[i] << ends;

	x.swap(y);

  cout << "\nnew x: ";
  for (int i=0; i<x.size(); ++i)
  cout << x[i] << ends;
 
  cout << "\n new y: ";
  for (int i=0; i<y.size(); ++i)
  cout << y[i] << ends;
}