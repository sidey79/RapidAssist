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

#include "TestDemo.h"
#include "rapidassist/gtesthelp.h"
#include "rapidassist/strings.h"

namespace ra { namespace test {

  void TestDemo::SetUp()
  {
  }

  void TestDemo::TearDown()
  {
  }

  std::string generateTestFile()
  {
    //for testing purpose only
    return std::string(__FILE__);
  }
  std::string getExpectedTestFilePath()
  {
    //for testing purpose only
    return std::string(__FILE__);
  }

  TEST_F(TestDemo, testCodeSample)
  {
    //create a dummy file
    static const int FILE_SIZE = 1000; //bytes
    const std::string path = ra::gtesthelp::getTestQualifiedName() + ".tmp"; //returns "TestDemo.testCodeSample.tmp"
    ASSERT_TRUE( ra::gtesthelp::createFile(path.c_str(), FILE_SIZE) );
  
    //test that a generated file is equals to the expected file
    std::string generatedFile = generateTestFile();
    std::string expectedFile = getExpectedTestFilePath();
    ASSERT_TRUE ( ra::gtesthelp::isFileEquals( expectedFile.c_str(), generatedFile.c_str()) );

    //split a string into multiple parts
    StringVector words = ra::strings::splitString("The quick brown fox jumps over the lazy dog", " ");
    size_t numWords = words.size();

    //converting numeric values to string
    std::string IntMaxStr = ra::strings::toString(UINT64_MAX); //returns "18446744073709551615"

    //search and replace in strings
    std::string whoiam = "My name is Antoine and I am a superhero.";
    int numReplaced = ra::strings::strReplace(whoiam, "hero", "vilan");
  }

} //namespace test
} //namespace ra
