CXX = g++
CXXFLAGS = -std=c++11 -Wall
OBJS = src/main.o src/Image.o src/TGA.o

OUT = project2.out

$(OUT): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(OUT)

src/main.o: src/main.cpp src/Image.h src/TGA.h
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o src/main.o

src/Image.o: src/Image.cpp src/Image.h src/TGA.h
	$(CXX) $(CXXFLAGS) -c src/Image.cpp -o src/Image.o

src/TGA.o: src/TGA.cpp src/TGA.h
	$(CXX) $(CXXFLAGS) -c src/TGA.cpp -o src/TGA.o

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OBJS) $(OUT)
	rm -rf output/*
