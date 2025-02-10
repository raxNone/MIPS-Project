# MIPS-Project
mips emulator project

```sh
git clone https://github.com/raxNone/MIPS-Project.git &&
cd MIPS-Project &&
make clean &&
make &&
make run

```

## 📌 기본 명령어

### 1. 빌드 (Build)
```sh
make
```
- `build/` 디렉터리에 오브젝트 파일을 생성하고 실행 파일을 빌드합니다.
- 최종 실행 파일은 `build/main`으로 생성됩니다.

### 2. 실행 (Run)
```sh
make run
```
- 빌드된 `build/main`을 실행합니다.
- 실행 시 `v3.0.0/data/input.txt` 같은 파일이 필요할 경우, 상대 경로를 확인하세요.

### 3. 디버그 빌드 (Debug Build)
```sh
make debug
```
- `-g` 옵션을 추가하여 디버깅 가능한 실행 파일을 생성합니다.
- `gdb build/main`을 사용하여 디버깅할 수 있습니다.

### 4. 클린업 (Clean)
```sh
make clean
```
- `build/` 디렉터리 내부의 모든 오브젝트 파일과 실행 파일을 삭제합니다.

## 📁 프로젝트 구조
```
프로젝트_루트/
│── v3.0.0/
│   ├── src/      # C++ 소스 파일 (.cpp)
│   ├── incs/     # 헤더 파일 (.hpp)
│   ├── data/     # 입력 파일 및 리소스
│
│── build/        # 빌드된 파일 저장 (Make 실행 시 생성됨)
│   ├── main      # 최종 실행 파일
│   ├── crow/     # Crow 라이브러리 포함
│
│── Makefile      # 빌드 스크립트
│── README.md     # 현재 문서
```

## ⚠️ 주의 사항
- `build/main`을 실행할 때 파일 입출력 경로를 확인하세요.
- 실행 시 `build/` 내부에서 실행하면 `../v3.0.0/data/` 같은 상대 경로를 사용해야 할 수도 있습니다.
- `make clean`을 실행하면 빌드된 파일이 삭제되므로, 다시 실행하려면 `make`를 먼저 실행하세요.

## ✅ 추가 사항
추가할 기능이나 수정할 내용이 있다면 `Makefile`을 직접 수정하거나 README를 업데이트하세요!

