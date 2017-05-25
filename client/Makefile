CXX = g++
CPPFLAGS += -I/user/local/include -I../proto -pthread
CXXFLAGS += -std=c++11
LDFLAGS += -L/usr/local/lib `pkg-config --libs grpc++ grpc` -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -lprotobuf -lpthread -ldl
PROTO_OBJS = ../proto/*.o


all: client

client: *.o
	$(CXX) $^ $(PROTO_OBJS) $(LDFLAGS) -o $@

%.o: %.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $^

clean:
	rm -rf *.o client
