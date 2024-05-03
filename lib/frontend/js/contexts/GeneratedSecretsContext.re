open Js.Typed_array;
open Promise_syntax;

type generatedSecrets = {
  encryptedMasterKey: ArrayBuffer.t,
  encryptedProtectionKey: ArrayBuffer.t,
  exportedProtectionKey: ArrayBuffer.t,
  encryptedVerificationKey: ArrayBuffer.t,
  exportedVerificationKey: ArrayBuffer.t,
};

let writeGeneratedSecrets =
    (~generatedSecrets: generatedSecrets): Js.Promise.t(Js.Json.t) => {
  let encryptedMasterKey = generatedSecrets.encryptedMasterKey;
  let encryptedProtectionKey = generatedSecrets.encryptedProtectionKey;
  let exportedProtectionKey = generatedSecrets.exportedProtectionKey;
  let encryptedVerificationKey = generatedSecrets.encryptedVerificationKey;
  let exportedVerificationKey = generatedSecrets.exportedVerificationKey;

  Js.Promise.resolve(
    Client_types_bs.write_generatedSecretsCore({
      encryptedMasterKey,
      encryptedProtectionKey,
      exportedProtectionKey,
      encryptedVerificationKey,
      exportedVerificationKey,
    }),
  );
};

let readGeneratedSecrets =
    (~generatedSecretsJson: Js.Json.t): Js.Promise.t(generatedSecrets) => {
  let Client_types_bs.{
        encryptedMasterKey,
        encryptedProtectionKey,
        exportedProtectionKey,
        encryptedVerificationKey,
        exportedVerificationKey,
      } =
    Client_types_bs.read_generatedSecretsCore(generatedSecretsJson);

  Js.Promise.resolve({
    encryptedMasterKey,
    encryptedProtectionKey,
    exportedProtectionKey,
    encryptedVerificationKey,
    exportedVerificationKey,
  });
};

let writeToSessionStorage =
    (~generatedSecrets: generatedSecrets): Js.Promise.t(unit) => {
  let+ generatedSecretsJson = writeGeneratedSecrets(~generatedSecrets);
  let generatedSecretsString = Js.Json.stringify(generatedSecretsJson);

  Dom.Storage.(
    sessionStorage |> setItem("generatedSecrets", generatedSecretsString)
  );
};

let requestFromServer = (): Js.Promise.t(option(generatedSecrets)) => {
  let* keysFromServer = ApiSecrets.get();
  switch (keysFromServer) {
  | None => resolve(None)
  | Some({
      encrypted_master_key,
      encrypted_protection_key,
      exported_protection_key,
      encrypted_verification_key,
      exported_verification_key,
    }) =>
    let generatedSecrets = {
      encryptedMasterKey: encrypted_master_key |> Base64_js.ArrayBuffer.decode,
      encryptedProtectionKey:
        encrypted_protection_key |> Base64_js.ArrayBuffer.decode,
      exportedProtectionKey:
        exported_protection_key |> Base64_js.ArrayBuffer.decode,
      encryptedVerificationKey:
        encrypted_verification_key |> Base64_js.ArrayBuffer.decode,
      exportedVerificationKey:
        exported_verification_key |> Base64_js.ArrayBuffer.decode,
    };
    resolve(Some(generatedSecrets));
  };
};

let readFromSessionStorage = (): Js.Promise.t(option(generatedSecrets)) => {
  let generatedSecretsJson =
    Dom.Storage.(sessionStorage |> getItem("generatedSecrets"))
    |> Option.map(Js.Json.parseExn);

  switch (generatedSecretsJson) {
  | Some(generatedSecretsJson) =>
    let* generatedSecrets = readGeneratedSecrets(~generatedSecretsJson);
    resolve(Some(generatedSecrets));
  | None => resolve(None)
  };
};

include Store.MakeContext({
  open Store;

  type t = option(generatedSecrets);

  let use = () => {
    let subscribers = React.useRef(Subscribers_js.create());
    let get = readFromSessionStorage;
    let set = generatedSecrets => {
      let+ _ =
        switch (generatedSecrets) {
        | Some(generatedSecrets) => writeToSessionStorage(~generatedSecrets)
        | None => Js.Promise.resolve()
        };
      Subscribers_js.forEach(subscribers.current, subscriber => subscriber());
    };
    let subscribe = callback => {
      let key = Subscribers_js.add(subscribers.current, callback);
      () => {
        Subscribers_js.delete(subscribers.current, key);
      };
    };
    {get, set, subscribe};
  };
});
