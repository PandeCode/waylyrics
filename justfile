setup:
	meson setup build
	rm -fr ./compile_commands.json
	ln -s ./build/compile_commands.json compile_commands.json

build:
	meson compile -C build
	rm -fr ./waylyrics.so

link:
	ln -s ./build/waylyrics.so ./waylyrics.so
link-opengl:
	ln -s ./build/wb_opengl.so ./waylyrics.so

restart:
	pkill -9 waybar
	waybar &
	disown

waylyrics: build link restart
opengl: build link-opengl restart

build-test:
	g++ -std=c++23 -O2 -Iinclude -Wall -Wextra -pedantic -lstdc++fs -lcurl -lsdbus-c++ -o test.out test/test.cpp
test: build-test
	./test.out

clean:
	rm -fr test waylyrics.so build compile_commands.json
