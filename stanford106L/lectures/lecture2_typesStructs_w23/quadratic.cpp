#include <string>
#include <iostream>
#include <cmath> //gives us pow and sqrt!

using std::string; using std::cout;
using std::cin; using std::pow;
using std::sqrt; using std::endl;

std::pair<bool, std::pair<double, double> > quadratic(double a, double b, double c){
	//get radical, test if negative, return false if so
	double radical = pow(b, 2) - (4*a*c);

	if(radical < 0){
		return {false, {-1, -1}};
	} else {
		double root1 = ( -1*b + sqrt(radical) ) / (2*a);
		double root2 = ( -1*b - sqrt(radical) ) / (2*a);
		return {true, {root1, root2}};
	}
}

int main(){
	//get 3 doubles (ax^2 + bx + c)
	double a, b, c;
	cout << "Input coefficients" << endl;
	cin >> a >> b >> c;
	//get roots if they exist
	// First way: std::pair<bool, std::pair<double, double>> res = quadratic(a, b, c);
	auto [exists, roots] = quadratic(a, b, c);
	//print accordingly
	if (exists) {
		double root1 = roots.first;
		double root2 = roots.second;
		cout << "Solutions are: " << root1 << " " << root2 << endl;
	} else {
		cout << "No solutions exist!" << endl;
	}
	return 0;
}