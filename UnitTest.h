/*
A C++20 header only unit test library.
MIT Licence.
apintandahalf would apprecaiate attribution.

Example use:

#include <UnitTest.h>

int times2(int x)
{
	return x * 2;
}
TEST(testtimes2)
{
	EXPECT_EQ(4, times2(2));
	ASSERT_EQ(6, times2(3));
}
TEST(testtimes2Wrong)
{
	ASSERT_NE(7, times2(3));
	EXPECT_NE(5, times2(2));
}
int main()
{
	return runAllTests();
}
*/

#include <iostream>
#include <source_location>
#include <functional>
#include <vector>


namespace
{
int fails = 0;
int tests_failed = 0;
int tests_ran = 0;

#define WRAP(a) \
	do { a } while (false)

#define REPORT \
	const auto src = std::source_location::current(); \
	std::cout << "Test failed: " << src.file_name() << "." << src.function_name() << "." << src.line() << ": " 

#define EXPECT_EQ(a,b) \
	WRAP (if (!((a) == (b))) { REPORT << #a << " == " #b << "\n"; fails += 1; } )

#define EXPECT_NE(a,b) \
	WRAP (if ((a) == (b)) { REPORT << #a << " != " #b << "\n"; fails += 1; } )

#define EXPECT_TRUE(a) \
	WRAP (if (!(a)) { REPORT << "!" << #a "\n"; fails += 1; } )

#define EXPECT_FALSE(a) \
	WRAP (if (a) { REPORT << #a "\n"; fails += 1; } )

#define ASSERT_EQ(a,b) \
	WRAP (if (!((a) == (b))) { REPORT << #a << " == " #b << "\n"; fails += 1; return; } )

#define ASSERT_NE(a,b) \
	WRAP (if ((a) == (b)) { REPORT << #a << " != " #b << "\n"; fails += 1; return; } )

#define ASSERT_TRUE(a) \
	WRAP (if (!(a)) { REPORT << "!" << #a "\n"; fails += 1; return; } )

#define ASSERT_FALSE(a) \
	WRAP (if (a) { REPORT << #a "\n"; fails += 1; return; } )

std::vector<std::function<void()>> fns;
bool registerTestFn(std::function<void()> fn)
{
	fns.push_back(fn);
	return true;
}

constexpr auto GREEN_TEXT_START = "\x1B[32m";
constexpr auto RED_TEXT_START   = "\x1B[31m";
constexpr auto COLOUR_TEXT_END  = "\033[0m\t\t";

#define TEST(TestCase) \
	namespace {\
	struct TestStruct##TestCase \
	{ \
		static void Test() \
		{\
		    ++tests_ran; \
			const auto currentFails = fails; \
			std::cout << GREEN_TEXT_START << "Testing " << #TestCase << COLOUR_TEXT_END << "\n"; \
			TestStruct##TestCase::TestBody(); \
			if (currentFails < fails) \
			{ \
				++tests_failed; \
				std::cout << RED_TEXT_START << "Tested " << #TestCase << COLOUR_TEXT_END << "\n"; \
			} \
			else \
			{ \
				std::cout << GREEN_TEXT_START << "Tested " << #TestCase << COLOUR_TEXT_END << "\n"; \
			} \
		} \
		static void TestBody(); \
	};} \
	static const auto sTestStruct##TestCase = registerTestFn(TestStruct##TestCase::Test); \
	void TestStruct##TestCase::TestBody()

int runAllTests()
{
	fails = 0;
	tests_failed = 0;
	tests_ran = 0;

	for (auto& fn : fns)
	{
		fn();
	}

	if (fails)
	{
		std::cout << RED_TEXT_START << "Ran " << tests_ran << " tests and " << tests_failed << " failed" << COLOUR_TEXT_END << "\n";
	}
	else
	{
		std::cout << GREEN_TEXT_START << "Ran " << tests_ran << " tests and none failed" << COLOUR_TEXT_END << "\n";
	}

	return fails;
}

}
