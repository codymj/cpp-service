# cpp-service

A template for building a C++ web service.

## Dependencies

- [boost](https://github.com/boostorg/boost)
- [libargon2](https://github.com/P-H-C/phc-winner-argon2)
- [libpqxx](https://github.com/jtv/libpqxx)
- [nlohmann/json](https://github.com/nlohmann/json)
- [quill](https://github.com/odygrd/quill)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)

## Build

    $ mkdir -p build
    $ cd build/ && cmake ..
    $ cmake --build . --target docker

## Run

Assumes PostgreSQL is running on the local machine.

    $ docker run -p 8080:8080 \
        -e POSTGRES_PASS=$POSTGRES_PASS \
        --rm --network="host" cpp-service:0.0.1 \
        /app/properties.yml

## TODO

- Containerize PostgreSQL and seed data.
- Implement structured JSON logging.
- Write documentation.
- Implement unit tests.
- Implement JSON validation middleware.
- ...
