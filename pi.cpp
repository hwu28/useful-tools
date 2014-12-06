//Calculates pi by integrating a unit circle (inefficient!)

#include <iostream>
#include <cmath>

using namespace std;

int main()
{
	long double sum = 0;
	long double i = 1;
	long double tmp;
	long long max;

	cerr << "Enter x, where 2^x is the number of samplings: ";
	cin >> max;

	if (max > 63)
	{
		cerr << "Too large" << endl;
		return 1;
	}

	max = pow(2,max+1);

	for (; i < max; i+=2)
	{
		tmp = i/max;
		sum += sqrt(1-tmp*tmp);
	}

	cout.precision(50);
	cout << sum*4/(max/2) << endl;
	return 0;
}
