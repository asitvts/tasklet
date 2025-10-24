#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>


static struct tasklet_struct* my_tasklet;

static void tasklet_func(unsigned long data){

	pr_info("performing tasklet function, changing %ld to 1\n", data );

	return;
}


static int __init my_init(void){
	
	
	my_tasklet = kmalloc(sizeof(struct tasklet_struct), GFP_KERNEL);
	if(!my_tasklet){
		pr_info("error while allocating memory for my_tasklet\n");
		return -ENOMEM;
	} 
	
	
	tasklet_init(my_tasklet,tasklet_func, 0);
	
	tasklet_schedule(my_tasklet);

	
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












