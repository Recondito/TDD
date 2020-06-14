#include "pch.h"
#include <gtest/gtest.h>
#include <vector>

struct range {

	bool includesFirst = false;
	bool includesLast = false;
	bool foundComma = false;
	std::string first = "";
	std::string last = "";
	int firstN;
	int lastN;
	std::vector<int> range_array;
	
	range(std::string desired_range) {
		if (desired_range[0] == '[') {
			includesFirst = true;
		}
		if (desired_range[0] == '(') {
			includesFirst = false;
		}
		if (desired_range.back() == ']') {
			includesLast = true;
		}
		if (desired_range.back() == ')') {
			includesLast = false;
		}
		for (int i = 1; i < desired_range.length() - 1; i++) {
			if (desired_range[i] == ',') {
				foundComma = true;
				continue;
			}
			if (foundComma == false) {
				first += desired_range[i];
			}
			if (foundComma == true) {
				last += desired_range[i];
			}
		}

		firstN = std::stoi(first, nullptr, 10);
		lastN = std::stoi(last, nullptr, 10);
		if (includesFirst == false) {
			firstN++;
		}
		if (includesLast == false) {
			lastN--;
		}

		for (int i = firstN; i <= lastN; i++) {
			range_array.push_back(i);
		}

	}
	~range() {

	}

	bool contains(unsigned int x) {
		if (range_array.empty()) {
			return false;
		}
		if (range_array.front() <= x && range_array.back() >= x) {
			return true;
		}
		else {
			return false;
		}
	}

	bool overlapsRange(std::vector<int> comparison) {
		for (int i = 0; i < range_array.size(); i++) {
			for (int j = 0; j < comparison.size(); j++) {
				if (range_array[i] == comparison[j]) {
					return true;
				}
			}
		}
		return false;
	}

	bool equals(std::vector<int> comparison) {
		if(comparison.size() != range_array.size()){
			return false;
		}
		if (comparison.front() == range_array.front() && comparison.back() == range_array.back()) {
			return true;
		}
	}

	bool containsRange(std::vector<int> comparison) {
		if (range_array.front() <= comparison.front() && range_array.back() >= comparison.back()) {
			return true;
		}
		else {
			return false;
		}
	}
};

TEST(ConstructorTest, ConstructorReturnsClosedRange) {
	range range("[3,7]");
	std::vector<int> expected_result = { 3,4,5,6,7 };
}

TEST(ConstructorTest, ConstructorReturnsOpenRange) {
	range range("(5,9)");
	std::vector<int> expected_result = { 6, 7, 8 };
	EXPECT_EQ(expected_result, range.range_array);
}

TEST(ConstructorTest, ConstructorReturnsEmptyExecution) {
	range range("(1,1)");
	EXPECT_EQ(true, range.range_array.empty());
}

TEST(ConstructorTest, ConstructorReturnsEmptyExecution2) {
	range range("(1,2)");
	EXPECT_EQ(true, range.range_array.empty());
}

TEST(ContainsTest, Contains6) {
	range range("[3,6]");
	EXPECT_EQ(true, range.contains(6));
}

TEST(ContainsTest, Contains0) {
	range range("(0,8]");
	EXPECT_EQ(false, range.contains(0));
}

TEST(ContainsTest, SignedIntegerOverflow) {
	range range("(0,8]");
	EXPECT_EQ(false, range.contains(2147483648));
}

TEST(ContainsTest, CannotConvert) {
	range range("(0,101]");
	EXPECT_EQ(true, range.contains('e'));
}

TEST(doesNotContainsTest, doesNotContains6) {
	range range("(5,6)");
	ASSERT_TRUE(!range.contains(6));
}

TEST(doesNotContainsTest, doesNotContains5) {
	range range("[6,7]");
	ASSERT_TRUE(!range.contains(5));
}

TEST(doesNotContainsTest, doesNotContains1) {
	range range("[1,3)");
	ASSERT_FALSE(!range.contains(1));
}

TEST(doesNotContainsTest, doesNotContains3) {
	range range("(2,6)");
	ASSERT_FALSE(!range.contains(3));
}


TEST(EndPointsTest, EndPoint34) {
	range range("(2,5)");
	EXPECT_EQ(3, range.range_array.front());
	EXPECT_EQ(4, range.range_array.back());
}

TEST(EndPointsTest, EndPoint15) {
	range range("[1,5]");
	EXPECT_EQ(1, range.range_array.front());
	EXPECT_EQ(5, range.range_array.back());
}

TEST(EndPointsTest, EndPoint99) {
	range range("(-10,10)");
	EXPECT_EQ(-9, range.range_array.front());
	EXPECT_EQ(9, range.range_array.back());
}

TEST(EndPointsTest, EndPoint1) {
	range range("[-1,1]");
	EXPECT_EQ(-1, range.range_array.front());
	EXPECT_EQ(1, range.range_array.back());
}

TEST(OverlapsRangeTest, 24Overlaps) {
	range comparing("[2,4]");
	range range("[3,6]");
	ASSERT_TRUE(range.overlapsRange(comparing.range_array));
}

TEST(OverlapsRangeTest, 45Overlaps) {
	range comparing("[4,5]");
	range range("[3,6]");
	ASSERT_TRUE(range.overlapsRange(comparing.range_array));
}

TEST(OverlapsRangeTest, 12Overlaps) {
	range comparing("[1,2]");
	range range("[3,6]");
	ASSERT_FALSE(range.overlapsRange(comparing.range_array));
}

TEST(OverlapsRangeTest, 710Overlaps) {
	range comparing("[7,10]");
	range range("[3,6]");
	ASSERT_FALSE(range.overlapsRange(comparing.range_array));
}

TEST(doesNotOverlapsRangeTest, 24NotOverlaps) {
	range comparing("[2,4]");
	range range("[3,6]");
	ASSERT_FALSE(!range.overlapsRange(comparing.range_array));
}

TEST(doesNotOverlapsRangeTest, 45NotOverlaps) {
	range comparing("[4,5]");
	range range("[3,6]");
	ASSERT_FALSE(!range.overlapsRange(comparing.range_array));
}

TEST(doesNotOverlapsRangeTest, 12NotOverlaps) {
	range comparing("[1,2]");
	range range("[3,6]");
	ASSERT_TRUE(!range.overlapsRange(comparing.range_array));
}

TEST(doesNotOverlapsRangeTest, 710NotOverlaps) {
	range comparing("[7,10]");
	range range("[3,6]");
	ASSERT_TRUE(!range.overlapsRange(comparing.range_array));
}

TEST(EqualsTest, 04Equals) {
	range comparing("(0,4)");
	range range("[1,3]");
	ASSERT_TRUE(range.equals(comparing.range_array));
}

TEST(EqualsTest, 13Equals) {
	range comparing("[1,3]");
	range range("[1,3]");
	ASSERT_TRUE(range.equals(comparing.range_array));
}

TEST(EqualsTest, 09Equals) {
	range comparing("(0,9)");
	range range("[1,3]");
	ASSERT_FALSE(range.equals(comparing.range_array));
}

TEST(EqualsTest, 11Equals) {
	range comparing("(-1,1)");
	range range("[1,3]");
	ASSERT_FALSE(range.equals(comparing.range_array));
}

TEST(NotEqualsTest, 04NotEquals) {
	range comparing("(0,4)");
	range range("[1,3]");
	ASSERT_FALSE(!range.equals(comparing.range_array));
}

TEST(NotEqualsTest, 13NotEquals) {
	range comparing("[1,3]");
	range range("[1,3]");
	ASSERT_FALSE(!range.equals(comparing.range_array));
}

TEST(NotEqualsTest, 09NotEquals) {
	range comparing("(0,9)");
	range range("[1,3]");
	ASSERT_TRUE(!range.equals(comparing.range_array));
}

TEST(NotEqualsTest, 11NotEquals) {
	range comparing("(-1,1)");
	range range("[1,3]");
	ASSERT_TRUE(!range.equals(comparing.range_array));
}

TEST(ContainsRangeTest, Contains37) {
	range comparing("(3,7)");
	range range("[1,10]");
	ASSERT_TRUE(range.containsRange(comparing.range_array));
}

TEST(ContainsRangeTest, Contains011) {
	range comparing("(0,11)");
	range range("[1,10]");
	ASSERT_TRUE(range.containsRange(comparing.range_array));
}

TEST(ContainsRangeTest, Contains19) {
	range comparing("(-1,9)");
	range range("[1,10]");
	ASSERT_FALSE(range.containsRange(comparing.range_array));
}

TEST(ContainsRangeTest, Contains101) {
	range comparing("[-10,1]");
	range range("[1,10]");
	ASSERT_FALSE(range.containsRange(comparing.range_array));
}

TEST(doesNotContainsRangeTest, NotContains37) {
	range comparing("(3,7)");
	range range("[1,10]");
	ASSERT_FALSE(!range.containsRange(comparing.range_array));
}

TEST(doesNotContainsRangeTest, NotContains011) {
	range comparing("(0,11)");
	range range("[1,10]");
	ASSERT_FALSE(!range.containsRange(comparing.range_array));
}

TEST(doesNotContainsRangeTest, NotContains19) {
	range comparing("(-1,9)");
	range range("[1,10]");
	ASSERT_TRUE(!range.containsRange(comparing.range_array));
}

TEST(doesNotContainsRangeTest, NotContains101) {
	range comparing("[-10,1]");
	range range("[1,10]");
	ASSERT_TRUE(!range.containsRange(comparing.range_array));
}

int main(int argc, char* argv[]) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}