#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "connector_api.h"

size_t dp_process_string(char * const string, char * const buffer, size_t const bytes_available, size_t * bytes_used_ptr, connector_bool_t need_quotes, connector_bool_t first_chunk);
connector_bool_t string_needs_quotes(char const * const string);

}

TEST_GROUP(string_needs_quotes) {};

TEST(string_needs_quotes, NoQuotes)
{
    char const string[] = "NoQuotesForThis";
    connector_bool_t const need_quotes = string_needs_quotes(string);
    CHECK_EQUAL(connector_false, need_quotes);
}

TEST(string_needs_quotes, QuotesForSpaced)
{
    char const string[] = "S p a c e d";
    connector_bool_t const need_quotes = string_needs_quotes(string);
    CHECK_EQUAL(connector_true, need_quotes);
}

TEST(string_needs_quotes, QuotesForTabulators)
{
    char const string[] = "\tTabulatedNeedsQuotes";
    connector_bool_t const need_quotes = string_needs_quotes(string);
    CHECK_EQUAL(connector_true, need_quotes);
}

TEST(string_needs_quotes, QuotesForDoubleQuotes)
{
    char const string[] = "\"What?-Beethoven\"";
    connector_bool_t const need_quotes = string_needs_quotes(string);
    CHECK_EQUAL(connector_true, need_quotes);
}

TEST(string_needs_quotes, NoQuotesForSingleQuotes)
{
    char const string[] = "'What?-Beethoven'";
    connector_bool_t const need_quotes = string_needs_quotes(string);
    CHECK_EQUAL(connector_false, need_quotes);
}

TEST(string_needs_quotes, QuotesForCommas)
{
    char const string[] = "C,S,V";
    connector_bool_t const need_quotes = string_needs_quotes(string);
    CHECK_EQUAL(connector_true, need_quotes);
}

TEST(string_needs_quotes, QuotesForCarriageReturn)
{
    char const string[] = "C\rS\rV";
    connector_bool_t const need_quotes = string_needs_quotes(string);
    CHECK_EQUAL(connector_true, need_quotes);
}

TEST(string_needs_quotes, QuotesForNewLine)
{
    char const string[] = "C\nS\nV";
    connector_bool_t const need_quotes = string_needs_quotes(string);
    CHECK_EQUAL(connector_true, need_quotes);
}

TEST(string_needs_quotes, QuotesForCrLf)
{
    char const string[] = "C\r\nS\r\nV";
    connector_bool_t const need_quotes = string_needs_quotes(string);
    CHECK_EQUAL(connector_true, need_quotes);
}

TEST_GROUP(dp_process_string_test) {};

TEST(dp_process_string_test, testNoSpaces)
{
    size_t bytes_processed;
    size_t bytes_copied = 0;
    char string[] = "No_spaces_string";
    char expected_string[] = "No_spaces_string";
    char buffer[] = "Fight procrastination... tomorrow!";

    bytes_processed = dp_process_string(string, buffer, sizeof buffer, &bytes_copied, string_needs_quotes(string), connector_true);
    CHECK_EQUAL(strlen(string), bytes_copied);
    CHECK_EQUAL(bytes_copied, bytes_processed);
    CHECK_EQUAL(0, memcmp(expected_string, buffer, strlen(expected_string)));
}

TEST(dp_process_string_test, testEmpty)
{
    size_t bytes_processed;
    size_t bytes_copied = 0;
    char string[] = "";
    char expected_string[] = "";
    char buffer[] = "Fight procrastination... tomorrow!";

    bytes_processed = dp_process_string(string, buffer, sizeof buffer, &bytes_copied, string_needs_quotes(string), connector_true);
    CHECK_EQUAL(strlen(string), bytes_copied);
    CHECK_EQUAL(bytes_copied, bytes_processed);
    CHECK_EQUAL(0, memcmp(expected_string, buffer, strlen(expected_string)));
}

TEST(dp_process_string_test, testSpaced)
{
    size_t bytes_processed;
    size_t bytes_copied = 0;
    char string[] = "s p a c e s";
    char expected_string[] = "\"s p a c e s\"";
    char buffer[] = "Fight procrastination... tomorrow!";

    bytes_processed = dp_process_string(string, buffer, sizeof buffer, &bytes_copied, string_needs_quotes(string), connector_true);
    CHECK_EQUAL(strlen(string), bytes_copied);
    CHECK_EQUAL(strlen(expected_string), bytes_processed);
    CHECK_EQUAL(0, memcmp(expected_string, buffer, strlen(expected_string)));
}

TEST(dp_process_string_test, testCommas)
{
    size_t bytes_processed;
    size_t bytes_copied = 0;
    char string[] = "c,o,m,m,a,s";
    char expected_string[] = "\"c,o,m,m,a,s\"";
    char buffer[] = "Fight procrastination... tomorrow!";

    bytes_processed = dp_process_string(string, buffer, sizeof buffer, &bytes_copied, string_needs_quotes(string), connector_true);
    CHECK_EQUAL(strlen(string), bytes_copied);
    CHECK_EQUAL(strlen(expected_string), bytes_processed);
    CHECK_EQUAL(0, memcmp(expected_string, buffer, strlen(expected_string)));
}

TEST(dp_process_string_test, testCarriage)
{
    size_t bytes_processed;
    size_t bytes_copied = 0;
    char string[] = "C\rA\nR\nR\nI\r \tAbulator";
    char expected_string[] = "\"C\rA\nR\nR\nI\r \tAbulator\"";
    char buffer[] = "Fight procrastination... tomorrow!";

    bytes_processed = dp_process_string(string, buffer, sizeof buffer, &bytes_copied, string_needs_quotes(string), connector_true);
    CHECK_EQUAL(strlen(string), bytes_copied);
    CHECK_EQUAL(strlen(expected_string), bytes_processed);
    CHECK_EQUAL(0, memcmp(expected_string, buffer, strlen(expected_string)));
}

TEST(dp_process_string_test, testQuotes)
{
    size_t bytes_processed;
    size_t bytes_copied = 0;
    char string[] = "\"Q\"u\"o\"t\"e\"s";
    char expected_string[] = "\"\\\"Q\\\"u\\\"o\\\"t\\\"e\\\"s\"";
    char buffer[] = "Fight procrastination... tomorrow!";

    bytes_processed = dp_process_string(string, buffer, sizeof buffer, &bytes_copied, string_needs_quotes(string), connector_true);
    CHECK_EQUAL(strlen(string), bytes_copied);
    CHECK_EQUAL(strlen(expected_string), bytes_processed);
    CHECK_EQUAL(0, memcmp(expected_string, buffer, strlen(expected_string)));
}

TEST(dp_process_string_test, testMixed)
{
    size_t bytes_processed;
    size_t bytes_copied = 0;
    char string[] = "M i,x \" E\nD\r ";
    char expected_string[] = "\"M i,x \\\" E\nD\r \"";
    char buffer[] = "Fight procrastination... tomorrow!";

    bytes_processed = dp_process_string(string, buffer, sizeof buffer, &bytes_copied, string_needs_quotes(string), connector_true);
    CHECK_EQUAL(strlen(string), bytes_copied);
    CHECK_EQUAL(strlen(expected_string), bytes_processed);
    CHECK_EQUAL(0, memcmp(expected_string, buffer, strlen(expected_string)));
}

TEST(dp_process_string_test, testNotEnoughSize)
{
    size_t bytes_processed;
    size_t bytes_copied = 0;
    char string[] = "string";
    char expected_string[] = "str";
    char buffer[4];

    bytes_processed = dp_process_string(string, buffer, sizeof buffer, &bytes_copied, string_needs_quotes(string), connector_true);
    CHECK_EQUAL(strlen(string), bytes_processed);
    CHECK_EQUAL(bytes_copied, bytes_copied);
    STRCMP_EQUAL(expected_string, buffer);
}

TEST(dp_process_string_test, testNotEnoughSizeQuoted)
{
    size_t bytes_processed;
    size_t bytes_copied = 0;
    char string[] = "s,tr";
    char expected_string[] = "\"s,";
    char buffer[4];

    bytes_processed = dp_process_string(string, buffer, sizeof buffer, &bytes_copied, string_needs_quotes(string), connector_true);
    CHECK_EQUAL(4, bytes_copied);
    STRCMP_EQUAL(expected_string, buffer);
    CHECK_EQUAL(strlen("\"s,tr\""), bytes_processed);
}

TEST(dp_process_string_test, testBufferOverrun)
{
    size_t bytes_processed;
    size_t bytes_copied = 0;
    char string[] = "Just a simple string, except for that comma, nothing special";
    char buffer[128];

    memset(buffer, '*', sizeof buffer);
    bytes_processed = dp_process_string(string, buffer, 5, &bytes_copied, string_needs_quotes(string), connector_true);

    CHECK(buffer[bytes_processed] != '\0');
}
