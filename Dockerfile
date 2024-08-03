# Build stage
FROM alpine:latest AS builder

# Install build dependencies
RUN apk add --no-cache \
    build-base \
    cmake \
    git \
    python3 \
    autoconf \
    automake \
    libtool \
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

# Copy CMake configuration files
RUN mkdir -p /usr/local/share/cmake && \
    cp -r /usr/local/lib/cmake/libpqxx /usr/local/share/cmake/

# Copy your application source code
COPY . .

# Build your application
RUN cmake -D CMAKE_BUILD_TYPE=Release . && make -j16

# Final stage
FROM alpine:latest

# Define arguments
ARG APP_NAME
ARG APP_TAG
ARG BUILD_DATE

# Install runtime dependencies
RUN apk add --no-cache libstdc++ libpq argon2-dev

# Copy runtime libraries and files
COPY --from=builder /usr/local/lib/libpqxx*.a /usr/local/lib
COPY --from=builder /usr/local/lib/libpqxx*.la /usr/local/lib
COPY --from=builder /usr/local/lib/libspdlog*.so* /usr/local/lib
COPY --from=builder /usr/local/lib/libyaml-cpp*.so* /usr/local/lib
COPY --from=builder /usr/lib/libpq.so* /usr/local/lib

# Copy CMake files
COPY --from=builder /usr/local/share/cmake/libpqxx /usr/share/cmake/libpqxx

# Copy the built executable from the builder stage
COPY --from=builder /app/bin/Release/cpp-service /usr/local/bin

# Copy the properties file
COPY --from=builder /app/properties.yml /usr/local/bin
CMD ["ls /usr/local/bin"]

# Set the entrypoint
ENTRYPOINT ["/usr/local/bin/cpp-service"]

# Label
LABEL org.opencontainers.image.created=${BUILD_DATE}
LABEL org.opencontainers.image.title=${APP_NAME}
LABEL org.opencontainers.image.authors="Cody Johnson <codyj@protonmail.com>"
LABEL org.opencontainers.image.source="https://github.com/codymj/cpp-service"
LABEL org.opencontainers.image.revision=${APP_TAG}
LABEL org.opencontainers.image.vendor="CMJ"
