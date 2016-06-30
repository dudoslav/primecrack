# primecrack
Program that divides number into 2 fractions, but unusable for large numbers

##Usage
  * -h  show help
  * -x  followed by hexadecimal number
  * -d  followed by decimal number
  * -o  followed by octal number
  
##Using numbers from RSA, generating keys with openssl
1. ```openssl genrsa -des3 -out priv 64```
  * Generate rsa private key. 64 is recomended size for fast division
2. ```openssl rsa -in priv -outform PEM -pubout -out publ```
  * Takes in private key and generates public key
3. ```openssl rsa -pubin -in publ -text -noout```
  * Output should look like:
    * Public-Key: (64 bit)
    * Modulus: 11875096159523629387 (0xa4ccd10a4602e94b)
    * Exponent: 65537 (0x10001)
  * Modulus is the number you want to divide
