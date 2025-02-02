# g++ -std=c++20 -O2 -Wall -Wextra -pedantic -lstdc++fs -lgtk3 -lcurl -lsdbus-c++ -c -fPIC -o waylyrics.so waylyrics.cpp

_:
	meson setup build
	rm -fr ./compile_commands.json
	ln -s ./build/compile_commands.json compile_commands.json

waylyrics: build-waylyrics
	pkill -9 waybar
	waybar &
	disown

build-waylyrics:
	meson compile -C build
	rm -fr ./waylyrics.so
	ln -s ./build/wb_cffi.so ./waylyrics.so

test: build-test
	./test

build-test:
	g++ -std=c++20 -O2 -Wall -Wextra -pedantic -lstdc++fs -lcurl -lsdbus-c++ -o test test.cpp

clean:
	rm -fr test waylyrics.so build compile_commands.json
