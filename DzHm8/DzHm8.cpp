
#include <iostream>
#include <vector>
#include <string>

using namespace std;

/* Task 1
Написать фреймворт для тестирования (можно взять из урока).*/

void assert_true(bool expression, const string& test_name)
{
    if (expression)
        cout << "[" << test_name << "] - " << "Test passed" << endl;
    else
        cerr << "[" << test_name << "] - " << "Test failed" << endl;
}

void assert_false(bool expression, const string& test_name)
{
    if (!expression)
        cout << "[" << test_name << "] - " << "Test passed" << endl;
    else
        cerr << "[" << test_name << "] - " << "Test failed" << endl;
}

template<typename T>
void assert_equals(const T& lhs, const T& rhs, const string& test_name)
{
    if (lhs == rhs)
        cout << "[" << test_name << "] - " << "Test passed" << endl;
    else
        cerr << "[" << test_name << "] - " << "Test failed" << endl;
}

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


vector<int>sort(const vector<int>& data)
{
    if (data.empty()) {
        throw invalid_argument("data is empty");
    }
    vector<int>result{ data };
    for (size_t i = 0u; i < result.size(); ++i)
        for (size_t j = i; j > 0 && result[j] < result[j - 1]; --j)
            swap(result[j], result[j - 1]);
    return result;
}

void test_sort_with_empty_data()
{
    EXPECT_EXEPTION({
       sort({});
        }, invalid_argument);
}

void test_sort_data_with_one_ellement()
{
    ASSERT_EQUALS(vector<int>{ 1 }, sort({ 1 }));
}

void test_sort_data_with_many_ellements()
{
    vector<int>lhs = { 1, 2, 3, 5, 6, 6 };
    vector<int>rhs = sort( { 1, 6, 3, 6, 2, 5 }); 
    ASSERT_EQUALS(lhs, rhs);
}

void test_compare_data()
{
    double value = 5.7 * 2;
    double double_value = value * 2;
    ASSERT_TRUE((double_value / 2 - value) < numeric_limits<double>::epsilon());
}

/* Task 2
 Добавить unit-тесты в проект домашнего задания из первого урока. */

int main()
{
    setlocale(LC_ALL, "rus");
    {
    cout << "\nЗадание 1\n" << endl;

    test_compare_data();
    test_sort_with_empty_data();
    test_sort_data_with_one_ellement();
    test_sort_data_with_many_ellements();
    }

    {
        cout << "\nЗадание 2\n" << endl;

    }

    return EXIT_SUCCESS;
}


