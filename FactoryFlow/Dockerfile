FROM ubuntu:24.04

# 패키지 설치 중 사용자 입력을 요구하지 않도록 설정
ENV DEBIAN_FRONTEND=noninteractive

# C++ 빌드 도구 + FactoryFlow 의존 라이브러리 설치
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    libboost-system-dev \
    libpqxx-dev \
    nlohmann-json3-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# 컨테이너 내부 작업 폴더
WORKDIR /app

# 현재 FactoryFlow 프로젝트 파일들을 컨테이너로 복사
COPY . .

# CMake 설정
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Collector 빌드
RUN cmake --build build --config Release -j2

# Collector가 사용할 기본 TCP 포트
EXPOSE 9000

# 컨테이너가 시작될 때 Collector 실행
CMD ["./build/FactoryFlowCollector"]