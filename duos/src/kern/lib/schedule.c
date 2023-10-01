#include <schedule.h>
#include <types.h>

ReadyQ_TypeDef ready_queue;
TCB_TypeDef *current, *__sleep;

const uint16_t init_task_id = 1000;
uint16_t last_assigned = init_task_id;


void _schedule(void)
{
    //if the current task is running, put it in ready queue
    if(current->status == RUNNING)
    {
        current->status = READY;
        add_to_queue(current);
    }
    //get current front of queue
    TCB_TypeDef *front = pop();
    current = front;
    //start running it
    current->status = RUNNING;
    return;
}

//task related
uint16_t generate_task_id(void)
{
    return last_assigned++;
}
void create_task(TCB_TypeDef *tcb, void (*task_func)(void), uint32_t *stack)
{
    //default value
    tcb->magic_number =  0xFECABAA0;
    //get a unique task id for this task
    tcb->task_id = generate_task_id();
    //Newly created task should be in ready state
    tcb->status = READY;
    //Function pointer assign
    tcb->run = task_func;
    //point psp to the stack
    tcb->psp = stack;
    //Storing the following into the PSP of task
    *(--tcb->psp) = DUMMY_XPSR;  // xPSR
    *(--tcb->psp) = (uint32_t) task_func;  // PC (task entry point)
    *(--tcb->psp) = 0xFFFFFFFD;  // LR
	
    *(--tcb->psp) = 0x0000000;  // R12 - storing actual object address in here
    *(--tcb->psp) = 0x0000000;  // R3
    *(--tcb->psp) = 0x0000000;  // R2
    *(--tcb->psp) = 0x0000000;  // R1
    *(--tcb->psp) = 0x0000000;  // R0

    for(int i=0;i<8;i++)
    {
        if(i==0)
        {
            //next store the task's pointer
            *(--tcb->psp) = (uint32_t) tcb;
        }
        else
        {
            //fill with dummy values
            *(--tcb->psp) = 0x0000000;
        }
    }
}
//Code to start a particular task
void start_task(void)
{
    if(ready_queue.size == 0)//if the queue is empty
    {
        //No task
        return;
    }
    //Fetch front of the queue
    TCB_TypeDef *front = pop();
    //current pointer pointing to the front
    current = front;
    //kprintf("front psp: %x\n",front->psp);
    //Set status as running
    current->status = RUNNING;
    //save psp into r5
    //__asm volatile("mov r12, %0"::"r"(front->psp));
    unsigned int psp_val = front->psp;
    __asm volatile("mov r5, %[v]": : [v] "r" ((psp_val)));
    __asm volatile("stmdb r13!, {r5}");
    __asm volatile (
        "stmdb r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
        "svc 121\n"
        "nop\n"
        "ldmia r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
    );
    //Service call to start task __SYS_start_task
    
    //__asm volatile("svc 121");
    //kprintf("here\n");
}

// queue related operations
//set the max task, and other parameters of our queue
void initialize_queue(void)
{
    ready_queue.max = MAX_TASK;
    ready_queue.size = 0;
    ready_queue.st = 0;
    ready_queue.ed = -1;
}
void add_to_queue(TCB_TypeDef *t)
{
    //slot available
    if(ready_queue.max >= ready_queue.size+ 1)
    {
        ready_queue.ed++;
        if(ready_queue.ed >= ready_queue.max)ready_queue.ed = 0;
        //add task to the ed
        ready_queue.q[ready_queue.ed] = t;
        ready_queue.size++;
    }
    else
    {
        //Task limit reached
    }
}
TCB_TypeDef *pop(void)
{
    if(ready_queue.size == 0)
    {
        add_to_queue(__sleep);
    }
    int front = ready_queue.st;
    ready_queue.st++;
    //Mod operation here ensures the Round robin operation as this will become a circular operation
    if(ready_queue.st >= ready_queue.max)ready_queue.st=0;
    ready_queue.size--;
    return *((ready_queue.q)+front);
}
//Set current task as sleep
void set_sleeping_task(TCB_TypeDef *s)
{
    __sleep = s;
    return;
}
void __attribute__((naked))PendSV_Handler(void)
{
  //Clear all pending interrupts
  SCB->ICSR |= (1<<27);

  //save current context
  __asm volatile(
    "mrs r0, psp\n"
    "stmdb r0!, {r4-r11}\n"
    "push {lr}\n"
  );
  __asm volatile("mov %0, r0": "=r"(current->psp):);
  //Schedule next task
  _schedule();
  __asm volatile(
    "mov r0, %0": :"r"(current->psp)
  );
  __asm volatile(
    "ldmia r0!,{r4-r11}\n"
    "msr psp, r0\n"
    "pop {lr}\n"
    "bx lr\n"
  );
}
