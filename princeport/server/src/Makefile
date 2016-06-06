../bin/server:main.c pp_system.c pp_tpool.c pp_order.c pp_tranfile.c
	gcc *.c -o ../bin/server -lpthread
.PHONY:clean
clean:
	rm ../bin/server
.PHONY:run
run:
	../bin/server
