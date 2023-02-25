MYSQL = -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -ldl -lssl -lcrypto -lresolv -lm -lrt -I/usr/include/mysql
test: ../out/MySqlConnector.o ../out/test.o ../out/myTime.o
	g++ -std=c++11 ../out/MySqlConnector.o ../out/test.o ../out/myTime.o -o test $(MYSQL)
../out/test.o: test.cpp
	g++ -std=c++11 -c test.cpp -o ../out/test.o
../out/MySqlConnector.o: MySqlConnector.cpp ../myTime/myTime.h ../config/mysql_config.h
	g++ -std=c++11 -c MySqlConnector.cpp -o ../out/MySqlConnector.o  $(MYSQL)
clean:
	rm ../out/test.o ../out/MySqlConnector.o test