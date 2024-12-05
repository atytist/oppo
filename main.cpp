#include <fstream>
#include <iostream>
#include <clocale>
#include <cstring>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <regex>
#define Width 10
using namespace std;
struct measure_press {
	string date;
	float height;
	int value;
};
measure_press parsing(measure_press measure, string str) {
	regex regular_date("([\\d]+[\\.][\\d]+[\\.][\\d\\n]+[^\\s])");   //. - любое кол-во символов \d - цифровые \. - точка \s - пробел
	regex regular_height("([\\d]+[,][\\d]+)");
	regex regular_value("(^[\\d][ ]+|[ ][\\d]+[ ]|[ ][\\d]+$)");
	string buffer;
	smatch buf;
	if (regex_search(str, buf, regular_date)) {
		measure.date = buf.str();
	}
	if (regex_search(str, buf, regular_height)) {
		measure.height = stof(buf.str());
	}
	if (regex_search(str, buf, regular_value)) {
		measure.value = stoi(buf.str());
	}
	return measure;
}
vector<measure_press> ReadFIle(const string& path, vector<measure_press> data) {
	ifstream ist(path);
	string buffer;
	while (!ist.eof()) {
		getline(ist, buffer);
		measure_press measure;
		measure = parsing(measure, buffer); // если сразу парсить то не надо будет создавать дополнительный вектор<стринг> а сразу работать со структурным
		data.push_back(measure);
	}
	ist.close();
	return data;
}
bool CompByHeight(measure_press left, measure_press right) {
	return left.height < right.height;
}
bool CompByDate(measure_press left, measure_press right) {
	return left.date < right.date;
}
bool CompByValue(measure_press left, measure_press right) {
	return left.value < right.value;
}
vector<measure_press> SortVectorBy(vector<measure_press> data, int mode) {
	switch (mode) {
	case 1: sort(begin(data), end(data), CompByDate); break;
	case 2: sort(begin(data), end(data), CompByHeight); break;
	case 3: sort(begin(data), end(data), CompByValue); break;
	default:
		return vector< measure_press>();
	}

	return data;
}
void PrintData(vector<measure_press> data) {

	cout << setw(Width) << "Дата" << setw(Width) << "Высота" << setw(Width) << "Значение" << endl;
	for (const auto& measure : data) {
		cout << setw(Width) << measure.date << setw(Width) << measure.height << setw(Width) << measure.value << endl;
	}

}

vector<measure_press> vector_1date(vector<measure_press> data) {
	for (const auto& measure : data) {
		cout << measure.date << endl;
	}
	cout << "choose date: ";
	string date;
	cin >> date;
	vector<measure_press> filtered_data;
	for (const auto& measure : data) {
		if (measure.date == date) {
			filtered_data.push_back(measure);
		}
	}
	return filtered_data;
}


int main() {
	setlocale(LC_ALL, "Russian");
	string path = "oppo_1_in.txt";
	vector<measure_press> data;    // распарсированные данные 
	vector<measure_press> Sorted_Data;
	vector<measure_press> filtered_data;

	int mode = 0;

	data = ReadFIle(path, data);

	cout << "choose mode: 1 - standart mode, 2 - choose date ";
	int method;
	cin >> method;

	if (method == 2) {
		filtered_data = vector_1date(data);
		PrintData(filtered_data);
		cout << "1(Sort By Date)  2(Sort By Height)  3(Sort By Value)  4(Exit)" << endl;
		cin >> mode;


		switch (mode) {
		case 1:
		case 2:
		case 3:
			Sorted_Data = SortVectorBy(filtered_data, mode);
			PrintData(Sorted_Data);
			break;
		case 4:
			break;
		default:
			cout << "Wrong command " << endl;
			system("pause");
		}
	}

	else if (method == 1) {
		while (mode != 4) {
			PrintData(data);
			cout << "1(Sort By Date)  2(Sort By Height)  3(Sort By Value)  4(Exit)" << endl;
			cin >> mode;


			switch (mode) {
			case 1:
			case 2:
			case 3:
				Sorted_Data = SortVectorBy(data, mode);
				PrintData(Sorted_Data);
				break;
			case 4:
				break;
			default:
				cout << "Wrong command " << endl;
				system("pause");
			}

		}
	}

	return 0;
}