module State = {
  type t =
    | Initial
    | Generating
    | Failed
    | Finished;
};

let sleep = time =>
  Js.Promise.make((~resolve, ~reject as _) => {
    let unit = ();
    let f = () => resolve(. unit);
    ignore(Js.Global.setTimeout(~f, time));
  });

let useGenerateKeys = () => {
  let generatedSecretsStore = GeneratedSecretsContext.useContext();
  let (state, setState) = React.useState(() => State.Initial);
  let (logs, push, replaceKind, clear) =
    GetStartedPageGenerateLogs.useLogStack();

  let generateKeysCore =
    React.useCallback0((derivedSecrets: DerivedSecretsContext.derivedSecrets) => {
      open Promise_syntax;
      open Vault_js;

      setState(_ => Generating);
      let saltBuffer = derivedSecrets.saltBuffer;

      push({kind: Loading, message: "Generating Master Key"});
      let* masterKey = MasterKey.create(~saltBuffer);
      let* _ = sleep(500);
      replaceKind(Success);

      push({kind: Loading, message: "Generating Protection Keys"});
      let* freshProtectionKeys = ProtectionKeys.create();
      let* _ = sleep(500);
      replaceKind(Success);

      push({kind: Loading, message: "Generating Verification Keys"});
      let* freshVerificationKeys = VerificationKeys.create();
      let* _ = sleep(500);
      replaceKind(Success);

      push({kind: Loading, message: "Encrypting Private Keys"});
      let* encryptedMasterkey =
        Operations.wrapMasterKey(
          ~derivedKey=derivedSecrets.derivedKey,
          ~masterKey,
          ~masterKeyIv=derivedSecrets.masterKeyIv,
        );
      let* encryptedProtectionKey =
        Operations.wrapProtectionPrivateKey(
          ~masterKey,
          ~protectionPrivateKey=freshProtectionKeys.privateKey,
          ~protectionKeyIv=derivedSecrets.protectionKeyIv,
        );
      let* encryptedVerificationKey =
        Operations.wrapVerificationPrivateKey(
          ~masterKey,
          ~verificationPrivateKey=freshVerificationKeys.privateKey,
          ~verificationKeyIv=derivedSecrets.verificationKeyIv,
        );
      let* _ = sleep(500);
      replaceKind(Success);

      push({kind: Loading, message: "Exporting Public Keys"});
      let* exportedProtectionKey =
        Operations.exportProtectionPublicKey(
          ~protectionPublicKey=freshProtectionKeys.publicKey,
        );
      let* exportedVerificationKey =
        Operations.exportVerificationPublicKey(
          ~verificationPublicKey=freshVerificationKeys.publicKey,
        );
      let* _ = sleep(500);
      replaceKind(Success);

      push({kind: Loading, message: "Submitting"});
      let registerKeysPayload: Types_universal.Definitions_t.register_keys_payload = {
        encrypted_master_key:
          encryptedMasterkey |> Base64_js.ArrayBuffer.encode,
        encrypted_protection_key:
          encryptedProtectionKey |> Base64_js.ArrayBuffer.encode,
        exported_protection_key:
          exportedProtectionKey |> Base64_js.ArrayBuffer.encode,
        encrypted_verification_key:
          encryptedVerificationKey |> Base64_js.ArrayBuffer.encode,
        exported_verification_key:
          exportedVerificationKey |> Base64_js.ArrayBuffer.encode,
      };
      let* _ = ApiSecrets.post(registerKeysPayload);
      let* _ = sleep(500);
      replaceKind(Success);

      push({kind: Loading, message: "Saving"});
      let generatedSecrets: GeneratedSecretsContext.generatedSecrets = {
        encryptedMasterKey: encryptedMasterkey,
        encryptedProtectionKey,
        exportedProtectionKey,
        encryptedVerificationKey,
        exportedVerificationKey,
      };
      let* _ = generatedSecretsStore.set(Some(generatedSecrets));
      let* _ = sleep(500);
      replaceKind(Success);

      push({kind: Success, message: "Finished!"});
      setState(_ => Finished);
      let* _ = sleep(1500);

      Js.Promise.resolve();
    });

  let generateKeys =
    React.useCallback0(derivedSecrets => {
      generateKeysCore(derivedSecrets)
      |> Js.Promise.(
           catch(error => {
             open Promise_syntax;
             let* _ = sleep(500);
             replaceKind(Failure);
             setState(_ => Failed);
             error |> Js.Exn.anyToExnInternal |> reject;
           })
         )
    });

  (logs, state, generateKeys, clear);
};
