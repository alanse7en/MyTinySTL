// Main test file
#include "vectorTest.h"
#include "listTest.h"
#include "rb_treeTest.h"
#include "setTest.h"
#include <set>


int main(int argc, char ** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}