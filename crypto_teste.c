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
	struct crypto_cipher *crypto = NULL;
	u8 key[32] = "AAAABBBBAAAABBBBAAAABBBBAAAABBB\0"; //Vai ser usada depois para armazenar a key
	u8 source[16] = "OLATUDOBEMCOMVC\0";
	u8 criptografado[16], resultado[16];

	printk(KERN_INFO "Hello world 1.\n");

	crypto = crypto_alloc_cipher("aes", 0, 0);

	if (IS_ERR(crypto_cipher_tfm(crypto))) {
		pr_info("could not allocate skcipher handle\n");
		return PTR_ERR(crypto_cipher_tfm(crypto));
	}

	crypto_cipher_setkey(crypto, key, sizeof(key));

	printk("%s\n", crypto_cipher_tfm(crypto)->__crt_alg->cra_name);

	crypto_cipher_encrypt_one(crypto, criptografado, source);
	printk("%s\n", criptografado);

	crypto_cipher_decrypt_one(crypto, resultado, criptografado);
	printk("%s\n", resultado);

	crypto_free_tfm(crypto_cipher_tfm(crypto));

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world 1.\n");
}
