CXX = g++
CPPFLAGS += -I/usr/local/include 
LDFLAGS += -L/lib64 -lgtest

TESTS = BasicTest

all: $(TESTS)

BasicTest: BasicTest.o
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CPPFLAGS) -c $<	

clean:
	rm -rf *.o $(TESTS)
