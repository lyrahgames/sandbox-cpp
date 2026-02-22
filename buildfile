import pkgs = {*/ -*-tests/}
import tests = {*-tests/}

./: $pkgs
./: $tests: install = false
