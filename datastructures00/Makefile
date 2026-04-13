LIBS = packed_strarray packed_generic_array
OBJS = $(addprefix out/,$(LIBS:=.o))

TEST_BINS = bin/test_packed_strarray bin/test_packed_generic_array

task0: bin/test_packed_strarray
	   bin/test_packed_strarray

task1: bin/test_packed_generic_array
	   bin/test_packed_generic_array

bin/test_%: out/test_%.o out/test_util.o $(OBJS) $(STAFF_OBJS) | bin
	$(CC) $(CFLAGS) $^ -o $@

all: $(TEST_BINS)

CC = clang
ifndef NO_ASAN
  CFLAGS = -fsanitize=address,undefined
  ifeq ($(wildcard .debug),)
    $(shell $(CLEAN_COMMAND))
    $(shell touch .debug)
  endif
# Compiling without asan (run 'make NO_ASAN=true all')
else
  CFLAGS = -O3
  ifneq ($(wildcard .debug),)
    $(shell $(CLEAN_COMMAND))
    $(shell rm -f .debug)
  endif
endif

CFLAGS += -Iinclude -Wall -Wextra -g -fno-omit-frame-pointer -Wno-ignored-qualifiers

out/%.o: library/%.c | out
	@git commit -am "Autocommit of library for ${USER}" > /dev/null || true
	$(CC) -c $(CFLAGS) $^ -o $@
out/%.o: tests/%.c | out
	$(CC) -c $(CFLAGS) $^ -o $@

bin/test_suite_%: out/test_suite_%.o out/test_util.o $(OBJS) | bin
	$(CC) $(CFLAGS) $^ -o $@

test: $(TEST_BINS)
	set -e; for f in $(TEST_BINS); do echo $$f; $$f; echo; done

out:
	mkdir -p $@

bin:
	mkdir -p $@

# Removes all compiled files.
CLEAN_COMMAND = find out/ ! -name .gitignore -type f -delete && \
find bin/ ! -name .gitignore -type f -delete

clean:
	mkdir -p out bin
	$(CLEAN_COMMAND)

# This special rule tells Make that "all", "clean", and "test" are rules
# that don't build a file.
.PHONY: all clean test
# Tells Make not to delete the .o files after the executable is built
.PRECIOUS: out/%.o
