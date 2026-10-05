# GoogleTest template

Provisioned from [`Qode-Fleet-Control/fleet-template-v1`](https://github.com/Qode-Fleet-Control/fleet-template-v1) — the fleet
lifecycle contract (`bin/`, `fleet.conf`, `compose.yaml`, deploy workflows) with a GoogleTest starter laid on top.

A small C++17 library (`calc`: gcd, is_prime, fibonacci, split) and its GoogleTest suite, built with CMake and run with CTest. The suite shows plain `TEST()`s, a fixture (`TEST_F`), a value-parameterized test (`TEST_P` + `INSTANTIATE_TEST_SUITE_P`), `EXPECT_THROW`, and gMock matchers (`ElementsAre`). The image's default command runs the suite; it exits 0 only when every test passes.

## Origin

    hand-written (GoogleTest ships no project generator) — CMakeLists.txt follows GoogleTest's official "Quickstart: Building with CMake" (FetchContent of a pinned release zip, enable_testing(), GTest::gtest_main, include(GoogleTest) + gtest_discover_tests())


## Run it

### On the fleet

The fleet runs it as containers (the docker runtime): `bin/run` builds the image with
`docker compose build` and then stops — this is a job, so `DOCKER_START_CMD` is empty and nothing listens on `$PORT`.

### With docker

```sh
docker compose build
docker compose run --rm app            # runs the job; exit code = result
```

### Without docker

```sh
# Debian/Ubuntu: sudo apt install build-essential cmake   (CMake downloads GoogleTest itself)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j
ctest --test-dir build --output-on-failure
```

`fleet.conf` drives every script in `bin/`:

| step | docker runtime (fleet) | `FLEET_RUNTIME=process` |
|---|---|---|
| install | — | `(none)` |
| build | `docker compose build` | `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j` |
| start | `(none — a job)` | `(none — a job)` |

## Layout

- `include/calc/calc.h`, `src/calc.cpp` — the library under test (target `calc`).
- `tests/calc_test.cpp` — the suite (target `calc_test`, each test registered with CTest by `gtest_discover_tests`).
- `CMakeLists.txt` — GoogleTest v1.17.0 by FetchContent, pinned by SHA-256.
- `Dockerfile` — one `debian:trixie` stage: compiles library and tests at build time as non-root user `app`; `CMD ["ctest", "--test-dir", "build", "--output-on-failure"]`.
- `compose.yaml` — service `app`, no ports (a job), fleet variables passed through by name.

## Deviations from stock, and why

- Pinned to GoogleTest v1.17.0 with a `URL_HASH` (the quickstart's zip URL is a commit snapshot without a hash) and C++17 instead of the quickstart's C++14 minimum.
- The quickstart's single `hello_test.cc` is replaced by a small library and a suite that exercises the main GoogleTest features; the test also links `GTest::gmock` because it uses gMock matchers.
- The Docker image is single-stage on purpose: `ctest` needs CMake and the build tree at run time.
- `PORT`, `HEALTH_PATH`, `START_CMD` and `DOCKER_START_CMD` are empty by design: nothing listens on a port, and `bin/run` builds the image and stops there.

## Verified

2026-10-05, Docker 29.8 on linux/amd64, from the scaffold directory:

- `docker compose build` → built (GoogleTest fetched and compiled inside the build).
- `docker compose run --rm app` → exit 0: `100% tests passed, 0 tests failed out of 15`.
- `docker compose down --rmi local -v` → clean.

The no-docker path (`FLEET_RUNTIME=process`) was not run on a host toolchain; it is the same CMake build the image runs.

See `docs/fleet-lifecycle.md` for the lifecycle contract.
