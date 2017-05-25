CXX = g++
CPPFLAGS += -I/usr/local/include -pthread
CXXFLAGS += -std=c++11

PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

PROTOS_PATH = .

vpath %.proto $(PROTOS_PATH)

all: mgmt_svc.grpc.pb.o mgmt_svc.pb.o ds_svc.grpc.pb.o ds_svc.pb.o



%.grpc.pb.o: %.grpc.pb.cc %.pb.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $^

%.pb.o: %.pb.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $^

%.grpc.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

%.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=. $<

clean:
	rm -f *.o *.pb.cc *.pb.h
