.DEFAULT_GOAL := all
BUILD_DIR ?= build
BUILD_TYPE ?= Release
JOBS ?= 4
PYTHON ?= python3
CLANG_FORMAT ?= clang-format
CMAKE_ARGS ?=
CTEST_ARGS ?=

.PHONY: all build configure test test-gtest test-python test-cases test-unittest test-node install-hooks format format-changed format-check clean help
all: build
configure:
	cmake -S . -B "$(BUILD_DIR)" -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) $(CMAKE_ARGS)
build: configure
	cmake --build "$(BUILD_DIR)" --config $(BUILD_TYPE) --parallel $(JOBS)
test: test-gtest test-python
test-gtest: build
	cd "$(BUILD_DIR)" && ctest -C $(BUILD_TYPE) --output-on-failure $(CTEST_ARGS)
test-python: test-cases test-unittest
test-cases: build
	$(PYTHON) scripts/run_cases.py --build-dir "$(BUILD_DIR)"
	$(PYTHON) scripts/run_cases.py --build-dir "$(BUILD_DIR)" --example bfs_visualization
test-unittest:
	PYTHONDONTWRITEBYTECODE=1 $(PYTHON) -m unittest discover -s .github/scripts -p 'test_*.py'
test-node:
	npm --prefix tools/graph_viewer test
	node --test .github/scripts/clang-tidy-post.test.cjs
install-hooks:
	bash setup-git-hooks.sh
format:
	CLANG_FORMAT="$(CLANG_FORMAT)" bash format.sh all
format-changed:
	CLANG_FORMAT="$(CLANG_FORMAT)" bash format.sh changed
format-check:
	CLANG_FORMAT="$(CLANG_FORMAT)" bash format.sh check
clean:
	cmake --build "$(BUILD_DIR)" --config $(BUILD_TYPE) --target clean
help:
	@printf '%s\n' 'all/build: configure and build everything (default)' 'configure: configure CMake' 'test: gtest and Python checks' 'test-gtest: all CTest tests' 'test-python: file cases and unittest' 'test-cases: task and BFS input/output cases' 'test-unittest: Python unit tests' 'test-node: viewer and CI script tests' 'install-hooks: install formatting pre-commit hook' 'format: all C++ files' 'format-changed: local changes and new C++ files' 'format-check: check C++ formatting' 'clean: clean build outputs' 'Variables: BUILD_DIR BUILD_TYPE JOBS PYTHON CLANG_FORMAT CMAKE_ARGS CTEST_ARGS'
