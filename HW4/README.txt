Sharlotte Khit
Homework 4

Run:
node aes256cbc.mjs -e "How are things today?" thisisa_32_byte_long_key_I_think dog1234567890123

node aes256cbc.mjs -d <hex_ciphertext> thisisa_32_byte_long_key_I_think dog1234567890123

Notes:
- Key must be 32 bytes
- IV must be 16 bytes
- Encryption outputs hex