open Js.Typed_array;
open Vault;
open Vault.PromiseLet;

let salt_test = () => {
  let text_encoder = TextEncoder.create();
  let text_decoder = TextDecoder.create();

  let encoded_text = TextEncoder.encode(text_encoder, "ABCDEF0123456789");
  let salt_base = Salt.compute_base(encoded_text);
  let decoded_salt = TextDecoder.decode(text_decoder, salt_base);

  assert(Uint8Array.length(salt_base) == 256);
  Js.Console.log2("compute_salt:", decoded_salt);

  resolve();
};

let wrap_unwrap_master_key = () => {
  let* master_key = Vault.MasterKey.create();
  let* derived_key =
    Vault.DerivedKey.create("Maho_Akashi_9_20", master_key.salt_buffer);
  let* encrypted_master_key =
    Vault.Operations.wrap_master_key(
      derived_key.derived_encryption_key,
      derived_key.master_encryption_iv,
      master_key.master_key,
    );
  Js.Console.log("wrap_master_key: Finished");
  let* _ =
    Vault.Operations.unwrap_master_key(
      derived_key.derived_encryption_key,
      derived_key.master_encryption_iv,
      encrypted_master_key,
    );
  Js.Console.log("unwrap_master_key: Finished");
  resolve();
};

let wrap_unwrap_protection_private_key = () => {
  open Vault.ProtectionKeyPair;
  let* master_key = Vault.MasterKey.create();
  let* protection_key_pair = Vault.ProtectionKeyPair.create();
  let* encrypted_private_key =
    Vault.Operations.wrap_protection_private_key(
      master_key.master_key,
      master_key.protection_key_iv,
      protection_key_pair.private_key,
    );
  Js.Console.log("wrap_protection_private_key: Finished");
  let* _ =
    Vault.Operations.unwrap_protection_private_key(
      master_key.master_key,
      master_key.protection_key_iv,
      encrypted_private_key,
    );
  Js.Console.log("unwrap_protection_private_key: Finished");
  resolve();
};

let wrap_unwrap_verification_private_key = () => {
  open Vault.VerificationKeyPair;
  let* master_key = Vault.MasterKey.create();
  let* verification_key_pair = Vault.VerificationKeyPair.create();
  let* encrypted_private_key =
    Vault.Operations.wrap_verification_private_key(
      master_key.master_key,
      master_key.verification_key_iv,
      verification_key_pair.private_key,
    );
  Js.Console.log("wrap_verification_private_key: Finished");
  let* _ =
    Vault.Operations.unwrap_verification_private_key(
      master_key.master_key,
      master_key.verification_key_iv,
      encrypted_private_key,
    );
  Js.Console.log("unwrap_verification_private_key: Finished");
  resolve();
};

let wrap_unwrap_ephemeral_key = () => {
  open Vault.ProtectionKeyPair;

  let* key_pair = Vault.ProtectionKeyPair.create();
  let* ephemeral_key = Vault.EphemeralKey.create();
  let* encrypted_ephemeral_key =
    Vault.Operations.wrap_ephemeral_key(
      key_pair.public_key,
      ephemeral_key.ephemeral_key,
    );
  Js.Console.log("wrap_ephemeral_key: Finished");
  let* _ =
    Vault.Operations.unwrap_ephemeral_key(
      key_pair.private_key,
      encrypted_ephemeral_key,
    );
  Js.Console.log("unwrap_ephemeral_key: Finished");
  resolve();
};

let encrypt_decrypt_message = () => {
  let text_encoder = Vault.TextEncoder.create();
  let text_decoder = Vault.TextDecoder.create();

  let message = {js|ハッピーアラウンド！|js};
  let encoded_data =
    Uint8Array.buffer(Vault.TextEncoder.encode(text_encoder, message));

  Js.Console.log("encrypt_decrypt_message:");
  Js.Console.log2("  Plaintext Message:", message);

  let* ephemeral_key = Vault.EphemeralKey.create();
  let* encrypted_data =
    Vault.Operations.encrypt_data(
      ephemeral_key.ephemeral_key,
      ephemeral_key.message_iv,
      encoded_data,
    );
  Js.Console.log("  encrypt_data: Finished");
  let* decrypted_data =
    Vault.Operations.decrypt_data(
      ephemeral_key.ephemeral_key,
      ephemeral_key.message_iv,
      encrypted_data,
    );
  Js.Console.log("  decrypt_data: Finished");
  let decrypted_data = Uint8Array.fromBuffer(decrypted_data, ());
  let decrypted_message =
    Vault.TextDecoder.decode(text_decoder, decrypted_data);
  Js.Console.log2("  Decrypted Message:", decrypted_message);
  resolve();
};

let sign_verify_message = () => {
  open Vault.VerificationKeyPair;

  let text_encoder = Vault.TextEncoder.create();
  let message = {js|ハッピーアラウンド！|js};
  let encoded_data =
    Uint8Array.buffer(Vault.TextEncoder.encode(text_encoder, message));

  Js.Console.log("sign_verify_message:");
  Js.Console.log2("  Plaintext Message:", message);

  let* verification_key_pair = Vault.VerificationKeyPair.create();
  let* signature =
    Vault.Operations.sign_data(
      verification_key_pair.private_key,
      encoded_data,
    );
  Js.Console.log("  sign_data: Finished");
  let* success =
    Vault.Operations.verify_data(
      verification_key_pair.public_key,
      signature,
      encoded_data,
    );
  Js.Console.log2("  verify_data:", success);
  resolve();
};

let base64_utils = () => {
  let* fresh_master_key = MasterKey.create();
  let* fresh_derived_key =
    Vault.DerivedKey.create("password", fresh_master_key.salt_buffer);

  let* wrapped_master_key =
    Vault.Operations.wrap_master_key(
      fresh_derived_key.derived_encryption_key,
      fresh_derived_key.master_encryption_iv,
      fresh_master_key.master_key,
    );

  let base64 = Base64Utils.array_buffer_to_base64(wrapped_master_key);
  let base64' =
    base64
    |> Base64Utils.base64_to_array_buffer
    |> Base64Utils.array_buffer_to_base64;

  assert(base64 == base64');

  Js.Console.log("base64_utils: Finished");

  resolve();
};

let run_sequential = actions => {
  Belt.Array.reduce(actions, Js.Promise.resolve(), (current, next) => {
    current |> Js.Promise.then_(next)
  });
};

let run = () => {
  run_sequential([|
    salt_test,
    wrap_unwrap_master_key,
    wrap_unwrap_protection_private_key,
    wrap_unwrap_verification_private_key,
    wrap_unwrap_ephemeral_key,
    encrypt_decrypt_message,
    sign_verify_message,
    base64_utils,
  |]);
};
