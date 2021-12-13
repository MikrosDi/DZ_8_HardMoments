#define LESSON1_PERSON_H
#define LESSON1_PHONEBOOK_H

#include <regex>
#include <iostream>
#include <string>
#include <optional>
#include <utility>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

//Task 1

struct Person {
	string last_name;
	string name;
	optional<string> patronymic;
	Person(string last_name, string name, optional<string> patronymic) :
		last_name(move(last_name)), name(move(name)), patronymic(move(patronymic)) {};
	[[nodiscard]] const string& GetLastName() const;
	bool operator<(const Person& person2) const;
	bool operator==(const Person& person2) const;
	friend ostream& operator<< (ostream& out, const Person& person);
};

[[nodiscard]] const string& Person::GetLastName() const {
	return last_name;
}

bool Person::operator<(const Person& person2) const {
	return (tie(last_name, name, patronymic) < tie(person2.last_name, person2.name, person2.patronymic));
}

bool Person::operator==(const Person& person2) const {
	return (tie(last_name, name, patronymic) == tie(person2.last_name, person2.name, person2.patronymic));
}

ostream& operator<<(ostream& out, const Person& person) {
	out << person.last_name << " " << person.name;
	if (person.patronymic.has_value())
		out << " " << person.patronymic.value();
	return out;
}

//Task 2

class PhoneNumber {
protected:
	int country_code;
	int city_code;
	string number;
	optional<int> add_number;
public:
	PhoneNumber() : country_code(0), city_code(0), add_number(nullopt) {}
	PhoneNumber(int country, int city, string num, optional<int> add_num)
		: country_code{ country }, city_code{ city }, number{ move(num) }, add_number{ add_num } {}
	[[nodiscard]] int getCountryCode() const;
	[[nodiscard]] int GetCityCode() const;
	[[nodiscard]] string GetNumber() const;
	[[nodiscard]] optional<int> GetAddNumber() const;
	friend ostream& operator<<(ostream& out, const PhoneNumber& phone_number);
	friend bool operator<(const PhoneNumber& num1, const PhoneNumber& num2);
};

[[nodiscard]] int PhoneNumber::getCountryCode() const {
	return country_code;
}

[[nodiscard]] int PhoneNumber::GetCityCode() const {
	return city_code;
}

[[nodiscard]] string PhoneNumber::GetNumber() const {
	return (add_number.has_value() ? to_string(add_number.value()) : "");
}

[[nodiscard]] optional<int> PhoneNumber::GetAddNumber() const {
	if (add_number.has_value())
		return add_number.value();
	else
		return nullopt;
}

ostream& operator<<(ostream& out, const PhoneNumber& phone_number) {
	out << '+' << phone_number.getCountryCode() << " (" << phone_number.GetCityCode() << ") " << phone_number.GetNumber();
	return out;
}

bool operator<(const PhoneNumber& num1, const PhoneNumber& num2) {
	return tie(num1.country_code, num1.city_code, num1.number, num1.add_number) <
		tie(num2.country_code, num2.city_code, num2.number, num2.add_number);
}

//Task 3

class PhoneBook {
protected:
	vector<pair<Person, PhoneNumber>> phones;
public:
	PhoneBook(ifstream& is);
	void SortByName();
	void SortByPhone();
	void ChangePhoneNumber(const Person& person, PhoneNumber new_phone);
	pair<string, PhoneNumber> GetPhoneNumber(const string& surname);
	[[nodiscard]] vector<pair<Person, PhoneNumber>>& GetPhones();
	friend ostream& operator<< (ostream& out, PhoneBook& book);
};
Person GetPerson(const smatch& matches) {
	string name = matches[1].str();
	string family = matches[2].str();
	optional <string> patronymic;
	if (matches[3].matched) {
		patronymic = matches[3].str();
	}
	else {
		patronymic = nullopt;
	}
	Person p(name, family, patronymic);
	return p;
}

PhoneNumber GetPhone(const smatch& matches) {
	string country_code = matches[4].str();
	string city_code = matches[5].str();
	optional<int> add_number;
	if (matches[6].matched) {
		add_number = stoi(matches[6].str());
	}
	else {
		add_number = nullopt;
	}
	PhoneNumber phone(stoi(country_code), stoi(city_code),
		string("+").append(country_code).append(" ").append(city_code).
		append((add_number.has_value() ? string(" ").
			append(to_string(add_number.value())) : "")), add_number);
	return phone;
}

PhoneBook::PhoneBook(ifstream& is) {
	for (string line; getline(is, line);) {
		const regex name_regex(
			"([[:alpha:]]+) ([[:alpha:]]+) ?([[:alpha:]]+)? \\+?([[:digit:]]+) ([[:digit:]]+) ?([[:digit:]]+)?");
		smatch matches;
		if (regex_match(line, matches, name_regex)) {
			Person p = GetPerson(matches);
			PhoneNumber phone = GetPhone(matches);
			phones.emplace_back(p, phone);
		}
	}
}

void PhoneBook::SortByName() {
	sort(phones.begin(), phones.end());
}

void PhoneBook::SortByPhone() {
	sort(phones.begin(),
		phones.end(),
		[](const pair<Person, PhoneNumber>& phone1,
			const pair<Person, PhoneNumber>& phone2) {
				return phone1.second < phone2.second;
		});
}

void PhoneBook::ChangePhoneNumber(const Person& person, PhoneNumber new_phone) {
	auto user = find_if(phones.begin(), phones.end(), [&person](pair<Person, PhoneNumber>& entry) {
		return entry.first == person;
		});

	if (user != phones.end())
		user->second = move(new_phone);
}

pair<string, PhoneNumber> PhoneBook::GetPhoneNumber(const string& surname) {
	size_t count = 0;
	const PhoneNumber* phone{};
	for_each(phones.begin(), phones.end(), [&phone, surname, &count](pair<Person, PhoneNumber>& entry) {
		if (entry.first.GetLastName() == surname) {
			count++;
			phone = &entry.second;
		}
		});
	if (count == 0)
		return { "None found!", {} };
	else if (count > 1)
		return { string("Found ").append(to_string(count)).append("phones"), *phone };
	return { "", *phone };
}

[[nodiscard]] vector<pair<Person, PhoneNumber>>& PhoneBook::GetPhones() {
	return phones;
}

ostream& operator<<(ostream& out, PhoneBook& book) {
	for (const auto& iter : book.GetPhones()) {
		out << iter.first << ' ' << iter.second << endl;
	}
	return out;
}

/*DZ8-Test*/

namespace testing {

	class Test {
	public:
		static void assert_true(bool expression, const string& test_name) {
			if (expression)
				cout << "[" << test_name << "] - " << "Test passed" << endl;
			else
				cerr << "[" << test_name << "] - " << "Test failed" << endl;
		}

		static void assert_false(bool expression, const string& test_name) {
			if (!expression)
				cout << "[" << test_name << "] - " << "Test passed" << endl;
			else
				cerr << "[" << test_name << "] - " << "Test failed" << endl;
		}

		template<typename T>
		static void assert_equals(const T& lhs, const T& rhs, const string& test_name) {
			if (lhs == rhs)
				cout << "[" << test_name << "] - " << "Test passed" << endl;
			else
				cerr << "[" << test_name << "] - " << "Test failed" << endl;
		}

		static vector<int> sort(const vector<int>& data) {
			if (data.empty()) {
				throw invalid_argument("data is empty");
			}
			vector<int> result{ data };
			for (size_t i = 0u; i < result.size(); ++i)
				for (size_t j = i; j > 0 && result[j] < result[j - 1]; --j)
					swap(result[j], result[j - 1]);
			return result;
		}

#define EXIT_SUCCESS 0

#define EXPECT_EXEPTION(code, exception)                       \
try{                                                           \
code                                                           \
   cerr << "[" << __func__ << "] - " << "Test failed" << endl; \
}catch (const exception&) {                                    \
cout << "[" << __func__  + string(":") + to_string(__LINE__)<< "] - " << "Test passed" << endl;    \
}

#define ASSERT_TRUE(expression) \
assert_true((expression), __func__ + string(":") + to_string(__LINE__))

#define ASSERT_FALSE(expression) \
assert_false((expression), __func__)

#define ASSERT_EQUALS(lhs, rhs) \
assert_equals((lhs), (rhs), __func__ + string(":") + to_string(__LINE__))

		void test_sort_with_empty_data() {
			EXPECT_EXEPTION({
								Test::sort({});
				}, invalid_argument);
		}

		void test_sort_data_with_one_ellement() {
			Test::ASSERT_EQUALS(vector<int>{1}, Test::sort({ 1 }));
		}

		void test_sort_data_with_many_ellements() {
			vector<int> lhs = { 1, 2, 3, 5, 6, 6 };
			vector<int> rhs = Test::sort({ 1, 6, 3, 6, 2, 5 });
			Test::ASSERT_EQUALS(lhs, rhs);
		}

		void test_compare_data() {
			double value = 5.7 * 2;
			double double_value = value * 2;
			Test::ASSERT_TRUE((double_value / 2 - value) < numeric_limits<double>::epsilon());
		}
	};
}

class TestDate : public testing::Test {
protected:
	void SetUp();
	void TearDown();
	PhoneBook* phone_book{};
public:
	TestDate() {
		SetUp();
	}
	~TestDate() {
		TearDown();
	}
	int Test();
};
void TestDate::SetUp() {
	ifstream file("PhoneBook.txt"); // путь к файлу PhoneBook.txt
	phone_book = new PhoneBook(file);
}

void TestDate::TearDown() {
	delete phone_book;
}

int TestDate::Test() {
	{
		testing::Test::test_compare_data();
		testing::Test::test_sort_with_empty_data();
		testing::Test::test_sort_data_with_one_ellement();
		testing::Test::test_sort_data_with_many_ellements();
	}
	return EXIT_SUCCESS;
}


int main()
{
	setlocale(LC_ALL, "rus");

	cout << "\nЗадание 1 (1,2,3)\n" << endl;

	ifstream file("pb.txt"); // путь к файлу PhoneBook.txt
	PhoneBook phone_book(file);
	cout << phone_book;

	cout << "------SortByPhone-------" << endl;
	phone_book.SortByPhone();
	cout << phone_book;

	cout << "------SortByName--------" << endl;
	phone_book.SortByName();
	cout << phone_book;

	cout << "-----GetPhoneNumber-----" << endl;

	auto print_phone_number = [&phone_book](const string& surname) {
		cout << surname << "\t";
		auto answer = phone_book.GetPhoneNumber(surname);
		if (get<0>(answer).empty())
			cout << get<1>(answer);
		else
			cout << get<0>(answer);
		cout << endl;
	};

	print_phone_number("Ivanov");
	print_phone_number("Petrov");
	print_phone_number("Mikros");

	cout << "----ChangePhoneNumber----" << endl;
	phone_book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" }, PhoneNumber{ 7, 123, "15344458", nullopt });
	phone_book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" }, PhoneNumber{ 16, 465, "9155448", 13 });
	phone_book.ChangePhoneNumber(Person{ "Mikros", "Dima", nullopt }, PhoneNumber{ 16, 465, "9155448", 13 });
	cout << phone_book;

	cout << "\nЗадание 8 (1 и 2)\n" << endl;

	TestDate test;
	test.Test();

	return 0;
}
