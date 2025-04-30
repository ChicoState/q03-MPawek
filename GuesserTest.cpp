/**
 * Unit Tests for the class
**/

#include <gtest/gtest.h>
#include "Guesser.h"

class GuesserTest : public ::testing::Test
{
	protected:
		GuesserTest(){} //constructor runs before each test
		virtual ~GuesserTest(){} //destructor cleans up after tests
		virtual void SetUp(){} //sets up before each test (after constructor)
		virtual void TearDown(){} //clean up after each test, (before destructor)
};

// Test simple correct answer
TEST(GuesserTest, correct_test)
{
  	Guesser object ("Secret");
  	bool result = object.match ("Secret");
  	ASSERT_TRUE (result);
}

// Test simple incorrect answer
TEST(GuesserTest, incorrect_test)
{
	Guesser object ("Secret");
  	bool result = object.match ("secret");
  	ASSERT_FALSE (result);
}

// Test two wrong guesses and then correct answer (Should be true)
TEST(GuesserTest, correct_with_incorrect_test)
{
	Guesser object ("Secret");
  	object.match ("secre");
	object.match ("SecreT");
	bool result = object.match ("Secret");
  	ASSERT_TRUE (result);
}

// Test three wrong guesses and then correct answer (should be false due to remaining() countdown)
TEST(GuesserTest, brute_force_test)
{
	Guesser object ("Secret");
  	object.match ("secret");
	object.match ("secre");
	object.match ("SeCret");
	bool result = object.match ("Secret");
  	ASSERT_FALSE (result);
}

// Test the distance() function by triggering a lockout then submitting correct secret (should be false due to distance lockout)
TEST(GuesserTest, fail_distance_test)
{
	Guesser object ("Secret");
  	object.match ("S");
	bool result = object.match ("Secret");
	ASSERT_FALSE (result);
  	
}

// Test the distance() function by differing by a result of 2 chars, then submitting correct secret (should be true; within distance tolerances)
TEST(GuesserTest, pass_distance_test)
{
	Guesser object ("Secret");
  	object.match ("secrtt");
	bool result = object.match ("Secret");
	ASSERT_TRUE (result);
  	
}

// Test the remaining() function by giving incorrect answers within distance bounds (value should be 0 due to 3 incorrect tries)
TEST(GuesserTest, remaining_test)
{
	Guesser object ("Secret");
  	object.match ("secrtt");
	object.match ("secret");
	object.match ("SeCret");
	int result = object.remaining();
	ASSERT_EQ (0, result);
  	
}

// Test how remaining() function works when correct guesses are mixed in with incorrect ones (should be 3 due to correct guess reset)
// Comment above remaining() function describes how it should reset number of allowed guesses after 1 correct guess
TEST(GuesserTest, mixed_remaining_test)
{
	Guesser object ("Secret");
	object.match ("secret");
	object.match ("secre");
	object.match ("Secret");
	int result = object.remaining ();
	ASSERT_EQ (3, result);
}

// Test how remaining() function works when correct guesses are mixed in with incorrect ones (should be 2 due to correct guess reset, then incorrect guess)
// Comment above remaining() function describes how it should reset number of allowed guesses after 1 correct guess
TEST(GuesserTest, limit_of_remaining_test)
{
	Guesser object ("Secret");
	// 2 incorrect guesses, remaining() should be at 1
	object.match ("secret");
	object.match ("secre");
	// Correct guess should reset the counter
	object.match ("Secret");
	// 1 more incorrect guess, remaining() should be at 2
	object.match ("secrete");
	int result = object.remaining ();
	ASSERT_EQ (2, result);
}

// Test how remaining() function works when incorrect guesses are flagged by distance() function (should be 0 due to remaining() still changing to disguise distance lockout)
// Comment above remaining() function describes how it should accurately count remaining even after distance() lockout
TEST(GuesserTest, distance_lockout_test)
{
	Guesser object ("Secret");
	object.match ("SECRET");
	object.match ("");
	object.match ("not secret");
	int result = object.remaining ();
	ASSERT_EQ (0, result);
}

// Test how remaining() function works when incorrect guesses are flagged by distance() function (should be 0 due to remaining() still changing to disguise distance lockout)
// Comment above remaining() function describes how it should accurately count remaining even after distance() lockout
TEST(GuesserTest, extensive_distance_lockout_test)
{
	Guesser object ("Secret");
	object.match ("SECRET");
	object.match ("");
	object.match ("not secret");
	object.match ("secret");
	object.match ("SeCrEt");
	int result = object.remaining ();
	ASSERT_EQ (0, result);
}

// Test if empty string works (should be true, nothing stopping it)
TEST(GuesserTest, empty_string_test)
{
	Guesser object ("");
	bool result = object.match ("");
	ASSERT_TRUE (result);
  	
}

// Test if Guesser initializer handles big strings as it says it does; give same secret value as initially (Should be false; actual secret should be truncated to 32 chars)
TEST(GuesserTest, fail_big_string_test)
{
	Guesser object ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	bool result = object.match ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	ASSERT_FALSE (result);
  	
}

// Test if Guesser initializer handles big strings as it says it does; give same secret value as initially, but truncated down to 32 characters (Should be true, as guess has also been truncated)
TEST(GuesserTest, pass_big_string_test)
{
	Guesser object ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	bool result = object.match ("abcdefghijklmnopqrstuvwxyzABCDEF");
	ASSERT_TRUE (result);
  	
}

// Test if numbers and symbols can be used (should be true, nothing stopping it)
TEST(GuesserTest, pass_number_symbol_test)
{
	Guesser object ("1a!A2b@B");
	bool result = object.match ("1a!A2b@B");
	ASSERT_TRUE (result);
  	
}

// Test if numbers and symbols can be used (should be false, just checking if it can tell difference between characters)
TEST(GuesserTest, fail_number_symbol_test)
{
	Guesser object ("1a!A2b@B");
	bool result = object.match ("2a!A2b*B");
	ASSERT_FALSE (result);
  	
}

// Test if distance accurately allows guesses without lockout that are at least 3 chars longer than Secret (should be true due to the way distance() indicates it should work)
TEST(GuesserTest, pass_longer_guess_test)
{
	Guesser object ("S");
	object.match ("Ssssssssssssssssssssssssssssss");
	bool result = object.match ("S");
	ASSERT_TRUE (result);
}

// Test if distance() and remaining() accurately lock out guesses that are 3 chars longer than Secret after 3 incorrect attempts (should be false due to 3 incorrect guesses)
TEST(GuesserTest, fail_longer_guess_test)
{
	Guesser object ("S");
	object.match ("Ssssssssssssssssssssssssssssss");
	object.match ("Ssssssss");
	object.match ("Ssss");
	bool result = object.match ("S");
	ASSERT_FALSE (result);
}

// Test if distance accurately locks out guesses that are at least 3 chars shorter than Secret (should be false due to only longer guesses allowing lockout leeway)
TEST(GuesserTest, shorter_guess_test)
{
	Guesser object ("SSSSS");
	object.match ("SS");
	bool result = object.match ("SSSSS");
	ASSERT_FALSE (result);
}

// Attempting to see if distance() logic can be gamed (should be false if it's working correctly)
TEST(GuesserTest, distance_stress_test)
{
	Guesser object ("Se");
	object.match ("XXXXXXXXXXXXXXXXXXXXXXX");
	object.match ("afdsajlksajlfjsadlfjeiwfasdflaksjfd");
	bool result = object.match ("Se");
	ASSERT_FALSE (result);
}