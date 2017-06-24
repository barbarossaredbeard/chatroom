objects = client_main.o interface.o write_msg.o read_msg.o reg.o log.o trans_file.o
client:$(objects)
	gcc $(objects) -o client -lpthread
client_main.o:client_main.c
	gcc -c client_main.c
reg.o:reg.c
	gcc -c reg.c
interface.o:interface.c
	gcc -c interface.c
log.o:log.c
	gcc -c log.c
write_msg.o:write_msg.c
	gcc -c write_msg.c
read_msg.o:read_msg.c
	gcc -c read_msg.c
trans_file.o:trans_file.c
	gcc -c trans_file.c
clean:
	rm client $(objects)
