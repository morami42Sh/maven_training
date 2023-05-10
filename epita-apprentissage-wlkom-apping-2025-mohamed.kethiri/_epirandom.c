#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/random.h>
#include <linux/slab.h>

#define DEVICE_NAME "epirandom"
#define ALPHABET_SIZE 64

// Defintion de l'alphabet (base64)
static const char *default_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


// Definition des fonctions de rappel
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

// Definition des operations sur le fichier pour le nouveau peripherique
static struct file_operations fops =
{
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

// numero majeur du peripherique
static int major;

// parametre du module
static char *alphabet = NULL;
static int threshold = 4;

module_param(alphabet, charp, 0);
MODULE_PARM_DESC(alphabet, "Alphabet used to generate random characters (base64 if not specified)");

module_param(threshold, int, 0);
MODULE_PARM_DESC(threshold, "Threshold (1-5, defaut 4)");

static int __init epirandom_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major < 0)
    {
        printk(KERN_ALERT "epirandom load failed \n");
        return major;
    }

    if (alphabet == NULL) {
        alphabet = (char *) kmalloc(strlen(default_alphabet) + 1, GFP_KERNEL);
        if (!alphabet) {
            printk(KERN_ALERT "Failed to allocate memory for default alphabet\n");
            unregister_chrdev(major, DEVICE_NAME);
            return -ENOMEM;
        }
        strcpy(alphabet, default_alphabet);
    }

    if (threshold < 1 || threshold > 5) {
        printk(KERN_ALERT "Invalid threshold level %d, using default level 4\n", threshold);
        threshold = 4;
    }

    printk(KERN_INFO "epirandom module has been loaded: %d\n", major);
    printk(KERN_INFO "Alphabet used: %s\n", alphabet);
    printk(KERN_INFO "Threshold level: %d\n", threshold);
    return 0;
}

static void __exit epirandom_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    if (alphabet != NULL && alphabet != default_alphabet) {
        kfree(alphabet);
    }
    printk(KERN_INFO "epirandom module has been unloaded\n");
}

static int dev_open(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "epirandom device opened\n");
    return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
    printk(KERN_INFO "Sorry, epirandom is read only\n");
    return -EFAULT;
}

static int dev_release(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "epirandom device closed\n");
    return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
    int errors = 0;
    char *val = NULL;
    int val_len;
    int i, n, index;
    int max_random = ALPHABET_SIZE - 1;

    if (threshold == 1) {
        // Infinite random output
        val = (char *) kmalloc(len + 1, GFP_KERNEL);
        if (!val) {
            printk(KERN_ALERT "Failed to allocate memory for read buffer\n");
            return -ENOMEM;
        }
        while (1) {
            for (i = 0; i < len; i++) {
                if (alphabet != NULL) {
                    n = strlen(alphabet);
                    max_random = n - 1;
                    index = get_random_int() % n;
                } else {
                    index = get_random_int() % ALPHABET_SIZE;
                    while (index > max_random) {
                        index = get_random_int() % ALPHABET_SIZE;
                    }
                }
                val[i] = alphabet != NULL ? alphabet[index] : (char) index;
            }
            errors = copy_to_user(buffer, val, len);
            if (errors) {
                kfree(val);
                return -EFAULT;
            }
        }
    } else if (threshold == 2) {
        // Random output with limit
        val = (char *) kmalloc(len + 1, GFP_KERNEL);
        if (!val) {
            printk(KERN_ALERT "Failed to allocate memory for read buffer\n");
            return -ENOMEM;
        }
        for (i = 0; i < len; i++) {
            if (alphabet != NULL) {
                n = strlen(alphabet);
                max_random = n - 1;
                index = get_random_int() % n;
            } else {
                index = get_random_int() % ALPHABET_SIZE;
                while (index > max_random) {
                    index = get_random_int() % ALPHABET_SIZE;
                }
            }
            val[i] = alphabet != NULL ? alphabet[index] : (char) index;
        }
        val_len = i;
        errors = copy_to_user(buffer, val, val_len);
        kfree(val);
        return errors == 0 ? val_len : -EFAULT;
    } else if (threshold == 3) {
        // All possible random characters
        val = (char *) kmalloc(ALPHABET_SIZE + 1, GFP_KERNEL);
        if (!val) {
            printk(KERN_ALERT "Failed to allocate memory for read buffer\n");
            return -ENOMEM;
        }
        for (i = 0; i < ALPHABET_SIZE; i++) {
            val[i] = alphabet != NULL ? alphabet[i] : (char) i;
        }
        val_len = i;
        errors = copy_to_user(buffer, val, val_len);
        kfree(val);
        return errors == 0 ? val_len : -EFAULT;
    } else if (threshold == 4 || threshold == 5) {
        // urandom(4) behavior or custom alphabet behavior
        val = (char *) kmalloc(len + 1, GFP_KERNEL);
        if (!val) {
            printk(KERN_ALERT "Failed to allocate memory for read buffer\n");
            return -ENOMEM;
        }
        for (i = 0; i < len; i++) {
            if (alphabet != NULL) {
                n = strlen(alphabet);
                max_random = n - 1;
                index = get_random_int() % n;
            } else {
                index = get_random_int() % ALPHABET_SIZE;
                while (index > max_random) {
                    index = get_random_int() % ALPHABET_SIZE;
                }
            }
            val[i] = alphabet != NULL ? alphabet[index] : (char) index;
        }
        val_len = i;
        errors = copy_to_user(buffer, val, val_len);
        kfree(val);
        return errors == 0 ? val_len : -EFAULT;
    }

    return -EFAULT;
}

MODULE_LICENSE("GPL");
module_init(epirandom_init);
module_exit(epirandom_exit);

