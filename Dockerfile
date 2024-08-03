# Build stage
FROM alpine:latest AS builder

# Install build dependencies
RUN apk add --no-cache \
    build-base \
    cmake \
    clang \
    git \
    postgresql-dev \
    boost=~1.84 boost-dev=~1.84 \
    nlohmann-json=~3.11.3 \
    libpq \
    argon2-dev

# Set working directory
WORKDIR /app

# Build libpqxx
RUN git clone --branch 7.9.1 https://github.com/jtv/libpqxx.git && \
    cd libpqxx && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j16 && \
    make install

# Build spdlog
RUN git clone --branch v1.14.1 https://github.com/gabime/spdlog.git && \
    cd spdlog && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j16 && \
    make install

# Build yaml-cpp
RUN git clone --branch 0.8.0 https://github.com/jbeder/yaml-cpp.git && \
    cd yaml-cpp && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j16 && \
    make install

# Copy your application source code
COPY . .

# Build your application
RUN cmake -D CMAKE_BUILD_TYPE=Release \
    -D CMAKE_C_COMPILER=clang \
    -D CMAKE_CXX_COMPILER=clang++ \
    . && make -j16

# Final stage
FROM alpine:latest

ARG BUILD_NAME
ARG BUILD_REF
ARG BUILD_DATE

# Install runtime dependencies
RUN apk add --no-cache libstdc++ libpq argon2-dev

# Copy runtime libraries and files
COPY --from=builder /usr/lib/libpq.so* /usr/local/lib
COPY --from=builder /usr/local/lib/libpqxx*.a /usr/local/lib
COPY --from=builder /usr/local/lib/libpqxx*.la /usr/local/lib
COPY --from=builder /usr/local/lib/libspdlog*.so* /usr/local/lib
COPY --from=builder /usr/local/lib/libyaml-cpp*.so* /usr/local/lib

# Copy the built executable from the builder stage
RUN mkdir -p /app
COPY --from=builder /app/bin/Release/${BUILD_NAME} /app/app_binary
COPY --from=builder /app/properties.yml /app/properties.yml

# Set the entrypoint
ENTRYPOINT ["/app/app_binary"]

LABEL org.opencontainers.image.created=${BUILD_DATE} \
      org.opencontainers.image.title=${BUILD_NAME} \
      org.opencontainers.image.authors="Cody Johnson <codyj@protonmail.com>" \
      org.opencontainers.image.source="https://github.com/codymj/${BUILD_NAME}" \
      org.opencontainers.image.revision=${BUILD_REF} \
      org.opencontainers.image.vendor="CMJ"
