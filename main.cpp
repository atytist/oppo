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
enum ActionChoos {
	Sort_by_date      = 1,
	Sort_by_height    = 2,
	sort_by_value     = 3,
	Print_not_sorted  = 4,
	Print_Same        = 5,
	Sort_Same         = 6,
	Exit              = 7
};

enum flag_reg {
	date        = 1,
	height_pres = 2,
	height      = 3,
	value       = 4

};

struct measure_press {
	string date;
	float height;
	int value;
};

regex regular_date("([\\d]+[\\.][\\d]+[\\.][\\d\\n]+[^\\s])"); //. - любое кол-во символов \d - цифровые \. - точка \s - пробел
regex regular_height("([\\d]+[,][\\d]+)|(^[\\d]+[.][\\d]+$)");
regex regular_value("(^[\\d][ ]+|[ ][\\d]+[ ]|[ ][\\d]+$)|(^[\\d]+$)");

measure_press parsing(measure_press measure, string str) {
	string buffer;
	smatch buf;
	if (regex_search(str, buf, regular_date)) measure.date = buf.str();
	if (regex_search(str, buf, regular_height)) measure.height = stof(buf.str());
	if (regex_search(str, buf, regular_value)) measure.value = stoi(buf.str());
	return measure;
}

vector<measure_press> ReadFIle(const string& path, vector<measure_press> data) {
	ifstream ist(path);
	string buffer;
	while (!ist.eof()) {
		getline(ist, buffer);
		measure_press measure;
		measure = parsing(measure, buffer);
		data.push_back(measure);
	}
	ist.close();
	return data;
}

bool CompByHeight(measure_press left, measure_press right) { return left.height < right.height; }
bool CompByDate(measure_press left, measure_press right) { return left.date < right.date; }
bool CompByValue(measure_press left, measure_press right) { return left.value < right.value; }

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

bool range_in(string object, float delta, measure_press height) {
	float object_f = stof(object);
	if (height.height - delta <= object_f and object_f <= height.height + delta) return 0;
	else return 1;
}

vector <measure_press> FindSame(vector <measure_press> data, string object) {
	int flag = 0, i = 0;
	float delta;
	bool buf;

	if (regex_search(object, regular_date))             flag = 1;
	else if (regex_search(object, regular_height))      flag = 2;
	else if (regex_search(object, regular_value))       flag = 4;

	while (i != data.size()) {
		switch (flag) {
		case date: {
			if (data[i].date != object) { data.erase(data.begin() + i); i -= 1; }
			break;
		}
		case height_pres: {
			cout << "Print precision: (±...)\n";
			cin >> delta;
			flag = 3;
		}
		case height: {
			if (range_in(object, delta, data[i])) { data.erase(data.begin() + i); i -= 1; }
			break;
		}
		case value: {
			if (data[i].value != stoi(object)) { data.erase(data.begin() + i); i -= 1; }
			break;
		}
		}
		i += 1;
	}
	return data;
}

int main() {
	setlocale(LC_ALL, "Russian");
	string path = "oppo_1_in.txt", object;
	vector<measure_press> data, Sorted_Data, Same_Obj;
	int mode = 0, mode_same = 0;

	data = ReadFIle(path, data);

	while (mode != Exit) {
		cout << "1 (Sort By Date)\n"
			<< "2 (Sort By Height)\n"
			<< "3 (Sort By Value)\n"
			<< "4 (print data not sorted)\n"
			<< "5 (Print the same)\n"
			<< "6 (Sort the same)\n"
			<< "7 (Exit)\n";
		cin >> mode;
		system("cls");
		PrintData(data);

		switch (mode) {
		case Sort_by_date:
		case Sort_by_height:
		case sort_by_value: {
			Sorted_Data = SortVectorBy(data, mode);
			PrintData(Sorted_Data);
			break;
		}
		case Print_not_sorted: {
			PrintData(data);
			break;
		}
		case Print_Same: {
			cout << "Print Data or height or value: " << endl;
			cin >> object;
			Same_Obj.clear();
			Same_Obj = FindSame(data, object);
			PrintData(Same_Obj);
			break;
		}
		case Sort_Same: {
			if (Same_Obj.empty() == 1) {
				cout << "Print Data or height or value: " << endl;
				cin >> object;
				Same_Obj = FindSame(data, object);
				cout << "sort by: Date(1) Height(2) Value(3)" << endl;
				cin >> mode_same;
				Same_Obj = SortVectorBy(Same_Obj, mode_same);
				PrintData(Same_Obj);
			}
			else {
				cout << "sort by: Date(1) Height(2) Value(3)" << endl;
				cin >> mode_same;
				Same_Obj = SortVectorBy(Same_Obj, mode_same);
				PrintData(Same_Obj);
			}
			break;
		}
		case Exit:
			break;

		default: {
			cout << "Wrong command " << endl; 
			system("pause");
		}
		}

	}
	return 0;
}