
objects = server_main.o server_deal.o server_reg.o check.o server_login.o linklist.o server_view.o server_chat.o trans_file.o admin.o
server:$(objects)
	gcc $(objects) -o server -lsqlite3
server_main.o:server_main.c
	gcc -c server_main.c
server_deal.o:server_deal.c
	gcc -c server_deal.c
server_reg.o:server_reg.c
	gcc -c server_reg.c
check.o:check.c
	gcc -c check.c
server_login.o:server_login.c
	gcc -c server_login.c
linklist.o:linklist.c
	gcc -c linklist.c
server_view.o:server_view.c
	gcc -c server_view.c
server_chat.o:server_chat.c
	gcc -c server_chat.c
trans_file.o:trans_file.c
	gcc -c trans_file.c
admin.o:admin.c
	gcc -c admin.c
clean:
	rm server $(objects)
