#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int N = 11, badpoint = 0;
const unsigned int str = 190;
const unsigned int stolb = 14;
double* h = new double[N + 1];
double* l = new double[N + 1];
double* delta = new double[N + 1]; 
double* lambda = new double[N + 1]; 
double* c = new double[N + 1]; 
double* d = new double[N + 1];
double* b = new double[N + 1];
double* x = new double[N + 1]; 
double* z = new double[N + 1]; 


void cinCSV(double** y_arr) {
	fstream File1;
	File1.open("09_Irkutsk.csv", ios::in);
	string line, word;
	getline(File1, line);
	for (int count_row = 0; count_row < str; count_row++) {
		getline(File1, line);
		stringstream s(line);
		for (int count_column = 0; count_column < stolb; count_column++) {
			getline(s, word, ',');
			y_arr[count_row][count_column] = stof(word);
		}
	}
}
//elementy >100 & vibor stolb
int bad(double** y_arr) {
	int c;
	cout << "Choose the stolb" << endl;
	cin >> c;
	for (int i = 0; i < str; i++) {
		if (y_arr[i][c] > 100)
			badpoint++;

	}

	return c;
}

//new array bez elementov > 100
void selectdata(double** y_arr, int count9, double* newy, double* x, int stl) {

	int p = 0;
	for (int i = 0; i < str; i++) {
		if (y_arr[i][stl] < 100) {
			newy[p] = y_arr[i][stl];
			p++;
		}
	}
	for (int i = 0; i < N + 1; i++) {
		x[i] = i;
	}
}

void del(double** y_arr) {
	delete[] z;
	delete[] x;
	delete[] h;
	delete[] l;
	delete[] delta;
	delete[] lambda;
	delete[] c;
	delete[] d;
	delete[] b;
	for (int count = 0; count < str; count++)
		delete[]y_arr[count];
}

void koef() {
	int k = 0;
	for (k = 1; k <= N; k++) {
		h[k] = x[k] - x[k - 1];
		l[k] = (z[k] - z[k - 1]) / h[k];
	}
	delta[1] = -h[2] / (2 * (h[1] + h[2]));
	lambda[1] = 1.5 * (l[2] - l[1]) / (h[1] + h[2]);
	for (k = 3; k <= N; k++) {
		delta[k - 1] = -h[k] / (2 * h[k - 1] + 2 * h[k] + h[k - 1] * delta[k - 2]);
		lambda[k - 1] = (3 * l[k] - 3 * l[k - 1] - h[k - 1] * lambda[k - 2]) /
			(2 * h[k - 1] + 2 * h[k] + h[k - 1] * delta[k - 2]);
	}
	c[0] = 0;
	c[N] = 0;
	for (k = N; k >= 2; k--) {
		c[k - 1] = delta[k - 1] * c[k] + lambda[k - 1];
	}
	for (k = 1; k <= N; k++) {
		d[k] = (c[k] - c[k - 1]) / (3 * h[k]);
		b[k] = l[k] + (2 * c[k] * h[k] + h[k] * c[k - 1]) / 3;
	}
}

void printresult(double* z) {
	int i = 0;
	cout << "\nA[i]       B[i]       C[i]       D[i]\n";
	for (i = 1; i <= N; i++) {
		cout << z[i] << "     " << b[i] << "     " << c[i] << "     " << d[i] << endl;
	}
	cout << endl;
}

// nahodim tochki & zap v file
void res(double* y) {
	int k;
	ofstream fout;
	fout.open("CSpoints.txt");
	for (double s = 0; s <= N; s += 0.1) {
		//find k, where s in [x_k-1; x_k]
		for (k = 1; k <= N; k++) {
			if (s >= x[k - 1] && s <= x[k]) {
				break;
			}
		}

		double F = y[k] + b[k] * (s - x[k]) + c[k] * pow(s - x[k], 2) + d[k] * pow(s - x[k], 3);
		cout << s << " " << F << endl;
		fout << s << " " << F << endl;
	}
	cout << "The file has been created" << endl;
	fout.close();
}

void knots(double* z) {
	ofstream fout1;
	fout1.open("CSknots.txt");
	for (int p = 0; p <= N; p++) 
		fout1 << x[p] << " " << z[p] << endl;
	

}

int main() {
	double** y_arr = new double* [str];
	for (int count = 0; count < str; count++)
		y_arr[count] = new double[stolb];

	cinCSV(y_arr);
	
	int stl = bad(y_arr);


	double* newy = new double[str - badpoint];
	selectdata(y_arr, badpoint, newy, x, stl);

	cout << "Choose the first element " << endl;
	int start;
	cin >> start;

	// 12 elemetnov so start
	for (int i = 0; i <= N; i++) {
		z[i] = newy[start + i];
	}
	
	koef();
	printresult(z);
	res(z);
	knots(z);
	del(y_arr);
	return 0;
}