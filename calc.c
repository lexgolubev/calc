#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>

#define MAX_PROC_SIZE 100

static int arg1;
static int arg2;
static char op;
static int answer;

static struct proc_dir_entry *proc_arg1_entry;
static struct proc_dir_entry *proc_arg2_entry;
static struct proc_dir_entry *proc_op_entry;
static struct proc_dir_entry *proc_answer_entry;

void calculate()
{
  if (op == '+') {
    answer = arg1 + arg2;
  } else if (op == '-') {
    answer = arg1 - arg2;
  } else if (op == '*') {
    answer = arg1 * arg2;
  } else {
    answer = arg1 / arg2;
  }
}

int read_proc_arg1(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
    return sprintf(buf, "%d", arg1);
}

int read_proc_arg2(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
    return sprintf(buf, "%d", arg2);
}

int read_proc_op(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
    return sprintf(buf, "%c", op);
}

int read_proc_answer(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
    return sprintf(buf, "%d", answer);
}

int write_proc_arg1(struct file *file, const char *buf, int count, void *data)
{
    char tmp[30];
    if (count > MAX_PROC_SIZE)
        count = MAX_PROC_SIZE;
    if (copy_from_user(tmp, buf, count))
        return -EFAULT;
    sscanf(tmp, "%d", &arg1);

    return count;
}

int write_proc_arg2(struct file *file, const char *buf, int count, void *data)
{
    char tmp[30];
    if (count > MAX_PROC_SIZE)
        count = MAX_PROC_SIZE;
    if (copy_from_user(tmp, buf, count))
        return -EFAULT;
    sscanf(tmp, "%d", &arg2);

    return count;
}

int write_proc_op(struct file *file, const char *buf, int count, void *data)
{
    char tmp[30];
    if (count > MAX_PROC_SIZE)
        count = MAX_PROC_SIZE;
    if (copy_from_user(tmp, buf, count))
        return -EFAULT;
    sscanf(tmp, "%c", &op);

    return count;
}

int write_proc_answer(struct file *file, const char *buf, int count, void *data)
{
    char tmp[30];
    if (count > MAX_PROC_SIZE)
        count = MAX_PROC_SIZE;
    if (copy_from_user(tmp, buf, count))
        return -EFAULT;
    sscanf(tmp, "%d", &answer);

    return count;
}

int create_new_proc_entry()
{
    proc_arg1_entry = proc_create("proc_arg1", 0644, NULL, NULL);
    proc_arg2_entry = proc_create("proc_arg2", 0644, NULL, NULL);
    proc_op_entry = proc_create("proc_op", 0644, NULL, NULL);
    proc_answer_entry = proc_create("proc_answer", 0644, NULL, NULL);
    if (!(proc_arg1_entry && proc_arg2_entry && proc_op_entry && proc_answer_entry)) {
        printk(KERN_INFO "Error creating proc entries");
        return -ENOMEM;
    }
    proc_arg1_entry->read_proc = read_proc_arg1;
    proc_arg1_entry->write_proc = write_proc_arg1;
    proc_arg2_entry->read_proc = read_proc_arg2;
    proc_arg2_entry->write_proc = write_proc_arg2;
    proc_op_entry->read_proc = read_proc_op;
    proc_op_entry->write_proc = write_proc_op;
    proc_answer_entry->read_proc = read_proc_answer;
    proc_answer_entry->write_proc = write_proc_answer;
    printk(KERN_INFO "proc initialized");
    return 0;
}

int proc_init (void)
{
    create_new_proc_entry();
    return 0;
}

void proc_cleanup(void)
{
    printk(KERN_INFO "Cleanup_module\n");
    remove_proc_entry("proc_arg1", NULL);
    remove_proc_entry("proc_arg2", NULL);
    remove_proc_entry("proc_op", NULL);
    remove_proc_entry("proc_answer", NULL);
}
MODULE_LICENSE("GPL");
module_init(proc_init);
module_exit(proc_cleanup);
