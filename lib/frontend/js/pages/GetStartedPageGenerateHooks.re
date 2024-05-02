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
  let (state, setState) = React.useState(() => State.Initial);
  let (logs, push, replaceKind, clear) =
    GetStartedPageGenerateLogs.useLogStack();

  let generateKeysCore =
    React.useCallback0((clientSecrets: ClientSecretsContext.clientSecrets) => {
      open Promise_syntax;
      open Vault_js;

      setState(_ => Generating);
      let saltBuffer = clientSecrets.saltBuffer;

      push({kind: Loading, message: "Generating Master Key"});
      let* freshMasterKey = MasterKey.create(~saltBuffer);
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
        encrypted_master_key: wrappedMasterKey |> Base64_js.ArrayBuffer.encode,
        master_key_iv:
          clientSecrets.masterKeyIv |> Base64_js.Uint8Array.encode,
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
      replaceKind(Success);

      push({kind: Success, message: "Finished!"});
      setState(_ => Finished);
      let* _ = sleep(1500);

      Js.Promise.resolve();
    });

  let generateKeys =
    React.useCallback0(clientSecrets => {
      generateKeysCore(clientSecrets)
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
