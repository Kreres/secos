#include <asm.h>
#include <debug.h>
#include <info.h>
#include <exam/segmentation.h>
#include <exam/interruption.h>
#include <exam/pagination.h>
#include <exam/task.h>

void  __attribute__((section(".user1"))) user1(); 

void  __attribute__((section(".user2"))) user2(); 

