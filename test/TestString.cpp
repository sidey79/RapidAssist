/**********************************************************************************
 * MIT License
 * 
 * Copyright (c) 2018 Antoine Beauchamp
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *********************************************************************************/

#include "TestString.h"
#include "rapidassist/strings.h"
#include "rapidassist/environment.h"

namespace ra { namespace strings { namespace test
{
  
  //--------------------------------------------------------------------------------------------------
  void TestString::SetUp()
  {
  }
  //--------------------------------------------------------------------------------------------------
  void TestString::TearDown()
  {
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestString, testIsNumeric)
  {
    //lazy test
    for(int i=-100; i<=100; i++)
    {
      for(int j=0; j<=100; j++)
      {
        //build a string from i and j
        static const int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];

        //integers
        sprintf(buffer, "%d", i);
        ASSERT_TRUE(strings::isNumeric(buffer)) << "isNumeric(\"" << buffer << "\") returned false.";

        //floating point
        sprintf(buffer, "%d.%03d", i, j);
        ASSERT_TRUE(strings::isNumeric(buffer)) << "isNumeric(\"" << buffer << "\") returned false.";
      }
    }

    //test special case
    ASSERT_TRUE(strings::isNumeric("+12"));

    //plus or minus sign error
    ASSERT_FALSE(strings::isNumeric("12+123"));
    ASSERT_FALSE(strings::isNumeric("12-123"));
    ASSERT_FALSE(strings::isNumeric("+12+123"));
    ASSERT_FALSE(strings::isNumeric("+12-123"));

    //multiple dots
    ASSERT_FALSE(strings::isNumeric("12.345.67"));
    ASSERT_FALSE(strings::isNumeric("+12.345.67"));

    //alpha characters
    ASSERT_FALSE(strings::isNumeric("+12.34a"));
    ASSERT_FALSE(strings::isNumeric("+12.34!"));
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestString, testStrReplace)
  {
    //at the beginning
    {
      const std::string EXPECTED = "DeaDbeef";
      std::string str = "deadbeef";
      int numReplacements = strReplace(str, "d", "D");
      ASSERT_EQ(2, numReplacements);
      ASSERT_EQ(EXPECTED, str);
    }

    //at the end
    {
      const std::string EXPECTED = "beefDeaD";
      std::string str = "beefdead";
      int numReplacements = strReplace(str, "d", "D");
      ASSERT_EQ(2, numReplacements);
      ASSERT_EQ(EXPECTED, str);
    }

    //replace everything
    {
      const std::string EXPECTED = "aabbccddeeff";
      std::string str = "deadbeef";
      int numReplacements = strReplace(str, "deadbeef", "aabbccddeeff");
      ASSERT_EQ(1, numReplacements);
      ASSERT_EQ(EXPECTED, str);
    }

    //replace nothing
    {
      const std::string EXPECTED = "deadbeef";
      std::string str = "deadbeef";
      int numReplacements = strReplace(str, "notfound", "error");
      ASSERT_EQ(0, numReplacements);
      ASSERT_EQ(EXPECTED, str);
    }

    //replace short string by longer string
    {
      const std::string EXPECTED = "super deadbeef";
      std::string str = "deadbeef";
      int numReplacements = strReplace(str, "dead", "super dead");
      ASSERT_EQ(1, numReplacements);
      ASSERT_EQ(EXPECTED, str);
    }

    //replace long string by short string
    {
      const std::string EXPECTED = "dead!";
      std::string str = "deadbeef";
      int numReplacements = strReplace(str, "beef", "!");
      ASSERT_EQ(1, numReplacements);
      ASSERT_EQ(EXPECTED, str);
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestString, testToStringParseValue)
  {
    //uint64_t
    struct uint64_test
    {
      const char * EXPECTED_STR;
      uint64_t EXPECTED_VALUE;
    };
    static const uint64_test tests[] = {
      {"0", 0ull},
      {"54321", 54321ull},
      {"1234567890123456789", 1234567890123456789ull},
      {"576460752303423488", 0x800000000000000ull},
      {"1152921504606846975", 0xFFFFFFFFFFFFFFFull},
    };
    size_t numTests = sizeof(tests)/sizeof(tests[0]);
    for(size_t i=0; i<numTests; i++)
    {
      const char * EXPECTED_STR = tests[i].EXPECTED_STR;
      const uint64_t & EXPECTED_VALUE = tests[i].EXPECTED_VALUE;

      //assert toString()
      std::string actualStr = toString( EXPECTED_VALUE );
      ASSERT_EQ(EXPECTED_STR, actualStr);

      //assert parseValue()
      uint64_t actualValue = 0;
      bool parsed = parseValue(EXPECTED_STR, actualValue);
      ASSERT_TRUE(parsed);
      ASSERT_EQ(EXPECTED_VALUE, actualValue);
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestString, testCapitalizeFirstCharacter)
  {
    {
      //normal
      const std::string EXPECTED = "Deadbeef";
      const std::string value = "deadbeef";
      std::string actual = capitalizeFirstCharacter(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //empty
      const std::string EXPECTED = "";
      const std::string value = "";
      std::string actual = capitalizeFirstCharacter(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //single letter
      const std::string EXPECTED = "A";
      const std::string value = "a";
      std::string actual = capitalizeFirstCharacter(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //special character
      const std::string EXPECTED = "#!/bin/bash";
      const std::string value = "#!/bin/bash";
      std::string actual = capitalizeFirstCharacter(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //spaces
      const std::string EXPECTED = " foo";
      const std::string value = " foo";
      std::string actual = capitalizeFirstCharacter(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //numbers
      const std::string EXPECTED = "5foo";
      const std::string value = "5foo";
      std::string actual = capitalizeFirstCharacter(value);
      ASSERT_EQ(EXPECTED, actual);
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestString, testUppercase)
  {
    {
      //normal
      const std::string EXPECTED = "DEADBEEF";
      const std::string value = "deadbeef";
      std::string actual = uppercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //empty
      const std::string EXPECTED = "";
      const std::string value = "";
      std::string actual = uppercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //single letter
      const std::string EXPECTED = "A";
      const std::string value = "a";
      std::string actual = uppercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //special character
      const std::string EXPECTED = "#!/BIN/BASH";
      const std::string value = "#!/bin/bash";
      std::string actual = uppercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //spaces
      const std::string EXPECTED = " FOO";
      const std::string value = " foo";
      std::string actual = uppercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //numbers
      const std::string EXPECTED = "5FOO";
      const std::string value = "5foo";
      std::string actual = uppercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestString, testLowercase)
  {
    {
      //normal
      const std::string EXPECTED = "deadbeef";
      const std::string value = "DeAdBeEf";
      std::string actual = lowercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //empty
      const std::string EXPECTED = "";
      const std::string value = "";
      std::string actual = lowercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //single letter
      const std::string EXPECTED = "a";
      const std::string value = "A";
      std::string actual = lowercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //special character
      const std::string EXPECTED = "#!/bin/bash";
      const std::string value = "#!/bIn/BaSh";
      std::string actual = lowercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //spaces
      const std::string EXPECTED = " foo";
      const std::string value = " fOO";
      std::string actual = lowercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //numbers
      const std::string EXPECTED = "5foo";
      const std::string value = "5fOO";
      std::string actual = lowercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestString, testStreamOperators)
  {
    {
      //const void * value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = (environment::isProcess32Bit() ? "fooBAR0x12345678" : "fooBAR0x0000000012345678");
      const void * value = (const void *)0x12345678;
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //const std::string & value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBARtest";
      const std::string value = "test";
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //const char * value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBARtest";
      const char * value = "test";
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //const int8_t & value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBAR-23";
      const int8_t value = -23;
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //const uint8_t & value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBAR200";
      const uint8_t value = 200;
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //const int16_t & value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBAR-31234";
      const int16_t value = -31234;
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //const uint16_t & value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBAR51234";
      const uint16_t value = 51234;
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //const int32_t & value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBAR-2147483600";
      const int32_t value = -2147483600l; //min is -2147483648
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //const uint32_t & value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBAR4123456789";
      const uint32_t value = 4123456789ul; //max is 4294967295
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //const int64_t & value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBAR-9123456789012345678";
      const int64_t value = -9123456789012345678ll; //min is -9223372036854775808
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //const uint64_t & value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBAR18123456789012345678";
      const uint64_t value = 18123456789012345678ull; //max is 18446744073709551615
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestString, testRemoveEOL)
  {
    //test NULL
    {
      removeEOL(NULL);
    }

    //test empty string
    {
      char empty[] = "";
      removeEOL(empty);
    }

    //test windows
    {
      const std::string EXPECTED = "fooBAR";
      char buffer[] = "fooBAR\r\n";
      removeEOL(buffer);
      ASSERT_EQ(EXPECTED, buffer);
    }

    //test unix
    {
      const std::string EXPECTED = "fooBAR";
      char buffer[] = "fooBAR\n";
      removeEOL(buffer);
      ASSERT_EQ(EXPECTED, buffer);
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestString, testSplitString)
  {
    //test NULL
    {
      static const std::string INPUT = "Aa.Bb.Cc";
      StringVector list = splitString(INPUT, (const char *)NULL); //same as not found
      ASSERT_EQ(1, list.size());
      ASSERT_EQ(INPUT, list[0]);
    }

    //test no split found
    {
      static const std::string INPUT = "Aa.Bb.Cc";
      StringVector list = splitString(INPUT, "!");
      ASSERT_EQ(1, list.size());
      ASSERT_EQ(INPUT, list[0]);
    }

    //test found
    {
      static const std::string INPUT = "Aa.Bb.Cc";
      StringVector list = splitString(INPUT, ".");
      ASSERT_EQ(3, list.size());
      ASSERT_EQ("Aa", list[0]);
      ASSERT_EQ("Bb", list[1]);
      ASSERT_EQ("Cc", list[2]);
    }

    //test last character is separator
    {
      static const std::string INPUT = "Aa.Bb.Cc.";
      StringVector list = splitString(INPUT, ".");
      ASSERT_EQ(4, list.size());
      ASSERT_EQ("Aa", list[0]);
      ASSERT_EQ("Bb", list[1]);
      ASSERT_EQ("Cc", list[2]);
      ASSERT_EQ("",   list[3]);
    }

    //test first character is separator
    {
      static const std::string INPUT = ".Aa.Bb.Cc";
      StringVector list = splitString(INPUT, ".");
      ASSERT_EQ(4, list.size());
      ASSERT_EQ("",   list[0]);
      ASSERT_EQ("Aa", list[1]);
      ASSERT_EQ("Bb", list[2]);
      ASSERT_EQ("Cc", list[3]);
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestString, testJoinString)
  {
    //test NULL
    {
      static const std::string EXPECTED = "Aa.Bb.Cc";
      StringVector list = splitString(EXPECTED, ".");
      std::string joinStr = joinString(list, (const char *)NULL); //same as empty separator
      ASSERT_EQ(std::string("AaBbCc"), joinStr);
    }

    //test empty separator
    {
      static const std::string EXPECTED = "Aa.Bb.Cc";
      StringVector list = splitString(EXPECTED, ".");
      std::string joinStr = joinString(list, "");
      ASSERT_EQ(std::string("AaBbCc"), joinStr);
    }

    //test normal
    {
      static const std::string EXPECTED = "Aa.Bb.Cc";
      static const char * SEPARATOR = ".";
      StringVector list = splitString(EXPECTED, SEPARATOR);
      std::string joinStr = joinString(list, SEPARATOR);
      ASSERT_EQ(EXPECTED, joinStr);
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestString, testTrim)
  {
    {
      static const std::string value = "   abc   ";
      ASSERT_EQ("abc", trim(value));
      ASSERT_EQ("abc   ", trimLeft(value));
      ASSERT_EQ("   abc", trimRight(value));
    }

    {
      static const std::string value = "***abc***";
      ASSERT_EQ("abc", trim(value, '*'));
      ASSERT_EQ("abc***", trimLeft(value, '*'));
      ASSERT_EQ("***abc", trimRight(value, '*'));
    }

    ASSERT_EQ("", trim(""));
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestString, testReverse)
  {
    //assert even number of characters
    ASSERT_EQ("abc", reverse("cba"));

    //assert odd number of characters
    ASSERT_EQ("abcd", reverse("dcba"));

    ASSERT_EQ("", reverse(""));
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestString, testFormat)
  {
    std::string text = ra::strings::format("%d %s %c %3.2f", 23, "this is a string", 'e', 4.234);
    ASSERT_EQ("23 this is a string e 4.23", text );
  }
  //--------------------------------------------------------------------------------------------------
} //namespace test
} //namespace strings
} //namespace ra
