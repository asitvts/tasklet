#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>



/*

struct tasklet_struct {
    struct tasklet_struct *next;    // Pointer to next tasklet in the list
    unsigned long state;            // Tasklet's state (whether it's running, pending, etc.)
    unsigned long data;             // Custom data passed to the tasklet function
    void (*func)(unsigned long);    // Function to run when the tasklet executes
};


*/

/*
Function		Returns true when...					Context Type						Typical Code
in_irq()		Code is running in hard interrupt 		(top-half) context Hard IRQ			Inside an actual hardware interrupt handler 
in_softirq()	Code is running in softirq 				(bottom-half)context				SoftIRQ Inside softirq handler or tasklet
in_interrupt()	Code is in any interrupt context 		(hard or soft)Hard IRQ or SoftIRQ	Most generic “am I in interrupt?” test

*/


static struct tasklet_struct* my_tasklet;
static unsigned long flags;
static int counter =0;

static void get_irq_status(void){

	if(irqs_disabled()){
		pr_info("interrupts disabled\n");
	}
	else{
		pr_info("interrupts enabled\n");
	}
	
	return;
}

static void get_context_status(void){

	if(in_interrupt()){
		pr_info("inside (interrupt context) right now\n");
	}
	else{
		pr_info("inside (process context) right now\n");
	}
	
	return;

}

static void tasklet_func(unsigned long data){

	
	local_irq_save(flags);
	get_irq_status();
	get_context_status();
	
	
	printk(KERN_INFO "process name and id : %s  %d \n", current->comm, current->pid);		// thread name and id
	
	
	pr_info("value of counter : %d\n",counter);
	pr_info("performing tasklet function\n" );
	counter++;
	pr_info("value of counter : %d\n",counter);
	
	
	local_irq_restore(flags);
	get_irq_status();
	get_context_status();
	
	return;
}


static int __init my_init(void){
	
	
	my_tasklet = kmalloc(sizeof(struct tasklet_struct), GFP_KERNEL);
	if(!my_tasklet){
		pr_info("error while allocating memory for my_tasklet\n");
		return -ENOMEM;
	} 
	
	pr_info("in __init right now\n");
	get_irq_status();
	get_context_status();
	pr_info("going inside the handler now\n");
	
	
	tasklet_init(my_tasklet,tasklet_func, 0);		// third param is data for tasklet
	
	pr_info("bitmask of softirq beore schedule: %d\n",local_softirq_pending());
	tasklet_schedule(my_tasklet);
	pr_info("bitmask of softirq beore schedule: %d\n",local_softirq_pending());
	
	
	
	return 0;
}

static void __exit my_exit(void){
	
	if(my_tasklet){
		tasklet_kill(my_tasklet);
		kfree(my_tasklet);
		pr_info("cleanup done\n");
	}
	
}


module_init(my_init);
module_exit(my_exit);


MODULE_AUTHOR("asit");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("tasklet function");












