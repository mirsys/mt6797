#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/ioctl.h>

#include "aal_control.h"

#ifdef CONFIG_INPUT_ALSPS_BH1745
extern int BH1745_aal_enable_als(int enable);
extern int  BH1745_aal_get_als_data(void);
#else
#ifdef CONFIG_INPUT_ALSPS_APDS9922
extern int apds9921_aal_als_enable(int enable);
extern int apds9921_aal_get_als_data(void);
#endif
#endif
static int AAL_open(struct inode *inode, struct file *file)
{
	return nonseekable_open(inode, file);
}

static int AAL_release(struct inode *inode, struct file *file)
{
	file->private_data = NULL;
	return 0;
}

static long AAL_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	long err = 0;
	void __user *ptr = (void __user *) arg;
	int dat;
	uint32_t enable;

	switch (cmd) {

	case AAL_SET_ALS_MODE:
		if (copy_from_user(&enable, ptr, sizeof(enable))) {
			err = -EFAULT;
			goto err_out;
		}
#ifdef CONFIG_INPUT_ALSPS_BH1745
		err = BH1745_aal_enable_als(enable);
#else
#ifdef CONFIG_INPUT_ALSPS_APDS9922
		err = apds9921_aal_get_als_data(enable);
#endif
#endif
		if (err) {
			AAL_LOG("als driver don't support new arch, goto execute old arch: %ld\n", err);
		}
		break;

	case AAL_GET_ALS_MODE:
		AAL_LOG("AAL_GET_ALS_MODE do nothing\n");
		break;

	case AAL_GET_ALS_DATA:
#ifdef CONFIG_INPUT_ALSPS_BH1745
		dat = BH1745_aal_get_als_data();
#else
#ifdef CONFIG_INPUT_ALSPS_APDS9922
		dat = apds9921_aal_get_als_data();
#endif
#endif
		if (dat < 0) {
			AAL_LOG("alsps_aal_get_data fail\n");
		}

		if (copy_to_user(ptr, &dat, sizeof(dat))) {
			err = -EFAULT;
			goto err_out;
		}
		break;

	default:
		AAL_ERR("%s not supported = 0x%04x", __func__, cmd);
		err = -ENOIOCTLCMD;
		break;
	}

err_out:
		return err;
}

#ifdef CONFIG_COMPAT
static long AAL_compact_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	void __user *data32;

	data32 = compat_ptr(arg);
	return AAL_unlocked_ioctl(file, cmd, (unsigned long)data32);
}
#endif

static const struct file_operations AAL_fops = {
	.owner = THIS_MODULE,
	.open = AAL_open,
	.release = AAL_release,
	.unlocked_ioctl = AAL_unlocked_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = AAL_compact_ioctl,
#endif
};

static struct miscdevice AAL_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "aal_als",
	.fops = &AAL_fops,
};


/*----------------------------------------------------------------------------*/
static int __init AAL_init(void)
{
	int err;

	err = misc_register(&AAL_device);
	if (err)
		AAL_ERR("AAL_device misc_register failed: %d\n", err);

	AAL_LOG("OK!\n");
	return 0;
}
/*----------------------------------------------------------------------------*/
static void __exit AAL_exit(void)
{
	int err;

	err = misc_deregister(&AAL_device);
	if (err)
		AAL_ERR("AAL_device misc_deregister fail: %d\n", err);
}

late_initcall(AAL_init);
MODULE_AUTHOR("Mediatek");
MODULE_DESCRIPTION("AAL driver");
MODULE_LICENSE("GPL");
