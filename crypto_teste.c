#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/crypto.h>
#include <crypto/aes.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("");
MODULE_AUTHOR("");
MODULE_ALIAS("Crypto");

int init_module(void)
{
	struct crypto_tfm *crypto = NULL;
	unsigned char key[32]; //Vai ser usada depois para armazenar a key
	unsigned char criptografado[32];

	printk(KERN_INFO "Hello world 1.\n");

	crypto = crypto_alloc_base("aes", 0, 0);

	if (IS_ERR(crypto)) {
		pr_info("could not allocate skcipher handle\n");
		return PTR_ERR(crypto);
	}

	int i = crypto_aes_set_key(crypto, "AAAABBBBAAAABBBBAAAABBBBAAAABBBB", 32);

	printk("%s", crypto->__crt_alg->cra_name);
	
	//aes_encrypt(crypto, criptografado, "OLATUDOBEMKKKKKKKKKKKKKKKKKKKKKK");
	//crypto_cipher_encrypt_one(struct crypto_cipher *tfm, u8 *dst, const u8 *src)

	crypto_free_tfm(crypto);

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world 1.\n");
}
