MYSQL = -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -ldl -lssl -lcrypto -lresolv -lm -lrt -I/usr/include/mysql
OBJS = ./out/main.o ./out/HeadData.o ./out/DataEncoder.o ./out/DataProcesser.o ./out/Online.o ./out/UserService.o ./out/MySqlConnector.o ./out/myTime.o ./out/Server.o
JSON = -I/usr/include/json -ljsoncpp
main: $(OBJS)
	g++ -std=c++11 $(OBJS) -o main $(MYSQL) $(JSON)
./out/main.o: main.cpp
	g++ -std=c++11 -c main.cpp -o ./out/main.o
./out/HeadData.o: ./ProtocolHead/HeadData.cpp
	g++ -std=c++11 -c ./ProtocolHead/HeadData.cpp -o ./out/HeadData.o
./out/DataEncoder.o: ./ProtocolHead/DataEncoder.cpp
	g++ -std=c++11 -c ./ProtocolHead/DataEncoder.cpp -o ./out/DataEncoder.o
./out/DataProcesser.o: ./Service/DataProcesser.cpp
	g++ -std=c++11 -c ./Service/DataProcesser.cpp -o ./out/DataProcesser.o
./out/Online.o: ./Service/Online.cpp
	g++ -std=c++11 -c ./Service/Online.cpp -o ./out/Online.o $(JSON)
./out/UserService.o: ./Service/UserService.cpp
	g++ -std=c++11 -c ./Service/UserService.cpp -o ./out/UserService.o
./out/MySqlConnector.o: ./Dao/MySqlConnector.cpp ./myTime/myTime.h ./config/mysql_config.h
	g++ -std=c++11 -c ./Dao/MySqlConnector.cpp -o ./out/MySqlConnector.o  $(MYSQL)
./out/myTime.o: ./myTime/myTime.cpp
	g++ -std=c++11 -c ./myTime/myTime.cpp -o ./out/myTime.o
./out/Server.o: ./Server/Server.cpp
	g++ -std=c++11 -c ./Server/Server.cpp -o ./out/Server.o
clean:
	rm -rf ./out/*.o main test