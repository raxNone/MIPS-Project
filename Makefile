# 버전 설정
VER = v3.1.0

# 컴파일러 설정
CXX = g++
CXXFLAGS =-std=c++17 -I$(VER)/incs -Ibuild/crow -pthread

# 디렉터리 설정
SRC_DIR = $(VER)/src
INC_DIR = $(VER)/incs
BUILD_DIR = build/build

# 대상 실행 파일
TARGET = main

# 소스 파일 목록
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# 기본 빌드 규칙
all: $(TARGET)

# 실행 파일 생성
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 오브젝트 파일 생성 규칙
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 빌드 디렉토리 생성
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# 실행
run: $(TARGET)
	./$(TARGET)

# 클린업
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# 디버그 빌드
debug: CXXFLAGS += -g
debug: clean all
