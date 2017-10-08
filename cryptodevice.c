#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/crypto.h>
#include <linux/scatterlist.h>
#define  DEVICE_NAME "cryptodevice"
#define  CLASS_NAME  "cryptodevice"

MODULE_LICENSE("GPL");

static int    majorNumber;
static char   data[32];
static short  size_of_data;
static struct class*  cryptoClass  = NULL;
static struct device* cryptoDevice = NULL;
static char   operation;
static char   *key;

static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

module_param(key, charp, 0000);

static struct file_operations fops =
{
   .write = dev_write,
};

static int __init cryptodevice_init(void){
   printk(KERN_INFO "cryptodevice: inicializado com sucesso!\n");
   printk(KERN_INFO "cryptomodule: key secreta é %s\n", key);

   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber<0){
      printk(KERN_ALERT "cryptodevice: falha ao criar o major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "cryptodevice: major number criado corretamente: %d\n", majorNumber);

   cryptoClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(cryptoClass)){
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "cryptodevice: falha ao criar a classe do device\n");
      return PTR_ERR(cryptoClass);
   }
   printk(KERN_INFO "cryptodevice: classe do device criada com sucesso!\n");

   cryptoDevice = device_create(cryptoClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(cryptoDevice)){
      class_destroy(cryptoClass);
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "cryptodevice: falha ao criar o device\n");
      return PTR_ERR(cryptoDevice);
   }
   printk(KERN_INFO "cryptodevice: device criado com sucesso!\n");
   return 0;
}

static void __exit cryptodevice_exit(void){
   device_destroy(cryptoClass, MKDEV(majorNumber, 0));
   class_unregister(cryptoClass);
   class_destroy(cryptoClass);
   unregister_chrdev(majorNumber, DEVICE_NAME);
   printk(KERN_INFO "cryptodevice: finalizado com sucesso!\n");
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
	struct crypto_cipher *cryptoCipher = NULL;
   struct crypto_hash *cryptoHash;
   struct hash_desc cryptoHashDesc;
   struct scatterlist sg;
	u8 criptografado[16], descriptografado[16];
   unsigned char hashOutput[32];

	cryptoCipher = crypto_alloc_cipher("aes", 0, 0);

	if (IS_ERR(crypto_cipher_tfm(cryptoCipher))) {
		pr_info("cryptodevice: não foi possível alocar o handle para o cipher\n");
		return PTR_ERR(crypto_cipher_tfm(cryptoCipher));
	}

   cryptoHash = crypto_alloc_hash("sha1", 0, 0);

   if (IS_ERR(crypto_hash_tfm(cryptoHash))) {
      pr_info("cryptodevice: não foi possível alocar o handle para o hash\n");
      return PTR_ERR(crypto_hash_tfm(cryptoHash));
   }

   cryptoHashDesc.tfm = cryptoHash;
   cryptoHashDesc.flags = 0;

   if (crypto_cipher_setkey(cryptoCipher, key, 32 * sizeof(u8)) != 0) {
      pr_info("cryptodevice: não foi possível definir a key\n");
      return 1;
   }

   //TEM QUE VERIFICAR SE A PESSOA EH SACANA!!!!!!!!!! URGENTE
   
   sprintf(data, "%s", &(buffer[2]));
   size_of_data = strlen(data);

   operation = buffer[0];

   printk(KERN_INFO "cryptodevice: a operacao recebida é %c\n", operation);
   printk(KERN_INFO "cryptodevice: os dados recebidos são %s\n", data);
   
   //verificacao
   if(operation == 'c') {
   	   crypto_cipher_encrypt_one(cryptoCipher, criptografado, data);
   	   printk(KERN_INFO "cryptodevice: a operacao recebida é %c, resultado: ", operation);

         for(int i = 0; i < sizeof(criptografado); i++) {
            printk(KERN_CONT "%02x", criptografado[i]);
         }

         printk(KERN_CONT "\n");
   }
   else if(operation == 'd') {
      int j = 0;

      printk(KERN_INFO "cryptodevice: criptografado[]: ");
      for(int i = 0; i < 32; i++) {
         if(i % 2 == 0) {
            char aux[2];
            unsigned long res = 0;

            aux[0] = data[i];
            aux[1] = data[i + 1];

            kstrtol(aux, 16, &res);

            criptografado[j] = res;
            printk(KERN_CONT "%x", criptografado[j]);
            j++;
         }
      }

   	crypto_cipher_decrypt_one(cryptoCipher, descriptografado, criptografado);
   	printk(KERN_INFO "cryptodevice: a operacao recebida é %c, resultado: %s\n", operation, descriptografado);
   }
   else if(operation == 'h') {
      crypto_hash_init(&cryptoHashDesc);
      sg_init_one(&sg, data, 32);
      crypto_hash_update(&cryptoHashDesc, &sg, 32);
      crypto_hash_final(&cryptoHashDesc, hashOutput);

      printk(KERN_INFO "cryptodevice: a operacao recebida é %c, resultado: ", operation);

      for(int i = 0; i < sizeof(hashOutput); i++) {
         printk(KERN_CONT "%02x", hashOutput[i]);
      }
   }
   
   crypto_free_cipher(cryptoCipher);
   crypto_free_hash(cryptoHash);
   return len;
}

module_init(cryptodevice_init);
module_exit(cryptodevice_exit);
