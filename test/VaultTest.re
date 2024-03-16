open Js.Promise;
open Js.Typed_array;
open Vault.MasterKey;
open Vault.DerivedKey;
open Vault.RsaKeyPair;
open Vault.EphemeralKey;

let wrap_unwrap_master_key = () => {
  Vault.MasterKey.create()
  |> then_(master_key => {
       Vault.DerivedKey.create("Maho_Akashi_9_20", master_key.salt_buffer)
       |> then_(derived_key => {
            Vault.Operations.wrap_master_key(
              derived_key.derived_encryption_key,
              derived_key.encryption_iv,
              master_key.master_key,
            )
            |> then_(encrypted_master_key => {
                 Js.Console.log("wrap_master_key: Finished");
                 Vault.Operations.unwrap_master_key(
                   derived_key.derived_encryption_key,
                   derived_key.encryption_iv,
                   encrypted_master_key,
                 );
               })
            |> then_(_ => {
                 Js.Console.log("unwrap_master_key: Finished") |> resolve
               })
          })
     });
};

let wrap_unwrap_rsa_private_key = () => {
  Vault.MasterKey.create()
  |> then_(master_key => {
       Vault.RsaKeyPair.create()
       |> then_(rsa_key_pair => {
            Vault.Operations.wrap_rsa_private_key(
              master_key.master_key,
              master_key.rsa_private_encryption_iv,
              rsa_key_pair.private_key,
            )
            |> then_(encrypted_rsa_key => {
                 Js.Console.log("wrap_rsa_private_key: Finished");
                 Vault.Operations.unwrap_rsa_private_key(
                   master_key.master_key,
                   master_key.rsa_private_encryption_iv,
                   encrypted_rsa_key,
                 );
               })
            |> then_(_ => {
                 Js.Console.log("unwrap_rsa_private_key: Finished") |> resolve
               })
          })
     });
};

let wrap_unwrap_ephemeral_key = () => {
  Vault.RsaKeyPair.create()
  |> then_(rsa_key_pair => {
       Vault.EphemeralKey.create()
       |> then_(ephemeral_key => {
            Vault.Operations.wrap_ephemeral_key(
              rsa_key_pair.public_key,
              ephemeral_key.ephemeral_key,
            )
            |> then_(encrypted_ephemeral_key => {
                 Js.Console.log("wrap_ephemeral_key: Finished");
                 Vault.Operations.unwrap_ephemeral_key(
                   rsa_key_pair.private_key,
                   encrypted_ephemeral_key,
                 )
                 |> then_(_ => {
                      Js.Console.log("unwrap_ephemeral_key: Finished")
                      |> resolve
                    });
               })
          })
     });
};

let encrypt_decrypt_message = () => {
  let text_encoder = Vault.TextEncoder.create();
  let text_decoder = Vault.TextDecoder.create();

  let message = {js|ハッピーアラウンド！|js};
  let encoded_data =
    Uint8Array.buffer(Vault.TextEncoder.encode(text_encoder, message));

  Js.Console.log("encrypt_decrypt_message:");
  Js.Console.log2("  Plaintext Message:", message);

  Vault.EphemeralKey.create()
  |> then_(ephemeral_key => {
       Vault.Operations.encrypt_data(
         ephemeral_key.ephemeral_key,
         ephemeral_key.encryption_iv,
         encoded_data,
       )
       |> then_(encrypted_data => {
            Js.Console.log("  encrypt_data: Finished");
            Vault.Operations.decrypt_data(
              ephemeral_key.ephemeral_key,
              ephemeral_key.encryption_iv,
              encrypted_data,
            )
            |> then_(decrypted_data => {
                 Js.Console.log("  decrypt_data: Finished");
                 let decrypted_data =
                   Uint8Array.fromBuffer(decrypted_data, ());
                 let decrypted_message =
                   Vault.TextDecoder.decode(text_decoder, decrypted_data);
                 Js.Console.log2("  Decrypted Message:", decrypted_message)
                 |> resolve;
               });
          })
     });
};

let run_sequential = actions => {
  Belt.Array.reduce(actions, Js.Promise.resolve(), (current, next) => {
    current |> Js.Promise.then_(next)
  });
};

let run = () => {
  run_sequential([|
    wrap_unwrap_master_key,
    wrap_unwrap_rsa_private_key,
    wrap_unwrap_ephemeral_key,
    encrypt_decrypt_message,
  |]);
};
