# Ubuntu 24.04 provides GCC 13 and Clang 18; keep formatting aligned with CI.
ARG VARIANT=ubuntu-24.04
FROM mcr.microsoft.com/devcontainers/cpp:${VARIANT}

# GCC, CMake and GDB are provided by the C++ base image.
# clang-tidy-19 matches .github/workflows/clang-tidy-review.yaml.
RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get -y install --no-install-recommends \
        clang \
        clang-format \
        clangd \
        clang-tidy-19 \
        libgtest-dev \
        ninja-build \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

COPY first-run-notice.txt /tmp/scripts/
