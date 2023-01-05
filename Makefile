#########################################################
#             PROJECT 4: GERP Makefile             #
#########################################################

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS  = -g3 
OFLAG = -O2


# Rule to make gerp executable
gerp: main.o gerp.o FSTree.o DirNode.o hashTable.o stringProcessing.o
	$(CXX) $(CXXFLAG) $(OFLAG) -o gerp stringProcessing.o main.o gerp.o \
								FSTree.o \
								DirNode.o \
								hashTable.o \

gerp.o: gerp.h gerp.cpp hashTable.cpp stringProcessing.cpp FSTree.o \
		DirNode.o
	$(CXX) $(CXXFLAG) $(OFLAG) -c gerp.cpp

main.o: main.cpp gerp.cpp hashTable.cpp stringProcessing.cpp FSTree.o DirNode.o
	$(CXX) $(CXXFLAG) $(OFLAG) -c main.cpp

# Rule to build hashTable.o
hashTable.o: hashTable.cpp hashTable.h 
	$(CXX) $(CXXFLAG) $(OFLAG) -c hashTable.cpp

# This rule builds the FSTreeTraversal object file into an executeable 
treeTraversal: FSTreeTraversal.o FSTree.o DirNode.o
	$(CXX) $(LDFLAGS) $(OFLAG) -o treeTraversal FSTreeTraversal.o FSTree.o \
								DirNode.o

# Rule to build FSTreeTraversal.o
FSTreeTraversal.o: FSTreeTraversal.cpp FSTree.o DirNode.o
	$(CXX) $(CXXFLAG) $(OFLAG) -c FSTreeTraversal.cpp

# Rule to build stringProcessing.o
stringProcessing.o: stringProcessing.cpp stringProcessing.h
	$(CXX) $(CXXFLAG) $(OFLAG) -c stringProcessing.cpp

# This rule builds the unit test using the files included for testing
unit_test: unit_test_driver.o stringProcessing.o hashTable.o \
			gerp.o DirNode.o FSTree.o
	$(CXX) $^

provide:
	provide comp15 proj4_gerp README Makefile FSTreeTraversal.cpp \
	stringProcessing.h stringProcessing.cpp hashTable.h hashTable.cpp \
	main.cpp gerp.h gerp.cpp unit_tests.h 
