#include <iostream>
#include <sstream>
#include <stack>
#include <cctype>

using namespace std;

//#define QUIET

int is_next_num(string &expr, unsigned int pos) //-1 -> eof, 0 -> no, 1 -> yes
{
	if (pos >= expr.size())
		return -1;
	for (; pos < expr.size(); pos++) //seeks past ' 's
		if (expr[pos] != ' ')
			break;
	if (pos >= expr.size())
		return -1;
	if (isdigit(expr[pos])) //this wont work for eg. -n, so more checking
		return 1;
	if (pos+1 == expr.size())
		return 0;
	else if (expr[pos+1] == ' ')
		return 0;
	return 1;
}

//TODO: this doesn't work if we have something like: 1 -y +
//ie negative y doesn't work, but its not a big problem
double eval(string &expr, double x, double y) //rpn
{
	stringstream ss(expr);
	stack<double> results;
#ifdef QUIET
	bool already_warned = false;
#endif
	while (is_next_num(expr, ss.tellg()) != -1)
	{
		if (is_next_num(expr, ss.tellg()))
		{
			double num;
			ss >> num;
			results.push(num);
		}
		else
		{
			double n;
			char op;
			ss >> op;
			switch (op)
			{
				case 'x':
					results.push(x);
					break;
				case 'y':
					results.push(y);
					break;
				case '+':
					n = results.top();
					results.pop();
					results.top() += n;
					break;
				case '-':
					n = results.top();
					results.pop();
					results.top() -= n;
					break;
				case '*':
					n = results.top();
					results.pop();
					results.top() *= n;
					break;
				case '/':
					n = results.top();
					results.pop();
					results.top() /= n;
					break;
				default:
#ifdef QUIET
					if (!already_warned)
#endif
					cerr << "WARN: Ignoring recognized operator " << op << endl;
			}
		}
	}
#ifdef QUIET
	if (results.size() > 1u && !already_warned)
#else
	if (results.size() > 1u)
#endif
		cerr << "WARN: More than one number on the result stack" << endl;
#ifdef QUIET
	already_warned = true;
#endif
	return results.top();
}

int main()
{
	double x, h, target_x;
	double y, y_prime;
	string expr;

	(cout << "Enter the differential equation (in RPN): ").flush();
	getline(cin, expr);
	cout << "Enter starting x: ";
	cin >> x;
	cout << "Enter starting y: ";
	cin >> y;
	cout << "Enter starting h: ";
	cin >> h;
	cout << "Enter target x: ";
	cin >> target_x;
	y_prime = eval(expr, x, y);
#ifndef QUIET
	cout << "y0' = " << y_prime << endl;
#endif
	for (int i = 1; x < target_x; i++)
	{
		x += h;
		y += h*y_prime;
		y_prime = eval(expr, x, y);
#ifndef QUIET
		cout << 'x' << i << "  = " << x << endl;
		cout << 'y' << i << "  = " << y << endl;
		cout << 'y' << i << "' = " << y_prime << endl;
#endif
	}
	cout << "The answer is " << y << endl;
}
