FROM mcr.microsoft.com/devcontainers/cpp:1.2.7-debian12 AS base
ENV DEBIAN_FRONTEND=noninteractive

# install google test
RUN set -x; \
	git clone https://github.com/google/googletest.git; \
	cd googletest; \
	mkdir build; \
	cd build; \
	cmake ..; \
	make; \
	make install

FROM debian:12.9-slim AS build
ENV DEBIAN_FRONTEND=noninteractive

COPY --from=base /usr/local/include/ /usr/local/include/
COPY --from=base /usr/local/lib/ /usr/local/lib/

RUN set -x; \
  apt-get update && apt-get install -y --no-install-recommends \
  git \
  ca-certificates \
  curl \
  gpg \
  build-essential \
  lcov \
  clang-format \
  cmake ; \
  rm -rf /var/lib/apt/lists/*

WORKDIR /repo
COPY . /repo

RUN make

FROM gcr.io/distroless/cc-debian12 AS release
ENV DEBIAN_FRONTEND=noninteractive

COPY --from=build /repo/ircserv /usr/local/bin/ircserv

ENTRYPOINT ["ircserv"]
CMD ["8080", "password"]
