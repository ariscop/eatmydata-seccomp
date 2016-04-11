eatmydata: eatmydata.c filter.h
	gcc -Wall $< -o $@
	setcap cap_sys_admin=ep $@

%.h: %.bpf
	bpfc -f C -i $< > $@
