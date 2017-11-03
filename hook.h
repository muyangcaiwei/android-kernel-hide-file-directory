#ifndef __HOOK_H__
#define __HOOK_H__

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/namei.h>
#include <linux/spinlock.h>

extern char *data_fs ; //hide dir 
extern char *dir_to_hide;

extern filldir_t real_filldir;

extern void hide_file(void);
extern void unhide_file(void);


#define TFFS_LOG_ERR(format, ...) printk(KERN_EMERG"<test ttt_error>[%s(%d)|%s]\t"format"\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define TFFS_LOG_INFO(format, ...) printk(KERN_EMERG"<ttt_info>[%s(%d)|%s]\t"format"\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

# define set_file_op(op, path, new, old)                            \
    do {                                                            \
        struct file *filp;                                          \
        struct file_operations *f_op;                               \
                                                                    \
        printk("test_hook:Opening the path: %s.\n", path);           \
        filp = filp_open(path, O_RDONLY, 0);                        \
        if (IS_ERR(filp)) {                                         \
            printk("test_hook:Failed to open %s with error %ld.\n",  \
                     path, PTR_ERR(filp));                          \
            old = NULL;                                             \
        } else {                                                    \
            printk("test_hook:Succeeded in opening: %s\n", filp->f_dentry ->d_name.name);    \
            f_op = (struct file_operations *)filp->f_op;            \
            old = f_op->op;                                         \
                                                                    \
            printk("test_hook:Changing file_op->" #op " from %p to %p.\n",  \
                     old, new);                                     \
            f_op->op = new;                                         \
        }                                                           \
    } while (0)


#endif

