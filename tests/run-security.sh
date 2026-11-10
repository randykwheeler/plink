#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
build_dir=$(mktemp -d)
flags="-std=c++11 -g -O1 -fno-omit-frame-pointer -fsanitize=address,undefined"
g++ $flags tests/security_rconnection.cpp -lcrypt -o "$build_dir/security-rconnection"
g++ $flags tests/security_socket.cpp -pthread -o "$build_dir/security-socket"
ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1 "$build_dir/security-rconnection"
ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1 "$build_dir/security-socket"
ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1 python3 tests/security_polyfill.py
