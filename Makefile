main.exe: main.o
	g++ -std=c++11 main.o -o main.exe

main.o: main.cpp
	g++ -std=c++11 -c main.cpp -o main.o

.PHONY: clean doc

clean:
	rm -rf *.o *.exe *.gch ./html ./latex

doc:
	doxygen Doxyfile
