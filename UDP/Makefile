all: bin/server bin/client

bin/server: src/server.c
	gcc -g src/server.c -o bin/server

server:
	./bin/server

bin/client: src/client.c
	gcc -g src/client.c -o bin/client

client:
	./bin/client

clean:
	rm -rf bin/*


