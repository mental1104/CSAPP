#include "show_bytes.h"

int main(int argc, char *argv[])
{
    int val = 12345;

    if (argc > 1) {
	if (argc > 1) {
	    val = strtol(argv[1], NULL, 0);
	}
	printf("calling test_show_bytes\n");
	test_show_bytes(val);
    } else {
	printf("calling show_twocomp\n");
	show_twocomp();
	printf("Calling simple_show_a\n");
	simple_show_a();
	printf("Calling simple_show_b\n");
	simple_show_b();
	printf("Calling float_eg\n");
	float_eg();
	printf("Calling string_ueg\n");
	string_ueg();
	printf("Calling string_leg\n");
	string_leg();
    }
    return 0;
}