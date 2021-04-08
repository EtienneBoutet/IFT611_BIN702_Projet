main:
	[ ! -d "bin" ] && mkdir -p "bin"; \
	g++ HirschBerg.cpp Source.cpp -o ./bin/HirschBerg

test:
	[ ! -d "bin" ] && mkdir -p "bin"; \
	g++ HirschBerg.cpp Tests.cpp -o ./bin/tests
	./bin/tests