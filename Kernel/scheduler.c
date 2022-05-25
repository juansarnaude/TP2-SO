//
// Created by monarch on 23/05/22.
//
#include "scheduler.h"

#pragma pack(push)  /* Push de la alineación actual */
#pragma pack (1)    /* Alinear las siguientes estructuras a 1 byte */

/* Contexto de cada task */
typedef struct {
    uint64_t rax, rbx, rcx, rdx, rbp, rdi, rsi, r8, r9, r10, r11, r12, r13, r14, r15;
    uint64_t rsp, rip, rflags;
    uint8_t window;
} TASK_CONTEXT;

#pragma pack(pop)   /* Reestablece la alineación actual */

#define TASK_ARR_SIZE 2

static TASK_CONTEXT tss[TASK_ARR_SIZE];    //Task array
static int amount = 0;          //Amount of tasks currently running
static int current = 0;         //Currently running task

static void * const userlandAddress = (void*)0x600000;
static void * const stepping = 0x100000;

void executeTask(int (*program)(int argc, char const * argv), int argc, char const *argv){
    //Adds a task to the task array.
    if (amount < TASK_ARR_SIZE){
        int position = current+1;
        tss[position].rip = program;
        tss[position].rsp = userlandAddress + (uint64_t) stepping * amount;
        tss[position].window = 0;
        if (amount){
            tss[position].window = 1;
            ncWindows(2);
        } else {
            ncWindows(1);
        }
        amount++;
    }
}

void nextTask(){
    if (amount > 1){
        _saveContext(&tss[current]);
        current = (current+1) % TASK_ARR_SIZE;
        ncCurrentWindow(tss[current].window);
        //Tengo que cargar el contexto AL FINAL DE LA INTERRUPCION
        _loadContext(&tss[current]);
    } else if (amount == 0) {
        haltcpu();
    }
}

static void move(int a, int b);

void exitTask(){
    for (int i = current; i < TASK_ARR_SIZE - 1; i++)
    {
        move(i, i+1);
    }
    amount--;
    nextTask();
}

static void move(int a, int b){
    tss[a].rax = tss[b].rax;
    tss[a].rbx = tss[b].rbx;
    tss[a].rcx = tss[b].rcx;
    tss[a].rdx = tss[b].rdx;
    tss[a].rsi = tss[b].rsi;
    tss[a].rdi = tss[b].rdi;
    tss[a].rbp = tss[b].rbp;
    tss[a].r8 = tss[b].r8;
    tss[a].r9 = tss[b].r9;
    tss[a].r10 = tss[b].r10;
    tss[a].r11 = tss[b].r11;
    tss[a].r12 = tss[b].r12;
    tss[a].r13 = tss[b].r13;
    tss[a].r14 = tss[b].r14;
    tss[a].r15 = tss[b].r15;
    tss[a].rsp = tss[b].rsp;
    tss[a].rip = tss[b].rip;
    tss[a].rflags = tss[b].rflags;
    tss[a].window = tss[b].window;
}
