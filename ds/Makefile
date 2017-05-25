CXX = g++
CPPFLAGS += -I/user/local/include -I./proto -pthread
CXXFLAGS += -std=c++11
LDFLAGS += -L/usr/local/lib `pkg-config --libs grpc++ grpc` -lgrpc++_reflection -lprotobuf -lpthread -ldl
PROTO_OBJS = ../proto/*.o


all: ds

ds: ds.o
	$(CXX) $^ $(PROTO_OBJS) $(LDFLAGS) -o $@

ds.o: ds.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $^

clean:
	rm -rf *.o ds
