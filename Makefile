BUILD_DIR = ./build
BASE_PKG_DIR = ./pkg

# ...

.PHONY: build
build:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_VERBOSE_MAKEFILE=ON ..  && make

.PHONY: test
test: build
	cd build/pkg/tests && make test

.PHONY: find
find:
	@bash -c 'base_dir="$(BASE_PKG_DIR)"; \
	include_dirs=(); \
	while IFS= read -r dir; do \
		if find "$$dir" -maxdepth 1 -type f \( -name "*.h" -o -name "*.hpp" \) | grep -q .; then \
			include_dirs+=("$$dir"); \
		fi; \
	done < <(find "$$base_dir" -type d); \
	for d in "$${include_dirs[@]}"; do \
		echo -n "-I$$d "; \
	done; \
	echo'
