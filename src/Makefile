CXX = g++
CXXFLAGS = -g -O3
LDFLAGS += $(shell pkg-config --libs glfw3)
LDFLAGS += $(shell pkg-config --libs glew)
MAKEFLAGS = -j$(shell nproc)

OBJS = main.o shader.o camera.o keyboard.o mouse.o mesh.o space.o autoload.o
TARGET = engine33

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.cpp %.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm $(OBJS)
	rm $(TARGET)
