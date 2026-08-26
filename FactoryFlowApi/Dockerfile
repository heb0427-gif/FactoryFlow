FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    pkg-config \
    libboost-system-dev \
    libasio-dev \
    libpqxx-dev \
    nlohmann-json3-dev \
    git \
    && rm -rf /var/lib/apt/lists/*

# Crow 설치
RUN git clone --depth 1 https://github.com/CrowCpp/Crow.git /tmp/Crow \
    && cp -r /tmp/Crow/include/* /usr/local/include/ \
    && rm -rf /tmp/Crow

WORKDIR /app

COPY . .

RUN cmake -S . -B build \
    && cmake --build build -j2

EXPOSE 8080

CMD ["./build/FactoryFlowApi"]