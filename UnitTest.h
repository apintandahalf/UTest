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
	return UTest::runAllTests();
}
*/

#include <iostream>
#include <source_location>
#include <functional>
#include <vector>


#define WRAP(a) \
	do { a } while (false)

#define REPORT \
	const auto src = std::source_location::current(); \
	std::cout << "Test failed: " << src.file_name() << "." << src.function_name() << "." << src.line() << ": " 

#define EXPECT_EQ(a,b) \
	WRAP (if (!((a) == (b))) { REPORT << #a << " == " #b << "\n"; UTest::s_fails += 1; } )

#define EXPECT_NE(a,b) \
	WRAP (if ((a) == (b)) { REPORT << #a << " != " #b << "\n"; UTest::s_fails += 1; } )

#define EXPECT_TRUE(a) \
	WRAP (if (!(a)) { REPORT << "!" << #a "\n"; UTest::s_fails += 1; } )

#define EXPECT_FALSE(a) \
	WRAP (if (a) { REPORT << #a "\n"; UTest::s_fails += 1; } )

#define ASSERT_EQ(a,b) \
	WRAP (if (!((a) == (b))) { REPORT << #a << " == " #b << "\n"; UTest::s_fails += 1; return; } )

#define ASSERT_NE(a,b) \
	WRAP (if ((a) == (b)) { REPORT << #a << " != " #b << "\n"; UTest::s_fails += 1; return; } )

#define ASSERT_TRUE(a) \
	WRAP (if (!(a)) { REPORT << "!" << #a "\n"; UTest::s_fails += 1; return; } )

#define ASSERT_FALSE(a) \
	WRAP (if (a) { REPORT << #a "\n"; UTest::s_fails += 1; return; } )

#define TEST(TestCase) \
	namespace {\
	struct TestStruct##TestCase \
	{ \
		static void Test() \
		{\
		    ++UTest::s_tests_ran; \
			const auto currentFails = UTest::s_fails; \
			std::cout << UTest::GREEN_TEXT_START << "Testing " << #TestCase << UTest::COLOUR_TEXT_END << "\n"; \
			TestStruct##TestCase::TestBody(); \
			if (currentFails < UTest::s_fails) \
			{ \
				++UTest::s_tests_failed; \
				std::cout << UTest::RED_TEXT_START << "Tested " << #TestCase << UTest::COLOUR_TEXT_END << "\n"; \
			} \
			else \
			{ \
				std::cout << UTest::GREEN_TEXT_START << "Tested " << #TestCase << UTest::COLOUR_TEXT_END << "\n"; \
			} \
		} \
		static void TestBody(); \
	};} \
	static const auto sTestStruct##TestCase = UTest::registerTestFn(TestStruct##TestCase::Test); \
	void TestStruct##TestCase::TestBody()

struct UTest
{
	static inline int s_fails = 0;
	static inline int s_tests_failed = 0;
	static inline int s_tests_ran = 0;

	static constexpr auto GREEN_TEXT_START = "\x1B[32m";
	static constexpr auto RED_TEXT_START = "\x1B[31m";
	static constexpr auto COLOUR_TEXT_END = "\033[0m\t\t";
	
	static inline std::vector<std::function<void()>> tests;
	static bool registerTestFn(std::function<void()> test)
	{
		tests.push_back(test);
		return true;
	}

	static int runAllTests()
	{
		s_fails = 0;
		s_tests_failed = 0;
		s_tests_ran = 0;

		for (auto& test : tests)
			test();

		if (s_fails)
			std::cout << RED_TEXT_START << "Ran " << s_tests_ran << " tests and " << s_tests_failed << " failed" << COLOUR_TEXT_END << "\n";
		else
			std::cout << GREEN_TEXT_START << "Ran " << s_tests_ran << " tests and none failed" << COLOUR_TEXT_END << "\n";

		return s_fails;
	}
};
