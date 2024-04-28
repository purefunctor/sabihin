open Promise_syntax;
open Types_universal;
open Vault_js;

module Stage = {
  type t =
    | MasterKey
    | ProtectionKeys
    | VerificationKeys
    | EncryptingKeys
    | ExportingKeys
    | Submitting
    | Finished;

  let toIndex =
    fun
    | MasterKey => 0
    | ProtectionKeys => 1
    | VerificationKeys => 2
    | EncryptingKeys => 3
    | ExportingKeys => 4
    | Submitting => 5
    | Finished => 6;

  let hints = [|
    "Creating Master Key.",
    "Creating Protection Keys.",
    "Creating Verification Keys.",
    "Encrypting Private Keys.",
    "Exporting Public Keys.",
    "Submitting Keys.",
    "Finishing up...",
  |];
};

let sleep = time => {
  Js.Promise.make((~resolve, ~reject as _) => {
    let unit = ();
    let f = () => resolve(. unit);
    ignore(Js.Global.setTimeout(~f, time));
  });
};

let generateKeys =
    (
      ~clientSecrets: ClientSecretsContext.clientSecrets,
      ~setStage: (option(Stage.t) => option(Stage.t)) => unit,
    )
    : Js.Promise.t(Definitions_t.register_keys_payload) => {
  let* saltBuffer = Salt.computeDigest(clientSecrets.clientRandom);

  setStage(_ => Some(Stage.MasterKey));
  let* freshMasterKey = MasterKey.create(~saltBuffer);
  let* _ = sleep(500);

  setStage(_ => Some(Stage.ProtectionKeys));
  let* freshProtectionKeys = ProtectionKeys.create();
  let* _ = sleep(500);

  setStage(_ => Some(Stage.VerificationKeys));
  let* freshVerificationKeys = VerificationKeys.create();
  let* _ = sleep(500);

  setStage(_ => Some(Stage.EncryptingKeys));
  let* wrappedMasterKey =
    Operations.wrapMasterKey(
      ~derivedKey=clientSecrets.derivedKey,
      ~masterKey=freshMasterKey.masterKey,
      ~masterKeyIv=clientSecrets.masterKeyIv,
    );
  let* wrappedProtectionKey =
    Operations.wrapProtectionPrivateKey(
      ~masterKey=freshMasterKey.masterKey,
      ~protectionPrivateKey=freshProtectionKeys.privateKey,
      ~protectionKeyIv=freshMasterKey.protectionKeyIv,
    );
  let* wrappedVerificationKey =
    Operations.wrapVerificationPrivateKey(
      ~masterKey=freshMasterKey.masterKey,
      ~verificationPrivateKey=freshVerificationKeys.privateKey,
      ~verificationKeyIv=freshMasterKey.verificationKeyIv,
    );
  let* _ = sleep(500);

  setStage(_ => Some(Stage.ExportingKeys));
  let* exportedProtectionKey =
    Operations.exportProtectionPublicKey(
      ~protectionPublicKey=freshProtectionKeys.publicKey,
    );
  let* exportedVerificationKey =
    Operations.exportVerificationPublicKey(
      ~verificationPublicKey=freshVerificationKeys.publicKey,
    );
  let* _ = sleep(500);

  setStage(_ => Some(Stage.Submitting));
  let registerKeysPayload: Definitions_t.register_keys_payload = {
    client_random_value:
      clientSecrets.clientRandom |> Base64_js.Uint8Array.encode,
    encrypted_master_key: wrappedMasterKey |> Base64_js.ArrayBuffer.encode,
    master_key_iv: clientSecrets.masterKeyIv |> Base64_js.Uint8Array.encode,
    encrypted_protection_key:
      wrappedProtectionKey |> Base64_js.ArrayBuffer.encode,
    exported_protection_key:
      exportedProtectionKey |> Base64_js.ArrayBuffer.encode,
    protection_key_iv:
      freshMasterKey.protectionKeyIv |> Base64_js.Uint8Array.encode,
    encrypted_verification_key:
      wrappedVerificationKey |> Base64_js.ArrayBuffer.encode,
    exported_verification_key:
      exportedVerificationKey |> Base64_js.ArrayBuffer.encode,
    verification_key_iv:
      freshMasterKey.verificationKeyIv |> Base64_js.Uint8Array.encode,
  };
  let* _ = ApiSecrets.post(registerKeysPayload);
  let* _ = sleep(500);

  setStage(_ => Some(Stage.Finished));
  let* _ = sleep(1500);

  Js.Promise.resolve(registerKeysPayload);
};

let useGenerateKeys = () => {
  let clientSecretsStore = ClientSecretsContext.useContext();
  let (stage, setStage) = React.useState(() => None);

  let canStart =
    React.useMemo1(
      () =>
        switch (stage) {
        | Some(_) => false
        | _ => true
        },
      [|stage|],
    );

  let generateKeysCallback =
    React.useCallback1(
      () => {
        let* clientSecrets = clientSecretsStore.get();
        let+ _ =
          switch (clientSecrets) {
          | Some(clientSecrets) =>
            if (canStart) {
              let* _ = generateKeys(~clientSecrets, ~setStage);
              Js.Promise.resolve();
            } else {
              Js.Promise.resolve();
            }
          | _ => Js.Promise.resolve()
          };
        ();
      },
      [|canStart|],
    );

  (stage, generateKeysCallback);
};
