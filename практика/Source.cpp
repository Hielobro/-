#include <cstdlib>
#include <iostream>// system
#include <cmath>
#include <vector>
using namespace std;

class Point//класс точки
{
public:
	Point(float a = 0, float b = 0)//конструктор инициализирующий координаты
	{
		x = a;
		y = b;
	}
	bool operator== (Point b)
	{
		if (this->x == b.x && this->y == b.y)
		{
			return true;
		}
		else return false;
	}
	~Point() {};
	inline void Set_x(float xx) { x = xx; }// функция установки координаты по х 
	inline void Set_y(float yy) { y = yy; }// функция установки координаты по y 
	inline void print_point() {cout << x << "." << y;}
private:
protected:
public:
//	vector <Point> neighbors[4];//массив соседей точки [0] левый сосед [1] верхний [2] правый [3] нижний
	float x;
	float y;
};

class Section//класс отрезка
{
public:
	Section()// пустой конструктор
	{
		a.Set_x(0);
		a.Set_y(0);
		b.Set_x(0);
		b.Set_y(0);
		length = 0;
	}
	Section(Point aa, Point bb)// конструктор по точкам
	{
		a = aa;
		b = bb;
		init_length();
	}
	inline void init_length()//функция установки длины отрезка
	{
		length = sqrt(pow((a.x - b.x), float(2)) + pow((a.y - b.y), float(2)));
	}
	~Section() {};

public:
	Point a;
	Point b;
	float length;// длина отрезка
};

class Sqad//класс квадрата
{
public:

	Sqad(Point* mass) //конструктор квадрата по массиву точек
	{
		for (int i = 1; i < 4; i++)
		{
			points[i] = mass[i];
		}
		init_sections();
		init_center();
	}
	Sqad(Point first, Point second, Point third, Point fourth) //конструктор по 4 точкам
	{
		points[0] = first;
		points[1] = second;
		points[2] = third;
		points[3] = fourth;
		init_sections();
		init_center();
	}
	void init_sections()
	{
		sections[0] = Section(points[0], points[1]);
		sections[1] = Section(points[1], points[2]);
		sections[2] = Section(points[2], points[3]);
		sections[3] = Section(points[3], points[1]);
		sidelength = sections[0].length;
	}
	void init_center()
	{
		float x;
		float y;
		x = sections[0].a.x + (sidelength / 2);
		y = sections[0].a.y + (sidelength / 2);
		center = Point(x, y);
	}

	~Sqad() {};

private:
protected:
public:
	Point points[4];
	Point center;
	Section sections[4];
	float sidelength;
};

class Bigpart//класс прямоугольника
{
public:
	Bigpart(Point a, Point b) //конструктор прямоугольника по нижней левой точке и верхней правой точке
	{
		points[0] = a;
		points[1] = Point(a.x, b.y);
		points[2] = b;
		points[3] = Point(b.x, a.y);
		//init_sections();
		init_length();
		init_center();
		init_point_matrix();
		is_divided = false;
	}
	Bigpart(Point a, Point b, Point c, Point d) //конструктор прямоугольника по 4 точкам
	{
		points[0] = a;
		points[1] = b;
		points[2] = c;
		points[3] = d;
		//init_sections();
		init_length();
		init_center();
		init_point_matrix();
		is_divided = false;
	}

	void init_center()
	{
		float x;
		float y;
		x = points[0].x + (horizontallength / 2);
		y = points[0].y + (verticallength / 2);
		center = Point(x, y);
		eps = return_l(points[0], center);
	}
	/*
	void init_sections()// заполнение массивов отрезков
	{
		sections[0] = Section(points[0], points[1]);
		sections[1] = Section(points[1], points[2]);
		sections[2] = Section(points[2], points[3]);
		sections[3] = Section(points[3], points[1]);
	}
*/	
	inline float  return_l(Point a, Point b){return sqrt(pow((a.x - b.x), float(2)) + pow((a.y - b.y), float(2)));}
	void init_length()//определение длин сторон прямоугольника
	{
		verticallength = return_l(points[0], points[1]); //sections[0].length;
		horizontallength = return_l(points[1], points[2]); //sections[0].length; // sections[1].length;
	}
	
	void init_point_matrix()//функция начального заполнения матрицы с точками прямоугольника
	{
		point_matrix.resize(2);
		point_matrix[0].resize(2);
		point_matrix[1].resize(2);
		point_matrix[0][0] = points[1];
		point_matrix[0][1] = points[2];
		point_matrix[1][0] = points[0];
		point_matrix[1][1] = points[3];
	}



	void divide_self_points(int number)// функция разделения сторон прямоугольника и получения новой матрицы с точками
	{
		vector<Point>::iterator it;
		float step = sqrt(pow((point_matrix[0][1].x - point_matrix[0][0].x), float(2)) + pow((point_matrix[0][1].y - point_matrix[0][0].y), float(2)))/number;
		for (int i = 0; i < point_matrix.size(); i++)
		{
			int size_before = point_matrix[i].size();
			for (int j = 0; j < size_before -1; j ++)
			{
				for (int k = 1; k < number; k++)
				{
				it= point_matrix[i].begin();
				float incr = step * k;
				point_matrix[i].insert(it + j * number + k, Point((point_matrix[i][j*number].x + incr), point_matrix[i][j].y));
				}
			}
			
		}
		step = sqrt(pow((point_matrix[1][0].x - point_matrix[0][0].x), float(2)) + pow((point_matrix[1][0].y - point_matrix[0][0].y), float(2))) / number;
		int size_before = point_matrix.size();
		int rowsize= point_matrix[0].size();
		vector< vector<Point>>::iterator it2;
		for (int i = 0; i < size_before - 1; i++)
		{	
				for (int k = 1; k < number; k++)
				{
					it2 = point_matrix.begin();
					point_matrix.insert(it2 + i * number + k, vector<Point>(rowsize));
					for (int j = 0; j < rowsize; j++)
					{
						point_matrix[i * number + k][j] = Point(point_matrix[i][j].x, point_matrix[(i)*number][j].y - step * k);
					}
				}
		}
		init_allcells();
		is_divided = true;
	}
	void init_allcells()//функция заполнения массива клеток по масиву точек 123
	{
		allcells.clear();
		for (int i = 0; i < point_matrix.size()-1; i++)
		{
			for (int j = 0; j < point_matrix[i].size()-1; j++)
			{
				allcells.push_back(Bigpart(point_matrix[i + 1][j], point_matrix[i][j], point_matrix[i][j + 1], point_matrix[i + 1][j + 1]));
			}
		}
	}
	void print_point_matrix()// печатает матрицу точек прямоугольника
	{
		for (int i = 0; i < point_matrix.size(); i++)
		{
			for (int j = 0; j < point_matrix[i].size(); j++)
			{
				cout << point_matrix[i][j].x << "." << point_matrix[i][j].y << " ";
			}
			cout << endl;
		}
	}
	void print_allcells()// рекурсивно печатает все ячейки
	{
		if (allcells.size() > 0)
		{
			for (int i = 0; i < allcells.size(); i++)
			{
				if (allcells[i].point_matrix.size() > 2)
				{
					allcells[i].print_allcells();
				}
				else
				{
					allcells[i].print_point_matrix();
				}
			}
			cout << endl;
		}
	}

	void get_cells_for_calculation(vector <Point> &mass)//заполняет массив точек без повторений для обработки
	{
		
		if (is_divided)
		{
			for (int i = 0; i < allcells.size(); i++)
			{
				allcells[i].get_cells_for_calculation(mass);
			}
		}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					if (find(mass.begin(), mass.end(), points[i]) == mass.end()) {
						mass.push_back(points[i]);
					}
				}

			}
		
	}

	~Bigpart() {};

private:
protected:
public:
//	Section sections[4];
	Point points[4];
//	vector<Section> allsections;
	vector<Bigpart> allcells;
	vector<vector<Point>> point_matrix;
	float verticallength;
	float horizontallength;
	float eps;
	Point center;
	bool is_divided;
};
float get_value_circle(Bigpart a, Point b, float radius)//функция вохвоащающая расстояние до окружности
{
	// (x – a)2 + (y – b)2 = R2
	float rez = sqrt(fabs(pow((a.center.x - b.x), float(2)) + pow((a.center.y - b.y), float(2)) - pow(radius, float(2))));
	return rez;
}
void dynamic_divide(Bigpart& a, Point &b,float radius, float min)// функция динамического разбиения квадрата незавершенная, считает неверно
{
	float val = 0;
	if (a.eps > 2*min)
	{
		val = get_value_circle(a, b, radius);
		if (val <= a.eps)//(get_value_circle(a, b, radius) == 1)
		{
			a.divide_self_points(2);
			for (int i = 0; i < a.allcells.size(); i++)
			{
				dynamic_divide(a.allcells[i], b, radius, min);
			}
		}
	}
}



int main()
{
	Point a(0, 0);
	Point b(100, 100);
	Bigpart pole(a, b);
	Point center = Point(50, 50);
	dynamic_divide(pole, center, 50., 3.);
	system("pause");
	return 0;
}