

CUnit Installation and Tryout...
==================================

1. To install CUnit...
	a/ sudo apt-get install synaptic
	b/ sudo apt-get install libcunit1 libcunit1-doc libcunit1-dev

This should install the library files you need and the header files required.
Look in /usr/lib/libcunit*.*  and there should be a few lib files.
and the header files should be in /usr/include/CUnit


2.  To Try out the Simple Example.
	a/ cd ~/tlr/host-devel/unittest/cunit/simple-example (or probably just cd simple-example)
	b/ make
	c/ ./utest


You should see something like this...

     CUnit - A unit testing framework for C - Version 2.1-2
     http://cunit.sourceforge.net/


Suite: max_test_suite
  Test: max_test_1 ...passed
  Test: max_test_2 ...passed
  Test: max_test_3 ...FAILED
    1. max_test.c:45  - CU_ASSERT_EQUAL(max(-1,4),4)
    2. max_test.c:46  - CU_ASSERT_EQUAL(max(4,-1),4)

Run Summary:    Type  Total    Ran Passed Failed Inactive
              suites      1      1    n/a      0        0
               tests      3      3      2      1        0
             asserts      8      8      6      2      n/a

Elapsed time =    0.000 seconds


  1. max_test.c:45  - CU_ASSERT_EQUAL(max(-1,4),4)
  2. max_test.c:46  - CU_ASSERT_EQUAL(max(4,-1),4)

Seems like there is a bug in this function so I look
at the function/module being tested which is max(n1,n2) in max.c ....
(The other files max_test.c is the unit test code which would be created as part of the unit tests
to drive the functions in max.c.  )



int max (int n1, int n2 )
{
   unsigned int en1 = n1;
   unsigned int en2 = n2;
   if ( en2 > en1 )  return en2;
   return en1;
}


So I change max() to be...


int max (int n1, int n2 )
{
   if ( n2 > n1 )  return n2;
   return n1;
}


     CUnit - A unit testing framework for C - Version 2.1-2
     http://cunit.sourceforge.net/


Suite: max_test_suite
  Test: max_test_1 ...passed
  Test: max_test_2 ...passed
  Test: max_test_3 ...passed

Run Summary:    Type  Total    Ran Passed Failed Inactive
              suites      1      1    n/a      0        0
               tests      3      3      3      0        0
             asserts      8      8      8      0      n/a

Elapsed time =    0.000 seconds

Yahooo,  wow unit testing really does work... It fixed my bug.



