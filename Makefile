all: 
	@echo "lolnope"

main:
	protoc -Iproto --cpp_out=src/proto proto/*.proto
	g++ src/main.cc src/proto/*.cc -o bin/main -Wall -lprotobuf

load_balancer:
	protoc -Iproto --cpp_out=src/proto proto/*.proto
	g++ src/load_balancer.cc src/proto/*.cc -o bin/load_balancer -Wall -lprotobuf -lfcgi -lfcgi++

server:
	protoc -Iproto --cpp_out=src/proto proto/*.proto
	g++ src/server.cc src/proto/*.cc -o bin/server -Wall -lprotobuf -lfcgi -lfcgi++

run_load_balancer:
	spawn-fcgi -p 9100 -n bin/load_balancer

run_server:
	spawn-fcgi -p 9000 -n bin/server
