#ifndef GTESTHELPER_H
#define GTESTHELPER_H

#include <string>
#include <vector>

namespace ra
{

  class gTestHelper
  {
    //------------------------
    // Singleton
    //------------------------
  private:
    gTestHelper();
  public:
    ~gTestHelper();
    static gTestHelper & getInstance();

  public:
    //------------------------
    // Inner classes & structures
    //------------------------

    typedef std::vector<std::string> StringVector;

    struct FILE_DIFF
    {
      size_t offset;    //offset in files where a difference is located
      unsigned char c1; //character of first  file
      unsigned char c2; //character of second file
    };

  public:
    //------------------------
    // Methods
    //------------------------

    //
    // Description:
    //  Returns true if the content of two files is identical.
    // Arguments:
    //   iFile1:          The path of the first  file.
    //   iFile2:          The path of the second file.
    //   oReason:         A textual reason why the files are not identical. Empty if files are identical.
    //   iMaxDifferences: The maximum number of textual differences.
    // 
    bool isFileEquals(const char* iFile1, const char* iFile2);
    bool isFileEquals(const char* iFile1, const char* iFile2, std::string & oReason);
    bool isFileEquals(const char* iFile1, const char* iFile2, std::string & oReason, size_t iMaxDifferences);

    //
    // Description:
    //  Returns the location of all differences in two files.
    // Arguments:
    //   iFile1:          The path of the first  file.
    //   iFile2:          The path of the second file.
    //   oDifferences:    The list of all differences within both files. Empty if files are identical.
    //   iMaxDifferences: The maximum number of differences.
    // 
    bool getFileDifferences(const char* iFile1, const char* iFile2, std::vector<FILE_DIFF> & oDifferences, size_t iMaxDifferences);

    //
    // Description:
    //  Returns true if the given text is found in a file.
    // Arguments:
    //   iFilename:   The path of the search file.
    //   iValue:      The search value.
    //   oLine:       The line number where iValue is found.
    //   oCharacter:  The character offset within the line.
    // 
    bool findInFile(const char* iFilename, const char* iValue, int & oLine, int & oCharacter);

    //
    // Description:
    //  Returns the content (each lines) of a file.
    // Arguments:
    //   iFilename:   The path of the file.
    //   oLines:      The content of the file line by line.
    // 
    bool getTextFileContent(const char* iFilename, gTestHelper::StringVector & oLines );

    //
    // Description:
    //  Creates a file of the given size. All bytes are sequential.
    // Arguments:
    //   iFilePath:   The path of the file.
    //   iSize:       The size in bytes of the file.
    // 
    bool createFile(const char * iFilePath, size_t iSize);

    //
    // Description:
    //  Creates text file.
    // Arguments:
    //   iFilePath:   The path of the file.
    // 
    bool createFile(const char * iFilePath);

    //
    // Description:
    //  Modify a given byte with the specified value.
    // Arguments:
    //   iFilePath: The path of the file.
    //   iOffset:   The offset of the modified byte.
    //   iValue:    The value of the replacement.
    // 
    void changeFileContent(const char * iFilePath, size_t iOffset, unsigned char iValue);

    //
    // Description:
    //  Finds the given arguments within an argument list.
    //  Returns true if the argument is found. Returns false otherwise.
    //  Arguments must be supplied with the following format: --name=value
    // Arguments:
    //   iName:   The name of the argument
    //   oValue:  The value of the argument (without the name of the argument)
    //   argc:    The number of arguments supplied in argv.
    //   argv:    The argument list.
    // 
    bool findArgument(const char * iName, std::string & oValue, int argc, char **argv);

    //
    // Description:
    //  This function build a GTEST filter automatically based on a given positive and a negative filter.
    //  The result filter format is the following:
    //    positive[-negative] where 
    //        [ ... ] is optionnal
    //        positive and negative are ':'-separated test case name lists
    // Arguments:
    //   iPositiveFilter:   positive filter
    //   iNegativeFilter:   negative filter
    //   iExistingFilter:   existing filter or a pre-build filter (build from a previous call to mergeFilter)
    // Example:
    //   calling mergeFilter("", "C21XB1.testSniperA2GRanging25:C21XB1.testSniperA2GRanging40") return the filter
    //   "*-C21XB1.testSniperA2GRanging25:C21XB1.testSniperA2GRanging40"
    // 
    std::string mergeFilter(const std::string & iPositiveFilter, const std::string & iNegativeFilter);
    std::string mergeFilter(const std::string & iPositiveFilter, const std::string & iNegativeFilter, const char * iExistingFilter);
    std::string mergeFilter(const std::string & iPositiveFilter, const std::string & iNegativeFilter, int argc, char **argv);

    //
    // Description:
    //  This function split a GTEST filter into a positive and a negative filter.
    // Notes:
    //   iFilter must not be NULL
    // Example:
    //   calling splitFilter("", ..., ...) returns the "" as positive filter and "" as negative filter
    //   calling splitFilter("*", ..., ...) returns the "*" as positive filter and "" as negative filter
    //   calling splitFilter("-TestLib.testCase", ..., ...) returns the "" as positive filter and "TestLib.testCase" as negative filter
    //   calling splitFilter("TestFoo.testFoo-TestLib.testCase:TestMath.testMath", ..., ...) returns the "TestFoo.testFoo" as positive filter and "TestLib.testCase:TestMath.testMath" as negative filter
    // 
    void splitFilter(const char * iFilter, std::string & oPositiveFilter, std::string & oNegativeFilter);

    //
    // Description
    //  Returns a list of all runnable test cases in a gTest compatible test.
    //  Each test case is listed in the following format: testsuite.testcase
    // Example:
    //  TestGTestHelper.testFilters
    // Arguments:
    //  iTestCasePath:    Path to the GTest compatible executable file
    //
    gTestHelper::StringVector getTestList(const char * iTestCasePath);

    //
    // Description
    //  Returns true if the specified processor flag is enabled
    //
    bool isProcessorX86();
    bool isProcessorX64();
    bool isDebugCode();
    bool isReleaseCode();

    //
    // Description
    //  Returns the name of the current Test Suite name. ie "TestFooClass"
    //
    std::string getTestSuiteName();

    //
    // Description
    //  Returns the name of the current Test Case name. ie "testConstructor"
    //
    std::string getTestCaseName();

    //
    // Description
    //  Returns the fully qualified name of the current test case name. ie "TestFooClass.testConstructor"
    //
    std::string getTestQualifiedName();

  };

} //namespace ra

#endif //GTESTHELPER_H
