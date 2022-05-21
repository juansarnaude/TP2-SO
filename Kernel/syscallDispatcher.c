#include <syscallDispatcher.h>
#include <moduleLoader.h>

void syscallDispatcher(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    switch(rax){
        case 0:
            sys_read(rdi,rsi,rdx);
            break;
        case 1:
            sys_write(rdi,rsi,rdx);
            break;
    }
        

}

void sys_read(const char* fd,char* buffer,size_t count){
    
}
void sys_write(const char* fd,const char* buffer,size_t count){
    
}