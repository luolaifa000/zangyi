OBJS = z_server.o z_alloc.o z_conf.o z_tool.o z_connection.o z_hashtable.o z_log.o z_epoll.o z_protocol.o
CTAGSO = -g -o
CTAGSC = -g -c
server : $(OBJS)
	gcc $(CTAGSO) server $(OBJS)
z_server.o : 
	gcc $(CTAGSC) z_server.c 
z_alloc.o : 
	gcc $(CTAGSC) ../alloc/z_alloc.c
z_conf.o : 
	gcc $(CTAGSC) ../conf/z_conf.c
z_tool.o : 
	gcc $(CTAGSC) ../tool/z_tool.c
z_connection.o : 
	gcc $(CTAGSC) ../connection/z_connection.c
z_hashtable.o : 
	gcc $(CTAGSC) ../hashtable/z_hashtable.c
z_log.o : 
	gcc $(CTAGSC) ../log/z_log.c
z_epoll.o : 
	gcc $(CTAGSC) ../epoll/z_epoll.c
z_protocol.o : 
	gcc $(CTAGSC) ../protocol/z_protocol.c
clean : 
	rm server $(OBJS)
