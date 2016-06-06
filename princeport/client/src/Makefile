../bin/server:main.c pp_system.c pp_order.c pp_tranfile.c
	gcc *.c -o ../bin/client -lpthread
.PHONY:clean
clean:
	rm ../bin/client
.PHONY:run
run:
	../bin/client
