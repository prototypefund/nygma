# fuzzing

example using `-fsanitize=fuzzer`

```shell
$ clang++ -x c++ -W -Wall -Wextra -Weverything -pedantic -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-exit-time-destructors -Wno-global-constructors -Wno-weak-vtables -Wno-gnu-case-range -Wno-padded -Wno-covered-switch-default -I../.. -std=c++2a -Ofast -ffast-math -fomit-frame-pointer -DNDEBUG -march=core-avx2 -mtune=generic -mlzcnt -mpopcnt -mcx16 -msse4.2 -mavx -mavx2 -I.. -I../libnygma -I../libriot -I../repositories/libunclassified -I../repositories/pest dns-trace.driver.cxx -fsanitize=fuzzer -fuse-ld=lld -lpthread -lrt -o dns-trace.driver
```

example using `honggfuzz`

```shell
../../honggfuzz/hfuzz_cc/hfuzz-clang++ -W -Wall -Wextra -Weverything -pedantic -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-exit-time-destructors -Wno-global-constructors -Wno-weak-vtables -Wno-gnu-case-range -Wno-padded -Wno-covered-switch-default -I../.. -std=c++2a -Ofast -ffast-math -fomit-frame-pointer -DNDEBUG -march=core-avx2 -mtune=generic -mlzcnt -mpopcnt -mcx16 -msse4.2 -mavx -mavx2 -I.. -I../libnygma -I../libriot -I../repositories/libunclassified -I../repositories/pest dns-trace.driver.cxx  -lpthread -o dns-trace.driver
```
