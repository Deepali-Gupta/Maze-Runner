extern void set_write_ports();
extern void open_write_port(int player);
extern void udp_write(char *msg, int player);
extern void close_write_port(int player);
extern void open_all_write_ports();
extern void send_to_all(float request,float x, float y);
//~ extern void close_all_write_ports();
