run:
	gcc main.c Bin/nano/nano.c Bin/portscanner/portscanner.c Bin/chat/client.c Bin/chat/server.c Bin/color/color.c Bin/stringFunc/stringFunc.c Bin/menu/menu.c Bin/simulatorCat/simulatorCat.c Bin/doomLite/doomLite.c Bin/spaceDown/spaceDown.c Bin/myFlappyBird/myFlappyBird.c -o Amina.exe -lShlwapi -lWs2_32 -lIphlpapi
	Amina.exe

clean:
	del Amina.exe