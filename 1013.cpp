#include <iostream>

using namespace std;

void decode(int num) {
	while (num > 0) {
		int power = 0;
		int res = 1;
		while (res <= num) {
			res *= 2;
			power++;
		}
		res /= 2;
		power--;
		cout << "2";
		if (power == 0) {
			cout << "(0)";
		} else if (power == 1) {
			cout << "";
		} else if (power == 2) {
			cout << "(2)";
		} else {
			cout << "(";
			decode(power);
			cout << ")";
		}
		num -= res;
		if (num != 0){
			cout << "+";
		}
	}
}

int main() {
	int num;
	cin >> num;
	decode(num);
}