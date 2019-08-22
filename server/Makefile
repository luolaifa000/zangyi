OBJS = z_server.o z_alloc.o z_log.o z_epoll.o

server : $(OBJS)
	gcc -o server $(OBJS)

z_server.o : 
	gcc -c z_server.c 

z_alloc.o : 
	gcc -c ../alloc/z_alloc.c


z_log.o : 
	gcc -c ../log/z_log.c

z_epoll.o : 
	gcc -c ../epoll/z_epoll.c

clean : 
	rm server $(OBJS)
