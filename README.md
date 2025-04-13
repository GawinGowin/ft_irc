[![C/C++ CI](https://github.com/GawinGowin/ft_irc/actions/workflows/cpp_ci.yml/badge.svg)](https://github.com/GawinGowin/ft_irc/actions/workflows/cpp_ci.yml)
[![Deploy static content to Pages](https://github.com/GawinGowin/ft_irc/actions/workflows/site_deploy.yml/badge.svg)](https://github.com/GawinGowin/ft_irc/actions/workflows/site_deploy.yml)
[![codecov](https://codecov.io/gh/GawinGowin/ft_irc/graph/badge.svg?token=BXEPD68ZQQ)](https://codecov.io/gh/GawinGowin/ft_irc)

<p align="center">
<a href="https://github.com/ayogun/42-project-badges">
<img src="https://raw.githubusercontent.com/ayogun/42-project-badges/refs/heads/main/badges/ft_irce.png">
</p>
</a>
<hr>
<p align="center">
Summary:<br>
This project is about creating your own IRC server.<br>
You will use an actual IRC client to connect to your server and test it.<br>
Internet is ruled by solid standards protocols that allow connected computers to interact<br>
with each other.<br>
It’s always a good thing to know.<br>
Version: 8<br>
</p>
<hr>

# How to run the project

## Make

```bash
make
```

## cmake

```
mkdir -p build \
cd build \
cmake -DCMAKE_VERBOSE_MAKEFILE=ON .. \
make
```

## Docker

```bash
docker compose up -d ft_irc
```

# Testing

## Unit tests

```bash
make test
```

## load tests

```bash
docker compose -f docker-compose.loadgen.yaml up -d
```

## E2E tests

The e2e tests are located in the `tests/e2e` directory.
The test cases jsons are located in the same directory.

```bash
docker compose up -d \
python3 tests/e2e/irc_e2e.py --org localhost:6667 --alt localhost:6668 $TESTCASE_JSONS ...  \
```
