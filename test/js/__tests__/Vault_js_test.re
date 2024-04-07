open Jest;
open Expect;
open Js.Typed_array;
open Vault;
open PromiseLet;

describe("Salt", () => {
  test("it works", () => {
    let text_encoder = TextEncoder.create();
    let text_decoder = TextDecoder.create();

    let encoded_text = TextEncoder.encode(text_encoder, "ABCDEF0123456789");
    let salt_base = Salt.compute_base(encoded_text);
    let decoded_salt = TextDecoder.decode(text_decoder, salt_base);

    assert(Uint8Array.length(salt_base) == 256);

    expect(decoded_salt)
    |> toBe(
         "sabihin.phLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLABCDEF0123456789",
       );
  })
});

describe("Derived Key", () => {
  testPromise("it works", () => {
    let client_random = ClientRandom.create();
    let* salt_buffer = Salt.compute_digest(client_random);
    let* master_key = MasterKey.create(salt_buffer);
    let* derived_key = DerivedKey.create("Maho_Akashi_9_20", salt_buffer);
    let* encrypted_master_key =
      Operations.wrap_master_key(
        derived_key.derived_encryption_key,
        derived_key.master_key_iv,
        master_key.master_key,
      );
    let* _ =
      Vault.Operations.unwrap_master_key(
        derived_key.derived_encryption_key,
        derived_key.master_key_iv,
        encrypted_master_key,
      );
    resolve(pass);
  })
});

describe("Master Key", () => {
  testPromise("it wraps and unwraps a protection private key", () => {
    let client_random = Vault.ClientRandom.create();
    let* salt_buffer = Vault.Salt.compute_digest(client_random);
    let* master_key = Vault.MasterKey.create(salt_buffer);
    let* protection_key_pair = Vault.ProtectionKeyPair.create();
    let* encrypted_private_key =
      Vault.Operations.wrap_protection_private_key(
        master_key.master_key,
        master_key.protection_key_iv,
        protection_key_pair.private_key,
      );
    let* _ =
      Vault.Operations.unwrap_protection_private_key(
        master_key.master_key,
        master_key.protection_key_iv,
        encrypted_private_key,
      );
    resolve(pass);
  });

  testPromise("it wraps and unwraps a verification private key", () => {
    let client_random = Vault.ClientRandom.create();
    let* salt_buffer = Vault.Salt.compute_digest(client_random);
    let* master_key = Vault.MasterKey.create(salt_buffer);
    let* verification_key_pair = Vault.VerificationKeyPair.create();
    let* encrypted_private_key =
      Vault.Operations.wrap_verification_private_key(
        master_key.master_key,
        master_key.verification_key_iv,
        verification_key_pair.private_key,
      );
    let* _ =
      Vault.Operations.unwrap_verification_private_key(
        master_key.master_key,
        master_key.verification_key_iv,
        encrypted_private_key,
      );
    resolve(pass);
  });
});

describe("Protection Key Pair", () => {
  testPromise("it wraps and unwraps an ephemeral key", () => {
    let* key_pair = Vault.ProtectionKeyPair.create();
    let* ephemeral_key = Vault.EphemeralKey.create();
    let* encrypted_ephemeral_key =
      Vault.Operations.wrap_ephemeral_key(
        key_pair.public_key,
        ephemeral_key.ephemeral_key,
      );
    let* _ =
      Vault.Operations.unwrap_ephemeral_key(
        key_pair.private_key,
        encrypted_ephemeral_key,
      );
    resolve(pass);
  })
});

describe("Verification Key Pair", () => {
  testPromise("it signs and verifies a message", () => {
    let text_encoder = Vault.TextEncoder.create();

    let message = {js|ハッピーアラウンド！|js};
    let encoded_data =
      Uint8Array.buffer(Vault.TextEncoder.encode(text_encoder, message));

    let* verification_key_pair = Vault.VerificationKeyPair.create();
    let* signature =
      Vault.Operations.sign_data(
        verification_key_pair.private_key,
        encoded_data,
      );

    let* success =
      Vault.Operations.verify_data(
        verification_key_pair.public_key,
        signature,
        encoded_data,
      );

    resolve(expect(success) |> toBe(true));
  })
});

describe("Ephemeral Key", () => {
  testPromise("it encrypts and decrypts a message", () => {
    let text_encoder = Vault.TextEncoder.create();
    let text_decoder = Vault.TextDecoder.create();

    let message = {js|ハッピーアラウンド！|js};
    let encoded_data =
      Uint8Array.buffer(Vault.TextEncoder.encode(text_encoder, message));

    let* ephemeral_key = Vault.EphemeralKey.create();
    let* encrypted_data =
      Vault.Operations.encrypt_data(
        ephemeral_key.ephemeral_key,
        ephemeral_key.message_iv,
        encoded_data,
      );
    let* decrypted_data =
      Vault.Operations.decrypt_data(
        ephemeral_key.ephemeral_key,
        ephemeral_key.message_iv,
        encrypted_data,
      );
    let decrypted_data = Uint8Array.fromBuffer(decrypted_data, ());
    let decrypted_message =
      Vault.TextDecoder.decode(text_decoder, decrypted_data);
    resolve(expect(message) |> toBe(decrypted_message));
  })
});

describe("Base64Utils", () => {
  testPromise("it works round trip", () => {
    let client_random = Vault.ClientRandom.create();
    let* salt_buffer = Vault.Salt.compute_digest(client_random);
    let* fresh_master_key = Vault.MasterKey.create(salt_buffer);
    let* fresh_derived_key = Vault.DerivedKey.create("password", salt_buffer);
    let* wrapped_master_key =
      Vault.Operations.wrap_master_key(
        fresh_derived_key.derived_encryption_key,
        fresh_derived_key.master_key_iv,
        fresh_master_key.master_key,
      );

    let base64 = Base64Utils.array_buffer_to_base64(wrapped_master_key);
    let base64' =
      base64
      |> Base64Utils.base64_to_array_buffer
      |> Base64Utils.array_buffer_to_base64;

    resolve(expect(base64) |> toBe(base64'));
  })
});
