#!/usr/bin/env node

import crypto from "crypto";

const [, , mode, input, keyStr, ivStr] = process.argv;

function fail(message) {
  console.error("Error:", message);
  process.exit(1);
}

// Validate arguments
if (!mode || !input || !keyStr || !ivStr) {
  fail("Usage: node aes256cbc.mjs -e|-d <text> <key> <iv>");
}

if (mode !== "-e" && mode !== "-d") {
  fail("First argument must be -e (encrypt) or -d (decrypt)");
}

// Validate key
const key = Buffer.from(keyStr, "utf-8");
if (key.length !== 32) {
  fail(`Key must be exactly 32 bytes (got ${key.length})`);
}

// Validate IV
const iv = Buffer.from(ivStr, "utf-8");
if (iv.length !== 16) {
  fail(`IV must be exactly 16 bytes (got ${iv.length})`);
}

try {
  if (mode === "-e") {
    if (input.length === 0) {
      fail("Input text cannot be empty for encryption");
    }

    const cipher = crypto.createCipheriv("aes-256-cbc", key, iv);
    let encrypted = cipher.update(input, "utf-8", "hex");
    encrypted += cipher.final("hex");

    console.log(encrypted);
  } else if (mode === "-d") {
    // Validate hex input
    if (!/^[0-9a-fA-F]+$/.test(input)) {
      fail("Ciphertext must be a valid hexadecimal string");
    }

    const decipher = crypto.createDecipheriv("aes-256-cbc", key, iv);

    let decrypted;
    try {
      decrypted = decipher.update(input, "hex", "utf-8");
      decrypted += decipher.final("utf-8");
    } catch (e) {
      fail(
        "Decryption failed (possible wrong key, IV, or corrupted ciphertext)",
      );
    }

    console.log(decrypted);
  }
} catch (err) {
  fail(err.message || "Unexpected error occurred");
}
