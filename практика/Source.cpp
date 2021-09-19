#include <cstdlib>
#include <iostream>// system
#include <cmath>
#include <vector>
using namespace std;

class Point//класс точки
{
public:


	Point(float a = 0, float b = 0)
	{
		x = a;
		y = b;
	}
	~Point() {};
	inline void Set_x(float xx) { x = xx; }
	inline void Set_y(float yy) { y = yy; }

private:
protected:
public:
	float x;
	float y;
};

class Section//класс отрезка
{
public:
	Section()
	{
		a.Set_x(0);
		a.Set_y(0);
		b.Set_x(0);
		b.Set_y(0);
		length = 0;
	}
	Section(Point aa, Point bb)
	{
		a = aa;
		b = bb;
		init_length();
	}
	inline void init_length()
	{
		length = sqrt(pow((a.x - b.x), float(2)) + pow((a.y - b.y), float(2)));
	}
	~Section() {};

public:
	Point a;
	Point b;
	float length;
};

class Sqad
{
public:

	Sqad(Point* mass) //
	{
		for (int i = 1; i < 4; i++)
		{
			points[i] = mass[i];
		}
		init_sections();
	}
	Sqad(Point first, Point second, Point third, Point fourth) //
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

class Bigpart
{
public:
	Bigpart(Point a, Point b) //
	{
		points[0] = a;
		points[1] = Point(a.x, b.y);
		points[2] = b;
		points[3] = Point(b.x, a.y);
		init_sections();
		init_length();
		init_point_matrix();
	}

	void init_sections()
	{
		sections[0] = Section(points[0], points[1]);
		sections[1] = Section(points[1], points[2]);
		sections[2] = Section(points[2], points[3]);
		sections[3] = Section(points[3], points[1]);
	}

	void init_length()
	{
		verticallength = sections[0].length;
		horizontallength = sections[1].length;

	}

	void init_point_matrix()
	{
		point_matrix.resize(2);
		point_matrix[0].resize(2);
		point_matrix[1].resize(2);
		point_matrix[0][0] = points[1];
		point_matrix[0][1] = points[2];
		point_matrix[1][0] = points[0];
		point_matrix[1][1] = points[3];


	}

	void divide_self_points(int number)
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
					//float incr = step * k;
					//point_matrix[i].insert(it + j * number + k, Point((point_matrix[i][j * number].x + incr), point_matrix[i][j].y));
				}
		}
	}

	void print_point_matrix()
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
	~Bigpart() {};

private:
protected:
public:
	Section sections[4];
	Point points[4];
	vector<Section> allsections;
	vector<Sqad> allquads;
	vector<vector<Point>> point_matrix;
	//vector<vector<Bigpart>> point_matrixa;
	float verticallength;
	float horizontallength;
};

int main()
{
	Point a(0, 0);
	Point b(100, 100);
	Bigpart pole(a, b);
	pole.print_point_matrix();
	pole.divide_self_points(2);
	pole.print_point_matrix();
	pole.divide_self_points(2);
	pole.print_point_matrix();
	pole.divide_self_points(3);
	pole.print_point_matrix();
	system("pause");
	return 0;
}