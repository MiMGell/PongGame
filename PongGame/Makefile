win:
	g++ -static -static-libgcc -static-libstdc++ src/** -o build/Pong.exe -O2 -Wall -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows

linux:
	g++ src/** -o build/Pong.x86_64 -O2 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
