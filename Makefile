main: main.cpp Headers/FileOperations/* Headers/TUIController.hpp Headers/MatrixOps.hpp
	g++ -o ./FinalProj Headers/FileOperations/* Headers/TUIController.hpp Headers/MatrixOps.hpp main.cpp
	mkdir input
	mkdir output
	cp T1.txt input/
	cp T2-faltante.txt input/

clean:
	rm -f  ./FinalProj
	rm -rf input
	rm -rf output
