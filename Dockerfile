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

WORKDIR /repo
COPY . /repo

RUN make

FROM debian:12.9-slim AS ci

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
	cmake ; \
	rm -rf /var/lib/apt/lists/*

FROM debian:12.9-slim AS release

ENV DEBIAN_FRONTEND=noninteractive
COPY --from=base /repo/ft_irc /usr/local/bin/ft_irc

ENTRYPOINT ["ft_irc"]
CMD ["8080", "password"]
