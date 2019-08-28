OBJS = z_test.o z_alloc.o z_conf.o z_tool.o z_connection.o z_hashtable.o z_log.o z_epoll.o z_protocol.o

ztest : $(OBJS)
	gcc -g -o ztest $(OBJS)
z_test.o : 
	gcc -g -c z_test.c 
z_alloc.o : 
	gcc -g -c ../alloc/z_alloc.c
z_conf.o : 
	gcc -g -c ../conf/z_conf.c
z_tool.o : 
	gcc -g -c ../tool/z_tool.c
z_connection.o : 
	gcc -g -c ../connection/z_connection.c
z_hashtable.o : 
	gcc -g -c ../hashtable/z_hashtable.c
z_log.o : 
	gcc -g -c ../log/z_log.c
z_epoll.o : 
	gcc -g -c ../epoll/z_epoll.c
z_protocol.o : 
	gcc -g -c ../protocol/z_protocol.c
clean : 
	rm ztest $(OBJS)
