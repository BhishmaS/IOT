
#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>

#include "mbedtls/aes.h"
#include "mbedtls/pk.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"

#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_CS 18
#define LORA_RST 14
#define LORA_IRQ 34

const char* private_pem_key = \

"-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIEogIBAAKCAQEAk9bkei8kN5312Ls+P8xx/w/pC6GzGnqTCey2E0v0wP8akbYD\n" \
"+vkFGTSjdKozwSJlfhmGYvJ/kUwP/XKckXjAKhs4k+hdA1kKk6ClzgZm700LmwA5\n" \
"xeQNTF3gol1/i/SUq//DQKY4asVB0uaLYA1mUWJyv5f6DIAPlaW2uc/r6aEWlkxr\n" \
"ANYIxYw47+YTCWHG5odOZ5F3dchOnPWPGxzIY35IZOlfoREEIFrM1AqmUzxzJXGn\n" \
"2oh+9rVN36OaMzxUMCl4HuyNZg/KvseuPnTAV5v8Mg/YyxDBjBFsLvVa4Cq4uwsn\n" \
"qJWnUQfWgeowHtgEvjX+8aaaP9scG4WmsNmMSQIDAQABAoIBAFbqphT3ljFkmt86\n" \
"5lQX3IV6UybVgDj3C29wTXN22oSE9pfoDNsGgGrbOaYjUp4fnM/QLeQKcdl4gIYf\n" \
"VdFozoFy04aRu+118fPtJM3Fkcrq3wHX1h9z/iWBo8mRcqVTu/yLN73Qe6a6JgLa\n" \
"K+Livd7nJFNkBExJFeCDNrRze100xRRZiq4nNYUwGOfUMn9jAPHLX4RG3JR2cdiL\n" \
"QJCg7Q0GKw/xWAvGvL+rY9KgTdJUG1SgRmdNejUlm2Tp6tX3u8WcDXk540X5R1ZJ\n" \
"hOrGcGC3T6/jXQLXrGD3uKzhmW+2dLLef/p7IsEC1GhzOH9pt6FJBvGDZ6UB7vwt\n" \
"/ifFOgECgYEAwovR2jPyhwzUL+QHVk+fGU0R3d44O8h7PGoH4FpWb+pa0BiihrPL\n" \
"WcjFq/tHGAumQmwwSAy9WcjOaVu0mqswbGXrJb7c2gH/3wT7Kfr146xfAdg2Kfuh\n" \
"jeecaQ2lbgWgHrvFjbzJR71q6JrluXOiboxHIhl2yAh98G9WzWik3yECgYEAwooV\n" \
"J8jZiuoZ1kJAf1YTGNTDNSPStaDsPKKmSk6lAdwuid1KLs1O9riIjDCcwDaJyAGx\n" \
"WSmaf/YNX0GhGoHuGfU4GHuSjDskycAXhXiPaV8cf6AIStqeXgPCq/Reg4IbsbwQ\n" \
"kzsEHBjwEKf+RzY+bgfwAQMDSMJFFDD68TdH0CkCgYAF1ILZcy0VdwD03nSJ1aEC\n" \
"9RhP+dTdlch7qwHJa2ujtvY5vkNAniq96hJicQlUYyT8YJmhzx/tjOMCrUjgSbkN\n" \
"xpAhdXLQ5bZxTIjqXSb5R7UhLQWShIP26LX7P/lxLqH5yc7FzJ6Lk9WcWuf/gsOf\n" \
"ZHzNPM4eeIKtreiAB17JgQKBgHiWf2aZ6b9iPYHCr5g/Erdhq19VRVew2L79U3bc\n" \
"KQxuH6r1sPYx7bCh0oKT35aGOAHyiqxqTYrkSoWxeNuljqIojw0eDElAoB0vJWVm\n" \
"xf7bJh82Y+2CdohwbEuKvstl6yTRHrUk7/c6vhSVDdm2lhw+b1cxNufzBD7v3Z2I\n" \
"VsuBAoGAWJZli8/7CtfsTr57NDZKlBdNJXT/UF+EJou3D4QGaUBxan0BEB8nCDVc\n" \
"3q0MH4efEmWz93zTt4ic2vP4bs5rC7prZWQD/sVEz+7Zg7Po1rygXyNtVpkGz7W4\n" \
"WwNMqtEVr6EdGm3pRygL1XFGepf26nC91faHv8fU7rmnDOqAJUo=\n" \
"-----END RSA PRIVATE KEY-----";

unsigned char cipherRSA[MBEDTLS_MPI_MAX_SIZE];

void setup() {
  SPI.begin(LORA_SCK,LORA_MISO,LORA_MOSI,LORA_CS);
  LoRa.setSPI(SPI);
  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);
  
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

int count  = 0, size=0;
void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("\nReceived packet: \n");
    while (LoRa.available()) {
      cipherRSA[size++] = (char)LoRa.read();
      // Serial.printf("%02x", (int) cipherRSA[size++]);
    }

    count++;
    if (count == 2) {
      for (int  i = 0; i < size; i++) {
        Serial.printf("%02x", (int) cipherRSA[i]);
      }
      mbedtls_pk_context pk;              // stores a public or private key
      mbedtls_ctr_drbg_context ctr_drbg;  // random number generator context
      mbedtls_entropy_context entropy;
      mbedtls_entropy_init(&entropy);
      mbedtls_ctr_drbg_init(&ctr_drbg);
      const char *pers = "decrypt";
      // Decrypt using RSA
      size_t olen = 256;
      unsigned char result[1024];
      mbedtls_pk_init( &pk );
      mbedtls_entropy_init(&entropy);
      int ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func,
                            &entropy, (const unsigned char *) pers,
                            strlen( pers )
                          );
      ret = mbedtls_pk_parse_key( &pk, (unsigned char*)private_pem_key, strlen(private_pem_key)+1, NULL, 0);  // No password for key
      
      int lenCipher = olen;
      ret = mbedtls_pk_decrypt(&pk,
                              cipherRSA,  
                              lenCipher, 
                              result, 
                              &olen, 
                              sizeof(result),
                              mbedtls_ctr_drbg_random,
                              &ctr_drbg
                              );

      if (ret == 0) {
        Serial.print("\n\nRSA Decrypted Data: ");
        result[15] = '\0';
        Serial.printf("%s", result);
      } else {
        Serial.printf(" failed\n  ! mbedtls_pk_decrypt returned -0x%04x\n", -ret);
      }

      count  = 0;
      size = 0;
    }
  }
}